void Gains_PixelChargeAndStat()
{
//=========Macro generated from canvas: c1/c1,
//=========  (Thu May 23 15:27:47 2019) by ROOT version 6.12/07
   TCanvas *c1 = new TCanvas("c1", "c1,",1,1,600,576);
   gStyle->SetOptFit(1);
   gStyle->SetOptTitle(0);
   c1->Range(-151.3828,-0.1235294,1013.1,1.111765);
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
   
   TH1D *ChargePIE__91 = new TH1D("ChargePIE__91","",687,0,4000);
   ChargePIE__91->SetStats(0);
   ChargePIE__91->SetLineColor(2);
   ChargePIE__91->SetLineWidth(2);
   ChargePIE__91->SetMarkerColor(2);
   ChargePIE__91->SetMarkerStyle(22);
   ChargePIE__91->GetXaxis()->SetTitle("Charge [ADC/mm]");
   ChargePIE__91->GetXaxis()->SetRange(1,138);
   ChargePIE__91->GetXaxis()->SetLabelFont(42);
   ChargePIE__91->GetXaxis()->SetLabelSize(0.035);
   ChargePIE__91->GetXaxis()->SetTitleSize(0.035);
   ChargePIE__91->GetXaxis()->SetTitleFont(42);
   ChargePIE__91->GetYaxis()->SetTitle("Number of Clusters");
   ChargePIE__91->GetYaxis()->SetLabelFont(42);
   ChargePIE__91->GetYaxis()->SetLabelSize(0.035);
   ChargePIE__91->GetYaxis()->SetTitleSize(0.035);
   ChargePIE__91->GetYaxis()->SetTitleOffset(1.7);
   ChargePIE__91->GetYaxis()->SetTitleFont(42);
   ChargePIE__91->GetZaxis()->SetLabelFont(42);
   ChargePIE__91->GetZaxis()->SetLabelSize(0.035);
   ChargePIE__91->GetZaxis()->SetTitleSize(0.035);
   ChargePIE__91->GetZaxis()->SetTitleFont(42);
   ChargePIE__91->Draw("");
   
   TH1D *ChargePIB__92 = new TH1D("ChargePIB__92","",687,0,4000);
   ChargePIB__92->SetStats(0);
   ChargePIB__92->SetLineColor(4);
   ChargePIB__92->SetLineWidth(2);
   ChargePIB__92->SetMarkerColor(4);
   ChargePIB__92->SetMarkerStyle(20);
   ChargePIB__92->GetXaxis()->SetTitle("Charge [ADC/mm]");
   ChargePIB__92->GetXaxis()->SetRange(1,138);
   ChargePIB__92->GetXaxis()->SetLabelFont(42);
   ChargePIB__92->GetXaxis()->SetLabelSize(0.035);
   ChargePIB__92->GetXaxis()->SetTitleSize(0.035);
   ChargePIB__92->GetXaxis()->SetTitleFont(42);
   ChargePIB__92->GetYaxis()->SetTitle("Number of Clusters");
   ChargePIB__92->GetYaxis()->SetLabelFont(42);
   ChargePIB__92->GetYaxis()->SetLabelSize(0.035);
   ChargePIB__92->GetYaxis()->SetTitleSize(0.035);
   ChargePIB__92->GetYaxis()->SetTitleOffset(1.7);
   ChargePIB__92->GetYaxis()->SetTitleFont(42);
   ChargePIB__92->GetZaxis()->SetLabelFont(42);
   ChargePIB__92->GetZaxis()->SetLabelSize(0.035);
   ChargePIB__92->GetZaxis()->SetTitleSize(0.035);
   ChargePIB__92->GetZaxis()->SetTitleFont(42);
   ChargePIB__92->Draw("same");
   
   TH1D *ChargePIE__93 = new TH1D("ChargePIE__93","",687,0,4000);
   ChargePIE__93->SetStats(0);
   ChargePIE__93->SetLineColor(2);
   ChargePIE__93->SetLineWidth(2);
   ChargePIE__93->SetMarkerColor(2);
   ChargePIE__93->SetMarkerStyle(22);
   ChargePIE__93->GetXaxis()->SetTitle("Charge [ADC/mm]");
   ChargePIE__93->GetXaxis()->SetRange(1,138);
   ChargePIE__93->GetXaxis()->SetLabelFont(42);
   ChargePIE__93->GetXaxis()->SetLabelSize(0.035);
   ChargePIE__93->GetXaxis()->SetTitleSize(0.035);
   ChargePIE__93->GetXaxis()->SetTitleFont(42);
   ChargePIE__93->GetYaxis()->SetTitle("Number of Clusters");
   ChargePIE__93->GetYaxis()->SetLabelFont(42);
   ChargePIE__93->GetYaxis()->SetLabelSize(0.035);
   ChargePIE__93->GetYaxis()->SetTitleSize(0.035);
   ChargePIE__93->GetYaxis()->SetTitleOffset(1.7);
   ChargePIE__93->GetYaxis()->SetTitleFont(42);
   ChargePIE__93->GetZaxis()->SetLabelFont(42);
   ChargePIE__93->GetZaxis()->SetLabelSize(0.035);
   ChargePIE__93->GetZaxis()->SetTitleSize(0.035);
   ChargePIE__93->GetZaxis()->SetTitleFont(42);
   ChargePIE__93->Draw("same");
   
   TLegend *leg = new TLegend(0.59,0.83,0.79,0.93,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("ChargePIB","PIB","L");
   entry->SetLineColor(4);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("ChargePIE","PIE","L");
   entry->SetLineColor(2);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   
   TPaveText *pt = new TPaveText(0.28,0.95,0.85,0.98,"brNDC");
   pt->SetFillColor(0);
   pt->SetTextAlign(11);
   TText *pt_LaTex = pt->AddText("CMS Preliminary 2017  -  Particle Gain");
   pt->Draw();
   TLine *line = new TLine(300,0,300,0);
   line->SetLineStyle(2);
   line->SetLineWidth(3);
   line->Draw();
   
   pt = new TPaveText(0.6,0.61,0.75,0.7,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetLineColor(4);
   pt->SetTextAlign(12);
   pt->SetTextColor(4);
   pt_LaTex = pt->AddText("Entries : 0
");
   pt_LaTex = pt->AddText("Mean    :   0.00
");
   pt_LaTex = pt->AddText("RMS     :   0.00
");
   pt->Draw();
   
   pt = new TPaveText(0.6,0.52,0.75,0.61,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetLineColor(2);
   pt->SetTextAlign(12);
   pt->SetTextColor(2);
   pt_LaTex = pt->AddText("Entries : 0
");
   pt_LaTex = pt->AddText("Mean    :   0.00
");
   pt_LaTex = pt->AddText("RMS     :   0.00
");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
