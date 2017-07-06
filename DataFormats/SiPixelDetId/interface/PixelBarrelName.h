#ifndef SiPixelDetId_PixelBarrelName_H
#define SiPixelDetId_PixelBarrelName_H

/** \class PixelBarrelName
 * Module name (as in PixelDatabase) in barrel
 */

#include "DataFormats/SiPixelDetId/interface/PixelModuleName.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"

class DetId;
class TrackerTopology;

class PixelBarrelName : public PixelModuleName {
public:

  enum Shell { mO = 1, mI = 2 , pO =3 , pI =4 };

  /// ctor from DetId
  // NOTE suggest taking DetId by value
  PixelBarrelName(const DetId &, const TrackerTopology* tt, bool phase=false);

  // do not use, works only for phase0 and old pixel classes
  // NOTE to be removed completely
  PixelBarrelName(const DetId &, bool phase=false);

  /// ctor for defined name with dummy parameters
  // NOTE ??? in any case make this one explicit!!!
 PixelBarrelName(Shell shell=mO, int layer=0, int module=0, int ladder=0, bool phase=false)
   : PixelModuleName(true),
    thePart(shell), theLayer(layer), theModule(module), theLadder(ladder), phase1(phase)
  { }

  /// ctor from name string
  // NOTE externalize from class? take also const TrackerTopology*?
  PixelBarrelName(std::string name, bool phase=false);

  // NOTE define in cc
  virtual ~PixelBarrelName() { }

  // NOTE private
  inline int convertLadderNumber(int oldLadder);

  /// from base class
  virtual std::string name() const;

  Shell shell() const { return thePart; }

  /// layer id
  int layerName() const { return theLayer; }

  /// module id (index in z)
  int moduleName() const { return theModule; }

  /// ladder id (index in phi)
  int ladderName() const { return theLadder; }

  /// sector id
  int sectorName() const;

  /// full or half module
  bool isHalfModule() const;

  /// module Type
  virtual PixelModuleName::ModuleType  moduleType() const;

  /// return the DetId
  // NOTE first should go, we may store the DetId to make the replacement easier
  PXBDetId getDetId();
  DetId getDetId(const TrackerTopology* tt);

  /// check equality of modules from datamemebers
  virtual bool operator== (const PixelModuleName &) const;

private:
  // NOTE these could all be short (but is that packed efficiently?)
  Shell thePart;
  int theLayer, theModule, theLadder;
  bool phase1;
};

std::ostream & operator<<( std::ostream& out, const PixelBarrelName::Shell& t);
#endif
