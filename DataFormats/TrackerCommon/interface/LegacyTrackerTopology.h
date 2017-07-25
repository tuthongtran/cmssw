#ifndef TRACKER_TOPOLOGY_LEGACY_H
#define TRACKER_TOPOLOGY_LEGACY_H

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include <memory>

namespace LegacyTrackerTopology {
  /**
   * Construct a TrackerTopology from a trackerParameters.xml file
   * WARNING: this header has been introduced to call a TrackerTopology
   * object whenever it is not possible to access it through an EventSetup.
   * Do not use it if that is the case.
   */
  std::unique_ptr<TrackerTopology> getTrackerTopology(const std::string& xmlFileName);
};

#endif // TRACKER_TOPOLOGY_LEGACY_H
