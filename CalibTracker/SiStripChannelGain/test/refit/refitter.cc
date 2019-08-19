#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TLeaf.h"
#include "APV.h"
#include "progressBar.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>

#pragma link C++ defined_in "APV.h";

#include <string>
#include <iostream>
#include <fstream>

using namespace std;


class tkmap_class
{
  //int a;
  //int b;

public:
  int b;
  int all_statistics;
  //TH1 *histo;
  void clean(APV * a);
  void fit(TH2 * hist, int bin, int detId, int apvId, int range_min, int range_max, APV * a);
  //void fit_TOB_5_and_6(TH2 * hist, int bin, int detId, int apvId, APV * a);
};


  void tkmap_class::clean(APV * a){
  a->MPV = 0;
  a->MPV_error = 0;
  a->chi2 = 0;
  a->ndf = 0;
  a->nEntries = 0;
  //a->ApvId = 0;//
}

void tkmap_class::fit(TH2 * hist, int bin, int detId, int apvId, int range_min, int range_max, APV * a)
{
  a->ApvId = apvId;
  a->DetId = detId;
  a->rawId = bin;
  
  TH1 * charge = hist->ProjectionY("proj",bin,bin);

  //histo = charge;
  if (charge->GetEntries() < 20){
    clean(a);
  }

  /*
  TF1 * Landau = new TF1("myLandau","landau",50,5400);
  Landau->SetParameter(1,300);

  // First pass to get idea on MPV.
  charge->Fit(Landau,"0QR WW");
  //Second pass to get real peak.
  double MPV = Landau->GetParameter(1);
  delete Landau;
  Landau = new TF1("myLandau","landau",MPV-10,500);
  Landau->SetParameter(1,MPV);
  charge->Fit(Landau,"0QLR","");
  // MPV is parameter 1 (0=constant, 1=MPV, 2=Sigma)
  */


  float bin_content = -1;
  float max_content = -1;
  int bin_max = -1;

  TH1D * charge_clone = (TH1D *)charge->Clone("charge_clone");
  charge_clone->Rebin(10);

  for(int i = 0; i < charge_clone->GetNbinsX(); i ++){
    bin_content = charge_clone->GetBinContent(i);

    if(bin_content > max_content and charge_clone->GetXaxis()->GetBinCenter(i) > 100){
      max_content = bin_content;
      bin_max = i;
    }
  }

  //if(bin_max > -1){
  int MaxBin_value = charge_clone->GetXaxis()->GetBinCenter(bin_max);

  //bin_getter = MaxBin_value;
  //if(charge->Integral(MaxBin_value-40, MaxBin_value+1000) > 100){
  TF1* MyLandau = new TF1("MyLandau","landau", charge->GetXaxis()->GetXmin(), charge->GetXaxis()->GetXmax());
  //TF1* MyLandau = new TF1("MyLandau","landau",LowRange, HighRange);
  MyLandau->SetParameter(1,300);
  charge->Fit(MyLandau,"0Q", "L", MaxBin_value-range_min, MaxBin_value+range_max);
  
  b = charge->Integral(charge->FindBin(MaxBin_value-range_min), charge->FindBin(MaxBin_value+range_max));  
  all_statistics = charge->Integral();  

  a->MPV                    = MyLandau->GetParameter(1);  //MPV
  a->MPV_error              = MyLandau->GetParError(1);   //MPV error
  a->chi2                  = MyLandau->GetChisquare();
  a->ndf                   = MyLandau->GetNDF();
  a->nEntries              = charge->GetEntries();

  delete MyLandau;
  delete charge;
  delete charge_clone;
  //}
  //return MaxBin_value;
}


int main(int argc, char ** argv){
  if (argc < 3){
    cerr<<"Expecting input and output files as arguments..."<<endl;
    return 0;
  }


  tkmap_class tkmap_class1;


  Char_t txtOUT[100]="File_debug.txt";
  //cout << "Opening a txt file with candidate events " << txtOUT << endl;
  ofstream output_txt;
  output_txt.open(txtOUT);

  TFile * input = new TFile(argv[1]);
  TH2   * charge  = (TH2* ) input->Get("SiStripCalib/Charge_Vs_Index_StdBunch");
  TTree * gains   = (TTree* ) input->Get("SiStripCalib/APVGain");
  
  TLeaf *  DetIdLeaf= gains->GetLeaf("DetId");
  TLeaf *  APVIdLeaf= gains->GetLeaf("APVId");
  TLeaf *  BinLeaf  = gains->GetLeaf("Bin");
  TLeaf *  RLeaf  = gains->GetLeaf("R");
  TLeaf *  ZLeaf  = gains->GetLeaf("z");

  progressBar p(gains->GetEntries());
  APV a;

  TFile * output = new TFile(argv[2],"RECREATE");
  //TFile *bad_histo = new TFile("bad_histos.root", "recreate");
  TTree * outTree = new TTree("APVGain","APVGain");
  outTree->Branch("APV",&a);
  outTree->Write();
  for (int entry = 0; entry < gains->GetEntries(); entry++){
    gains->GetEntry(entry);
    if(a.R > 90 and a.R < 120 and a.Z > -100 and a.Z < 100){ 
      tkmap_class1.fit(charge,BinLeaf->GetValue(), DetIdLeaf->GetValue(), APVIdLeaf->GetValue(), 100, 500, &a);
      outTree->Fill();
    }else{
      tkmap_class1.fit(charge,BinLeaf->GetValue(), DetIdLeaf->GetValue(), APVIdLeaf->GetValue(), 40, 1000,  &a);
      outTree->Fill();}


    //outTree->Fill();
    /*a.Z = ZLeaf->GetValue();
    a.R = RLeaf->GetValue();
    outTree->Fill();*/
    p.step(entry);

    if(a.MPV < 0){
      char *histname = new char[10];
      //TH1 *histo = charge->ProjectionY("proj",BinLeaf->GetValue(),BinLeaf->GetValue());
      //sprintf(histname,"histo_%d",entry);
      a.Z = ZLeaf->GetValue();
      a.R = RLeaf->GetValue();

      uint32_t myDetId = a.DetId;
      auto subdet = (myDetId>>25)&0x7; // TIB = 3, TID = 4, TOB = 5, TEC = 6
      // for TIB and TOB
      auto layerBarrel = (myDetId>>14)&0x7;
      // for TID
      auto sideTID = (myDetId>>13)&0x3;
      auto wheelTID = (myDetId>>11)&0x3;
      auto ringTID = (myDetId>>9)&0x3;
      // for TEC
      auto sideTEC = (myDetId>>18)&0x3;
      auto wheelTEC = (myDetId>>14)&0xF;
      auto ringTEC = (myDetId>>5)&0x7;

      output_txt << "MPV " << a.MPV << "\n"
		 << "MPV err " << a.MPV_error << "\n" 
		 << "DetId " << a.DetId << "\n"
		 << "Z " << a.Z << "\n" 
		 << "R " << a.R << "\n"
		 << "Integral " << tkmap_class1.b << "\n"
		 << "Statistics " << tkmap_class1.all_statistics << "\n"
		 << "TIB = 3, TID = 4, TOB = 5, TEC = 6 " << subdet << "\n"
		 << "for TIB and TOB " << layerBarrel << "\n"
		 << "for TID side " << sideTID << "\n"
		 << "for TID wheel " << wheelTID << "\n"
		 << "for TID ring " << ringTID << "\n"
                 << "for TEC side " << sideTEC << "\n"
                 << "for TEC wheel " <<wheelTEC << "\n"
                 << "for TEC ring " << ringTEC << "\n";

    }

  }
  p.stop();
  //bad_histo->Close();
  outTree->Write();
  input->Close();
  output->Close();
  output_txt.close();

}

