#include "CondFormats/SiStripObjects/interface/SiStripApvSimulationParameters.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CLHEP/Random/RandFlat.h"

namespace {
  PhysicsTools::Calibration::HistogramF2D calculateZInt(const SiStripApvSimulationParameters::LayerParameters& params) {
    auto hZInt = PhysicsTools::Calibration::HistogramF2D(params.upperLimitsX(), params.upperLimitsY());
    for (int i{0}; i != params.numberOfBinsX() + 2; ++i) {
      for (int j{0}; j != params.numberOfBinsY() + 2; ++j) {
        float zInt = 0.;
        for (int k{0}; k != params.numberOfBinsZ() + 2; ++k) {
          zInt += params.binContent(i, j, k);
        }
        hZInt.setBinContent(i, j, zInt);
      }
    }
    return hZInt;
  }
}  // namespace

void SiStripApvSimulationParameters::calculateIntegrals() {
  if (m_barrelParam.size() != m_barrelParam_zInt.size()) {
    m_barrelParam_zInt.resize(m_barrelParam.size());
    for (unsigned int i{0}; i != m_barrelParam.size(); ++i) {
      m_barrelParam_zInt[i] = calculateZInt(m_barrelParam[i]);
    }
  }
}

bool SiStripApvSimulationParameters::putTIB(SiStripApvSimulationParameters::layerid layer,
                                            SiStripApvSimulationParameters::LayerParameters&& params) {
  if ((layer > m_nTIB) || (layer < 1)) {
    edm::LogError("SiStripApvSimulationParameters")
        << "[" << __PRETTY_FUNCTION__ << "] layer index " << layer << " out of range [1," << m_nTIB << "]";
    return false;
  }
  m_barrelParam[layer - 1] = params;
  m_barrelParam_zInt[layer - 1] = calculateZInt(params);
  return true;
}

bool SiStripApvSimulationParameters::putTOB(SiStripApvSimulationParameters::layerid layer,
                                            SiStripApvSimulationParameters::LayerParameters&& params) {
  if ((layer > m_nTOB) || (layer < 1)) {
    edm::LogError("SiStripApvSimulationParameters") << "[" << __PRETTY_FUNCTION__ << "] layer index " << layer
                                                    << " out of range [1," << m_nTOB << ")";
    return false;
  }
  m_barrelParam[m_nTIB + layer - 1] = params;
  m_barrelParam_zInt[m_nTIB + layer - 1] = calculateZInt(params);
  return true;
}

float SiStripApvSimulationParameters::sampleBarrel(layerid layerIdx,
                                                   float z,
                                                   float pu,
                                                   CLHEP::HepRandomEngine* engine) const {
  if (m_barrelParam.size() != m_barrelParam_zInt.size()) {
    throw cms::Exception("LogicError") << "z-integrals of 3D histograms have not been calculated";
  }
  const auto layerParam = m_barrelParam[layerIdx];
  const int iz = layerParam.findBinX(z);
  const int ip = layerParam.findBinY(pu);
  const float norm = m_barrelParam_zInt[layerIdx].binContent(iz, ip);
  const auto val = CLHEP::RandFlat::shoot(engine) * norm;
  if (val < layerParam.binContent(iz, ip, 0)) {  // underflow
    return layerParam.upperLimitsZ()[0];
  } else if (norm - val < layerParam.binContent(iz, ip, layerParam.numberOfBinsZ() + 1)) {  // overflow
    return layerParam.upperLimitsZ()[layerParam.numberOfBinsZ()];
  } else {  // loop over bins, return center of our bin
    float sum = layerParam.binContent(iz, ip, 0);
    for (int i{1}; i != layerParam.numberOfBinsZ() + 1; ++i) {
      sum += layerParam.binContent(iz, ip, i);
      if (sum > val) {
        return .5 * (layerParam.upperLimitsZ()[i - 1] + layerParam.upperLimitsZ()[i]);
      }
    }
  }
  throw cms::Exception("LogicError") << "Problem drawing a random number from the distribution";
}
