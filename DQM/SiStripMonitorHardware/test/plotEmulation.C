#include "iostream"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <cmath>

#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "TLatex.h"


void plotEmulation(){//main

  //TFile *file = TFile::Open("./SpyDisplay.root");
  TFile *file = TFile::Open("./SpyDisplay321054_Test.root");

  //unsigned int runNumber = 128646;
  unsigned int runNumber = 321054;

  //const unsigned int nEvts = 9;
  //unsigned int evtNumber[nEvts] = {1,56,100,186,228,289,342,378,389};
  //const unsigned int nEvts = 20;
  //unsigned int evtNumber[nEvts] = {9,10,11,12,13,14,15,16,57,58,59,60,61,62,63,64,105,106,107,108};
  const unsigned int nEvts = 1;
  unsigned int evtNumber[nEvts] = {10};

  //const unsigned int nDets = 11;
  //unsigned int detId[nDets] = {436265548,436265552,436265556,436265560,436265576, 
  //			       436265580,436265584,436265588,436265592,
  //			       369121566,369124733};
  const unsigned int nDets = 19;
  unsigned int detId[nDets] = {470079220, 470083621, 470083622, 470083625, 470083626, 470083653, 470083654, 470083657, 470083658, 470083684, 470083688, 470083692, 470083716, 470083720, 470083724, 470083728, 470083780, 470083784, 470083788};

  //const unsigned int nDets = 1;
  //unsigned int detId[nDets] = {470079220};

  TString lBaseDir = "SiStripSpyDisplay/Display/run";
  lBaseDir += runNumber;
  lBaseDir += "_event";

  gStyle->SetOptStat(0);

  TLatex lat;
 
  TCanvas *myc = new TCanvas("myc","",800,800);
  myc->Divide(2,2);
 
  TString lEvtDir;

  for (unsigned int iDet(0); iDet<nDets; iDet++){

    TH1F *p_mode[4][nEvts];

    for (unsigned int iEv(0); iEv<nEvts; iEv++){

      lEvtDir = lBaseDir;
      lEvtDir += evtNumber[iEv];
      lEvtDir += "/detID_";

      TString lDetDir;

      lDetDir = lEvtDir;
      lDetDir += detId[iDet];
      lDetDir += "/";

      if (file->cd(lDetDir)){
	p_mode[0][iEv] = (TH1F*)gDirectory->Get("PedestalValues");
	p_mode[1][iEv] = (TH1F*)gDirectory->Get("PostPedestal");
	p_mode[2][iEv] = (TH1F*)gDirectory->Get("PostCommonMode");
	p_mode[3][iEv] = (TH1F*)gDirectory->Get("ZeroSuppressedDigi");
	p_mode[0][iEv]->SetTitle("Pedestal Values");
	p_mode[1][iEv]->SetTitle("Post Pedestal Subtraction");
	p_mode[2][iEv]->SetTitle("Post Common Mode Subtraction");
	p_mode[3][iEv]->SetTitle("Post Zero Suppression");
        std::cout << p_mode[0][iEv]->GetEntries()<<" ";
        std::cout << p_mode[1][iEv]->GetEntries()<<" ";
        std::cout << p_mode[2][iEv]->GetEntries()<<" ";
        std::cout << p_mode[3][iEv]->GetEntries()<<std::endl;
      }
      else {
	std::cout << " -- Dir " << lDetDir << " not found." << std::endl;
      }

      for (unsigned int i(0); i<4; i++){
	myc->cd(i+1);
	if (!p_mode[i][iEv]) continue;
	p_mode[i][iEv]->SetLineColor(iEv+1);
	if (iEv == 0) p_mode[i][iEv]->Draw();
	else p_mode[i][iEv]->Draw("same");
      }

 
    }//loop on evts

    TString save = "PLOTS/";
    std::ostringstream lmkdir;
    lmkdir << "PLOTS/" << runNumber;
    mkdir(lmkdir.str().c_str(),0755);
    save += runNumber;
    save += "/FEDEmulation_";
    save += detId[iDet];
    save += ".png";
    
    myc->Print(save);
      


  }//loop on detids


}//main
