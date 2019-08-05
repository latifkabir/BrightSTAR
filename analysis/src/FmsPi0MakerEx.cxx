// Filename: FmsPi0Maker.C
// Description: Pi0 (two photons) Invariant Mass directly from MuDST files.
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
    //vector<StFmsPointPair*> pointPairs;
public:
    FmsPi0Maker(TString name): StMaker("FmsPi0InvMass")
    {	
	Init();
    }
    ~FmsPi0Maker()
    {
	
    }
    Int_t Init()
    {
	mDst = (StMuDst*)GetInputDS("MuDst");
	event = (StEvent*)GetInputDS("StEvent");
	cout << "Here:"<<mDst<<"\t"<<event<<endl;
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
	fmsColl = event->fmsCollection();

	if(!fmsColl)
	{
	    cout<<"No FMS data for this event"<<endl;
	    return -1;
	}

	vector<StFmsPointPair*>& pointPairs = fmsColl->pointPairs();
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
	}
    }
};			


void FmsPi0MakerEx()
{
    TStRunList::MakeFileList(16072047, 3);
    TString fileList = TStar::Config->GetFileList();
    TH1D *massDist = new TH1D("massDist","massDist",100,0.0,0.0);
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);
    muDstMaker->SetStatus("*", 0);
    muDstMaker->SetStatus("Fms*", 1);

    FmsPi0Maker *pi0Mkr = new FmsPi0Maker("Pi0Maker");
    pi0Mkr->Set1dHist(massDist);
    StEventMaker* eventMk = new StEventMaker();
    
    chain->Init();    
    chain->EventLoop();
    chain->Finish();

    massDist->Draw();
}
