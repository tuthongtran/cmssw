#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/DataRecord/interface/SiStripCondDataRecords.h"
#include "CondFormats/SiStripObjects/interface/SiStripApvSimulationParameters.h"
#include <fstream>
#include <boost/range/adaptor/indexed.hpp>

class SiStripApvSimulationParametersESProducer : public edm::ESProducer {
public:
  explicit SiStripApvSimulationParametersESProducer(const edm::ParameterSet& conf);
  ~SiStripApvSimulationParametersESProducer() override {}

  std::unique_ptr<SiStripApvSimulationParameters> produce(const SiStripApvSimulationParametersRcd& record);

private:
  std::vector<edm::FileInPath> baselineFiles_TOB_;
  std::vector<edm::FileInPath> baselineFiles_TIB_;
  unsigned int baseline_nBins_;
  float baseline_min_;
  float baseline_max_;
  std::vector<float> puBinEdges_;
  std::vector<float> zBinEdges_;

  SiStripApvSimulationParameters::LayerParameters makeLayerParameters(const std::string& apvBaselinesFileName) const;
};

SiStripApvSimulationParametersESProducer::SiStripApvSimulationParametersESProducer(const edm::ParameterSet& conf)
    : baseline_nBins_(conf.getParameter<unsigned int>("apvBaselines_nBinsPerBaseline")),
      baseline_min_(conf.getParameter<double>("apvBaselines_minBaseline")),
      baseline_max_(conf.getParameter<double>("apvBaselines_maxBaseline")) {
  setWhatProduced(this);
  for (const auto x : conf.getParameter<std::vector<double>>("apvBaselines_puBinEdges")) {
    puBinEdges_.push_back(x);
  }
  for (const auto x : conf.getParameter<std::vector<double>>("apvBaselines_zBinEdges")) {
    zBinEdges_.push_back(x);
  }
  baselineFiles_TIB_ = {conf.getParameter<edm::FileInPath>("apvBaselinesFile_tib1"),
                        conf.getParameter<edm::FileInPath>("apvBaselinesFile_tib2"),
                        conf.getParameter<edm::FileInPath>("apvBaselinesFile_tib3"),
                        conf.getParameter<edm::FileInPath>("apvBaselinesFile_tib4")};
  baselineFiles_TOB_ = {conf.getParameter<edm::FileInPath>("apvBaselinesFile_tob1"),
                        conf.getParameter<edm::FileInPath>("apvBaselinesFile_tob2"),
                        conf.getParameter<edm::FileInPath>("apvBaselinesFile_tob3"),
                        conf.getParameter<edm::FileInPath>("apvBaselinesFile_tob4"),
                        conf.getParameter<edm::FileInPath>("apvBaselinesFile_tob5"),
                        conf.getParameter<edm::FileInPath>("apvBaselinesFile_tob6")};
}

SiStripApvSimulationParameters::LayerParameters SiStripApvSimulationParametersESProducer::makeLayerParameters(
    const std::string& apvBaselinesFileName) const {
  // Prepare histograms
  unsigned int nZBins = zBinEdges_.size();
  unsigned int nPUBins = puBinEdges_.size();

  if (nPUBins == 0 || nZBins == 0 || baseline_nBins_ == 0) {
    throw cms::Exception("MissingInput") << "The parameters for the APV simulation are not correctly configured\n";
  }
  std::vector<float> baselineBinEdges{};
  auto baseline_binWidth = (baseline_max_ - baseline_min_) / baseline_nBins_;
  for (unsigned i{0}; i != baseline_nBins_; ++i) {
    baselineBinEdges.push_back(baseline_min_ + i * baseline_binWidth);
  }
  baselineBinEdges.push_back(baseline_max_);

  SiStripApvSimulationParameters::LayerParameters layerParams{zBinEdges_, puBinEdges_, baselineBinEdges};

  // Read apv baselines from text files
  std::vector<double> theAPVBaselines;
  std::ifstream apvBaselineFile(apvBaselinesFileName.c_str());
  if (!apvBaselineFile.good()) {
    throw cms::Exception("FileError") << "Problem opening APV baselines file: " << apvBaselinesFileName;
  }
  std::string line;
  while (std::getline(apvBaselineFile, line)) {
    if (!line.empty()) {
      std::istringstream lStr{line};
      double value;
      while (lStr >> value) {
        theAPVBaselines.push_back(value);
      }
    }
  }
  if (theAPVBaselines.empty()) {
    throw cms::Exception("WrongAPVBaselines")
        << "Problem reading from APV baselines file " << apvBaselinesFileName << ": no values read in";
  }

  if (theAPVBaselines.size() != nZBins * nPUBins * baseline_nBins_) {
    throw cms::Exception("WrongAPVBaselines") << "Problem reading from APV baselines file " << apvBaselinesFileName
                                              << ": number of baselines read different to that expected i.e. nZBins * "
                                                 "nPUBins * apvBaselines_nBinsPerBaseline_";
  }

  // Put baselines into histograms
  for (auto const& apvBaseline : theAPVBaselines | boost::adaptors::indexed(0)) {
    unsigned int binInCurrentHistogram = apvBaseline.index() % baseline_nBins_ + 1;
    unsigned int binInZ = int(apvBaseline.index()) / (nPUBins * baseline_nBins_) + 1;
    unsigned int binInPU = int(apvBaseline.index() - binInZ * (nPUBins)*baseline_nBins_) / baseline_nBins_ + 1;

    layerParams.setBinContent(binInZ, binInPU, binInCurrentHistogram, apvBaseline.value());
  }

  return layerParams;
}

std::unique_ptr<SiStripApvSimulationParameters> SiStripApvSimulationParametersESProducer::produce(
    const SiStripApvSimulationParametersRcd& record) {
  auto apvSimParams =
      std::make_unique<SiStripApvSimulationParameters>(baselineFiles_TIB_.size(), baselineFiles_TOB_.size());
  for (unsigned int i{0}; i != baselineFiles_TIB_.size(); ++i) {
    if ( ! apvSimParams->putTIB(i + 1, makeLayerParameters(baselineFiles_TIB_[i].fullPath())) ) {
      throw cms::Exception("SiStripApvSimulationParameters") << "Could not add parameters for TIB layer " << (i+1);
    } else {
      LogDebug("SiStripApvSimulationParameters") << "Added parameters for TIB layer " << (i+1);
    }
  }
  for (unsigned int i{0}; i != baselineFiles_TOB_.size(); ++i) {
    if ( ! apvSimParams->putTOB(i + 1, makeLayerParameters(baselineFiles_TOB_[i].fullPath())) ) {
      throw cms::Exception("SiStripApvSimulationParameters") << "Could not add parameters for TOB layer " << (i+1);
    } else {
      LogDebug("SiStripApvSimulationParameters") << "Added parameters for TOB layer " << (i+1);
    }
  }
  return apvSimParams;
}

#include "FWCore/Framework/interface/ModuleFactory.h"
DEFINE_FWK_EVENTSETUP_MODULE(SiStripApvSimulationParametersESProducer);
