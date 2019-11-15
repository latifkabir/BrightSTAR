// Filename: RunFmsAnTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:25:43 2019 (-0500)
// URL: jlab.org/~latif

void RunFmsAnTreeMaker()
{
    TStRunList::MakeFileList(16072057, 1);
    TString fileList = TStar::Config->GetFileList();

    TFile *f = new TFile("FmsPi0Reconst.root", "RECREATE");
    TH1D *massDist = new TH1D("massDist","#pi^{0} invariant mass [GeV]; M_{#pi^{0}} [GeV]",200,0.0,1.0);
    TH2D *xyDist = new TH2D("xyDist", "xyDist", 200, -100.0, 100.0, 200, -100.0, 100.0);
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);
    St_db_Maker* dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
    StFmsDbMaker* fmsDb = new StFmsDbMaker("fmsDb");
    
    muDstMaker->SetStatus("*", 0);
    muDstMaker->SetStatus("Fms*", 1);
    muDstMaker->SetStatus("MuEvent*", 1);
    StEventMaker* eventMk = new StEventMaker();
    StFmsHitMaker*   fmsHitMk   = new StFmsHitMaker();
    StFmsPointMaker* fmsPointMk = new StFmsPointMaker();
    
    TStFmsRpTreeMaker *pi0Mkr = new TStFmsRpTreeMaker("Pi0Maker");
    pi0Mkr->Set1dHist(massDist);
    pi0Mkr->Set2dHist(xyDist);
    
    chain->Init();    
    chain->EventLoop(1000);  // Run specified number of events
    chain->Finish();

    TCanvas *c1 = new TCanvas();
    massDist->Draw();
    TCanvas *c2 = new TCanvas();
    xyDist->Draw("colz");
    f->Write();
}
