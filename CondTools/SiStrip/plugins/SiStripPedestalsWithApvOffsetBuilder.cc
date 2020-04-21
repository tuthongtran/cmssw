/*
 * Apply a per-APV shift to the pedestals, to get more homogeneous common modes
 */

#include <memory>

#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripProcessedRawDigi.h"

#include "CondFormats/SiStripObjects/interface/SiStripPedestals.h"
#include "CondFormats/SiStripObjects/interface/SiStripFedCabling.h"
#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"
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

  edm::FileInPath m_shiftsFile;
  uint32_t m_printDebug;
  edm::ESGetToken<SiStripPedestals, SiStripPedestalsRcd> m_origPedestalsToken;
  edm::ESGetToken<SiStripFedCabling, SiStripFedCablingRcd> m_fedCablingToken;
};

SiStripPedestalsWithApvOffsetBuilder::SiStripPedestalsWithApvOffsetBuilder(const edm::ParameterSet& iConfig) {
  m_origPedestalsToken = esConsumes<SiStripPedestals, SiStripPedestalsRcd>();
  m_fedCablingToken = esConsumes<SiStripFedCabling, SiStripFedCablingRcd>();

  m_shiftsFile = iConfig.getParameter<edm::FileInPath>("shifts");
  edm::LogInfo("SiStripPedestalsWithApvOffsetBuilder") << "Reading shifts from " << m_shiftsFile.fullPath();
  m_printDebug = iConfig.getUntrackedParameter<uint32_t>("printDebug", 5);
}

void SiStripPedestalsWithApvOffsetBuilder::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  edm::ParameterSetDescription desc;
  desc.setComment("Shift pedestals per APV, to get the same common mode");
  desc.add<edm::FileInPath>("shifts");
  desc.addUntracked<uint32_t>("printDebug", 5);
  descriptions.add("shiftPedestalsPerAPV", desc);
}

void SiStripPedestalsWithApvOffsetBuilder::analyze(const edm::Event& evt, const edm::EventSetup& iSetup) {
  const auto& origPedestals = iSetup.getData(m_origPedestalsToken);

  const auto& fedCabling = iSetup.getData(m_fedCablingToken);

  edm::DetSetVector<SiStripProcessedRawDigi> shifts;
  static const decltype(shifts)::detset empty_shifts{};
  std::ifstream shiftsFile{m_shiftsFile.fullPath()};
  uint16_t fedId, fedCh, apvId;
  float shift;
  while ( shiftsFile >> fedId >> fedCh >> apvId >> shift ) {
    const auto conn = fedCabling.fedConnection(fedId, fedCh);
    auto& shiftds = shifts.find_or_insert(conn.detId());
    if ( shiftds.empty() ) { // initialize with zeroes
      for ( std::size_t i{0}; i != conn.nApvs(); ++i ) {
        shiftds.push_back(0.);
      }
    }
    const auto iAPV = 2*conn.apvPairNumber() + apvId;
    if ( shiftds[iAPV].adc() != 0. ) {
      edm::LogError("SiStripPedestalsWithApvOffsetBuilder") << "Duplicate entry for DetId " << std::hex << conn.detId() << std::dec << " APV#" << iAPV << ", replacing previous " << shiftds[iAPV].adc() << " with new value " << shift;
    } else {
      LogDebug("SiStripPedestalsWithApvOffsetBuilder") << "Read shift for DetId " << std::hex << conn.detId() << std::dec << " APV#" << iAPV << ": " << shift;
    }
    shiftds[iAPV] = shift;
  }

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
    const auto itShifts = shifts.find(det);
    const auto& detShifts = ( shifts.end() != itShifts ) ? *itShifts : empty_shifts;
    for ( std::size_t iAPV = 0; iAPV != nAPVs; ++iAPV ) {
      const float apvShift = (!detShifts.empty()) ? detShifts[iAPV].adc() : 0.;
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
