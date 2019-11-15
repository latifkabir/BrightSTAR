// Filename: TStFmsRpTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif

#include "TStFmsRpTreeMaker.h"
#include "StEvent/StFmsPointPair.h"
#include "StEvent/StFmsCollection.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"

ClassImp(TStFmsRpTreeMaker)

//_____________________________________________________________________________ 
TStFmsRpTreeMaker::TStFmsRpTreeMaker(const char *name):StMaker(name)
{
    //FMS Buffer
    mFmsPairE = new Double_t[kMaxPairs];
    mFmsPairM = new Double_t[kMaxPairs];
    mFmsPairPt = new Double_t[kMaxPairs];
    mFmsPairEta = new Double_t[kMaxPairs];
    mFmsPairPhi = new Double_t[kMaxPairs];
    mFmsPairZgg = new Double_t[kMaxPairs];
    mFmsPairDgg = new Double_t[kMaxPairs];
    mFmsPairX = new Double_t[kMaxPairs];
    mFmsPairY = new Double_t[kMaxPairs];

    //RP Buffer
    mRpTrackBranch = new Int_t[kMaxTracks];
    mRpTrackTheta = new Double_t[kMaxTracks];
    mRpTrackTheta_x = new Double_t[kMaxTracks];
    mRpTrackTheta_y = new Double_t[kMaxTracks];
    mRpTrackEta = new Double_t[kMaxTracks];
    mRpTrackPhi = new Double_t[kMaxTracks];
    mRpTrackP = new Double_t[kMaxTracks];
    mRpTrackPt = new Double_t[kMaxTracks];
    mRpTrackXi = new Double_t[kMaxTracks];
    mRpTrackMt = new Double_t[kMaxTracks];    
}

//_____________________________________________________________________________ 
TStFmsRpTreeMaker::~TStFmsRpTreeMaker()
{
    //FMS buffer
    delete[] mFmsPairE;
    delete[] mFmsPairM;
    delete[] mFmsPairPt;
    delete[] mFmsPairEta;
    delete[] mFmsPairPhi;
    delete[] mFmsPairZgg;
    delete[] mFmsPairDgg;
    delete[] mFmsPairX;
    delete[] mFmsPairY;

    //RP Buffer
    delete[] mRpTrackBranch;
    delete[] mRpTrackTheta;
    delete[] mRpTrackTheta_x;
    delete[] mRpTrackTheta_y;
    delete[] mRpTrackEta;
    delete[] mRpTrackPhi;
    delete[] mRpTrackP;
    delete[] mRpTrackPt;
    delete[] mRpTrackXi;
    delete[] mRpTrackMt;
}


//_____________________________________________________________________________ 
Int_t TStFmsRpTreeMaker::Init()
{
    mFmsDbMk = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    if (!mFmsDbMk)
    {
	LOG_ERROR <<"TStFmsRpTreeMaker::InitRun - !StFmsDbMaker" <<endl;
	return kStFatal;
    }
    mTree = new TTree("T", "FMS + RP Tree");
    SetBranches();
    return kStOK;
}

//_____________________________________________________________________________ 
Int_t TStFmsRpTreeMaker::SetBranches()
{
    mTree->Branch("fms_nPairs", &mFmsNpairs, "fms_nPairs/I");
    mTree->Branch("fms_pairE", mFmsPairE, "fms_pairE[fms_nPairs]/D");    
    mTree->Branch("fms_pairM", mFmsPairM, "fms_pairM[fms_nPairs]/D");    
    mTree->Branch("fms_pairPt", mFmsPairPt, "fms_pairPt[fms_nPairs]/D");    
    mTree->Branch("fms_pairEta", mFmsPairEta, "fms_pairEta[fms_nPairs]/D");    
    mTree->Branch("fms_pairPhi", mFmsPairPhi, "fms_pairPhi[fms_nPairs]/D");    
    mTree->Branch("fms_pairZgg", mFmsPairZgg, "fms_pairZgg[fms_nPairs]/D");    
    mTree->Branch("fms_pairDgg", mFmsPairDgg, "fms_pairDgg[fms_nPairs]/D");    
    mTree->Branch("fms_pairX", mFmsPairX, "fms_pairX[fms_nPairs]/D");    
    mTree->Branch("fms_pairY", mFmsPairY, "fms_pairY[fms_nPairs]/D");

    mTree->Branch("rp_nTracks", &mRpNtracks, "rp_nTracks/I");
    mTree->Branch("rp_trackBranch", mRpTrackBranch, "rp_trackBranch[rp_nTracks]/D");        
    mTree->Branch("rp_trackTheta", mRpTrackTheta, "rp_trackTheta[rp_nTracks]/D");        
    mTree->Branch("rp_trackTheta_x", mRpTrackTheta_x, "rp_trackTheta_x[rp_nTracks]/D");        
    mTree->Branch("rp_trackTheta_y", mRpTrackTheta_y, "rp_trackTheta_y[rp_nTracks]/D");        
    mTree->Branch("rp_trackEta", mRpTrackEta, "rp_trackEta[rp_nTracks]/D");        
    mTree->Branch("rp_trackPhi", mRpTrackPhi, "rp_trackPhi[rp_nTracks]/D");        
    mTree->Branch("rp_trackXi", mRpTrackXi, "rp_trackXi[rp_nTracks]/D");        
    mTree->Branch("rp_trackMt", mRpTrackMt, "rp_trackMt[rp_nTracks]/D");        
}
    
//_____________________________________________________________________________
Int_t TStFmsRpTreeMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");

    if (!mMuDst || !mEvent)
    {
	cout <<"TStFmsRpTreeMaker::Make - !MuDst or !StEvent" <<endl;
	return kStErr;
    }
    Int_t status = kStOK;
    status = MakeEvent();
    if(status != kStOK)
	return status;
    status = MakeFms();
    if(status != kStOK)
	return status;
    status = MakeRps();
   
    return status;    
}
//_____________________________________________________________________________
Int_t TStFmsRpTreeMaker::MakeEvent()
{
    //bXing id (long, short), evet number, spin state, 
    //Trigger ids    
    
    // BBC, ZDC, VPD branches <------- To be verified
    for(int ew = 0; ew < 2; ew++)
    {
	// BBC
	bbcADCSum[ew] = muEvent->triggerData()->bbcADCSum((StBeamDirection)ew);
	bbcADCSumLarge[ew] = muEvent->triggerData()->bbcADCSumLargeTile((StBeamDirection)ew);
	bbcEarliestTDC[ew] = muEvent->triggerData()->bbcEarliestTDC((StBeamDirection)ew);
	bbcEarliestTDCLarge[ew] = muEvent->triggerData()->bbcEarliestTDCLarge((StBeamDirection)ew);
	// ZDC
	zdcADCSum[ew] = 0;
	for(int pmt = 1; pmt <= 3; pmt++)
	{
	    zdcADCSum[ew] += muEvent->triggerData()->zdcADC((StBeamDirection)ew,pmt); // is this correct?
	}
	// VPD
	vpdADCSum[ew] = 0;
	for(int pmt = 1; pmt <= 16; pmt++)
	{
	    vpdADCSum[ew] += muEvent->triggerData()->vpdADC((StBeamDirection)ew,pmt); // is this correct?
	}
    } 
    tofMultiplicity = muEvent->triggerData()->tofMultiplicity();
    
    return kStOk;    
}
//_____________________________________________________________________________
Int_t TStFmsRpTreeMaker::MakeFms()
{
    //Must read from reconstructed StEvent (not StMuDst)
    mFmsColl = (StFmsCollection*)mEvent->fmsCollection();

    if(!mFmsColl)
    {
	cout<<"No FMS data for this mEvent"<<endl;
	return kStSkip;
    }

    mPointPairs = mFmsColl->pointPairs();
    mFmsNpairs = mFmsColl->numberOfPointPairs();
    for (Int_t i = 0; i < mFmsNpairs; ++i)
    {
	mPair = mPointPairs[i];

	mFmsPairE[i] = mPair->energy();
	mFmsPairM[i] = mPair->mass();	      
	mFmsPairPt[i] = mPair->pT();
	mFmsPairEta[i] = mPair->eta();
	mFmsPairPhi[i] = mPair->phi();
	mFmsPairZgg[i] = mPair->zgg();
	mFmsPairDgg[i] = mPair->dgg();
	mFmsPairX[i] = mPair->x();
	mFmsPairY[i] = mPair->y();	
    }
    
    return kStOk;    
}
//_____________________________________________________________________________
Int_t TStFmsRpTreeMaker::MakeRps()
{
    mRpsMuColl = mMuDst->RpsCollection();
    if(!mRpsMuColl)
    {
	cout<<"No RP data for this event"<<endl;
	return kStSkip;
    }
    
    mRpNtracks = mRpsMuColl->numberOfTracks();
    for(Int_t i = 0; i < mRpNtracks; ++i)
    {
	mRpTrackBranch[i] = mRpsMuColl->track(i)->branch();	
	mRpTrackTheta[i] = 1000.0*mRpsMuColl->track(i)->theta();	
	mRpTrackTheta_x[i] = 1000.0*mRpsMuColl->track(i)->theta(0);	
	mRpTrackTheta_x[i] = 1000.0*mRpsMuColl->track(i)->theta(1);	
	mRpTrackEta[i] = mRpsMuColl->track(i)->eta();
	mRpTrackPhi[i] = mRpsMuColl->track(i)->phi();
	RpTrackPt[i] = mRpsMuColl->track(i)->pt();		
	RpTrackP[i] = mRpsMuColl->track(i)->p();		
	RpTrackXi[i] = mRpsMuColl->track(i)->xi(100.0); // Beam momentum is approximate		
	RpTrackMt[i] = -1.0*mRpsMuColl->track(i)->t(100.0);		
    }
            
    return kStOk;    
}
//_____________________________________________________________________________
Int_t TStFmsRpTreeMaker::Finish()
{
    //Write histograms to root file etc.

    return kStOK;
}

