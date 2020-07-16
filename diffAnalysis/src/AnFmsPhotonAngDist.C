// Filename: AnFmsPhotonAngDist.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jul 16 17:35:56 2020 (-0400)
// URL: jlab.org/~latif


#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"

class FmsPhotonAngDistMaker: public StMaker
{
    StMuDst *mDst;   // Get StMuDst
    StEvent* event;
    StFmsDbMaker* mFmsDbMk;
    TString name;
    StFmsCollection *fmsColl;    
    StMuFmsHit *fmsHit;
    StMuFmsCluster *fmsCluster;
    StMuFmsPoint *fmsPoint;
    Int_t nPoints;
    Double_t pointEta;
    Double_t pointPhi;
    TH1D *hist1d[2];
    TH2D *hist2d;
    vector<StFmsPoint*> points;
    
public:
    FmsPhotonAngDistMaker(TString name): StMaker(name)
    {	
	Init();
    }    
    ~FmsPhotonAngDistMaker()
    {
	
    }
    Int_t Init()
    {
    	mFmsDbMk = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
	if (!mFmsDbMk) { LOG_ERROR <<"TStFmsTreeMaker::InitRun - !StFmsDbMaker" <<endl; return kStFatal; }

	return kStOk;	
    }
    void Set1dHist(TH1D *h1d, Int_t index)
    {
	hist1d[index] = h1d;	
    }
    void Set2dHist(TH2D *h2d)
    {
	hist2d = h2d;	
    }
    Int_t Make()
    {
	mDst = (StMuDst*)GetInputDS("MuDst");
	event = (StEvent*)GetInputDS("StEvent");	

	if (!mDst || !event)
	{
	    cout <<"TStFmsTreeMaker::Make - !MuDst or !StEvent" <<endl;
	    return kStErr;
	}
	
	fmsColl = (StFmsCollection*)event->fmsCollection();

	if(!fmsColl)
	{
	    cout<<"No FMS data for this event"<<endl;
	    return kStSkip;
	}

        points = fmsColl->points();
        nPoints = fmsColl->numberOfPairs();
	for (Int_t i = 0; i < nPoints; ++i)
	{
	    StFmsPoint* point = points[i];
	    pointEta = point->XYZ().pseudoRapidity();
	    pointPhi = point->XYZ().phi();	      

	    hist1d[0]->Fill(pointEta);
	    hist1d[1]->Fill(pointPhi);
	    hist2d->Fill(point->XYZ().x(), point->XYZ().y());
	}
	return kStOk;
    }
};			

void AnFmsPhotonAngDist(TString fileList)
{
    // TStRunList::MakeFileList(16072057, 1);
    // TString fileList = TStar::Config->GetFileList();

    TFile *f = new TFile("FmsPi0Reconst.root", "RECREATE");
    TH1D *etaDist = new TH1D("etaDist","Eta Dist; #eta", 100, 2, 4.5);
    TH1D *phiDist = new TH1D("phiDist","Phi Dist; #phi [rad]",100, -3.2, 3.2);
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
    // FPS
    StFmsFpsMaker * fpsMk = new StFmsFpsMaker();
    fpsMk->setReadMuDST(1);      // (1) // (reverse 0<->1 convention as FmsHitMaker and FmsPointMaker)
    
    FmsPhotonAngDistMaker *pi0Mkr = new FmsPhotonAngDistMaker("Pi0Maker");
    pi0Mkr->Set1dHist(etaDist);
    pi0Mkr->Set2dHist(xyDist);
    
    chain->Init();    
    chain->EventLoop(1000);  // Run specified number of events
    chain->Finish();

    TCanvas *c1 = new TCanvas();
    etaDist->Draw();
    TCanvas *c2 = new TCanvas();
    xyDist->Draw("colz");
    f->Write();
}


