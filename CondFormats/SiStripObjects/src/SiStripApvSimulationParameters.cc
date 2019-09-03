#include "CondFormats/SiStripObjects/interface/SiStripApvSimulationParameters.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CLHEP/Random/RandFlat.h"

bool SiStripApvSimulationParameters::putTIB(SiStripApvSimulationParameters::layerid layer, LayerParameters&& params) {
  if ( ( layer > m_nTIB ) || ( layer < 1 ) ) {
    edm::LogError("SiStripApvSimulationParameters") << "[" << __PRETTY_FUNCTION__ << "] layer index " << layer << " out of range [1," << m_nTIB << "]";
    return false;
  }
  m_barrelParam[layer-1] = params;
  return true;
}

bool SiStripApvSimulationParameters::putTOB(SiStripApvSimulationParameters::layerid layer, LayerParameters&& params) {
  if ( ( layer > m_nTIB+m_nTOB ) || ( layer < m_nTIB+1 ) ) {
    edm::LogError("SiStripApvSimulationParameters") << "[" << __PRETTY_FUNCTION__ << "] layer index " << layer << " out of range [" << m_nTIB+1 << "," << m_nTOB << ")";
    return false;
  }
  m_barrelParam[m_nTIB+layer-1] = params;
  return true;
}

float SiStripApvSimulationParameters::sampleBarrel(layerid layerIdx, float z, float pu, CLHEP::HepRandomEngine* engine) const {
  const auto layerParam = m_barrelParam[layerIdx];
  const int iz = layerParam.findBinX(z);
  const int ip = layerParam.findBinX(pu);
  float norm = 0.;
  for ( int i{0}; i != layerParam.numberOfBinsZ()+2; ++i ) {
    norm += layerParam.binContent(iz, ip, i);
  }
  auto val = CLHEP::RandFlat::shoot(engine)*norm;
  if ( val < layerParam.binContent(iz, ip, 0) ) { // underflow
    return layerParam.upperLimitsZ()[0];
  } else if ( norm-val < layerParam.binContent(iz, ip, layerParam.numberOfBinsZ()+1) ) { // overflow
    return layerParam.upperLimitsZ()[layerParam.numberOfBinsZ()];
  } else { // loop over bins, return center of our bin
    float sum = layerParam.binContent(iz, ip, 0);
    for ( int i{1}; i != layerParam.numberOfBinsZ()+1; ++i ) {
      sum += layerParam.binContent(iz, ip, i);
      if ( sum > val ) {
        return .5*(layerParam.upperLimitsZ()[i-1]+layerParam.upperLimitsZ()[i]);
      }
    }
  }
  throw cms::Exception("LogicError") << "Problem drawing a random number from the distribution";
}
