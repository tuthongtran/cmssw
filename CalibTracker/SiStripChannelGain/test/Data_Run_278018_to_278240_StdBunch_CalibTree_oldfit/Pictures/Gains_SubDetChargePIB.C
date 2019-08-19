void Gains_SubDetChargePIB()
{
//=========Macro generated from canvas: c1/c1,
//=========  (Thu May 23 15:27:02 2019) by ROOT version 6.12/07
   TCanvas *c1 = new TCanvas("c1", "c1,",0,0,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptTitle(0);
   c1->Range(-753.6232,-0.1235294,5043.478,1.111765);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.13);
   c1->SetRightMargin(0.18);
   c1->SetTopMargin(0.05);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   
   TH1D *ChargePIB__1 = new TH1D("ChargePIB__1","ChargePIB",687,0,4000);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *ptstats_LaTex = ptstats->AddText("Mean  =      0");
   ptstats_LaTex = ptstats->AddText("Std Dev   =      0");
   ptstats->SetOptStat(1100);
   ptstats->SetOptFit(111);
   ptstats->Draw();
   ChargePIB__1->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(ChargePIB__1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#1fff00");
   ChargePIB__1->SetLineColor(ci);
   ChargePIB__1->GetXaxis()->SetLabelFont(42);
   ChargePIB__1->GetXaxis()->SetLabelSize(0.035);
   ChargePIB__1->GetXaxis()->SetTitleSize(0.035);
   ChargePIB__1->GetXaxis()->SetTitleFont(42);
   ChargePIB__1->GetYaxis()->SetLabelFont(42);
   ChargePIB__1->GetYaxis()->SetLabelSize(0.035);
   ChargePIB__1->GetYaxis()->SetTitleSize(0.035);
   ChargePIB__1->GetYaxis()->SetTitleOffset(0);
   ChargePIB__1->GetYaxis()->SetTitleFont(42);
   ChargePIB__1->GetZaxis()->SetLabelFont(42);
   ChargePIB__1->GetZaxis()->SetLabelSize(0.035);
   ChargePIB__1->GetZaxis()->SetTitleSize(0.035);
   ChargePIB__1->GetZaxis()->SetTitleFont(42);
   ChargePIB__1->Draw("");
   
   TF1 *MyLandau1 = new TF1("MyLandau","landau",0,5400, TF1::EAddToList::kDefault);
   MyLandau1->SetFillColor(19);
   MyLandau1->SetFillStyle(0);
   MyLandau1->SetMarkerStyle(20);
   MyLandau1->SetLineColor(2);
   MyLandau1->SetLineWidth(3);
   MyLandau1->GetXaxis()->SetLabelFont(42);
   MyLandau1->GetXaxis()->SetLabelOffset(0.007);
   MyLandau1->GetXaxis()->SetLabelSize(0.05);
   MyLandau1->GetXaxis()->SetTitleOffset(1.1);
   MyLandau1->GetXaxis()->SetTitleFont(42);
   MyLandau1->GetYaxis()->SetLabelFont(42);
   MyLandau1->GetYaxis()->SetLabelOffset(0.007);
   MyLandau1->GetYaxis()->SetLabelSize(0.05);
   MyLandau1->GetYaxis()->SetTitleOffset(1.35);
   MyLandau1->GetYaxis()->SetTitleFont(42);
   MyLandau1->SetParameter(0,0);
   MyLandau1->SetParError(0,0);
   MyLandau1->SetParLimits(0,0,0);
   MyLandau1->SetParameter(1,300);
   MyLandau1->SetParError(1,0);
   MyLandau1->SetParLimits(1,0,0);
   MyLandau1->SetParameter(2,0);
   MyLandau1->SetParError(2,0);
   MyLandau1->SetParLimits(2,0,0);
   MyLandau1->Draw("same");
   
   TPaveText *pt = new TPaveText(0.28,0.95,0.85,0.98,"brNDC");
   pt->SetFillColor(0);
   pt->SetTextAlign(11);
   TText *pt_LaTex = pt->AddText("CMS Preliminary 2017  -  Particle Gain");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
