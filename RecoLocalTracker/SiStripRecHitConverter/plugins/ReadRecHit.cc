// File: ReadRecHit.cc
// Description:  see ReadRecHit.h
// Author:  C.Genta
//
//--------------------------------------------
#include <memory>
#include <string>
#include <iostream>

#include "RecoLocalTracker/SiStripRecHitConverter/plugins/ReadRecHit.h"

//#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
//#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"

//#include "FWCore/Framework/interface/ESHandle.h"
//#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
namespace cms
{

  ReadRecHit::ReadRecHit(edm::ParameterSet const& conf) : 
    readRecHitAlgorithm_(conf) ,
    conf_(conf)
  {
    //    produces<SiStripRecHit2DCollection>();
    rechitsmatched_ = consumes<SiStripMatchedRecHit2DCollection>(conf_.getParameter<edm::InputTag>( "rechitsmatched" ));
    rechitsrphi_ = consumes<SiStripRecHit2DCollection>(conf_.getParameter<edm::InputTag>( "rechitsrphi" ));
    rechitsstereo_ = consumes<SiStripRecHit2DCollection>(conf_.getParameter<edm::InputTag>( "rechitsstereo" ));
  }


  // Virtual destructor needed.
  ReadRecHit::~ReadRecHit() { }  

  // Functions that gets called by framework every event
  void ReadRecHit::analyze(const edm::Event& e, const edm::EventSetup& es)
  {
    using namespace edm;
    std::string rechitProducer = conf_.getParameter<std::string>("RecHitProducer");

    // Step A: Get Inputs 
    edm::Handle<SiStripMatchedRecHit2DCollection> rechitsmatched;
    edm::Handle<SiStripRecHit2DCollection> rechitsrphi;
    edm::Handle<SiStripRecHit2DCollection> rechitsstereo;

    e.getByToken(rechitsmatched_, rechitsmatched);
    e.getByToken(rechitsrphi_, rechitsrphi);
    e.getByToken(rechitsstereo_, rechitsstereo);

    edm::LogInfo("ReadRecHit")<<"Matched hits:";
    readRecHitAlgorithm_.run(rechitsmatched.product());
    edm::LogInfo("ReadRecHit")<<"Rphi hits:";
    readRecHitAlgorithm_.run(rechitsrphi.product());
    edm::LogInfo("ReadRecHit")<<"Stereo hits:";
    readRecHitAlgorithm_.run(rechitsstereo.product());
  }

}
