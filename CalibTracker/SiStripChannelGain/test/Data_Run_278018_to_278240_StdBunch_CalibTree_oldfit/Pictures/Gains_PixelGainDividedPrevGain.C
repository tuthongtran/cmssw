void Gains_PixelGainDividedPrevGain()
{
//=========Macro generated from canvas: c1/c1,
//=========  (Thu May 23 15:27:48 2019) by ROOT version 6.12/07
   TCanvas *c1 = new TCanvas("c1", "c1,",0,0,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptTitle(0);
   c1->Range(-0.3783188,0.3394132,2.531826,4.549506);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetLogy();
   c1->SetTickx(1);
   c1->SetTicky(1);
   c1->SetLeftMargin(0.13);
   c1->SetRightMargin(0.18);
   c1->SetTopMargin(0.05);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   c1->SetFrameFillStyle(0);
   c1->SetFrameBorderMode(0);
   
   TH1D *DiffWRTPrevGainPIB__122 = new TH1D("DiffWRTPrevGainPIB__122","",250,0,2);
   DiffWRTPrevGainPIB__122->SetBinContent(126,11520);
   DiffWRTPrevGainPIB__122->SetEntries(11520);
   DiffWRTPrevGainPIB__122->SetStats(0);
   DiffWRTPrevGainPIB__122->SetLineColor(4);
   DiffWRTPrevGainPIB__122->SetLineWidth(2);
   DiffWRTPrevGainPIB__122->SetMarkerColor(4);
   DiffWRTPrevGainPIB__122->SetMarkerStyle(20);
   DiffWRTPrevGainPIB__122->GetXaxis()->SetTitle("New Gain / Previous Gain");
   DiffWRTPrevGainPIB__122->GetXaxis()->SetRange(1,251);
   DiffWRTPrevGainPIB__122->GetXaxis()->SetLabelFont(42);
   DiffWRTPrevGainPIB__122->GetXaxis()->SetLabelSize(0.035);
   DiffWRTPrevGainPIB__122->GetXaxis()->SetTitleSize(0.035);
   DiffWRTPrevGainPIB__122->GetXaxis()->SetTitleFont(42);
   DiffWRTPrevGainPIB__122->GetYaxis()->SetTitle("Number of APV");
   DiffWRTPrevGainPIB__122->GetYaxis()->SetLabelFont(42);
   DiffWRTPrevGainPIB__122->GetYaxis()->SetLabelSize(0.035);
   DiffWRTPrevGainPIB__122->GetYaxis()->SetTitleSize(0.035);
   DiffWRTPrevGainPIB__122->GetYaxis()->SetTitleOffset(1.7);
   DiffWRTPrevGainPIB__122->GetYaxis()->SetTitleFont(42);
   DiffWRTPrevGainPIB__122->GetZaxis()->SetLabelFont(42);
   DiffWRTPrevGainPIB__122->GetZaxis()->SetLabelSize(0.035);
   DiffWRTPrevGainPIB__122->GetZaxis()->SetTitleSize(0.035);
   DiffWRTPrevGainPIB__122->GetZaxis()->SetTitleFont(42);
   DiffWRTPrevGainPIB__122->Draw("HIST");
   
   TH1D *DiffWRTPrevGainPIB__123 = new TH1D("DiffWRTPrevGainPIB__123","",250,0,2);
   DiffWRTPrevGainPIB__123->SetBinContent(126,11520);
   DiffWRTPrevGainPIB__123->SetEntries(11520);
   DiffWRTPrevGainPIB__123->SetStats(0);
   DiffWRTPrevGainPIB__123->SetLineColor(4);
   DiffWRTPrevGainPIB__123->SetLineWidth(2);
   DiffWRTPrevGainPIB__123->SetMarkerColor(4);
   DiffWRTPrevGainPIB__123->SetMarkerStyle(20);
   DiffWRTPrevGainPIB__123->GetXaxis()->SetTitle("New Gain / Previous Gain");
   DiffWRTPrevGainPIB__123->GetXaxis()->SetRange(1,251);
   DiffWRTPrevGainPIB__123->GetXaxis()->SetLabelFont(42);
   DiffWRTPrevGainPIB__123->GetXaxis()->SetLabelSize(0.035);
   DiffWRTPrevGainPIB__123->GetXaxis()->SetTitleSize(0.035);
   DiffWRTPrevGainPIB__123->GetXaxis()->SetTitleFont(42);
   DiffWRTPrevGainPIB__123->GetYaxis()->SetTitle("Number of APV");
   DiffWRTPrevGainPIB__123->GetYaxis()->SetLabelFont(42);
   DiffWRTPrevGainPIB__123->GetYaxis()->SetLabelSize(0.035);
   DiffWRTPrevGainPIB__123->GetYaxis()->SetTitleSize(0.035);
   DiffWRTPrevGainPIB__123->GetYaxis()->SetTitleOffset(1.7);
   DiffWRTPrevGainPIB__123->GetYaxis()->SetTitleFont(42);
   DiffWRTPrevGainPIB__123->GetZaxis()->SetLabelFont(42);
   DiffWRTPrevGainPIB__123->GetZaxis()->SetLabelSize(0.035);
   DiffWRTPrevGainPIB__123->GetZaxis()->SetTitleSize(0.035);
   DiffWRTPrevGainPIB__123->GetZaxis()->SetTitleFont(42);
   DiffWRTPrevGainPIB__123->Draw("same HIST");
   
   TH1D *DiffWRTPrevGainPIE__124 = new TH1D("DiffWRTPrevGainPIE__124","",250,0,2);
   DiffWRTPrevGainPIE__124->SetBinContent(126,4320);
   DiffWRTPrevGainPIE__124->SetEntries(4320);
   DiffWRTPrevGainPIE__124->SetStats(0);
   DiffWRTPrevGainPIE__124->SetLineColor(2);
   DiffWRTPrevGainPIE__124->SetLineWidth(2);
   DiffWRTPrevGainPIE__124->SetMarkerColor(2);
   DiffWRTPrevGainPIE__124->SetMarkerStyle(22);
   DiffWRTPrevGainPIE__124->GetXaxis()->SetTitle("New Gain / Previous Gain");
   DiffWRTPrevGainPIE__124->GetXaxis()->SetRange(1,251);
   DiffWRTPrevGainPIE__124->GetXaxis()->SetLabelFont(42);
   DiffWRTPrevGainPIE__124->GetXaxis()->SetLabelSize(0.035);
   DiffWRTPrevGainPIE__124->GetXaxis()->SetTitleSize(0.035);
   DiffWRTPrevGainPIE__124->GetXaxis()->SetTitleFont(42);
   DiffWRTPrevGainPIE__124->GetYaxis()->SetTitle("Number of APV");
   DiffWRTPrevGainPIE__124->GetYaxis()->SetLabelFont(42);
   DiffWRTPrevGainPIE__124->GetYaxis()->SetLabelSize(0.035);
   DiffWRTPrevGainPIE__124->GetYaxis()->SetTitleSize(0.035);
   DiffWRTPrevGainPIE__124->GetYaxis()->SetTitleOffset(1.7);
   DiffWRTPrevGainPIE__124->GetYaxis()->SetTitleFont(42);
   DiffWRTPrevGainPIE__124->GetZaxis()->SetLabelFont(42);
   DiffWRTPrevGainPIE__124->GetZaxis()->SetLabelSize(0.035);
   DiffWRTPrevGainPIE__124->GetZaxis()->SetTitleSize(0.035);
   DiffWRTPrevGainPIE__124->GetZaxis()->SetTitleFont(42);
   DiffWRTPrevGainPIE__124->Draw("same HIST");
   
   TLegend *leg = new TLegend(0.59,0.83,0.79,0.93,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("DiffWRTPrevGainPIB","PIB","L");
   entry->SetLineColor(4);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("DiffWRTPrevGainPIE","PIE","L");
   entry->SetLineColor(2);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   
   TPaveText *pt = new TPaveText(0.6,0.61,0.75,0.7,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetLineColor(4);
   pt->SetTextAlign(12);
   pt->SetTextColor(4);
   TText *pt_LaTex = pt->AddText("Entries : 11520
");
   pt_LaTex = pt->AddText("Mean    :   1.00
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
   pt_LaTex = pt->AddText("Entries : 4320
");
   pt_LaTex = pt->AddText("Mean    :   1.00
");
   pt_LaTex = pt->AddText("RMS     :   0.00
");
   pt->Draw();
   
   pt = new TPaveText(0.28,0.95,0.85,0.98,"brNDC");
   pt->SetFillColor(0);
   pt->SetTextAlign(11);
   pt_LaTex = pt->AddText("CMS Preliminary 2017  -  Particle Gain");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
