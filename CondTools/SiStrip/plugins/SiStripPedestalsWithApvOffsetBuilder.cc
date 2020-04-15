/*
 * Apply a per-APV shift to the pedestals, to get more homogeneous common modes
 */

#include <memory>

#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "CondFormats/SiStripObjects/interface/SiStripPedestals.h"
#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"
#include "CalibFormats/SiStripObjects/interface/SiStripFecCabling.h"
#include "CalibTracker/Records/interface/SiStripDependentRecords.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

class SiStripPedestalsWithApvOffsetBuilder : public edm::one::EDAnalyzer<> {
public:
  explicit SiStripPedestalsWithApvOffsetBuilder(const edm::ParameterSet&);
  ~SiStripPedestalsWithApvOffsetBuilder() override {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
private:
  void analyze(const edm::Event&, const edm::EventSetup&) override;

  uint32_t m_printDebug;
  edm::ESGetToken<SiStripPedestals, SiStripPedestalsRcd> m_origPedestalsToken;
  edm::ESGetToken<SiStripFedCabling, SiStripFedCablingRcd> m_fedCablingToken;
  edm::ESGetToken<SiStripFecCabling, SiStripFecCablingRcd> m_fecCablingToken;
};

SiStripPedestalsWithApvOffsetBuilder::SiStripPedestalsWithApvOffsetBuilder(const edm::ParameterSet& iConfig) {
  m_origPedestalsToken = esConsumes<SiStripPedestals, SiStripPedestalsRcd>();
  m_fedCablingToken = esConsumes<SiStripFedCabling, SiStripFedCablingRcd>();
  m_fecCablingToken = esConsumes<SiStripFecCabling, SiStripFecCablingRcd>();

  m_printDebug = iConfig.getUntrackedParameter<uint32_t>("printDebug", 5);
}

void SiStripPedestalsWithApvOffsetBuilder::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  edm::ParameterSetDescription desc;
  desc.setComment("Shift pedestals per APV, to get the same common mode");
  desc.addUntracked<uint32_t>("printDebug", 5);
  descriptions.add("shiftPedestalsPerAPV", desc);
}

void SiStripPedestalsWithApvOffsetBuilder::analyze(const edm::Event& evt, const edm::EventSetup& iSetup) {

  const auto& origPedestals = iSetup.getData(m_origPedestalsToken);
  // const auto& fedCabling = iSetup.getData(m_fedCablingToken);
  // const auto& fecCabling = iSetup.getData(m_fecCablingToken);

  auto pedestals = std::make_unique<SiStripPedestals>();
  std::vector<uint32_t> detids;
  origPedestals.getDetIds(detids);
  uint32_t count{0};
  for ( const DetId det : detids ) {
    const auto pedRange = origPedestals.getRange(det);
    SiStripPedestals::InputVector newPed;
    const auto nPedsInRange = (static_cast<std::size_t>(pedRange.second-pedRange.first)*8)/10;
    if ( nPedsInRange % 128 )
      edm::LogError("SiStripPedestalsWithApvOffsetBuilder") << "Pedestals no multiple of 128: " << nPedsInRange;
    newPed.reserve(nPedsInRange);
    const auto nAPVs = nPedsInRange / 128;
    for ( std::size_t iAPV = 0; iAPV != nAPVs; ++iAPV ) {
      // TODO get the actual shift from somewhere
      const float apvShift = 0.;
      for ( std::size_t i = 128*iAPV; i != 128*(iAPV+1); ++i ) {
        newPed.push_back(origPedestals.getPed(i, pedRange)+apvShift);
      }
      if ( count < m_printDebug ) {
        edm::LogInfo("SiStripPedestalsWithApvOffsetBuilder") << "DetId " << std::hex << det.rawId() << std::dec << " shift: " << apvShift;
        ++count;
      }
    }
    pedestals->put(det, newPed);
  }

  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if (mydbservice.isAvailable()) {
    mydbservice->writeOne(pedestals.get(), mydbservice->currentTime(), "SiStripPedestalsRcd");
  } else {
    edm::LogError("SiStripPedestalsWithApvOffsetBuilder") << "Service is unavailable" << std::endl;
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
DEFINE_FWK_MODULE(SiStripPedestalsWithApvOffsetBuilder);
