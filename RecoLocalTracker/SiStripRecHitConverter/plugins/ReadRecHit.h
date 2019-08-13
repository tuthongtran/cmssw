#ifndef ReadRecHit_h
#define ReadRecHit_h

/** \class ReadRecHit
 *
 * ReadRecHit is a analyzer which reads rechits
 *
 * \author C. Genta
 *
 *
 ************************************************************/

#include "FWCore/Framework/interface/Frameworkfwd.h" 
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/Common/interface/DetSetVectorNew.h"


#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"

#include "RecoLocalTracker/SiStripRecHitConverter/interface/ReadRecHitAlgorithm.h"

namespace cms
{
  class ReadRecHit : public edm::EDAnalyzer
  {
  public:

    explicit ReadRecHit(const edm::ParameterSet& conf);

    virtual ~ReadRecHit();

    virtual void analyze(const edm::Event& e, const edm::EventSetup& c);

  private:
    edm::EDGetTokenT<edm::DetSetVector<SiStripMatchedRecHit2DCollection>> rechitsmatched_;
    edm::EDGetTokenT<edm::DetSetVector<SiStripRecHit2DCollection>> rechitsrphi_;
    edm::EDGetTokenT<edm::DetSetVector<SiStripRecHit2DCollection>> rechitsstereo_;

    ReadRecHitAlgorithm readRecHitAlgorithm_;
    edm::ParameterSet conf_;

  };
}


#endif
