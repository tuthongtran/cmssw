import FWCore.ParameterSet.Config as cms

process = cms.Process('TEST1')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')

process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('hiRecoDM nevts:2'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('/store/data/Run2016B/VRZeroBias0/RAW/v2/000/273/162/00000/FC5C4807-0718-E611-BAB7-02163E01436A.root'),
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/f/fbury/work/HybridStudy/SpyRawToDigis321054.root'),
    #secondaryFileNames = cms.untracked.vstring(),
)

#process.source = cms.Source("NewEventStreamFileReader",
#fileNames = cms.untracked.vstring('root://eoscms.cern.ch//store/t0streamer/Data/HIPhysicsVirginRaw/000/262/296/run262296_ls0223_streamHIPhysicsVirginRaw_StorageManager.dat'
#)
#)

# Output definition
process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    fileName = cms.untracked.string('/afs/cern.ch/user/f/fbury/work/HybridStudy/RECO321054.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
   ),
   
    outputCommands = cms.untracked.vstring('keep FEDRawDataCollection_myRawDataCollector_*_*')
    
)



process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = '75X_dataRun2_HLTHI_v4'

## Offline Silicon Tracker Zero Suppression
from RecoLocalTracker.SiStripZeroSuppression.DefaultAlgorithms_cff import *
process.siStripZeroSuppression.produceRawDigis = False
process.siStripZeroSuppression.produceCalculatedBaseline = False
process.siStripZeroSuppression.produceBaselinePoints = False
process.siStripZeroSuppression.storeCM = True
process.siStripZeroSuppression.produceHybridFormat = True
#process.siStripZeroSuppression.Algorithms.APVInspectMode = "HybridEmulation"
process.siStripZeroSuppression.Algorithms.APVInspectMode = "Hybrid"
#process.siStripZeroSuppression.Algorithms.APVRestoreMode = "Flat"
process.siStripZeroSuppression.Algorithms.CommonModeNoiseSubtractionMode = 'Median'
process.siStripZeroSuppression.Algorithms.MeanCM = 512
process.siStripZeroSuppression.Algorithms.DeltaCMThreshold = 20
process.siStripZeroSuppression.Algorithms.Use10bitsTruncation = True
#process.siStripZeroSuppression.Algorithms.doAPVRestore = False
#process.siStripZeroSuppression.Algorithms.ForceNoRestore = True


process.TFileService = cms.Service("TFileService",
        fileName=cms.string("/afs/cern.ch/user/f/fbury/work/HybridStudy/Baselines321054.root"))

process.hybridAna = cms.EDAnalyzer("SiStripHybridFormatAnalyzer",

    srcDigis =  cms.InputTag('siStripZeroSuppression','VirginRaw'),
    srcAPVCM =  cms.InputTag('siStripZeroSuppression','APVCMVirginRaw'),
    nModuletoDisplay = cms.uint32(10000),
    plotAPVCM	= cms.bool(True)
)

process.siStripDigiToRawData = cms.EDProducer(
    "SiStripDigiToRawModule",
    InputModuleLabel = cms.string('siStripZeroSuppression'),
    InputDigiLabel = cms.string('VirginRaw'),
    FedReadoutMode = cms.string('ZERO_SUPPRESSED'),
    UseFedKey = cms.bool(False),
    UseWrongDigiType = cms.bool(False),
    CopyBufferHeader = cms.bool(False),
    RawDataTag = cms.InputTag('rawDataCollector')
)

process.myRawDataCollector = cms.EDProducer("RawDataCollectorByLabel",
    verbose = cms.untracked.int32(0),     # 0 = quiet, 1 = collection list, 2 = FED list
    RawCollectionList = cms.VInputTag( #cms.InputTag('hcalRawData'),
                                       cms.InputTag('siStripDigiToRawData'),
                   #                     cms.InputTag('rawDataRepacker')
                                       cms.InputTag('rawDataCollector')
                   #                    cms.InputTag('siPixelRawData'),
    ),
)
								  
# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.siStripDigis)
#process.reconstruction_step = cms.Path(process.striptrackerlocalreco+process.moddedZS+process.moddedClust+process.baselineAna+process.moddedbaselineAna+process.clusterMatching)
process.reconstruction_step = cms.Path(process.striptrackerlocalreco*process.hybridAna*process.siStripDigiToRawData*process.myRawDataCollector)
#process.reconstruction_step = cms.Path(process.striptrackerlocalreco);
#process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)


# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step, process.RECOoutput_step)
