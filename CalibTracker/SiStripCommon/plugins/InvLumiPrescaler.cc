#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "DataFormats/Scalers/interface/LumiScalers.h"
#include <random>

class InvLumiPrescaler : public edm::stream::EDFilter<> {
public:
  explicit InvLumiPrescaler(edm::ParameterSet const&);
  ~InvLumiPrescaler() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
private:
  bool filter(edm::Event& e, const edm::EventSetup&) final;

  uint32_t seedOffset_;
  double scale_;
  edm::EDGetTokenT<LumiScalersCollection> scalerToken_;
};

InvLumiPrescaler::InvLumiPrescaler(edm::ParameterSet const& ps)
  : seedOffset_(ps.getParameter<uint32_t>("seedOffset"))
  , scale_(ps.getParameter<int>("scale"))
  , scalerToken_(consumes<LumiScalersCollection>(ps.getParameter<edm::InputTag>("lumiScalers")))
{}

InvLumiPrescaler::~InvLumiPrescaler() {}

bool InvLumiPrescaler::filter(edm::Event& evt, const edm::EventSetup& )
{
  edm::Handle<LumiScalersCollection> lumiScalers;
  evt.getByToken(scalerToken_, lumiScalers);
  const float instLumi = ( lumiScalers.isValid() && !lumiScalers->empty() ) ? lumiScalers->begin()->instantLumi() : 0.;
  if ( instLumi < scale_ ) {
    return true;
  } else {
    const auto runNum  = static_cast<uint32_t>(evt.id().run());
    const auto lumiNum = static_cast<uint32_t>(evt.id().luminosityBlock());
    const auto evtNum  = static_cast<uint32_t>(evt.id().event());
    const uint32_t seed = (lumiNum<<10) + (runNum<<20) + evtNum + seedOffset_;
    std::uniform_real_distribution<float> dist{0., 1.};
    std::mt19937 rg{seed};
    return ( dist(rg) < scale_/instLumi );
  }
}

void InvLumiPrescaler::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  edm::ParameterSetDescription desc;
  desc.add<uint32_t>("seedOffset", 0)->setComment("Random generator seed offset");
  desc.add<double>("scale", 5000)->setComment("Scale parameter a: the prescale is 1 for l<a and l/a for l>a");
  desc.add<edm::InputTag>("lumiScalers", edm::InputTag("scalersRawToDigi"))->setComment("Lumi scalers input");
  descriptions.add("invLumiPrescaler", desc);
}

DEFINE_FWK_MODULE(InvLumiPrescaler);
