from __future__ import print_function
import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("PedestalsDiff")

options = VarParsing.VarParsing("analysis")
options.register('globalTag', "auto:run2_data", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "GlobalTag")
options.register('runNumber', 306054, VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.int, "run number")
options.register('localDB', "dbfile.db", VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "Local payload sqlite file")
options.parseArguments()

## MessageLogger
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.categories.append("SiStripComparePedestals")
process.MessageLogger.destinations = cms.untracked.vstring("cout")
process.MessageLogger.cout = cms.untracked.PSet(
    threshold = cms.untracked.string("INFO"),
    default = cms.untracked.PSet(limit=cms.untracked.int32(0)),
    FwkReport = cms.untracked.PSet(limit=cms.untracked.int32(-1), reportEvery=cms.untracked.int32(1000)),
    SiStripComparePedestals=cms.untracked.PSet(limit=cms.untracked.int32(-1))
    )
process.MessageLogger.statistics.append('cout')

## ideal geometry, conditions DB
process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag,options.globalTag, '')
print("Using Global Tag:", process.GlobalTag.globaltag._value)

process.source = cms.Source("EmptySource", firstRun=cms.untracked.uint32(options.runNumber), numberEventsInRun=cms.untracked.uint32(1))
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(1))

from CondCore.CondDB.CondDB_cfi import CondDB
process.shiftedPedestals = cms.ESSource("PoolDBESSource",
        CondDB.clone(connect=cms.string("sqlite_file:{0}".format(options.localDB))),
        toGet = cms.VPSet(cms.PSet(
            record=cms.string("SiStripPedestalsRcd"),
            tag=cms.string("modifiedPedestals"),
            label=cms.untracked.string("modifiedPedestals"))
            ))
#process.es_prefer_sistrippedestals = cms.ESPrefer("PoolDBESSource", "GlobalTag")

process.load("CondTools.SiStrip.diffPedestals_cfi")
process.diffPedestals.pedestalsA = cms.ESInputTag('', '')
process.diffPedestals.pedestalsB = cms.ESInputTag('', 'modifiedPedestals')
process.p = cms.Path(process.diffPedestals)
