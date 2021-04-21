#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include "DataFormats/Scalers/interface/LumiScalers.h"

class TkInstLumiTableProducer : public edm::stream::EDProducer<> {
public:
  explicit TkInstLumiTableProducer(const edm::ParameterSet& params)
      : m_name(params.getParameter<std::string>("name")),
        m_doc(params.existsAs<std::string>("doc") ? params.getParameter<std::string>("doc") : ""),
        m_extension(params.existsAs<bool>("extension") ? params.getParameter<bool>("extension") : false),
        m_scalerToken(consumes<LumiScalersCollection>(params.getParameter<edm::InputTag>("lumiScalers"))) {
    produces<nanoaod::FlatTable>();
  }

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<std::string>("name", "");
    desc.add<std::string>("doc", "");
    desc.add<bool>("extension", false);
    desc.add<edm::InputTag>("lumiScalers", edm::InputTag("scalersRawToDigi"));
    descriptions.add("tkInstLumiTable", desc);
  }

  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;

private:
  const std::string m_name;
  const std::string m_doc;
  bool m_extension;

  const edm::EDGetTokenT<LumiScalersCollection> m_scalerToken;
};

void TkInstLumiTableProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  auto out = std::make_unique<nanoaod::FlatTable>(1, m_name, true, m_extension);

  out->addColumnValue<int>("bx", iEvent.bunchCrossing(), "Bunch-crossing ID");

  float instLumi{0.}, pu{0.};
  edm::Handle<LumiScalersCollection> lumiScalers;
  iEvent.getByToken(m_scalerToken, lumiScalers);
  if (lumiScalers.isValid()) {
    if (lumiScalers->begin() != lumiScalers->end()) {
      instLumi = lumiScalers->begin()->instantLumi();
      pu = lumiScalers->begin()->pileup();
    }
  }
  out->addColumnValue<float>("instLumi", instLumi, "Instantaneous luminosity");
  out->addColumnValue<float>("PU", pu, "Pileup");

  iEvent.put(std::move(out));
}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(TkInstLumiTableProducer);
