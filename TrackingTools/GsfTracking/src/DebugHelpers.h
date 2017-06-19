#ifdef EDM_ML_DEBUG

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/DTWireId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"

namespace {
   inline
   void dump(TrackingRecHit const & hit, int hitcounter, const TrackerTopology* trackerTopology) {
    if (hit.isValid()) {
      LogTrace("GsfTrackFitters")<< " ----------------- HIT #" << hitcounter << " (VALID)-----------------------\n"
	<< "  HIT IS AT R   " << hit.globalPosition().perp() << "\n"
	<< "  HIT IS AT Z   " << hit.globalPosition().z() << "\n"
	<< "  HIT IS AT Phi " << hit.globalPosition().phi() << "\n"
	<< "  HIT IS AT Loc " << hit.localPosition() << "\n"
	<< "  WITH LocError " << hit.localPositionError() << "\n"
	<< "  HIT IS AT Glo " << hit.globalPosition() << "\n"
	<< "SURFACE POSITION" << "\n"
	<< hit.surface()->position()<<"\n"
	<< "SURFACE ROTATION" << "\n"
	<< hit.surface()->rotation()
        <<  "dimension " << hit.dimension();

      DetId hitId = hit.geographicalId();

      LogDebug("GsfTrackFitters") << " hit det=" << hitId.rawId();

      if(hitId.det() == DetId::Tracker) {
	switch(hitId.subdetId()) {
	  case StripSubdetector::TIB:
	    LogDebug("GsfTrackFitters") << " I am TIB " << trackerTopology->tibLayer(hitId); break;
	  case StripSubdetector::TOB:
	    LogDebug("GsfTrackFitters") << " I am TOB " << trackerTopology->tobLayer(hitId); break;
	  case StripSubdetector::TEC:
	    LogDebug("GsfTrackFitters") << " I am TEC " << trackerTopology->tecWheel(hitId); break;
	  case StripSubdetector::TID:
	    LogDebug("GsfTrackFitters") << " I am TID " << trackerTopology->tidWheel(hitId); break;
	  case PixelSubdetector::PixelBarrel:
	    LogDebug("GsfTrackFitters") << " I am PixBar " << trackerTopology->pxbLayer(hitId); break;
	  case PixelSubdetector::PixelEndcap:
	    LogDebug("GsfTrackFitters") << " I am PixFwd " << trackerTopology->pxfDisk(hitId); break;
	  default:
	    LogDebug("GsfTrackFitters") << " UNKNOWN TRACKER HIT TYPE ";
	}
      }
      else if(hitId.det() == DetId::Muon) {
	if(hitId.subdetId() == MuonSubdetId::DT)
	  LogDebug("GsfTrackFitters") << " I am DT " << DTWireId(hitId);
	else if (hitId.subdetId() == MuonSubdetId::CSC )
	  LogDebug("GsfTrackFitters") << " I am CSC " << CSCDetId(hitId);
	else if (hitId.subdetId() == MuonSubdetId::RPC )
	  LogDebug("GsfTrackFitters") << " I am RPC " << RPCDetId(hitId);
	else if (hitId.subdetId() == MuonSubdetId::GEM )
	  LogDebug("GsfTrackFitters") << " I am GEM " << GEMDetId(hitId);

	else if (hitId.subdetId() == MuonSubdetId::ME0 )
	  LogDebug("GsfTrackFitters") << " I am ME0 " << ME0DetId(hitId);
	else 
	  LogDebug("GsfTrackFitters") << " UNKNOWN MUON HIT TYPE ";
      }
      else
	LogDebug("GsfTrackFitters") << " UNKNOWN HIT TYPE ";

    } else {
      LogDebug("GsfTrackFitters")
	<< " ----------------- INVALID HIT #" << hitcounter << " -----------------------";
    }
   }
}
#include <sstream>
   inline void dump(TrajectoryStateOnSurface const & tsos, const char * header) {
     std::ostringstream ss; ss<< " weights ";
     for (auto const & c : tsos.components()) ss << c.weight() << '/';
     ss << "\nmomentums ";
     for (auto const & c : tsos.components()) ss << c.globalMomentum().mag() << '/';
     ss << "\ndeltap/p ";
    for (auto const & c : tsos.components()) ss << std::sqrt(tsos.curvilinearError().matrix()(0,0))/c.globalMomentum().mag() << '/';
     LogTrace("GsfTrackFitters")
      << header  << "! size " << tsos.components().size() << ss.str() << "\n"
      <<" with local position " << tsos.localPosition() << "\n"
      << tsos;
   }
#else
namespace {
   inline void dump(TrackingRecHit const &, int, const TrackerTopology*) {}
   inline void dump(TrajectoryStateOnSurface const &, const char *){}
}
#endif

