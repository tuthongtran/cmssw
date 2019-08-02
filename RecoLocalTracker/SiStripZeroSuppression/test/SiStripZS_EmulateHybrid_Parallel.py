import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('CHECK',eras.Run2_HI)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
###process.load('Configuration.StandardSequences.DigiToRaw_Repack_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound'),
            
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1) 
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/f/fbury/work/HybridStudy/SpyRawToDigis321779.root'),
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step1 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RAWoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('whatever.root'),
    outputCommands = process.RAWEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

from EventFilter.RawDataCollector.rawDataCollectorByLabel_cfi import rawDataCollector
process.load("RecoLocalTracker.SiStripZeroSuppression.SiStripZeroSuppression_cfi")
from RecoLocalTracker.SiStripZeroSuppression.SiStripZeroSuppression_cfi import siStripZeroSuppression
process.load("EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi")
from EventFilter.SiStripRawToDigi.SiStripDigiToRaw_cfi import SiStripDigiToRaw

##
## WF 1: emulate hybrid, repack, unpack, zero-suppress, repack
##
inputVR = cms.InputTag("siStripDigis", "VirginRaw")
# step1
process.zsHybridEmu = process.siStripZeroSuppression.clone(  # Raw -> ZS in hybrid -> Digis
    produceRawDigis=False,
    produceHybridFormat=True,
    Algorithms=process.siStripZeroSuppression.Algorithms.clone(
        APVInspectMode = "HybridEmulation",
        APVRestoreMode = "",
        CommonModeNoiseSubtractionMode = 'Median',
        ##CutToAvoidSignal    = 1., ## 2./2
        #MeanCM = 0,
        Use10bitsTruncation = True
        ),
    RawDigiProducersList=cms.VInputTag(inputVR)
    )
process.zsHybrid = process.siStripZeroSuppression.clone(      # Full software (with inspect and restore) ZS -> digis
    RawDigiProducersList = cms.VInputTag(
        cms.InputTag("zsHybridEmu", "VirginRaw"),
        cms.InputTag("zsHybridEmu", "ProcessedRaw"),
        cms.InputTag("zsHybridEmu", "ScopeMode"),
        cms.InputTag("zsHybridEmu", "ZeroSuppressed")
        ),
    Algorithms=process.siStripZeroSuppression.Algorithms.clone(
        APVInspectMode = "Hybrid",
        ),
    )
##
## WF 2: zero-suppress, repack
##
# ZS:       process.siStripZeroSuppression 
process.zsClassic = process.siStripZeroSuppression.clone(      # Without hybrid
        RawDigiProducersList=cms.VInputTag(inputVR),
        Algorithms=process.siStripZeroSuppression.Algorithms.clone(
        APVInspectMode = "Null",
        APVRestoreMode = "",
        CommonModeNoiseSubtractionMode = 'Median',
        ),
    )


##
## Modify some settings for consistenc between the two
##
process.siStripZeroSuppression.Algorithms = process.siStripZeroSuppression.Algorithms.clone(
        CommonModeNoiseSubtractionMode = 'Median',
        )

## unpack both

process.diffRawZS = cms.EDAnalyzer("SiStripDigiDiff",
        A = cms.InputTag("zsHybrid", "ZeroSuppressed"),
        B = cms.InputTag("zsClassic", "ZeroSuppressed"),
        nDiffToPrint=cms.untracked.uint64(100),
        IgnoreAllZeros=cms.bool(True), ## workaround for packer removing all zero strips for ZS
        TopBitsToIgnore = cms.uint32(0),
        BottomBitsToIgnore = cms.uint32(1),
        )
process.digiStatDiff = cms.EDAnalyzer("SiStripDigiStatsDiff",
        A = cms.InputTag("zsHybrid", "ZeroSuppressed"),
        B = cms.InputTag("zsClassic", "ZeroSuppressed"),
        )
process.load("RecoLocalTracker.SiStripClusterizer.SiStripClusterizer_RealData_cfi")
process.clusterizeZS1 = process.siStripClusters.clone(DigiProducersList=cms.VInputTag(cms.InputTag("zsHybrid", "ZeroSuppressed")))
process.clusterizeZS2 = process.siStripClusters.clone(DigiProducersList=cms.VInputTag(cms.InputTag("zsClassic", "ZeroSuppressed")))
process.clusterStatDiff = cms.EDAnalyzer("SiStripClusterStatsDiff",
        A = cms.InputTag("clusterizeZS2"),
        B = cms.InputTag("clusterizeZS1"),
        )
process.DigiToRawZS = cms.Sequence(
        process.siStripZeroSuppression * process.zsHybridEmu *  process.zsHybrid * process.zsClassic *
        # Analyze #
        process.diffRawZS * process.digiStatDiff * process.clusterizeZS1 * process.clusterizeZS2 * process.clusterStatDiff
        )
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("diffhistos.root"),
        closeFileFast = cms.untracked.bool(True),
        )

del process.siStripQualityESProducer.ListOfRecordToMerge[0] # Because spy data is outside or a run

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.DigiToRawZS = cms.Path(process.DigiToRawZS)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWoutput_step = cms.EndPath(process.RAWoutput)

# Schedule definition
#process.schedule = cms.Schedule(process.raw2digi_step,process.DigiToRawZS,process.endjob_step,process.RAWoutput_step)
process.schedule = cms.Schedule(process.DigiToRawZS,process.endjob_step,process.RAWoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

#process.MessageLogger = cms.Service(
#    "MessageLogger",
#    destinations = cms.untracked.vstring(
#        "log_checkhybrid"
#        ),
#    debugModules = cms.untracked.vstring("diffRawZS", "zsHybridEmu", "zsHybrid", "zsClassic", "siStripZeroSuppression"),
#    categories=cms.untracked.vstring("SiStripZeroSuppression", "SiStripDigiDiff")
#    )
#
