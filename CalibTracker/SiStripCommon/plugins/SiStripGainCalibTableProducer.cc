#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"

#include "DataFormats/GeometrySurface/interface/TrapezoidalPlaneBounds.h"
#include "DataFormats/GeometrySurface/interface/RectangularPlaneBounds.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/PixelGeomDetUnit.h"
#include "CalibFormats/SiStripObjects/interface/SiStripGain.h"
#include "CalibTracker/Records/interface/SiStripGainRcd.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit1D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

class SiStripGainCalibTableProducer : public edm::stream::EDProducer<> {
public:
  explicit SiStripGainCalibTableProducer(const edm::ParameterSet& params)
      : m_name(params.getParameter<std::string>("name")),
        m_doc(params.existsAs<std::string>("doc") ? params.getParameter<std::string>("doc") : ""),
        m_extension(params.existsAs<bool>("extension") ? params.getParameter<bool>("extension") : false),
        m_tracks_token(consumes<edm::View<reco::Track>>(params.getParameter<edm::InputTag>("Tracks"))),
        m_association_token(consumes<TrajTrackAssociationCollection>(params.getParameter<edm::InputTag>("Tracks"))) {
    produces<nanoaod::FlatTable>();
  }

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    edm::ParameterSetDescription desc;
    desc.add<std::string>("name", "gain");
    desc.add<std::string>("doc", "");
    desc.add<bool>("extension", false);
    desc.add<edm::InputTag>("Tracks", edm::InputTag{"generalTracks"});
    descriptions.add("siStripGainCalibTable", desc);
  }

  void produce(edm::Event& iEvent, const edm::EventSetup& iSetup) override;

private:
  const std::string m_name;
  const std::string m_doc;
  bool m_extension;

  const edm::EDGetTokenT<edm::View<reco::Track>> m_tracks_token;
  const edm::EDGetTokenT<TrajTrackAssociationCollection> m_association_token;

  std::map<DetId, double> m_thicknessMap;
  double thickness(DetId id, const TrackerGeometry* tGeom);
};

namespace {
  template <typename VALUES>
  void addColumn(nanoaod::FlatTable* table, const std::string& name, VALUES&& values, const std::string& doc) {
    using value_type = typename std::remove_reference<VALUES>::type::value_type;
    table->template addColumn<value_type>(name, values, doc);
  }

  int findTrackIndex(const edm::View<reco::Track>& tracks, const reco::Track* track) {
    for (auto iTr = tracks.begin(); iTr != tracks.end(); ++iTr) {
      if (&(*iTr) == track) {
        return iTr - tracks.begin();
      }
    }
    return -2;
  }

  struct HitCluster {
    uint32_t det;
    const SiStripCluster* strip;
    const SiPixelCluster* pixel;
    HitCluster(uint32_t detId, const SiStripCluster* strip, const SiPixelCluster* pixel)
        : det(detId), strip(strip), pixel(pixel) {}
  };
  std::vector<HitCluster> getClusters(const TrackingRecHit* hit) {
    const auto simple1d = dynamic_cast<const SiStripRecHit1D*>(hit);
    const auto simple = dynamic_cast<const SiStripRecHit2D*>(hit);
    const auto matched = dynamic_cast<const SiStripMatchedRecHit2D*>(hit);
    const auto pixel = dynamic_cast<const SiPixelRecHit*>(hit);
    std::vector<HitCluster> clusters;
    if (matched) {
      clusters.emplace_back(matched->monoId(), &matched->monoCluster(), nullptr);
      clusters.emplace_back(matched->stereoId(), &matched->stereoCluster(), nullptr);
    } else if (simple) {
      clusters.emplace_back(simple->geographicalId().rawId(), simple->cluster().get(), nullptr);
    } else if (simple1d) {
      clusters.emplace_back(simple1d->geographicalId().rawId(), simple1d->cluster().get(), nullptr);
    } else if (pixel) {
      clusters.emplace_back(pixel->geographicalId().rawId(), nullptr, pixel->cluster().get());
    }
    return clusters;
  }

  bool isFarFromBorder(const TrajectoryStateOnSurface& trajState, uint32_t detId, const TrackerGeometry* tGeom) {
    const auto gdu = tGeom->idToDetUnit(detId);
    if ((!dynamic_cast<const StripGeomDetUnit*>(gdu)) && (!dynamic_cast<const PixelGeomDetUnit*>(gdu))) {
      edm::LogWarning("SiStripGainCalibTableProducer")
          << "DetId " << detId << " does not seem to belong to the tracker";
      return false;
    }
    const auto plane = gdu->surface();
    const auto trapBounds = dynamic_cast<const TrapezoidalPlaneBounds*>(&plane.bounds());
    const auto rectBounds = dynamic_cast<const RectangularPlaneBounds*>(&plane.bounds());

    static constexpr double distFromBorder = 1.0;
    double halfLength = 0.;
    if (trapBounds) {
      halfLength = trapBounds->parameters()[3];
    } else if (rectBounds) {
      halfLength = .5 * gdu->surface().bounds().length();
    } else {
      return false;
    }

    const auto pos = trajState.localPosition();
    const auto posError = trajState.localError().positionError();
    if (std::abs(pos.y()) + posError.yy() >= (halfLength - distFromBorder))
      return false;

    return true;
  }
}  // namespace

double SiStripGainCalibTableProducer::thickness(DetId id, const TrackerGeometry* tGeom) {
  const auto it = m_thicknessMap.find(id);
  if (m_thicknessMap.end() != it) {
    return it->second;
  } else {
    double detThickness = 1.;
    const auto gdu = tGeom->idToDetUnit(id);
    const auto isPixel = (dynamic_cast<const PixelGeomDetUnit*>(gdu) != nullptr);
    const auto isStrip = (dynamic_cast<const StripGeomDetUnit*>(gdu) != nullptr);
    if (!isPixel && !isStrip) {
      edm::LogWarning("SiStripGainCalibTableProducer")
          << "DetId " << id.rawId() << " doesn't seem to belong to the Tracker";
    } else {
      detThickness = gdu->surface().bounds().thickness();
    }
    m_thicknessMap[id] = detThickness;
    return detThickness;
  }
}

void SiStripGainCalibTableProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::ESHandle<TrackerGeometry> tGeom;
  iSetup.get<TrackerDigiGeometryRecord>().get(tGeom);
  edm::ESHandle<SiStripGain> stripGains;
  iSetup.get<SiStripGainRcd>().get(stripGains);

  edm::Handle<edm::View<reco::Track>> tracks;
  iEvent.getByToken(m_tracks_token, tracks);
  edm::Handle<TrajTrackAssociationCollection> trajTrackAssociations;
  iEvent.getByToken(m_association_token, trajTrackAssociations);

  // columns
  std::vector<int> c_trackindex;
  std::vector<uint32_t> c_rawid;
  std::vector<double> c_localdirx;
  std::vector<double> c_localdiry;
  std::vector<double> c_localdirz;
  std::vector<uint16_t> c_firststrip;
  std::vector<uint16_t> c_nstrips;
  std::vector<bool> c_saturation;
  std::vector<bool> c_overlapping;
  std::vector<bool> c_farfromedge;
  std::vector<int> c_charge;
  std::vector<double> c_path;
  // NOTE only very few types are supported by NanoAOD, but more could be added (to discuss with XPOG / core software)
  // NOTE removed amplitude vector, I don't think it was used anywhere
  std::vector<float> c_gainused;      // NOTE was double
  std::vector<float> c_gainusedTick;  // NOTE was double

  for (const auto& assoc : *trajTrackAssociations) {
    const auto traj = assoc.key.get();
    const auto track = assoc.val.get();

    for (const auto& meas : traj->measurements()) {
      const auto& trajState = meas.updatedState();
      if (!trajState.isValid())
        continue;

      // there can be 2 (stereo module), 1 (no stereo module), or 0 (no pixel or strip hit) clusters
      for (const auto hitCluster : getClusters(meas.recHit()->hit())) {
        bool saturation = false;
        bool overlapping = false;
        unsigned int charge = 0;
        double prevGain = -1;
        double prevGainTick = -1;
        int firstStrip = 0;
        int nStrips = 0;
        if (hitCluster.strip) {
          const auto& ampls = hitCluster.strip->amplitudes();
          firstStrip = hitCluster.strip->firstStrip();
          nStrips = ampls.size();
          if (stripGains.isValid()) {
            prevGain = stripGains->getApvGain(firstStrip / 128, stripGains->getRange(hitCluster.det, 1), 1);
            prevGainTick = stripGains->getApvGain(firstStrip / 128, stripGains->getRange(hitCluster.det, 0), 1);
          }
          charge = hitCluster.strip->charge();
          saturation = std::any_of(ampls.begin(), ampls.end(), [](uint8_t amp) { return amp >= 254; });

          overlapping = (((firstStrip % 128) == 0) || ((firstStrip / 128) != ((firstStrip + nStrips) / 128)) ||
                         (((firstStrip + nStrips) % 128) == 127));
        } else if (hitCluster.pixel) {
          const auto& ampls = hitCluster.pixel->pixelADC();
          const int firstRow = hitCluster.pixel->minPixelRow();
          const int firstCol = hitCluster.pixel->minPixelCol();
          firstStrip = ((firstRow / 80) << 3 | (firstCol / 52)) * 128;  //Hack to save the APVId
          nStrips = 0;
          for (const auto amp : ampls) {
            charge += amp;
            if (amp >= 254)
              saturation = true;
          }
        }
        const auto trackDir = trajState.localDirection();
        const auto cosine = trackDir.z() / trackDir.mag();
        const auto path = (10. * thickness(hitCluster.det, tGeom.product())) / std::abs(cosine);
        const auto farFromEdge =
            (hitCluster.strip ? isFarFromBorder(trajState, hitCluster.det, tGeom.product()) : true);
        c_trackindex.push_back(findTrackIndex(*tracks, track));
        c_rawid.push_back(hitCluster.det);
        c_localdirx.push_back(trackDir.x());
        c_localdiry.push_back(trackDir.y());
        c_localdirz.push_back(trackDir.z());
        c_firststrip.push_back(firstStrip);
        c_nstrips.push_back(nStrips);
        c_saturation.push_back(saturation);
        c_overlapping.push_back(overlapping);
        c_farfromedge.push_back(farFromEdge);
        c_charge.push_back(charge);
        c_path.push_back(path);
        c_gainused.push_back(prevGain);
        c_gainusedTick.push_back(prevGainTick);
      }
    }
  }

  auto out = std::make_unique<nanoaod::FlatTable>(c_trackindex.size(), m_name, false, m_extension);
  addColumn(out.get(), "trackindex", c_trackindex, "<doc>");
  addColumn(out.get(), "rawid", c_rawid, "<doc>");
  // addColumn(out.get(), "localdirx", c_localdirx, "<doc>");
  // addColumn(out.get(), "localdiry", c_localdiry, "<doc>");
  // addColumn(out.get(), "localdirz", c_localdirz, "<doc>");
  // addColumn(out.get(), "firststrip", c_firststrip, "<doc>");
  // addColumn(out.get(), "nstrips", c_nstrips, "<doc>");
  addColumn(out.get(), "saturation", c_saturation, "<doc>");
  addColumn(out.get(), "overlapping", c_overlapping, "<doc>");
  addColumn(out.get(), "farfromedge", c_farfromedge, "<doc>");
  addColumn(out.get(), "charge", c_charge, "<doc>");
  // addColumn(out.get(), "path", c_path, "<doc>");
  // ExtendedCalibTree: also charge/path
  addColumn(out.get(), "gainused", c_gainused, "<doc>");
  addColumn(out.get(), "gainusedTick", c_gainusedTick, "<doc>");

  iEvent.put(std::move(out));
}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SiStripGainCalibTableProducer);
