// Filename: TStFmsRpTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif

#include "TStFmsRpTreeMaker.h"
#include "StEvent/StFmsPointPair.h"
#include "StEvent/StFmsCollection.h"
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"

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
    mRpTrackBranch = new Int_t[kMaxRpTracks];
    mRpTrackTheta = new Double_t[kMaxRpTracks];
    mRpTrackTheta_x = new Double_t[kMaxRpTracks];
    mRpTrackTheta_y = new Double_t[kMaxRpTracks];
    mRpTrackEta = new Double_t[kMaxRpTracks];
    mRpTrackPhi = new Double_t[kMaxRpTracks];
    mRpTrackP = new Double_t[kMaxRpTracks];
    mRpTrackPt = new Double_t[kMaxRpTracks];
    mRpTrackXi = new Double_t[kMaxRpTracks];
    mRpTrackMt = new Double_t[kMaxRpTracks];    
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
    cout << "-------->Triggers Included:" <<endl;
    for(mIt = mTrigIDs.begin(); mIt != mTrigIDs.end(); ++mIt)
	cout <<"------->: "<<*mIt <<endl;
    
    mFmsDbMk = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    if (!mFmsDbMk)
    {
	LOG_ERROR <<"TStFmsRpTreeMaker::InitRun - !StFmsDbMaker" <<endl;
	return kStFatal;
    }

    if(!mTree)
    {
	mFile = new TFile("FmsRpTree.root", "RECREATE");
	mTree = new TTree("T", "FMS + RP Tree");
	mSaveFile = kTRUE;
    }
    else
	mSaveFile = kFALSE;
    SetBranches();
    
    return kStOK;
}

//_____________________________________________________________________________ 
Int_t TStFmsRpTreeMaker::InitRun(int runnumber)
{
    mSpinDbMaker = static_cast<StSpinDbMaker*>(GetMaker("spinDb"));
    mSpinDbMaker->print();

    return kStOK;
}

//_____________________________________________________________________________ 
void TStFmsRpTreeMaker::SetBranches()
{
    //Event branches    
    mTree->Branch("evt_id", &mEventId, "evt_id/I");
    mTree->Branch("evt_bSpin", &mBspin, "evt_bSpin/S");
    mTree->Branch("evt_ySpin", &mYspin, "evt_ySpin/S");
    
    mTree->Branch("evt_bbcADCSum", mBbcADCSum, "evt_bbcADCSum[2]/I");
    mTree->Branch("evt_bbcADCSumLarge", mBbcADCSumLarge, "evt_bbcADCSumLarge[2]/I");
    mTree->Branch("evt_bbcEarliestTDC", mBbcEarliestTDC, "evt_bbcEarliestTDC[2]/I");
    mTree->Branch("evt_bbcEarliestTDCLarge", mBbcEarliestTDCLarge, "evt_bbcEarliestTDCLarge[2]/I");

    mTree->Branch("evt_zdcADCSum", mZdcADCSum, "evt_zdcADCSum[2]/I");
    mTree->Branch("evt_vpdADCSum", mVpdADCSum, "evt_vpdADCSum[2]/I");

    mTree->Branch("evt_tofMultiplicity", &mTofMultiplicity, "evt_tofMultiplicity/I");
    
    //FMS branches
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

    //RP branches
    mTree->Branch("rp_nTracks", &mRpNtracks, "rp_nTracks/I");
    mTree->Branch("rp_trackBranch", mRpTrackBranch, "rp_trackBranch[rp_nTracks]/D");        
    mTree->Branch("rp_trackTheta", mRpTrackTheta, "rp_trackTheta[rp_nTracks]/D");        
    mTree->Branch("rp_trackTheta_x", mRpTrackTheta_x, "rp_trackTheta_x[rp_nTracks]/D");        
    mTree->Branch("rp_trackTheta_y", mRpTrackTheta_y, "rp_trackTheta_y[rp_nTracks]/D");        
    mTree->Branch("rp_trackEta", mRpTrackEta, "rp_trackEta[rp_nTracks]/D");        
    mTree->Branch("rp_trackPhi", mRpTrackPhi, "rp_trackPhi[rp_nTracks]/D");        
    mTree->Branch("rp_trackXi", mRpTrackXi, "rp_trackXi[rp_nTracks]/D");        
    mTree->Branch("rp_trackMt", mRpTrackMt, "rp_trackMt[rp_nTracks]/D");
    mTree->Branch("rp_trackP", mRpTrackP, "rp_trackP[rp_nTracks]/D");
    mTree->Branch("rp_trackPt", mRpTrackPt, "rp_trackPt[rp_nTracks]/D");
}
//_____________________________________________________________________________
void TStFmsRpTreeMaker::ResetBuffer()
{
    mEventId = -1;
    mBspin = 0;
    mYspin = 0;
    
    std::fill_n(mBbcADCSum, 2, 0);    
    std::fill_n(mBbcADCSumLarge, 2, 0);    
    std::fill_n(mBbcEarliestTDC, 2, -1);    
    std::fill_n(mBbcEarliestTDCLarge, 2, -1);    
    std::fill_n(mZdcADCSum, 2, 0);    
    std::fill_n(mVpdADCSum, 2, 0);    

    mFmsNpairs = 0;
    std::fill_n(mFmsPairE, kMaxPairs, -1);    
    std::fill_n(mFmsPairM, kMaxPairs, -1);    
    std::fill_n(mFmsPairPt, kMaxPairs, -1);    
    std::fill_n(mFmsPairEta, kMaxPairs, -1);    
    std::fill_n(mFmsPairPhi, kMaxPairs, -1);    
    std::fill_n(mFmsPairZgg, kMaxPairs, -1);    
    std::fill_n(mFmsPairDgg, kMaxPairs, -1);    
    std::fill_n(mFmsPairX, kMaxPairs, -999);    
    std::fill_n(mFmsPairY, kMaxPairs, -999);    

    mRpNtracks = 0;
    std::fill_n(mRpTrackBranch, kMaxRpTracks, -1);    
    std::fill_n(mRpTrackTheta, kMaxRpTracks, -999);    
    std::fill_n(mRpTrackTheta_x, kMaxRpTracks, -999);    
    std::fill_n(mRpTrackTheta_y, kMaxRpTracks, -999);    
    std::fill_n(mRpTrackEta, kMaxRpTracks, -999);    
    std::fill_n(mRpTrackPhi, kMaxRpTracks, -999);    
    std::fill_n(mRpTrackXi, kMaxRpTracks, -999);    
    std::fill_n(mRpTrackMt, kMaxRpTracks, -999);    
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
    mMuEvent = mMuDst->event();

    ResetBuffer();
    
    Int_t status = kStOK;
    status = MakeEvent();
    if(status != kStOK)
	return status;
    status = MakeFms();
    if(status != kStOK)
	return status;
    status = MakeRps();

    mTree->Fill();

    return status;    
}
//_____________________________________________________________________________
Int_t TStFmsRpTreeMaker::MakeEvent()
{
    //bXing id (long, short), evet number, spin state, 
    //Trigger ids    

    mEventId = mMuEvent->eventId();
    
    //Trigger flag
    mTrigFlag = 0;
    for(mIt = mTrigIDs.begin(); mIt != mTrigIDs.end(); ++mIt)
    {
	mTrigFlag = mMuEvent->triggerIdCollection().nominal().isTrigger(*mIt);
	if(mTrigFlag)
	    break;
    }
    if(mTrigFlag == 0)
	return kStSkip;

    //Skip LED trigger events here
    //Skip abort gap events here
    
    // BBC, ZDC, VPD branches <------- To be verified/revisited
    for(Int_t ew = 0; ew < 2; ew++)
    {
	// BBC
	mBbcADCSum[ew] = mMuEvent->triggerData()->bbcADCSum((StBeamDirection)ew);
	mBbcADCSumLarge[ew] = mMuEvent->triggerData()->bbcADCSumLargeTile((StBeamDirection)ew);
	mBbcEarliestTDC[ew] = mMuEvent->triggerData()->bbcEarliestTDC((StBeamDirection)ew);
	mBbcEarliestTDCLarge[ew] = mMuEvent->triggerData()->bbcEarliestTDCLarge((StBeamDirection)ew);
	// ZDC
	mZdcADCSum[ew] = 0;
	for(Int_t pmt = 1; pmt <= 3; pmt++)
	{
	    mZdcADCSum[ew] += mMuEvent->triggerData()->zdcADC((StBeamDirection)ew,pmt); // is this correct?
	}
	// VPD
	mVpdADCSum[ew] = 0;
	for(Int_t pmt = 1; pmt <= 16; pmt++)
	{
	    mVpdADCSum[ew] += mMuEvent->triggerData()->vpdADC((StBeamDirection)ew,pmt); // is this correct?
	}
    } 
    mTofMultiplicity = mMuEvent->triggerData()->tofMultiplicity();


    // StRunInfo* runInfo = &(mMuEvent->runInfo());
    // mFill = runInfo->beamFillNumber(blue);
    // mBbcCo = runInfo->bbcCoincidenceRate();
    // mBbcE = runInfo->bbcEastRate();
    // mBbcW = runInfo->bbcWestRate();
    //--------- Spin States -------------
    mBunchid7bit = mMuEvent->triggerData()->bunchId7Bit();
    mSpin4bit = mSpinDbMaker->spin4usingBX7(mBunchid7bit); 
 
    // get spin2bit from mSpin4bit
    switch(mSpin4bit) 
    {
    case 5:
    {
	mBspin = -1;
	mYspin = -1;
	break;
    }
    case 6:
    {
	mBspin = -1;
	mYspin = +1;	
	break;
    }
    case 9:
    {
	mBspin = +1;
	mYspin = -1;	
	break;
    }
    case 10:
    {
	mBspin = +1;
	mYspin = +1;
	break;
    }
    default: //Undefined
	mBspin = 0;
	mYspin = 0;      
    }
        
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
	mRpTrackTheta_y[i] = 1000.0*mRpsMuColl->track(i)->theta(1);	
	mRpTrackEta[i] = mRpsMuColl->track(i)->eta();
	mRpTrackPhi[i] = mRpsMuColl->track(i)->phi();
	mRpTrackPt[i] = mRpsMuColl->track(i)->pt();		
	mRpTrackP[i] = mRpsMuColl->track(i)->p();		
	mRpTrackXi[i] = mRpsMuColl->track(i)->xi(100.0); // Beam momentum is approximate		
	mRpTrackMt[i] = -1.0*mRpsMuColl->track(i)->t(100.0);		
    }
            
    return kStOk;    
}
//_____________________________________________________________________________
Int_t TStFmsRpTreeMaker::Finish()
{
    //Write histograms to root file etc.
    if(mSaveFile)
    {
	mFile->Write();
	mFile->Close();
    }
    return kStOK;
}

