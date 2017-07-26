#ifndef TRACKER_TOPOLOGY_STANDALONE_H
#define TRACKER_TOPOLOGY_STANDALONE_H

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"

namespace StandaloneTrackerTopology {
  /**
   * Construct a TrackerTopology from a trackerParameters.xml file
   * WARNING: this header has been introduced to call a TrackerTopology
   * object whenever it is not possible to access it through an EventSetup.
   * Do not use it if that is the case.
   */
  TrackerTopology fromTrackerParametersXML(const std::string& xmlFileName);
};

#endif // TRACKER_TOPOLOGY_STANDALONE_H
