#include "TH1F.h"
#include "TCanvas.h"
#include "TObject.h"
#include "TFile.h"
#include "TPaveStats.h"
#include "TGraphErrors.h"
#include "TGaxis.h"
#include "TROOT.h"
#include "TF1.h"
#include "TLegend.h"
#include "TKey.h"
#include "TClass.h"
#include "TPRegexp.h"
#include <TROOT.h>
#include <TStyle.h>
#include <TColor.h>
#include <TString.h>

#include "iostream"
#include "vector"
#include "math.h"
#include "map"
#include "iterator"



void  displayHybridAnalyzerMacro(TString file, int limit){
    gROOT->SetBatch(true);
    gStyle->SetOptStat(0);
    /* Open file and generate canvas */
    TFile *f;//, *fo;
    TCanvas *C;
    C = new TCanvas();
    f = new TFile(file);
    TString dir_classic = "classicAna/Digis";
    TString dir_hybrid = "hybridAna/Digis";
    f->cd();

    f->cd(dir_classic); 

    TIter nextkey(gDirectory->GetListOfKeys());
    TKey *key;

    /* Start canvas printing */
    C->Print("HybridAnalyzer.pdf[");

    int counter = 0;

    /* Loop over histograms */
    while ((key = (TKey*)nextkey())) {
        TObject *obj = key->ReadObj();
        if (counter>limit) break;
        if ( !obj->IsA()->InheritsFrom( "TH1" ) ) continue; 
        /* Loop over selection */
        bool found = false;
        int n_event = -1;
        int n_run = -1;
        int n_detid = -1;
        int idx = 0;
        /* Get the event number, detid and run info */
        TString name  = obj->GetName();

        //TPRegexp re_event("ev_\\d+");        
        //TString event = name(re_event);
        //event.Replace(0,3,"");
        //n_event = event.Atoi();

        //TPRegexp re_run("run_\\d+");        
        //TString run = name(re_run);
        //run.Replace(0,4,"");
        //n_run = run.Atoi();

        //TPRegexp re_detid("Id_\\d+");        
        //TString detid = name(re_detid);
        //detid.Replace(0,3,"");
        //n_detid = detid.Atoi();

        TString title;
        //title.Form("Run %d, Event %d, DetId %d",n_run,n_event,n_detid);
        title = obj->GetName();
        std::cout<<title<<std::endl;

        C->Clear();

        /* ZS1 histograms */
        TH1F* h_classic = (TH1F*)key->ReadObj();

        TLegend leg(0.6,0.8,0.9,0.9);
        leg.SetHeader("Legend","C");
        leg.AddEntry(h_classic,"Digis with classic ZS","l");

        h_classic->SetLineWidth(2);
        h_classic->SetLineColor(601);
        h_classic->SetTitle(title);
        h_classic->SetXTitle("StripNumber");
        h_classic->SetYTitle("Charge (ADC counts)");
        h_classic->Draw("hist p l");
        f->cd();
        TH1F* h_hybrid = (TH1F*) f->Get(dir_hybrid+"/"+obj->GetName());

        if(h_hybrid!=0){
            h_hybrid->SetLineWidth(2);
            h_hybrid->SetLineColor(634);
            leg.AddEntry(h_hybrid,"Digis with hybrid ZS","l");
            h_hybrid->Draw("hist p l same");
        }
        else
            continue;

        /* Legend and save */
        leg.Draw();
        C->Update();
        //	fo->cd();
        //	C->Write();
        C->Print("HybridAnalyzer.pdf","Title:"+title);
        //C->SaveAs(TString("img/")+obj->GetName()+TString(".png"));
        counter++;
    }
    C->Print("HybridAnalyzer.pdf");
    C->Print("HybridAnalyzer.pdf]");
}
