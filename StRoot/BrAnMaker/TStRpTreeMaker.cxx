// Filename: TStRpTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif

#include "TStRpTreeMaker.h"
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"
#include "StEvent/StTriggerId.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"

ClassImp(TStRpTreeMaker)

//_____________________________________________________________________________ 
TStRpTreeMaker::TStRpTreeMaker(const char *name):StMaker(name)
{
    mEvtCountHist = new TH1D("EvtCountHist", "Event counts", 20, 0, 20);
    mBeamMom = 100.0; //Overwrite from runMacro
    
    //Event Buffer
    mTriggers = new Int_t[kMaxTriggers];

    //RP Buffer
    mRpTrackType = new Int_t[kMaxRpTracks];
    mRpTrackNplanes = new Int_t[kMaxRpTracks];
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
TStRpTreeMaker::~TStRpTreeMaker()
{
    delete[] mTriggers;

    //RP Buffer
    delete[] mRpTrackType;
    delete[] mRpTrackNplanes;
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

    delete mEvtCountHist;
}


//_____________________________________________________________________________ 
Int_t TStRpTreeMaker::Init()
{
    cout << "-------->Triggers Included:" <<endl;
    for(mIt = mTrigIDs.begin(); mIt != mTrigIDs.end(); ++mIt)
	cout <<"------->: "<<*mIt <<endl;
    
    mFile = new TFile(mOutName, "RECREATE");
    mTree = new TTree("T", "RP Tree");

    SetBranches();
    
    return kStOK;
}

//_____________________________________________________________________________ 
Int_t TStRpTreeMaker::InitRun(int runnumber)
{
    mSpinDbMaker = static_cast<StSpinDbMaker*>(GetMaker("spinDb"));
    mSpinDbMaker->print();

    return kStOK;
}

//_____________________________________________________________________________ 
void TStRpTreeMaker::SetBranches()
{
    //Event branches    
    mTree->Branch("evt_run", &mRunNumber, "evt_run/I");
    mTree->Branch("evt_id", &mEventId, "evt_id/I");
    mTree->Branch("evt_bSpin", &mBspin, "evt_bSpin/S");
    mTree->Branch("evt_ySpin", &mYspin, "evt_ySpin/S");
    mTree->Branch("evt_nTrig", &mNtrig, "evt_nTrig/I");
    mTree->Branch("evt_Triggers", mTriggers, "evt_Triggers[evt_nTrig]/I");
    
    mTree->Branch("evt_bbcADCSum", mBbcADCSum, "evt_bbcADCSum[2]/I");
    mTree->Branch("evt_bbcADCSumLarge", mBbcADCSumLarge, "evt_bbcADCSumLarge[2]/I");
    mTree->Branch("evt_bbcEarliestTDC", mBbcEarliestTDC, "evt_bbcEarliestTDC[2]/I");
    mTree->Branch("evt_bbcEarliestTDCLarge", mBbcEarliestTDCLarge, "evt_bbcEarliestTDCLarge[2]/I");

    mTree->Branch("evt_zdcADCSum", mZdcADCSum, "evt_zdcADCSum[2]/I");
    mTree->Branch("evt_vpdADCSum", mVpdADCSum, "evt_vpdADCSum[2]/I");

    mTree->Branch("evt_tofMultiplicity", &mTofMultiplicity, "evt_tofMultiplicity/I");
    

    //RP branches
    mTree->Branch("rp_nTracks", &mRpNtracks, "rp_nTracks/I");
    mTree->Branch("rp_trackType", mRpTrackType, "rp_trackType[rp_nTracks]/I");        
    mTree->Branch("rp_trackNplanes", mRpTrackNplanes, "rp_trackNplanes[rp_nTracks]/I");        
    mTree->Branch("rp_trackBranch", mRpTrackBranch, "rp_trackBranch[rp_nTracks]/I");        
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
void TStRpTreeMaker::ResetBuffer()
{
    mRunNumber = -1;
    mEventId = -1;
    mBspin = 0;
    mYspin = 0;
    mNtrig = 0;

    std::fill_n(mTriggers, kMaxTriggers, -1);    
    std::fill_n(mBbcADCSum, 2, 0);    
    std::fill_n(mBbcADCSumLarge, 2, 0);    
    std::fill_n(mBbcEarliestTDC, 2, -1);    
    std::fill_n(mBbcEarliestTDCLarge, 2, -1);    
    std::fill_n(mZdcADCSum, 2, 0);    
    std::fill_n(mVpdADCSum, 2, 0);    

    mRpNtracks = 0;
    std::fill_n(mRpTrackType, kMaxRpTracks, -1);    
    std::fill_n(mRpTrackNplanes, kMaxRpTracks, -1);    
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
Int_t TStRpTreeMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");

    if (!mMuDst || !mEvent)
    {
	cout <<"TStRpTreeMaker::Make - !MuDst or !StEvent" <<endl;
	return kStErr;
    }
    mMuEvent = mMuDst->event();

    // Event filtering has been moved to TStFmsRpFilterMaker class.
    // if(!AcceptEvent())
    // 	return kStSkip;
    
    ResetBuffer();
    
    Int_t status = kStOK;
    status = MakeEvent();
    if(status != kStOK)
	return status;

    status = MakeRps();

    mTree->Fill();

    return status;    
}
//_____________________________________________________________________________
Bool_t TStRpTreeMaker::AcceptEvent()
{
    //Trigger flag
    mTrigFlag = 0;
    for(mIt = mTrigIDs.begin(); mIt != mTrigIDs.end(); ++mIt)
    {
	mTrigFlag = mMuEvent->triggerIdCollection().nominal().isTrigger(*mIt);
	if(mTrigFlag)
	    break;
    }
    if(mTrigFlag == 0)
	return kFALSE;
    mRpsMuColl = mMuDst->RpsCollection();
    if(!mRpsMuColl)
    {
	cout<<"No RP data for this event"<<endl;
	return kFALSE;
    }    
    if(mRpsMuColl->numberOfTracks() < 1)
	return kFALSE;

    //Skip LED trigger events here
    //Skip abort gap events here
    
    return kTRUE;
}
//_____________________________________________________________________________
Int_t TStRpTreeMaker::MakeEvent()
{
    //Run no. and event no.     
    mRunNumber = mMuEvent->runNumber();
    mEventId = mMuEvent->eventId();

    //Trigger ids
    mTrigMuColl = &mMuEvent->triggerIdCollection();
    if(mTrigMuColl)
    {
	const StTriggerId trgIDs = mTrigMuColl->nominal();
	mNtrig = trgIDs.triggerIds().size();
	for(Int_t i = 0; i < mNtrig; i++)
	    mTriggers[i] = trgIDs.triggerIds().at(i);
    }
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
Int_t TStRpTreeMaker::MakeRps()
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
	mRpTrackType[i] = (mRpsMuColl->track(i)->type() == StMuRpsTrack::rpsGlobal) ? 1 : 0;
	mRpTrackNplanes[i] = mRpsMuColl->track(i)->planesUsed();
	mRpTrackBranch[i] = mRpsMuColl->track(i)->branch();	
	mRpTrackTheta[i] = 1000.0*mRpsMuColl->track(i)->theta();	
	mRpTrackTheta_x[i] = 1000.0*mRpsMuColl->track(i)->theta(0);	
	mRpTrackTheta_y[i] = 1000.0*mRpsMuColl->track(i)->theta(1);	
	mRpTrackEta[i] = mRpsMuColl->track(i)->eta();
	mRpTrackPhi[i] = mRpsMuColl->track(i)->phi();
	mRpTrackPt[i] = mRpsMuColl->track(i)->pt();		
	mRpTrackP[i] = mRpsMuColl->track(i)->p();		
	mRpTrackXi[i] = mRpsMuColl->track(i)->xi(mBeamMom); // Beam momentum is approximate		
	mRpTrackMt[i] = -1.0*mRpsMuColl->track(i)->t(mBeamMom);		
    }
            
    return kStOk;    
}
//_____________________________________________________________________________
Int_t TStRpTreeMaker::Finish()
{
    //Write histograms to root file etc.
    mFile->cd();
    mEvtCountHist->Write();
    mFile->Write();
    mFile->Close();
    
    return kStOK;
}

