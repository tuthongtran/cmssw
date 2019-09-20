#include "TH1.h"
#include "TF1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TPaveText.h"
#include "TMinuit.h"

Double_t langaufun(Double_t *x, Double_t *par)
{
  //Fit parameters:
  //par[0]=Width (scale) parameter of Landau density
  //par[1]=Most Probable (MP, location) parameter of Landau density
  //par[2]=Total area (integral -inf to inf, normalization constant)
  //par[3]=Width (sigma) of convoluted Gaussian function
  //
  //In the Landau distribution (represented by the CERNLIB approximation),
  //the maximum is located at x=-0.22278298 with the location parameter=0.
  //This shift is corrected within this function, so that the actual
  //maximum is identical to the MP parameter.

  // Numeric constants
  Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
  Double_t mpshift  = -0.22278298;       // Landau maximum location

  // Control constants
  Double_t np = 100.0;      // number of convolution steps
  Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

  // Variables
  Double_t xx;
  Double_t mpc;
  Double_t fland;
  Double_t sum = 0.0;
  Double_t xlow,xupp;
  Double_t step;
  Double_t i;


  // MP shift correction
  mpc = par[1] - mpshift * par[0];

  // Range of convolution integral
  xlow = x[0] - sc * par[3];
  xupp = x[0] + sc * par[3];

  step = (xupp-xlow) / np;

  // Convolution integral of Landau and Gaussian by sum
  for(i=1.0; i<=np/2; i++) {
    xx = xlow + (i-.5) * step;
    fland = TMath::Landau(xx,mpc,par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0],xx,par[3]);

    xx = xupp - (i-.5) * step;
    fland = TMath::Landau(xx,mpc,par[0]) / par[0];
    sum += fland * TMath::Gaus(x[0],xx,par[3]);
  }

  return (par[2] * step * sum * invsq2pi / par[3]);
}



TH1D *langaucon(TH1D *his, Double_t *fitrange, Double_t *startvalues, int num)
{

  Int_t i;
  Char_t FunName[100];

  sprintf(FunName,"Fitfcn_%s",his->GetName());

  TF1 *ffitold = (TF1*)gROOT->GetListOfFunctions()->FindObject(FunName);
  if (ffitold) delete ffitold;

  TF1 *ffit = new TF1(FunName,langaufun,fitrange[0],fitrange[1],4);
  ffit->SetParameters(startvalues);
  ffit->SetParNames("Width","MP","Area","GSigma");
  his->FillRandom(FunName, num);
  
  return (his);              // return convoluted function

}

pair<TH1D *, TString> langaufit_real(TH1D *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF)
{
  // Once again, here are the Landau * Gaussian parameters:
  //   par[0]=Width (scale) parameter of Landau density
  //   par[1]=Most Probable (MP, location) parameter of Landau density
  //   par[2]=Total area (integral -inf to inf, normalization constant)
  //   par[3]=Width (sigma) of convoluted Gaussian function
  //
  // Variables for langaufit call:
  //   his             histogram to fit
  //   fitrange[2]     lo and hi boundaries of fit range
//   startvalues[4]  reasonable start values for the fit
//   parlimitslo[4]  lower parameter limits
//   parlimitshi[4]  upper parameter limits
//   fitparams[4]    returns the final fit parameters
//   fiterrors[4]    returns the final fit errors
//   ChiSqr          returns the chi square
//   NDF             returns ndf

Int_t i;
Char_t FunName[100];

sprintf(FunName,"Fitfcn_%s",his->GetName());

TF1 *ffitold = (TF1*)gROOT->GetListOfFunctions()->FindObject(FunName);
if (ffitold) delete ffitold;

 TF1 *ffit = new TF1(FunName,langaufun,his->GetXaxis()->GetXmin(),his->GetXaxis()->GetXmax(),4);//fitrange[0],fitrange[1],4);
 ffit->SetParameters(startvalues);
 ffit->SetParNames("Width","MP","Area","GSigma");


  for (i=0; i<4; i++) {
    ffit->SetParLimits(i, parlimitslo[i], parlimitshi[i]);
  }

  his->Fit(FunName,"QRB0","L",fitrange[0],fitrange[1]);//RB0   // fit within specified range, use ParLimits, do not plot
  
  cout << "fit status " << gMinuit->fStatus << "\n";
  TString status = gMinuit->fCstatu.Data();
  cout << "fit status " << status << "\n";
  ffit->GetParameters(fitparams);    // obtain fit parameters
  for (i=0; i<4; i++) {
    fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
  }
  ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
  NDF[0] = ffit->GetNDF();           // obtain ndf

  
  return make_pair((TH1D *)ffit, status); //return the fit function and the status of the fit
}


void langaus() {

  printf("Fitting...\n");

  // Setting fit range and values for the convolution
  Double_t fr[2];
  Double_t sv[4];

  fr[0]=0;//MaxBin_value-100;
  fr[1]=1400;//MaxBin_value+500;
  sv[0]=25; sv[1]=300; sv[2]=10000; sv[3]=40;

  Double_t chisqr;
  Int_t    ndf;

  TH1D *his = new TH1D("","",100,0,1000);

  TH2D *histo = new TH2D("","",5400, 100, 5500, 150, 200, 350);
  histo->SetMarkerStyle(2);
  histo->GetXaxis()->SetTitle("Entries");
  histo->GetYaxis()->SetTitle("Errors");

  TH2D *histo_entries = new TH2D("","",10, 1000, 5000, 100, 0, 100);//4900, 100, 5000
  histo_entries->SetMarkerStyle(2);
  histo_entries->GetXaxis()->SetTitle("Entries");
  histo_entries->GetYaxis()->SetTitle("# good fit");

  TH1D *normal_histo = new TH1D("", "", 4900, 100, 5000);

  for(int i = 1000; i < 5000; i=i+500){ //i = number of entries used for the generatino of the convolutiono
    int count = 0;
    int conv = 0;
    cout << "i " << i << "\n";

    //for(int j = 0; j < 100; j++){ //repeat for j times the fit and regenerate the convolution

      TH1D *fitsnr = langaucon(his,fr,sv, i);
      TH1D *fitsnr_clone = (TH1D *)fitsnr->Clone();
      fitsnr_clone->Rebin(10);
      
      float bin_content = -1;
      float max_content = -1;
      int bin_max = -1;
      
      for(int i = 0; i < fitsnr_clone->GetNbinsX(); i ++){
	bin_content = fitsnr_clone->GetBinContent(i);
	
	if(bin_content > max_content and fitsnr_clone->GetXaxis()->GetBinCenter(i) > 100){
	max_content = bin_content;
	bin_max = i;
	}
	
      }
      
      int MaxBin_value = fitsnr_clone->GetXaxis()->GetBinCenter(bin_max);
      cout << "max bin by me " << MaxBin_value << "\n";
      cout << "Get Maximum " << (TH1 *)fitsnr_clone->GetMaximumX() << "\n";      

      Double_t new_fr[2];
      new_fr[0] =100;//MaxBin_value-100;
      new_fr[1] =500;//MaxBin_value+500;
      
      Double_t new_sv[4], new_pllo[4], new_plhi[4], new_fp[4], new_fpe[4];
      new_pllo[0]=0.5; new_pllo[1]=0; new_pllo[2]=1.0; new_pllo[3]=0;
      new_plhi[0]=100; new_plhi[1]=500.0; new_plhi[2]=1000000000.0; new_plhi[3]=10000;
      new_sv[0]=1; new_sv[1]=1; new_sv[2]=1; new_sv[3]=1;
      

      //for(int j = 0; j < 100; j++){
      TH1D *fit = langaufit_real((TH1D *)fitsnr_clone,new_fr,new_sv,new_pllo,new_plhi,new_fp,new_fpe,&chisqr,&ndf).first;
      TString str = langaufit_real((TH1D *)fitsnr_clone,new_fr,new_sv,new_pllo,new_plhi,new_fp,new_fpe,&chisqr,&ndf).second;
      cout << str <<"\n";
      //string out = "CONVERGED";
      if(str.Contains("CONV")){
	conv++;
	//histo->Fill(i,new_fp[1]);
	//normal_histo->Fill(i);
      }else{count++;}
      //}
    histo_entries->Fill(i, conv);
    cout <<  conv << "\n";
  }
  //normal_histo->Draw();
  //cout << count << "\n";
  //histo_entries->Draw();
  //histo_entries->Draw();
  //TF1* fit = new TF1("MyLandau","landau",fitsnr_clone->GetXaxis()->GetXmin(), fitsnr_clone->GetXaxis()->GetXmax());
  //MyLandau->SetParameter(1,300);
  //(TH1D *)fitsnr_clone->Fit(MyLandau,"0Q", "L", 100, 500);

  //gStyle->SetOptStat(1111);
  //gStyle->SetOptFit(111);
  //gStyle->SetLabelSize(0.03,"x");
  //gStyle->SetLabelSize(0.03,"y");

  //(TH1D *)fitsnr->Rebin(10);
  //fitsnr->Draw("EP");
  //fit->SetLineColor(kRed);
  //fit->SetLineStyle(kDashed);
  //fit->Draw("lsame");

}
