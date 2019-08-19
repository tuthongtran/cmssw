void Gains_PixelGainVsPrevGain()
{
//=========Macro generated from canvas: c1/c1,
//=========  (Thu May 23 15:27:49 2019) by ROOT version 6.12/07
   TCanvas *c1 = new TCanvas("c1", "c1,",0,0,600,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptTitle(0);
   c1->Range(0.2513044,0.3447059,2.164348,1.897647);
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
   
   TH2D *GainVsPrevGainPIB__130 = new TH2D("GainVsPrevGainPIB__130","",100,0,2,100,0,2);
   GainVsPrevGainPIB__130->SetBinContent(5253,11520);
   GainVsPrevGainPIB__130->SetEntries(11520);
   GainVsPrevGainPIB__130->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#1fff00");
   GainVsPrevGainPIB__130->SetLineColor(ci);
   GainVsPrevGainPIB__130->SetMarkerColor(4);
   GainVsPrevGainPIB__130->SetMarkerStyle(20);
   GainVsPrevGainPIB__130->SetMarkerSize(0.3);
   GainVsPrevGainPIB__130->GetXaxis()->SetTitle("Previous Gain");
   GainVsPrevGainPIB__130->GetXaxis()->SetRange(26,91);
   GainVsPrevGainPIB__130->GetXaxis()->SetLabelFont(42);
   GainVsPrevGainPIB__130->GetXaxis()->SetLabelSize(0.035);
   GainVsPrevGainPIB__130->GetXaxis()->SetTitleSize(0.035);
   GainVsPrevGainPIB__130->GetXaxis()->SetTitleFont(42);
   GainVsPrevGainPIB__130->GetYaxis()->SetTitle("New Gain");
   GainVsPrevGainPIB__130->GetYaxis()->SetRange(26,91);
   GainVsPrevGainPIB__130->GetYaxis()->SetLabelFont(42);
   GainVsPrevGainPIB__130->GetYaxis()->SetLabelSize(0.035);
   GainVsPrevGainPIB__130->GetYaxis()->SetTitleSize(0.035);
   GainVsPrevGainPIB__130->GetYaxis()->SetTitleOffset(1.6);
   GainVsPrevGainPIB__130->GetYaxis()->SetTitleFont(42);
   GainVsPrevGainPIB__130->GetZaxis()->SetLabelFont(42);
   GainVsPrevGainPIB__130->GetZaxis()->SetLabelSize(0.035);
   GainVsPrevGainPIB__130->GetZaxis()->SetTitleSize(0.035);
   GainVsPrevGainPIB__130->GetZaxis()->SetTitleFont(42);
   GainVsPrevGainPIB__130->Draw("");
   
   TH2D *GainVsPrevGainPIE__131 = new TH2D("GainVsPrevGainPIE__131","",100,0,2,100,0,2);
   GainVsPrevGainPIE__131->SetBinContent(5253,4320);
   GainVsPrevGainPIE__131->SetEntries(4320);
   GainVsPrevGainPIE__131->SetStats(0);

   ci = TColor::GetColor("#1fff00");
   GainVsPrevGainPIE__131->SetLineColor(ci);
   GainVsPrevGainPIE__131->SetMarkerColor(2);
   GainVsPrevGainPIE__131->SetMarkerStyle(22);
   GainVsPrevGainPIE__131->SetMarkerSize(0.3);
   GainVsPrevGainPIE__131->GetXaxis()->SetTitle("Previous Gain");
   GainVsPrevGainPIE__131->GetXaxis()->SetRange(26,91);
   GainVsPrevGainPIE__131->GetXaxis()->SetLabelFont(42);
   GainVsPrevGainPIE__131->GetXaxis()->SetLabelSize(0.035);
   GainVsPrevGainPIE__131->GetXaxis()->SetTitleSize(0.035);
   GainVsPrevGainPIE__131->GetXaxis()->SetTitleFont(42);
   GainVsPrevGainPIE__131->GetYaxis()->SetTitle("New Gain");
   GainVsPrevGainPIE__131->GetYaxis()->SetRange(26,91);
   GainVsPrevGainPIE__131->GetYaxis()->SetLabelFont(42);
   GainVsPrevGainPIE__131->GetYaxis()->SetLabelSize(0.035);
   GainVsPrevGainPIE__131->GetYaxis()->SetTitleSize(0.035);
   GainVsPrevGainPIE__131->GetYaxis()->SetTitleOffset(1.6);
   GainVsPrevGainPIE__131->GetYaxis()->SetTitleFont(42);
   GainVsPrevGainPIE__131->GetZaxis()->SetLabelFont(42);
   GainVsPrevGainPIE__131->GetZaxis()->SetLabelSize(0.035);
   GainVsPrevGainPIE__131->GetZaxis()->SetTitleSize(0.035);
   GainVsPrevGainPIE__131->GetZaxis()->SetTitleFont(42);
   GainVsPrevGainPIE__131->Draw(" same");
   TLine *line = new TLine(0.5,0.5,1.8,1.8);
   line->SetLineStyle(2);
   line->SetLineWidth(3);
   line->Draw();
   
   TLegend *leg = new TLegend(0.59,0.83,0.79,0.93,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("GainVsPrevGainPIB","PIB","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(4);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1.3);
   entry->SetTextFont(42);
   entry=leg->AddEntry("GainVsPrevGainPIE","PIE","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(2);
   entry->SetMarkerStyle(22);
   entry->SetMarkerSize(1.3);
   entry->SetTextFont(42);
   leg->Draw();
   
   TPaveText *pt = new TPaveText(0.15,0.9,0.3,0.93,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetLineColor(4);
   pt->SetTextAlign(12);
   pt->SetTextColor(4);
   TText *pt_LaTex = pt->AddText("Entries : 11520
");
   pt->Draw();
   
   pt = new TPaveText(0.15,0.87,0.3,0.9,"brNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetLineColor(2);
   pt->SetTextAlign(12);
   pt->SetTextColor(2);
   pt_LaTex = pt->AddText("Entries : 4320
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
