from __future__ import print_function

## adapted from produceCalibrationTree_template_cfg.py

import FWCore.ParameterSet.Config as cms
from CalibTracker.SiStripCommon.shallowTree_test_template import *

process = cms.Process('CALIB')
process.load('CalibTracker.Configuration.setupCalibrationTree_cff')
process.load('Configuration/StandardSequences/MagneticField_cff')
process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('CalibTracker.SiStripCommon.ShallowEventDataProducer_cfi') #event Info

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, "auto:run2_mc")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.Services_cff')
process.add_( cms.Service( "TFileService",
                           fileName = cms.string("CalibTreeMC_classic.root"),
                           closeFileFast = cms.untracked.bool(True)  ) )

process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring("file:/afs/cern.ch/user/d/davidp/private/tkdpg/CT20/CMSSW_11_2_0_pre3/src/1311.0_MinBias_13+MinBias_13+DIGIUP15+RECOMINUP15+HARVESTMINUP15+ALCAMINUP15/SiStripCalMinBias.root"),
     )

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.CalibrationTracks.src = cms.InputTag("ALCARECOSiStripCalMinBias")
process.shallowTracks.Tracks  = cms.InputTag("ALCARECOSiStripCalMinBias")

process.prescaleEvent.prescale = 1

process.eventInfo = cms.EDAnalyzer(
   "ShallowTree",
   outputCommands = cms.untracked.vstring(
      'drop *',
      'keep *_shallowEventRun_*_*',
      )
)

process.TkCalPath_StdBunch   = cms.Path(process.TkCalSeq_StdBunch   *process.shallowEventRun*process.eventInfo)

process.schedule = cms.Schedule(process.TkCalPath_StdBunch)

process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('OtherCMS',
        'StdException',
        'Unknown',
        'BadAlloc',
        'BadExceptionType',
        'ProductNotFound',
        'DictionaryNotFound',
        'InsertFailure',
        'Configuration',
        'LogicError',
        'UnimplementedFeature',
        'InvalidReference',
        'NullPointerError',
        'NoProductSpecified',
        'EventTimeout',
        'EventCorruption',
        'ScheduleExecutionFailure',
        'EventProcessorFailure',
        'FileInPathError',
        'FileOpenError',
        'FileReadError',
        'FatalRootError',
        'MismatchedInputFiles',
        'ProductDoesNotSupportViews',
        'ProductDoesNotSupportPtr',
        'NotFound')
)
