/*
 * Diff two pedestals payloads
 */

#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "CondFormats/SiStripObjects/interface/SiStripPedestals.h"
#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

class SiStripComparePedestals : public edm::one::EDAnalyzer<> {
public:
  explicit SiStripComparePedestals(const edm::ParameterSet&);
  ~SiStripComparePedestals() override {}

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
private:
  void analyze(const edm::Event&, const edm::EventSetup&) override;

  edm::ESGetToken<SiStripPedestals, SiStripPedestalsRcd> m_pedestalsAToken;
  edm::ESGetToken<SiStripPedestals, SiStripPedestalsRcd> m_pedestalsBToken;
};

SiStripComparePedestals::SiStripComparePedestals(const edm::ParameterSet& iConfig) {
  m_pedestalsAToken = esConsumes<SiStripPedestals, SiStripPedestalsRcd>(iConfig.getParameter<edm::ESInputTag>("pedestalsA"));
  m_pedestalsBToken = esConsumes<SiStripPedestals, SiStripPedestalsRcd>(iConfig.getParameter<edm::ESInputTag>("pedestalsB"));
}

void SiStripComparePedestals::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  edm::ParameterSetDescription desc;
  desc.setComment("Diff between two pedestals tags");
  desc.add<edm::ESInputTag>("pedestalsA", edm::ESInputTag{});
  desc.add<edm::ESInputTag>("pedestalsB", edm::ESInputTag{});
  descriptions.add("diffPedestals", desc);
}

namespace {
  /**
   * Make the union of two vectors (the former is modified in place, and sorted).
   */
  template<typename VEC>
  void make_union(VEC& combined, const VEC& other) {
    std::sort(std::begin(combined), std::end(combined));
    VEC toAdd;
    for ( const auto val : other ) {
      const auto it = std::lower_bound(std::begin(combined), std::end(combined), val);
      if ( ( std::end(combined) == it ) || ( *it != val ) ) {
        toAdd.push_back(val);
      }
    }
    std::copy(std::begin(toAdd), std::end(toAdd), std::back_inserter(combined));
    std::sort(std::begin(combined), std::end(combined));
  }

  bool is_empty(const SiStripPedestals::Range& pedestalsRange) {
    return pedestalsRange.first == pedestalsRange.second;
  }
  std::size_t nPedestals(const SiStripPedestals::Range& pedestalsRange) {
    return (static_cast<std::size_t>(pedestalsRange.second-pedestalsRange.first)*8)/10;
  }
}

void SiStripComparePedestals::analyze(const edm::Event& evt, const edm::EventSetup& iSetup) {

  const auto& pedestalsA = iSetup.getData(m_pedestalsAToken);
  const auto& pedestalsB = iSetup.getData(m_pedestalsBToken);

  std::vector<uint32_t> detids;
  // collect all DetIds in both payloads
  pedestalsA.getDetIds(detids);
  std::vector<uint32_t> detidsB, onlyB;
  pedestalsB.getDetIds(detidsB);
  make_union(detids, detidsB);

  std::size_t nPedCompared{0}, nDetCompared{0}, nPedDiff{0};
  for ( const DetId det : detids ) {
    const auto pedRangeA = pedestalsA.getRange(det);
    const auto pedRangeB = pedestalsB.getRange(det);
    if ( is_empty(pedRangeA) ) {
      edm::LogInfo("SiStripComparePedestals") << "No pedestals in payload A for DetId " << std::hex << det.rawId() << std::dec;
    } else if ( is_empty(pedRangeB) ) {
      edm::LogInfo("SiStripComparePedestals") << "No pedestals in payload B for DetId " << std::hex << det.rawId() << std::dec;
    } else { // in both
      const auto nPedsA = nPedestals(pedRangeA);
      const auto nPedsB = nPedestals(pedRangeB);
      if ( nPedsA != nPedsB ) {
        edm::LogInfo("SiStripComparePedestals") << "Different number of pedestals for DetId " << std::hex << det.rawId() << std::dec << ": " << nPedsA << " for payload A and " << nPedsB << " for payload B";
      } else {
        if ( nPedsA % 128 )
          edm::LogError("SiStripComparePedestals") << "Number of pedestals is no multiple of 128: " << nPedsA;
        for ( std::size_t i = 0; i != nPedsA; ++i ) {
          const auto pedA = pedestalsA.getPed(i, pedRangeA);
          const auto pedB = pedestalsB.getPed(i, pedRangeB);
          if ( pedA != pedB ) {
            edm::LogInfo("SiStripComparePedestals") << "Different pedestal value for DetId " << std::hex << det.rawId() << std::dec << " strip " << i << "(APV#" << (i/128) << "): A=" << pedA << ", B=" << pedB;
            ++nPedDiff;
          }
          ++nPedCompared;
        }
      }
      ++nDetCompared;
    }
  }
  edm::LogInfo("SiStripComparePedestals") << "Compared " << nPedCompared << " pedestals for " << nDetCompared << " modules; " << nPedDiff << " are different";
}

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
DEFINE_FWK_MODULE(SiStripComparePedestals);
