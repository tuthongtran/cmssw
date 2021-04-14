#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESWatcher.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CalibTracker/Records/interface/SiStripQualityRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripQuality.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
#include "CalibTracker/SiStripESProducers/interface/SiStripQualityHelpers.h"
#include "DQMServices/Core/interface/DQMEDHarvester.h"

class SiStripBadStripFromQualityDBWriter : public DQMEDHarvester {
public:
  explicit SiStripBadStripFromQualityDBWriter(const edm::ParameterSet&);
  ~SiStripBadStripFromQualityDBWriter() override {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  void endRun(edm::Run const&, edm::EventSetup const&) override;
  void dqmEndJob(DQMStore::IBooker&, DQMStore::IGetter&) override;

private:
  std::string rcdName_, openIOVAt_;
  uint32_t openIOVAtTime_;
  bool addBadCompFromFedErr_;
  float fedErrCutoff_;

  edm::ESGetToken<SiStripQuality, SiStripQualityRcd> stripQualityToken_;
  edm::ESGetToken<SiStripFedCabling, SiStripFedCablingRcd> fedCablingToken_;
  edm::ESWatcher<SiStripQualityRcd> stripQualityWatcher_;
  const SiStripFedCabling* fedCabling_;
  const SiStripQuality* siStripQuality_;
};

SiStripBadStripFromQualityDBWriter::SiStripBadStripFromQualityDBWriter(const edm::ParameterSet& iConfig)
    : rcdName_{iConfig.getParameter<std::string>("record")},
      openIOVAt_{iConfig.getUntrackedParameter<std::string>("OpenIovAt", "beginOfTime")},
      openIOVAtTime_{iConfig.getUntrackedParameter<uint32_t>("OpenIovAtTime", 1)},
      addBadCompFromFedErr_{iConfig.getUntrackedParameter<bool>("AddBadComponentsFromFedErrors", false)},
      fedErrCutoff_{float(iConfig.getUntrackedParameter<double>("FedErrorBadComponentsCutoff", 0.8))},
      stripQualityToken_{esConsumes<edm::Transition::EndRun>()},
      fedCablingToken_{addBadCompFromFedErr_ ? decltype(fedCablingToken_){esConsumes<edm::Transition::EndRun>()}
                                             : decltype(fedCablingToken_){}} {}

void SiStripBadStripFromQualityDBWriter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<std::string>("record", "");
  desc.addUntracked<std::string>("OpenIovAt", "beginOfTime");
  desc.addUntracked<uint32_t>("OpenIovAtTime", 1);
  desc.addUntracked<bool>("AddBadComponentsFromFedErrors", false);
  desc.addUntracked<double>("FedErrorBadComponentsCutoff", 0.8);
  descriptions.add("siStripBadStripFromQualityDBWriter", desc);
}

void SiStripBadStripFromQualityDBWriter::endRun(edm::Run const& /*run*/, edm::EventSetup const& iSetup) {
  if (stripQualityWatcher_.check(iSetup)) {
    siStripQuality_ = &iSetup.getData(stripQualityToken_);
    if (addBadCompFromFedErr_) {
      fedCabling_ = &iSetup.getData(fedCablingToken_);
    }
  }
}

void SiStripBadStripFromQualityDBWriter::dqmEndJob(DQMStore::IBooker& /*booker*/, DQMStore::IGetter& getter) {
  auto mergedQuality = SiStripQuality(*siStripQuality_);
  if (addBadCompFromFedErr_) {
    auto fedErrQuality = sistrip::badStripFromFedErr(getter, *fedCabling_, fedErrCutoff_);
    mergedQuality.add(fedErrQuality.get());
    mergedQuality.cleanUp();
    mergedQuality.fillBadComponents();
  }
  cond::Time_t time;
  edm::Service<cond::service::PoolDBOutputService> dbservice;
  if (dbservice.isAvailable()) {
    if (openIOVAt_ == "beginOfTime")
      time = dbservice->beginOfTime();
    else if (openIOVAt_ == "currentTime")
      time = dbservice->currentTime();
    else
      time = openIOVAtTime_;

    dbservice->writeOne(new SiStripBadStrip(mergedQuality), time, rcdName_);
  } else {
    edm::LogError("SiStripBadStripFromQualityDBWriter") << "Service is unavailable" << std::endl;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
DEFINE_FWK_MODULE(SiStripBadStripFromQualityDBWriter);
