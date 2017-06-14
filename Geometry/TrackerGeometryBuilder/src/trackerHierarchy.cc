#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "Geometry/TrackerGeometryBuilder/interface/trackerHierarchy.h"

#include<string>
#include<vector>

std::string trackerHierarchy(const TrackerTopology* tTopo, unsigned int rawid) {
  DetId id(rawid);
  int subdetid = id.subdetId();
  switch (subdetid) {
    
    // PXB
  case 1:
    {
      PXBDetId module(rawid);
      char theLayer  = module.layer();
      char theLadder = module.ladder();
      char theModule = module.module();
      char key[] = { 1, theLayer , theLadder, theModule};
      return std::string(key,4);
    }
    
    // P1XF
  case 2:
    {
      PXFDetId module(rawid);
      char thePanel  = module.panel();
      char theDisk   = module.disk();
      char theBlade  = module.blade();
      char theModule = module.module();
      char key[] = { 2,
		     char(module.side()),
		     thePanel , theDisk, 
		     theBlade, theModule};
      return std::string(key,6);
    }
  
  // TIB
  case 3:
    {
      char            theLayer  = tTopo->layer(rawid);
      std::vector<unsigned int> theString = tTopo->tibStringInfo(rawid);
      char             theModule = tTopo->module(rawid);
      //side = (theString[0] == 1 ) ? "-" : "+";
      //part = (theString[1] == 1 ) ? "int" : "ext";
      char key[] = { 3, 
		     theLayer, 
		     char(theString[0]),
		     char(theString[1]), 
		     char(theString[2]), 
		     theModule,
		     char(tTopo->tibGlued(rawid) ? tTopo->isStereo(rawid)+1 : 0)
      };
      return std::string(key, tTopo->tibGlued(rawid) ? 7 : 6);
    }
    
    // TID
  case 4:
    {
      unsigned int         theDisk   = tTopo->tidWheel(rawid);
      unsigned int         theRing   = tTopo->tidRing(rawid);
      // side = (tTopo->tidSide(rawid) == 1 ) ? "-" : "+";
      // part = (tTopo->tidOrder(rawid) == 1 ) ? "back" : "front";
      char key[] = { 4, 
		     char(tTopo->tidSide(rawid)),
		     char(theDisk) , 
		     char(theRing),
		     char(tTopo->tidOrder(rawid)), 
		     char(tTopo->tidModule(rawid)),
		     char(tTopo->tidGlued(rawid) ? tTopo->isStereo(rawid)+1 : 0)
      };
      return std::string(key,tTopo->tidGlued(rawid) ? 7 : 6);
    }
    
    // TOB
  case 5:
    {
      unsigned int              theLayer  = tTopo->layer(rawid);
      unsigned int              theModule = tTopo->module(rawid);
      //	side = (tTopo->tobSide(rawid) == 1 ) ? "-" : "+";
      char key[] = { 5, char(theLayer) , 
		     char(tTopo->tobSide(rawid)), 
		     char(tTopo->tobRod(rawid)), 
		     char(theModule),
		     char(tTopo->tobGlued(rawid) ? tTopo->isStereo(rawid)+1 : 0)
      };
      return std::string(key, tTopo->tobGlued(rawid) ?  6 : 5);
    }
    
    // TEC
  case 6:
    {
      unsigned int              theWheel  = tTopo->tecWheel(rawid);
      unsigned int              theModule = tTopo->module(rawid);
      unsigned int              theRing   = tTopo->tecRing(rawid);
      //	side  = (tTopo->tecSide(rawid) == 1 ) ? "-" : "+";
      //	petal = (tTopo->tecOrder(rawid) == 1 ) ? "back" : "front";
      // int out_side  = (tTopo->tecSide(rawid) == 1 ) ? -1 : 1;
      
      char key[] = { 6, 
		     char(tTopo->tecSide(rawid)),
		     char(theWheel),
		     char(tTopo->tecOrder(rawid)), 
		     char(tTopo->tecPetalNumber(rawid)),
		     char(theRing),
		     char(theModule),
		     char(tTopo->tecGlued(rawid) ? tTopo->isStereo(rawid)+1 : 0)
      };
      return std::string(key, tTopo->tecGlued(rawid) ? 8 : 7);
    }
  default:
    return std::string();
  }
}
