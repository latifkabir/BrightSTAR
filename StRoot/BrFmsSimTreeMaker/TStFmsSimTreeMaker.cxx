// Filename: TStFmsSimTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStFmsSimTreeMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuFmsCollection.h"
#include "StMuDSTMaker/COMMON/StMuFmsPoint.h"
#include "StSpinPool/StFmsTriggerMaker/StFmsTriggerMaker.h"


ClassImp(TStFmsSimTreeMaker)

//_____________________________________________________________________________ 
TStFmsSimTreeMaker::TStFmsSimTreeMaker(const char *name):StMaker(name)
{
    //

    //FMS Buffer
    mFmsPointArray = new TClonesArray("TStFmsPointData");
    mNevents = 0;    
}

//_____________________________________________________________________________ 
TStFmsSimTreeMaker::~TStFmsSimTreeMaker()
{
    //
}


//_____________________________________________________________________________ 
Int_t TStFmsSimTreeMaker::Init()
{
    mFile = new TFile(mOutName, "RECREATE");
    mTree = new TTree("T", "Sim Tree");

    mTree->Branch("runId", &mRunId, "runId");
    mTree->Branch("eventId", &mEventId, "eventId");
    mTree->Branch("trigBit", &mTriggerBit, "trigBit");
    mTree->Branch("fmsPoint", &mFmsPointArray, 256000, 99);

    mPointPhi_trg = new TH1D("pointPhi_trg", "FMS Point Phi for Triggered Events", 100, -3.2, 3.2);
    mPointPhi_notrg = new TH1D("pointPhi_notrg", "FMS Point Phi for all events (including no trigger)", 100, -3.2, 3.2);
    mPointXY_trg = new TH2D("pointXY_trg", "FMS point XY for triggered events", 100, -100, 100, 100, -100, 100);
    mPointXY_notrg = new TH2D("pointXY_notrg", "FMS point XY for all events (including no trigger) ", 100, -100, 100, 100, -100, 100);
    
    return kStOK;
}


//_____________________________________________________________________________
Int_t TStFmsSimTreeMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");
    
    // Check if mMuDst or mEvent is valid
    if(!mMuDst)
    {
	LOG_ERROR << "TSt<Template>Maker::Make - No MuDst found" <<endm;
	return kStFatal;
    }
    if(mNevents % 1000 == 0)
	cout << "Events Processed: " << mNevents <<endl;

    //---------------------- FMS Trigger Simulator ---------------------------------------
    StFmsTriggerMaker* fmstrig = (StFmsTriggerMaker*)GetMakerInheritsFrom("StFmsTriggerMaker");
    assert(fmstrig);


    Int_t trg1 = 0;                                                                                                           
    Int_t trg2 = 0; 
    Int_t trg3 = 0; 
    Int_t trg4 = 0; 
    Int_t trg5 = 0; 
    Int_t trg6 = 0;
    Int_t trg7 = 0; 
    Int_t trg8 = 0; 
    Int_t trg9 = 0;

    if( fmstrig->FmsSmallClusterTh0())  trg1 = 1;
    if( fmstrig->FmsSmallClusterTh1())  trg2 = 1;
    if( fmstrig->FmsSmallClusterTh2())  trg3 = 1;
    if( fmstrig->FmsLargeClusterTh0())  trg4 = 1;
    if( fmstrig->FmsLargeClusterTh1())  trg5 = 1;
    if( fmstrig->FmsLargeClusterTh2())  trg6 = 1;
    if( fmstrig->FmsJetPatchTh0())  trg7 = 1;
    if( fmstrig->FmsJetPatchTh1())  trg8 = 1;
    if( fmstrig->FmsJetPatchTh2())  trg9 = 1;
    fmstrig->Clear();


    mTriggerBit = 1000000000 + trg1*100000000 + trg2*10000000 + trg3*1000000 + trg4*100000 + trg5*10000 + trg6*1000 + trg7*100 + trg8*10 + trg9;

    cout<<" TRIGGER : SmBS 1 2 LgBS 1 2 JP 1 2   : "<<trg1<<"  "<<trg2<<"  "<<trg3<<"  "<<trg4<<"  "<<trg5<<"  "<<trg6<<"  "<<trg7<<"  "<<trg8<<"  "<<trg9<<"  "<<mTriggerBit<<endl;


    //---------- Fill FMS Information -----------------
    mFmsMuColl = mMuDst->muFmsCollection(); // Note: This is directly from MuDst. 
    for(Int_t i = 0; i < mFmsMuColl->numberOfPoints(); ++i)
    {
	mFmsPoint = mFmsMuColl->getPoint(i);
	mPointPhi_notrg->Fill(mFmsPoint->xyz().phi());
	mPointXY_notrg->Fill(mFmsPoint->xyz().x(), mFmsPoint->xyz().y());

	if(mTriggerBit > 1000000000)
	{
	    mPointXY_trg->Fill(mFmsPoint->xyz().x(), mFmsPoint->xyz().y());
	    mPointPhi_trg->Fill(mFmsPoint->xyz().phi());
	}
    }

    ++mNevents;    
    return kStOK;
}

//_____________________________________________________________________________
Int_t TStFmsSimTreeMaker::Finish()
{
    //Write histograms to root file etc.

    mFile->Write();
    
    return kStOK;
}
