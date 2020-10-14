# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: test1 --data -s DQM:siStripFEDMonitor --filein=file:/eos/cms/store/express/Commissioning2020/ExpressCosmics/FEVT/Express-v1/000/337/133/00000/FAF613FD-D1AF-4946-9005-B354B3497F6E.root --conditions 111X_dataRun3_HLT_v2 --scenario cosmics --eventcontent DQM --datatier DQM
import FWCore.ParameterSet.Config as cms



process = cms.Process('DQM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentCosmics_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('DQMServices.Core.DQMStoreNonLegacy_cff')
process.load('DQMOffline.Configuration.DQMOfflineCosmics_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/data/Commissioning2020/Cosmics/RAW/v1/000/337/133/00000/83B34E88-53B7-944D-AB30-819E58667B22.root',
        '/store/data/Commissioning2020/Cosmics/RAW/v1/000/337/133/00000/7ABC333B-C678-5144-840C-AF95D4794066.root',
        '/store/data/Commissioning2020/Cosmics/RAW/v1/000/337/133/00000/ED1969F4-7C3E-6443-8746-4182987D3643.root',
        '/store/data/Commissioning2020/Cosmics/RAW/v1/000/337/133/00000/58916BB4-B194-1041-87B4-11CF02BB09EB.root',
        '/store/data/Commissioning2020/Cosmics/RAW/v1/000/337/133/00000/0798C56E-A8F0-C847-A2BB-965EFD8CF54E.root',
        '/store/data/Commissioning2020/Cosmics/RAW/v1/000/337/133/00000/403695B1-78F1-9B4E-B890-2E07DBBA1C6C.root',
        '/store/data/Commissioning2020/Cosmics/RAW/v1/000/337/133/00000/48B404C6-381E-C949-A184-212B5A0E4256.root',
        '/store/data/Commissioning2020/Cosmics/RAW/v1/000/337/133/00000/B7976798-9169-8E46-95EC-F122D0697273.root'
        ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(

        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(1)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(1),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('test1 nevts:1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

#process.DQMoutput = cms.OutputModule("PoolOutputModule",
#    dataset = cms.untracked.PSet(
#        dataTier = cms.untracked.string('DQM'),
#        filterName = cms.untracked.string('')
#    ),
#    fileName = cms.untracked.string('test1_DQM.root'),
#    outputCommands = process.DQMEventContent.outputCommands,
#    splitLevel = cms.untracked.int32(0)
#)

##process.load("DQM.SiStripMonitorClient.SiStripDQMOnline_cff")

process.load("DQM.Integration.config.environment_cfi")
process.dqmEnv.subSystemFolder    = "SiStrip"
process.dqmSaver.tag = "SiStrip"
process.dqmSaver.backupLumiCount = 30

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '111X_dataRun3_HLT_v2', '')

# Path and EndPath definitions
process.pL1TkElectronsEllipticMatchHGC = cms.Path(process.L1TkElectronsEllipticMatchHGC)
process.pL1TkIsoElectronsHGC = cms.Path(process.L1TkIsoElectronsHGC)
process.pL1TkElectronsEllipticMatchCrystal = cms.Path(process.L1TkElectronsEllipticMatchCrystal)
process.pL1TkElectronsCrystal = cms.Path(process.L1TkElectronsCrystal)
process.pL1TkIsoElectronsCrystal = cms.Path(process.L1TkIsoElectronsCrystal)
process.pL1TkMuon = cms.Path(process.L1TkMuons+process.L1TkMuonsTP)
process.pL1TkPhotonsHGC = cms.Path(process.L1TkPhotonsHGC)
process.pL1TkElectronsLooseCrystal = cms.Path(process.L1TkElectronsLooseCrystal)
process.pL1TkPrimaryVertex = cms.Path(process.L1TkPrimaryVertex)
process.pL1TkElectronsLooseHGC = cms.Path(process.L1TkElectronsLooseHGC)
process.pL1TkElectronsHGC = cms.Path(process.L1TkElectronsHGC)
process.pL1TkPhotonsCrystal = cms.Path(process.L1TkPhotonsCrystal)
process.dqmoffline_step = cms.EndPath(process.siStripFEDMonitor*process.dqmEnv*process.dqmSaver)
process.endjob_step = cms.EndPath(process.endOfProcess)
#process.DQMoutput_step = cms.EndPath(process.DQMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.dqmoffline_step,process.endjob_step)#,process.DQMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


# Customisation from command line

#process.siStripFEDMonitor.PrintDebugMessages = 5

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

process.MessageLogger = cms.Service(
    "MessageLogger",
    destinations = cms.untracked.vstring(
        "log_federrors_1014"
        ),
    log_federrors_1014 = cms.untracked.PSet(
        #threshold = cms.untracked.string("DEBUG"),
        threshold = cms.untracked.string("WARNING"),
        default = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
        )
    ),
    #debugModules = cms.untracked.vstring("siStripFEDMonitor"),
    #categories=cms.untracked.vstring("FEDErrors", "SiStripMonitorHardware)
)
