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



TF1 *langaufit(TH1D *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF)
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

  TF1 *ffit = new TF1(FunName,langaufun,fitrange[0],fitrange[1],4);
  ffit->SetParameters(startvalues);
  ffit->SetParNames("Width","MP","Area","GSigma");

  for (i=0; i<4; i++) {
    ffit->SetParLimits(i, parlimitslo[i], parlimitshi[i]);
  }

  his->Fit(FunName,"RB0");   // fit within specified range, use ParLimits, do not plot
  ffit->GetParameters(fitparams);    // obtain fit parameters
  for (i=0; i<4; i++) {
    fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
  }
  ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
  NDF[0] = ffit->GetNDF();           // obtain ndf

  return (ffit);              // return fit function

}


/*Int_t langaupro(Double_t *params, Double_t &maxx, Double_t &FWHM) {

  // Seaches for the location (x value) at the maximum of the
  // Landau-Gaussian convolute and its full width at half-maximum.
  //
  // The search is probably not very efficient, but it's a first try.

  Double_t p,x,fy,fxr,fxl;
  Double_t step;
  Double_t l,lold;
  Int_t i = 0;
  Int_t MAXCALLS = 10000;


  // Search for maximum

  p = params[1] - 0.1 * params[0];
  step = 0.05 * params[0];
  lold = -2.0;
  l    = -1.0;


  while ( (l != lold) && (i < MAXCALLS) ) {
    i++;

    lold = l;
    x = p + step;
    l = langaufun(&x,params);

    if (l < lold)
      step = -step/10;

    p += step;
  }

  if (i == MAXCALLS)
    return (-1);

  maxx = x;

  fy = l/2;


  // Search for right x location of fy

  p = maxx + params[0];
  step = params[0];
  lold = -2.0;
  l    = -1e300;
  i    = 0;


  while ( (l != lold) && (i < MAXCALLS) ) {
    i++;

    lold = l;
    x = p + step;
    l = TMath::Abs(langaufun(&x,params) - fy);

    if (l > lold)
      step = -step/10;

    p += step;
  }

  if (i == MAXCALLS)
    return (-2);

  fxr = x;


  // Search for left x location of fy

  p = maxx - 0.5 * params[0];
  step = -params[0];
  lold = -2.0;
  l    = -1e300;
  i    = 0;

  while ( (l != lold) && (i < MAXCALLS) ) {
    i++;

    lold = l;
    x = p + step;
    l = TMath::Abs(langaufun(&x,params) - fy);

    if (l > lold)
      step = -step/10;

    p += step;
  }

  if (i == MAXCALLS)
    return (-3);


  fxl = x;

  FWHM = fxr - fxl;
  return (0);
  }*/




void langaus() {
  TFile *calib_tree = new TFile("Data_Run_278018_to_278240_StdBunch_CalibTree_oldfit/Gains_Tree.root");//DataNewFit_Run_278018_to_278018_AagBunch_CalibTree/Gains_Tree.root Data_Run_278018_to_278240_StdBunch_CalibTree_oldfit/Gains_Tree.root //DataNewFit_Run_278018_to_278240_AagBunch_CalibTree/Gains_Tree.root

  TH2D *histo = (TH2D*)calib_tree->Get("SiStripCalib/Charge_Vs_Index_StdBunch");
  TH1D *plot_charge = NULL;
  TH1D* no_rebin = NULL;
  unsigned int  tree1_Index;
  unsigned int  tree1_DetId;
  unsigned char tree1_APVId;
  unsigned char tree1_SubDet;
  float         tree1_x;
  float         tree1_y;
  float         tree1_z;
  float         tree1_Eta;
  float         tree1_R;
  float         tree1_Phi;
  float         tree1_Thickness;
  float         tree1_FitMPV;
  float         tree1_FitMPVErr;
  float         tree1_FitWidth;
  float         tree1_FitWidthErr;
  float         tree1_FitChi2NDF;
  double        tree1_Gain;
  double        tree1_PrevGain;
  double        tree1_NEntries;
  bool          tree1_isMasked;

  TTree *t1 = (TTree*)calib_tree->Get("SiStripCalib/APVGain");

  t1->SetBranchAddress("Index"             ,&tree1_Index      );
  t1->SetBranchAddress("DetId"             ,&tree1_DetId      );
  t1->SetBranchAddress("APVId"             ,&tree1_APVId      );
  t1->SetBranchAddress("SubDet"            ,&tree1_SubDet     );
  t1->SetBranchAddress("x"                 ,&tree1_x          );
  t1->SetBranchAddress("y"                 ,&tree1_y          );
  t1->SetBranchAddress("z"                 ,&tree1_z          );
  t1->SetBranchAddress("Eta"               ,&tree1_Eta        );
  t1->SetBranchAddress("R"                 ,&tree1_R          );
  t1->SetBranchAddress("Phi"               ,&tree1_Phi        );
  t1->SetBranchAddress("Thickness"         ,&tree1_Thickness  );
  t1->SetBranchAddress("FitMPV"            ,&tree1_FitMPV     );
  t1->SetBranchAddress("FitMPVErr"         ,&tree1_FitMPVErr  );
  t1->SetBranchAddress("FitWidth"          ,&tree1_FitWidth   );
  t1->SetBranchAddress("FitWidthErr"       ,&tree1_FitWidthErr);
  t1->SetBranchAddress("FitChi2NDF"        ,&tree1_FitChi2NDF );
  t1->SetBranchAddress("Gain"              ,&tree1_Gain       );
  t1->SetBranchAddress("PrevGain"          ,&tree1_PrevGain   );
  t1->SetBranchAddress("NEntries"          ,&tree1_NEntries   );
  t1->SetBranchAddress("isMasked"          ,&tree1_isMasked   );

  for(int i = 0; i < t1->GetEntries(); i++){
    t1->GetEntry(i);

    if(tree1_R < 120 && tree1_R > 55){//TOB1 //60-70 120 50
      if(plot_charge == NULL){
        plot_charge = histo->ProjectionY("TOB1",tree1_Index, tree1_Index);
        cout << "integral " << plot_charge->Integral() << "\n";
      }else
        {plot_charge->Add(histo->ProjectionY("TOB1",tree1_Index, tree1_Index)); }

    }
  }

  no_rebin = (TH1D*)plot_charge->Clone();
  plot_charge->Rebin(10);
  //TCanvas *canvas = new TCanvas("TOBL1","TOBL1",800,800,800,800);
  //canvas->cd();
  //no_rebin->Draw();
  //plot_charge->Print("all");

  Double_t xMin = plot_charge->GetXaxis()->GetXmin();
  Double_t xMax = plot_charge->GetXaxis()->GetXmax();

  float bin_content = -1;
  float max_content = -1;
  int bin_max = -1;

  
  for(int i = 0; i < plot_charge->GetNbinsX(); i ++){
    bin_content = plot_charge->GetBinContent(i);
  
    if(bin_content > max_content and plot_charge->GetXaxis()->GetBinCenter(i) > 100){
      max_content = bin_content;
      bin_max = i;
    }

  }

  
  int MaxBin_value = plot_charge->GetXaxis()->GetBinCenter(bin_max);
  
  TF1* MyLandau = new TF1("MyLandau","landau",no_rebin->GetXaxis()->GetXmin(), no_rebin->GetXaxis()->GetXmax());
  MyLandau->SetParameter(1,300);
  no_rebin->Fit(MyLandau,"0Q", "L", MaxBin_value-100, MaxBin_value+500);
  cout << "mean laundau " << MyLandau->GetParameter(1) << "\n";
  cout << "error laundau " << MyLandau->GetParError(1) << "\n";
  // Fill Histogram

  printf("Fitting...\n");

  // Setting fit range and start values
  Double_t fr[2];
  Double_t sv[4], pllo[4], plhi[4], fp[4], fpe[4];

  fr[0]=MaxBin_value-100;
  fr[1]=MaxBin_value+500;

  pllo[0]=0.5; pllo[1]=100; pllo[2]=1.0; pllo[3]=0.4;
  plhi[0]=3000; plhi[1]=500.0; plhi[2]=1000000.0; plhi[3]=100;
  sv[0]=1000; sv[1]=300; sv[2]=plot_charge->Integral(); sv[3]=40;

  Double_t chisqr;
  Int_t    ndf;

  TF1 *fitsnr = langaufit(no_rebin,fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf);

  //Double_t SNRPeak, SNRFWHM;
  //langaupro(fp,SNRPeak,SNRFWHM);

  printf("Fitting done\nPlotting results...\n");

  // Global style settings
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(111);
  gStyle->SetLabelSize(0.03,"x");
  gStyle->SetLabelSize(0.03,"y");

  //hSNR->GetXaxis()->SetRange(0,70);
  TCanvas *canvas = new TCanvas("TOBL1","TOBL1",800,800,800,800);
  canvas->cd();

  gStyle->SetOptStat("emroi");
  canvas->Update();
  TPaveText *    new_st    = (TPaveText *)fitsnr->FindObject("stats");
  TPaveText *    old_st    = (TPaveText *)MyLandau->FindObject("stats");
  canvas->Modified();


  TH1D *frame = new TH1D("","", 1400, 0, 1400);
  frame->SetStats(0);
  frame->SetMaximum(plot_charge->GetMaximum()+50);
  frame->GetXaxis()->SetTitle("Charge (ADC/mm)");
  frame->GetYaxis()->SetTitle("Entries");
  frame->Draw();
  no_rebin->Draw();
  //plot_charge->Draw("same");
  //plot_charge->GetXaxis()->SetTitle("Charge (ADC/mm)");
  //plot_charge->GetYaxis()->SetTitle("Entries");
  fitsnr->Draw("lsame");
  MyLandau->SetLineColor(kGreen);
  MyLandau->Draw("lsame");
  
  //TLine *newfit_line = new TLine(fitsnr->GetXaxis()->GetBinCenter(fitsnr->GetMaximum()),0,fitsnr->GetXaxis()->GetBinCenter(fitsnr->GetMaximum()),fitsnr->GetMaximum());
  /*  TLine *newfit_line = new TLine(fp[1],0,fp[1],fitsnr->GetMaximum());
  newfit_line->SetLineColor(kRed);
  newfit_line->SetLineStyle(kDashed);
  newfit_line->Draw("lsame");

  TLine *oldfit_line = new TLine(MyLandau->GetParameter(1),0,MyLandau->GetParameter(1),MyLandau->GetMaximum());
  oldfit_line->SetLineColor(kGreen);
  oldfit_line->SetLineStyle(kDashed);
  oldfit_line->Draw("lsame");*/
  
  TH1D *frame2 = new TH1D("","",1400, 0, 1400);//200, 70, 200);//100 , 0, 10);///280,20,300
  frame2->SetMaximum(2);
  frame2->SetMinimum(0);
  frame2->SetStats(0);
  frame2->GetYaxis()->SetTitle("Entries / Fit");
  frame2->GetYaxis()->SetNdivisions(508);
  //TH1D *h3 = (TH1D*)plot_charge->Clone("h3");
  TH1D *h3 = (TH1D*)no_rebin->Clone("h3");
  h3->Divide(MyLandau);
  h3->Sumw2();
  h3->SetMarkerStyle(8);
  h3->SetMarkerColor(kGreen);
  h3->SetLineColor(kBlack);
  TH1D *h4 = (TH1D*)plot_charge->Clone("h3");
  h4->Divide(fitsnr);
  h4->Sumw2();
  h4->SetMarkerStyle(8);
  h4->SetMarkerColor(kRed);
  h4->SetLineColor(kBlack);
  double canvasratio = 0.3;
  canvas->SetBottomMargin(canvasratio + (1-canvasratio)*canvas->GetBottomMargin()-canvasratio*canvas->GetTopMargin());
  canvasratio = 0.16;
  TPad *ratioPad = new TPad("BottomPad","",0,0,1,1);
  ratioPad->SetTopMargin((1-canvasratio) - (1-canvasratio)*ratioPad->GetBottomMargin()+canvasratio*ratioPad->GetTopMargin());
  ratioPad->SetFillStyle(4000);
  ratioPad->SetFillColor(4000);
  ratioPad->SetFrameFillColor(4000);
  ratioPad->SetFrameBorderMode(0);
  ratioPad->SetGrid(1,1);
  ratioPad->Draw("HISTO");
  frame2->Draw("same HISTO");
  ratioPad->cd();
  frame2->Draw("same HISTO");
  h3->Draw("EPsame HISTO");
  h4->Draw("EPsame HISTO");


  cout << fp[0] << " +-  " << fpe[0] << "\n"
       << fp[1] << " +-  " << fpe[1] <<"\n"
       << fp[2] << " +-  " << fpe[2] << "\n"
       << fp[3] << " +-  " << fpe[3] <<"\n";
  
}
