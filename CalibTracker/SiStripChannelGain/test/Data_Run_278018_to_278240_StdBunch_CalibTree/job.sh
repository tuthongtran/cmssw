#!/bin/bash
cd /afs/cern.ch/work/a/atalierc/CMSSW_10_2_0/src/CalibTracker/SiStripChannelGain/test;source /afs/cern.ch/cms/cmsset_default.sh;eval `scramv1 runtime -sh`;
cd /afs/cern.ch/work/a/atalierc/CMSSW_10_2_0/src/CalibTracker/SiStripChannelGain/test/Data_Run_278018_to_278240_StdBunch_CalibTree; 
pwd; 
ls; 
#python submitJob.py -f 278018 -l 278020 -p False -P True -m StdBunch -s /afs/cern.ch/work/a/atalierc/CMSSW_10_2_0/src/CalibTracker/SiStripChannelGain/test/7TeVData -a False
cmsRun Gains_Compute_cfg.py