#cmsRun run_simulation_step1_2016.py CT_def
cmsRun run_simulation_step2_gainOption2Tuned_2016.py gainOption2Tuned0.805 0.805 0.0775 0.02
cmsRun run_simulation_step3_gainOption2_2016.py gainOption2Tuned0.805
cmsRun trackCluster_test.py  gainOption2Tuned0.805step3.root
