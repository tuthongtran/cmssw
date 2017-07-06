#ifndef SiPixelDetId_PixelModuleName_H
#define SiPixelDetId_PixelModuleName_H

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

namespace std {
  class string;
  class ostream;
};

/** \class PixelModuleName
 * Base class to Pixel modules naming, provides a name as in PixelDatabase
 */

class PixelModuleName {
public:

  enum ModuleType { v1x2, v1x5, v1x8, v2x3, v2x4, v2x5, v2x8 } ;

  PixelModuleName(bool isBarrel) : barrel(isBarrel) { }
  // NOTE define in cc
  virtual ~PixelModuleName() { }

  /// true for barrel modules
  // NOTE this needs not be virtual
  virtual bool isBarrel() const { return barrel; }

  // NOTE proposed new interface: (DetId arg will trigger auto-conversion when passed uint32_t)
  // static bool isBarrel(DetId rawDetId) { return ( PixelSubdetector::PixelBarrel == rawDetId.subdetId() ); }
  static bool isBarrel(uint32_t rawDetId) { return ( 1==((rawDetId>>25)&0x7)); }

  /// associated name
  virtual std::string name() const = 0;

  /// module type
  // NOTE wondering whether we really need this
  virtual ModuleType moduleType() const = 0;

  /// check equality of modules
  // NOTE could store DetId here, no need for storing isBarrel, and could have non-virtual operator==
  virtual  bool operator== (const PixelModuleName &) const = 0;

private:
  bool barrel;
};

std::ostream & operator<<( std::ostream& out, const PixelModuleName::ModuleType & t);
#endif
