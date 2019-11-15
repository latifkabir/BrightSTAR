// Filename: FmsPi0Maker.C
// Description: Pi0 (two photons) Invariant Mass from on the fly recoonstruction.
// Author: Latif Kabir < latif@jlab.org >
// Created: Mon May 20 15:04:13 2019 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"

class FmsPi0Maker: public StMaker
{
    StMuDst *mDst;   // Get StMuDst
    StEvent* event;
    StFmsDbMaker*    mFmsDbMk;
    TString name;
    StFmsCollection *fmsColl;    
    StMuFmsHit *fmsHit;
    StMuFmsCluster *fmsCluster;
    StMuFmsPoint *fmsPoint;
    Int_t nPairs;
    Double_t pairE;
    Double_t pairM;
    TH1D *hist1d;
    TH2D *hist2d;
    vector<StFmsPointPair*> pointPairs;
public:
    FmsPi0Maker(TString name): StMaker(name)
    {	
	Init();
    }
    ~FmsPi0Maker()
    {
	
    }
    Int_t Init()
    {
    	mFmsDbMk = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    if (!mFmsDbMk) { LOG_ERROR <<"TStFmsTreeMaker::InitRun - !StFmsDbMaker" <<endl; return kStFatal; }

    return kStOk;
	
    }
    void Set1dHist(TH1D *h1d)
    {
	hist1d = h1d;	
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

        pointPairs = fmsColl->pointPairs();
        nPairs = fmsColl->numberOfPointPairs();
	for (Int_t i = 0; i < nPairs; ++i)
	{
	    StFmsPointPair* pair = pointPairs[i];
	    if (pair->energy() < 20.) continue;
	    if (pair->mass()   > 1.0) continue;
	    if (pair->zgg()    > 0.7) continue;

	    //Single point clusters only
	    if (pair->point(0)->nParentClusterPhotons() != 1 ||
		pair->point(1)->nParentClusterPhotons() != 1) continue;

	    pairE = pair->energy();
	    pairM = pair->mass();	      

	    hist1d->Fill(pairM);
	    hist2d->Fill(pair->x(), pair->y());
	}
	return kStOk;
    }
};			


void FmsPi0MakerEx()
{
    TStRunList::MakeFileList(16072057, 1);
    TString fileList = TStar::Config->GetFileList();

    TFile *f = new TFile("FmsPi0Reconst.root", "RECREATE");
    TH1D *massDist = new TH1D("massDist","#pi_{0} invariant mass [GeV]; M_{#pi_{0}} [GeV]",200,0.0,1.0);
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
    
    FmsPi0Maker *pi0Mkr = new FmsPi0Maker("Pi0Maker");
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
