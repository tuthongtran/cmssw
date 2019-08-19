#include "APV.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>
#include "CommonTools/TrackerMap/interface/TrackerMap.h"                        

using namespace std;

void plotter(){

  std::string TextToPrint = "CMS Preliminary 2016 : "; 

  TrackerMap* tkmap_MPV_new = new TrackerMap(TextToPrint+"New fit procedure - MPV/100 [ADC/mm]");                    
  TrackerMap* tkmap_MPV_old = new TrackerMap(TextToPrint+"Old fit procedure - MPV/100 [ADC/mm]");

  TrackerMap* tkmap_errors_new = new TrackerMap(TextToPrint+"New fit procedure - Errors on MPV [ADC/mm]");
  TrackerMap* tkmap_errors_old = new TrackerMap(TextToPrint+"Old fit procedure - Errors on MPV [ADC/mm]");

  TrackerMap* tkmap_MPV_diff = new TrackerMap(TextToPrint+"New fit procedure - Old fit procedure - MPV [ADC/mm]");
  TrackerMap* tkmap_errors_diff = new TrackerMap(TextToPrint+"New Errors - Old Errors - MPV [ADC/mm]");

  TrackerMap* tkmap_entries_new = new TrackerMap(TextToPrint+"New fit procedure entries");
  TrackerMap* tkmap_entries_old = new TrackerMap(TextToPrint+"Old fit procedure entries");
  TrackerMap* tkmap_entries_diff = new TrackerMap(TextToPrint+"New entries - Old entries");

  TFile * gainSTD = new TFile("Gains_Tree_ready_new.root");//bad_all.root
  TFile *calib_tree_old = new TFile("../Data_Run_278018_to_278240_StdBunch_CalibTree_oldfit/Gains_Tree.root"); //

  TTree * tSTD    = (TTree * ) gainSTD->Get("APVGain");
  APV  * aSTD = new APV;

  unsigned int  tree2_Index;
  unsigned int  tree2_DetId;
  unsigned char tree2_APVId;
  unsigned char tree2_SubDet;
  float         tree2_x;
  float         tree2_y;
  float         tree2_z;
  float         tree2_Eta;
  float         tree2_R;
  float         tree2_Phi;
  float         tree2_Thickness;
  float         tree2_FitMPV;
  float         tree2_FitMPVErr;
  float         tree2_FitWidth;
  float         tree2_FitWidthErr;
  float         tree2_FitChi2NDF;
  double        tree2_Gain;
  double        tree2_PrevGain;
  double        tree2_NEntries;
  bool          tree2_isMasked;

  TTree *t2 = (TTree*)calib_tree_old->Get("SiStripCalib/APVGain");

  t2->SetBranchAddress("Index"             ,&tree2_Index      );
  t2->SetBranchAddress("DetId"             ,&tree2_DetId      );
  t2->SetBranchAddress("APVId"             ,&tree2_APVId      );
  t2->SetBranchAddress("SubDet"            ,&tree2_SubDet     );
  t2->SetBranchAddress("x"                 ,&tree2_x          );
  t2->SetBranchAddress("y"                 ,&tree2_y          );
  t2->SetBranchAddress("z"                 ,&tree2_z          );
  t2->SetBranchAddress("Eta"               ,&tree2_Eta        );
  t2->SetBranchAddress("R"                 ,&tree2_R          );
  t2->SetBranchAddress("Phi"               ,&tree2_Phi        );
  t2->SetBranchAddress("Thickness"         ,&tree2_Thickness  );
  t2->SetBranchAddress("FitMPV"            ,&tree2_FitMPV     );
  t2->SetBranchAddress("FitMPVErr"         ,&tree2_FitMPVErr  );
  t2->SetBranchAddress("FitWidth"          ,&tree2_FitWidth   );
  t2->SetBranchAddress("FitWidthErr"       ,&tree2_FitWidthErr);
  t2->SetBranchAddress("FitChi2NDF"        ,&tree2_FitChi2NDF );
  t2->SetBranchAddress("Gain"              ,&tree2_Gain       );
  t2->SetBranchAddress("PrevGain"          ,&tree2_PrevGain   );
  t2->SetBranchAddress("NEntries"          ,&tree2_NEntries   );
  t2->SetBranchAddress("isMasked"          ,&tree2_isMasked   );



  tSTD->SetBranchAddress("APV",&aSTD);
  
  /*int detId = 0;
  int nAPV  = 1;
  float MPV_STD;
  float MPV_AAG;
  
  float error_STD;
  float error_AAG;
    
  float entries_STD;
  float entries_AAG;*/
  unsigned int previousMod =0;  

  for (int i = 0; i < tSTD->GetEntries(); i++){
    //for (int i = 0; i < 10000; i++){

    tSTD->GetEntry(i);
    t2->GetEntry(i);

    if (aSTD->DetId < 369120277 )continue;
    //if (aSTD->DetId != detId){
    if(previousMod>0 && aSTD->ApvId==0){
      if(aSTD->nEntries == 0){aSTD->MPV = 0;
	//cout << "DetId " << aSTD->DetId << " MPV " << aSTD->MPV << " previousMod " << previousMod <<"\n";
      }
      cout << "DetId " << aSTD->DetId << " MPV " << aSTD->MPV << " previousMod " << previousMod <<"\n";
      tkmap_MPV_new->fill(previousMod, aSTD->MPV/100);
      tkmap_MPV_old->fill(previousMod, tree2_FitMPV/100);
      tkmap_MPV_diff->fill(previousMod, aSTD->MPV - tree2_FitMPV);
      tkmap_errors_new->fill(previousMod, aSTD->MPV_error);
      tkmap_errors_old->fill(previousMod, tree2_FitMPVErr);
      tkmap_errors_diff->fill(previousMod, aSTD->MPV_error - tree2_FitMPVErr);
      tkmap_entries_new->fill(previousMod, aSTD->nEntries);
      tkmap_entries_old->fill(previousMod, tree2_NEntries);
      tkmap_entries_diff->fill(previousMod, aSTD->nEntries - tree2_NEntries);
    }

    previousMod = aSTD->DetId;

  }if(previousMod>0){//tkmap_entries->fill(previousMod, aSTD->nEntries);
    //tkmap_MPV->fill(previousMod, aSTD->MPV - tree2_FitMPV);
    tkmap_MPV_new->fill(previousMod, aSTD->MPV/100);
    tkmap_MPV_old->fill(previousMod, tree2_FitMPV/100);
    tkmap_MPV_diff->fill(previousMod, aSTD->MPV - tree2_FitMPV);
    tkmap_errors_new->fill(previousMod, aSTD->MPV_error);
    tkmap_errors_old->fill(previousMod, tree2_FitMPVErr);
    tkmap_errors_diff->fill(previousMod, aSTD->MPV_error - tree2_FitMPVErr);
    tkmap_entries_new->fill(previousMod, aSTD->nEntries);
    tkmap_entries_old->fill(previousMod, tree2_NEntries);
    tkmap_entries_diff->fill(previousMod, aSTD->nEntries - tree2_NEntries);

  }

  //tkmap_MPV->setTitle(" New Fit MPV");
  //tkmap_MPV->save(true,0, 50,"map_MPV_diif_new.png");
  //tkmap_entries->save(true,0,3000,"map_entries_diff_new.png");  
  tkmap_MPV_old->save(true,0, 3.5,"map_MPV_old.png");
  tkmap_MPV_new->save(true,0, 3.5,"map_MPV_new.png");
  tkmap_MPV_diff->save(true,0, 50,"map_MPV_diff.png");

  tkmap_errors_new->save(true, 0, 10,  "map_errors_new.png");  
  tkmap_errors_old->save(true, 0, 10,  "map_errors_old.png");
  tkmap_errors_diff->save(true, 0, 10,  "map_errors_diff.png");

  tkmap_entries_new->save(true,0,3000,"map_entries_new.png");
  tkmap_entries_old->save(true,0,3000,"map_entries_old.png");
  tkmap_entries_diff->save(true,-100,300,"map_entries_diff.png");
  //double MaxFit_new=0;  unsigned int previousMod_new=0;
  //for (unsigned int ientry = 0; ientry < tSTD->GetEntries(); ientry++) {
    //if(ientry%TreeStep==0){printf(".");fflush(stdout);}
    //tSTD->GetEntry(ientry);
    //if (aSTD->DetId<369120277)continue;
    //if(previousMod_new>0&&aSTD->ApvId==0){tkmap_1->fill(previousMod_new, MaxFit_new / 100/*aSTD->MPV / 100*/);  MaxFit_new=1;  }
    //previousMod_new = aSTD->DetId;
    //if(fabs(tree1_FitMPV-1.0)>fabs(MaxFit_new-1))MaxFit_new=tree1_FitMPV/100;

    //if(aSTD->MPV > MaxFit_new) MaxFit_new=aSTD->MPV;
  //}printf("\n");
  //if(previousMod_new>0){tkmap_1->fill(previousMod_new, MaxFit_new / 100/*tree1_FitMPV / 100*/); }
  //tkmap_1->setTitle("New Fit MPV / 100");
  //tkmap_1->save                 (true, 0, 3.5, "_TKMap_FitResults_Gains1_prova.png");

}

