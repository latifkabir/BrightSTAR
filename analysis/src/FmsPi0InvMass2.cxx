// Filename: FmsPi0InvMass2.C
// Description: Pi0 (two photons) Invariant Mass directly from MuDST files.
// Author: Latif Kabir < latif@jlab.org >
// Created: Mon May 20 15:04:13 2019 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"

void FmsPi0InvMass2()
{
    TStRunList::MakeFileList(16072047, 3);
    TString fileList = TStar::Config->GetFileList();
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "");
    muDstMaker->SetStatus("*",0);
    muDstMaker->SetStatus("Fms*",1);
    StFmsCollection *fmsColl;
    
    StMuDst *mDst = muDstMaker->muDst();   // Get StMuDst 
    StMuFmsHit *fmsHit;
    StMuFmsCluster *fmsCluster;
    StMuFmsPoint *fmsPoint;

    
    TChain *ch = muDstMaker->chain();
    TH1F *massDist = new TH1F("massDist","massDist",100,0.0,0.0);
    int iEvent = 0;
    //vector<StFmsPointPair*> pointPairs;
    Int_t nPairs;
    Double_t pairE;
    Double_t pairM;
    StEvent* event; 
    
    chain->Init();
    
    Int_t nEvents = ch->GetEntries(); 
    
    for (Int_t iev = 0; iev < nEvents; iev++)
    {
	chain->Clear();
	Int_t iret = chain->Make(iev);
	event = mDst->createStEvent();
	fmsColl = event->fmsCollection();

	if(!fmsColl)
	{
	    cout<<"No FMS data for this event"<<endl;
	    continue;
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

	    massDist->Fill(pairM);
	}
    }
    chain->Finish();
    massDist->Draw();
}			
