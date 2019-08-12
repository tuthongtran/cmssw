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

#include "iostream"
#include "vector"
#include "math.h"
#include "map"
//#include "iterator"
#include "iterator"



void  displayBaselineMacro(TString file, int limit){
    gStyle->SetOptStat(0);
    /* Select particular detId and event numbers */
    //std::map<int,int> selection_run_321054; // <event,detid>
    std::vector<pair<int,int>> selection_run_321054;
    //selection_run_321054.insert(std::make_pair(107,369124773));
    //selection_run_321054.insert(std::make_pair(18,470426026));
    //selection_run_321054.insert(std::make_pair(22,436228470));
    ////selection_run_321054.insert(std::make_pair(9,369121610)); // TEST, NOT A BAD BASELINE
    ////selection_run_321054.insert(std::make_pair(9,369120437)); // TEST, NOT A BAD BASELINE
    //selection_run_321054.insert(std::make_pair(23,470422441)); // TEST, NOT A BAD BASELINE
    //selection_run_321054.insert(std::make_pair(23,470422512)); // TEST, NOT A BAD BASELINE
    //selection_run_321054.insert(std::make_pair(23,470341157)); // TEST, NOT A BAD BASELINE
    //selection_run_321054.insert(std::make_pair(23,470442980)); // TEST, NOT A BAD BASELINE
    selection_run_321054.push_back(std::make_pair(23,470422441)); // TEST, NOT A BAD BASELINE
    selection_run_321054.push_back(std::make_pair(23,470422512)); // TEST, NOT A BAD BASELINE
    selection_run_321054.push_back(std::make_pair(23,470341157)); // TEST, NOT A BAD BASELINE
    selection_run_321054.push_back(std::make_pair(23,470442980)); // TEST, NOT A BAD BASELINE

    /* Open file and generate canvas */
    TFile *f;//, *fo;
    TString BaseDir;
    TString dir_ZS1[4];
    TString dir_ZS2[4];
    TString fullPath, title, subDet, genSubDet;
    TCanvas *C;
    C = new TCanvas();
    f = new TFile(file);
    //    fo = new TFile(ofile, "RECREATE");
    //BaseDir="DQMData/Results/SpyChannel/";
    dir_ZS1[0]="baselineAnalyzerZS1/ProcessedRawDigis";
    dir_ZS1[1]="baselineAnalyzerZS1/Baseline";
    dir_ZS1[2]="baselineAnalyzerZS1/Clusters";
    dir_ZS1[3]="baselineAnalyzerZS1/RawDigis";
    dir_ZS2[0]="baselineAnalyzerZS2/ProcessedRawDigis";
    dir_ZS2[1]="baselineAnalyzerZS2/Baseline";
    dir_ZS2[2]="baselineAnalyzerZS2/Clusters";
    dir_ZS2[3]="baselineAnalyzerZS2/RawDigis";
    f->cd();
    //	fo->Write();
    //	fo->Close();
    f->cd(dir_ZS1[0]); // Go inside ProcessedRawDigis

    TIter nextkey(gDirectory->GetListOfKeys());
    TKey *key;
    int objcounter=1;
    int histolimit =0;
    
    /* Start canvas printing */
    C->Print("Baseline.pdf[");

    /* Loop over histograms */
    while ((key = (TKey*)nextkey())) {
        if(histolimit< limit || limit==-1){ // If limit = -1, loop over all histograms
            histolimit++;
            TObject *obj = key->ReadObj();
            std::cout<<obj->GetName()<<std::endl;

            if ( obj->IsA()->InheritsFrom( "TH1" ) ) {
                /* Loop over selection */
                bool found = false;
                int n_event = -1;
                int n_run = -1;
                int n_detid = -1;
                
                for (auto const& it : selection_run_321054)
                {
                    /* Get the event number, detid and run info */
                    TString name  = obj->GetName();

                    TPRegexp re_event("ev:\\d+");        
                    TString event = name(re_event);
                    event.Replace(0,3,"");
                    n_event = event.Atoi();

                    TPRegexp re_run("run:\\d+");        
                    TString run = name(re_run);
                    run.Replace(0,4,"");
                    n_run = run.Atoi();

                    TPRegexp re_detid("Id:\\d+");        
                    TString detid = name(re_detid);
                    detid.Replace(0,3,"");
                    n_detid = detid.Atoi();
                    //std::cout<<n_event<<" "<<n_detid<<" "<<n_run<<std::endl;
                    if (it.first==n_event && it.second==n_detid) 
                    {
                        found = true;
                        break;
                    }
                }
                if (!found) continue;
                TString title;
                title.Form("Run %d, Event %d, DetId %d",n_run,n_event,n_run);
                std::cout<<title<<std::endl;

                //std::cout << "Found object n: " << objcounter << " Name: " << obj->GetName() << " Title: " << obj->GetTitle()<< std::endl;
                //std::cout<<n_event<<" "<<n_detid<<" "<<n_run<<std::endl;

                ++objcounter;

                C->Clear();

                /* ZS1 histograms */
                TH1F* h = (TH1F*)key->ReadObj();

                TLegend leg(0.5,0.7,0.9,0.9,"Legend");
                //leg.AddEntry(h,"VR - Ped - apvCM_{mean}","lep");
                leg.AddEntry(h,"Processed digis with hybrid ZS","lep");

                h->SetLineWidth(2);
                h->SetTitle(title);
                h->SetXTitle("StripNumber");
                h->SetYTitle("Charge (ADC counts)");
                h->Draw("hist p l");
                f->cd();
                TH1F* hb = (TH1F*) f->Get(dir_ZS1[1]+"/"+obj->GetName());

                if(hb!=0){
                    hb->SetLineWidth(2);
                    hb->SetLineStyle(2);
                    hb->SetLineColor(2);
                    leg.AddEntry(hb,"Baseline","lep");
                    hb->Draw("hist p l same");
                }

                f->cd();
                TH1F* hc = (TH1F*) f->Get(dir_ZS1[2]+"/"+obj->GetName());

                if(hc!=0){
                    hc->SetLineWidth(2);
                    hc->SetLineStyle(2);
                    hc->SetLineColor(3);
                    leg.AddEntry(hb,"Clusters","lep");
                    hc->Draw("hist p l same");
                }
                TH1F* hd = (TH1F*) f->Get(dir_ZS1[3]+"/"+obj->GetName());

                if(hd!=0){
                    hd->SetLineWidth(2);
                    hd->SetLineStyle(2);
                    hd->SetLineColor(6);
                    leg.AddEntry(hb,"Raw digis","lep");
                    hd->Draw("hist p l same");
                }

                else
                    std::cout << "not found " << obj->GetName()<< std::endl;


                /* ZS2 histograms */
                f->cd();
                TH1F* h2 = (TH1F*) f->Get(dir_ZS2[0]+"/"+obj->GetName());
                if (h2!=0){
                    h2->SetLineWidth(2);
                    h2->SetLineStyle(2);
                    h2->SetLineColor(7);
                    leg.AddEntry(h2,"Processed digis with classical ZS","lep");
                    h2->Draw("hist p l same");
                }

                leg.Draw();


                C->Update();
                //	fo->cd();
                //	C->Write();

                C->Print("Baseline.pdf","Title:"+title);
                //C->SaveAs(TString("img/")+obj->GetName()+TString(".png"));


            }
        }  
    }
    C->Print("Baseline.pdf");
    C->Print("Baseline.pdf]");
}
