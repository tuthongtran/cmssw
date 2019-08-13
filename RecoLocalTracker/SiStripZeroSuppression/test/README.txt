Launch config 
cmsRun SiStripZS_EmulateHybrid_Parallel.py

///////////   Display scripts ///////////

To compare the digis and clusters stats :
python compareDigiClusterStats.py
(change name of the root file in the script)

To plot the baseline (in root) :
.L displayBaselineMacro.C
displayBaselineMacro("diffhisto*.root")
-> Prints the output from SiStripBaselineAnalyzer.cc
-> Will find the selected event and detid as given in the vector selection in the script

To plot the Hybrid analyzer output (in root) :
.L displayHybridAnalyzer.C
displayHybridAnalyzer("diffhisto*.root",limit)
-> Prints the output of SiStripHybridFormatAnalyzer.cc
(only print a certain amount of detid -> limit)
