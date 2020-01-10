// Filename: TStAnTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif

#include "TStAnTreeMaker.h"
#include "StEvent/StFmsPointPair.h"
#include "StEvent/StFmsCollection.h"
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"
#include "StEvent/StTriggerId.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"

ClassImp(TStAnTreeMaker)

//_____________________________________________________________________________ 
TStAnTreeMaker::TStAnTreeMaker(const char *name):StMaker(name)
{
    mBeamMom = 100.0; //Overwrite from runMacro

    // Turn Off TObject streamer to save space on the file.
    // see: https://root.cern.ch/doc/master/classTClass.html#a606b0442d6fec4b1cd52f43bca73aa51
    TStEventData::Class()->IgnoreTObjectStreamer();
    TStTrackData::Class()->IgnoreTObjectStreamer();
    TStRpsTrackData::Class()->IgnoreTObjectStreamer();
    TStFmsPointPairData::Class()->IgnoreTObjectStreamer();
    
    //Event Buffer
    mEventArray = new TClonesArray("TStEventData"); 

    //Track Buffer
    mTrackArray = new TClonesArray("TStTrackData");
    
    //FMS Buffer
    mFmsArray = new TClonesArray("TStFmsPointPairData");

    //RP Buffer
    mRpsArray = new TClonesArray("TStRpsTrackData");
}

//_____________________________________________________________________________ 
TStAnTreeMaker::~TStAnTreeMaker()
{
    //Track buffer
    delete mEventArray;
    
    //Track buffer
    delete mTrackArray;
    
    //FMS buffer
    delete mFmsArray;
    
    //RP Buffer    
    delete mRpsArray;
}


//_____________________________________________________________________________ 
Int_t TStAnTreeMaker::Init()
{
    cout << "-------->Triggers Included:" <<endl;
    for(mIt = mTrigIDs.begin(); mIt != mTrigIDs.end(); ++mIt)
	cout <<"------->: "<<*mIt <<endl;
    
    mFmsDbMk = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    if (!mFmsDbMk)
    {
	LOG_ERROR <<"TStAnTreeMaker::InitRun - !StFmsDbMaker" <<endl;
	return kStFatal;
    }

    if(!mTree)
    {
	mFile = new TFile("AnTree.root", "RECREATE");
	mTree = new TTree("T", "An Tree");
	mSaveFile = kTRUE;
    }
    else
	mSaveFile = kFALSE;
    SetBranches();
    
    return kStOK;
}

//_____________________________________________________________________________ 
Int_t TStAnTreeMaker::InitRun(int runnumber)
{
    mSpinDbMaker = static_cast<StSpinDbMaker*>(GetMaker("spinDb"));
    mSpinDbMaker->print();

    return kStOK;
}

//_____________________________________________________________________________ 
void TStAnTreeMaker::SetBranches()
{
    //Event branches    
    mTree->Branch("event", &mEventArray, 256000, 99);

    //TPC Track branch
    mTree->Branch("track", &mTrackArray, 256000, 99);
        
    //FMS branches
    mTree->Branch("fmsPointPair", &mFmsArray, 256000, 99);

    //RP branches
    mTree->Branch("rpTrack", &mRpsArray, 256000, 99);

    cout << "Done setting branches..." <<endl;
}
//_____________________________________________________________________________
void TStAnTreeMaker::Reset()
{
    mEventArray->Clear();
    mTrackArray->Clear();
    mFmsArray->Clear();
    mRpsArray->Clear();
}
    
//_____________________________________________________________________________
Int_t TStAnTreeMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");

    if (!mMuDst || !mEvent)
    {
	cout <<"TStAnTreeMaker::Make - !MuDst or !StEvent" <<endl;
	return kStErr;
    }
    mMuEvent = mMuDst->event();

    // Event filtering has been moved to TStFmsRpFilterMaker class.
    // if(!AcceptEvent())
    // 	return kStSkip;
    Reset();
    
    Int_t status = kStOK;
    status = MakeEvent();
    if(status != kStOK)
	return status;
    status = MakeFms();
    if(status != kStOK)
	return status;
    status = MakeRps();

    status = MakeTrack();
    
    mTree->Fill();

    return status;    
}
//_____________________________________________________________________________
Bool_t TStAnTreeMaker::AcceptEvent()
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
Int_t TStAnTreeMaker::MakeEvent()
{
    mEventData =  new((*mEventArray)[i])TStEventData();	
    
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
Int_t TStAnTreeMaker::MakeFms()
{
    //Must read from reconstructed StEvent (not StMuDst)
    mFmsColl = (StFmsCollection*)mEvent->fmsCollection();

    if(!mFmsColl)
    {
	cout<<"No FMS data for this mEvent"<<endl;
	return kStSkip;
    }

    // mPointPairs = mFmsColl->pointPairs();
    mPointPairs = mFmsColl->pointPairsEnergySorted();
    mFmsNpairs = mFmsColl->numberOfPointPairs();

    for (Int_t i = 0; i < mFmsNpairs; ++i)
    {

	mFmsPointPairData =  new((*mFmsArray)[i])TStFmsPointPairData();	
	mPair = mPointPairs[i];

	mFmsPointPairData->mE = mPair->energy();
	mFmsPointPairData->mM = mPair->mass();	      
	mFmsPointPairData->mPt = mPair->pT();
	mFmsPointPairData->mEta = mPair->eta();
	mFmsPointPairData->mPhi = mPair->phi();
	mFmsPointPairData->Zgg = mPair->zgg();
	mFmsPointPairData->mDgg = mPair->dgg();
	mFmsPointPairData->mX = mPair->x();
	mFmsPointPairData->mY = mPair->y();	
    }
    
    return kStOk;    
}
//_____________________________________________________________________________
Int_t TStAnTreeMaker::MakeRps()
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
	mRpsTrackData =  new((*mTrackArray)[i])TStRpsTrackData();	
	
	mRpsTrackData->mType = (mRpsMuColl->track(i)->type() == StMuRpsTrack::rpsGlobal) ? 1 : 0;
	mRpsTrackData->mNplanes = mRpsMuColl->track(i)->planesUsed();
	mRpsTrackData->mBranch = mRpsMuColl->track(i)->branch();	
	mRpsTrackData->mTheta = 1000.0*mRpsMuColl->track(i)->theta();	
	mRpsTrackData->mTheta_x = 1000.0*mRpsMuColl->track(i)->theta(0);	
	mRpsTrackData->mTheta_y = 1000.0*mRpsMuColl->track(i)->theta(1);	
	mRpsTrackData->mEta = mRpsMuColl->track(i)->eta();
	mRpsTrackData->mPhi = mRpsMuColl->track(i)->phi();
	mRpsTrackData->mPt = mRpsMuColl->track(i)->pt();		
	mRpsTrackData->mP = mRpsMuColl->track(i)->p();		
	mRpsTrackData->mPx = mRpsMuColl->track(i)->pVec().x();		
	mRpsTrackData->mPy = mRpsMuColl->track(i)->pVec().y();		
	mRpsTrackData->mPz = mRpsMuColl->track(i)->pVec().z();		
	mRpsTrackData->mXi = mRpsMuColl->track(i)->xi(mBeamMom); // Beam momentum is approximate		
	mRpsTrackData->mMt = -1.0*mRpsMuColl->track(i)->t(mBeamMom);	
    }

    return kStOk;    
}

//_____________________________________________________________________________
Int_t TStAnTreeMaker::MakeTrack()
{

    if(!mMuDst)
    {
	LOG_ERROR << "TStAnTreeMaker::MakeTrack - No MuDst found" <<endm;
	return kStFatal;
    }

    for(Int_t i = 0; i < mMuDst->numberOfPrimaryTracks(); ++i)
    {

	mTrack = mMuDst->primaryTracks(i);
	mTrackData =  new((*mTrackArray)[i])TStTrackData();
	
	mTrackData->mQ = mTrack->charge();
	mTrackData->mEta = mTrack->eta();
	mTrackData->mPhi = mTrack->phi();
	mTrackData->mPt = mTrack->pt();	      	
    }

    return kStOK;
}

//_____________________________________________________________________________
Int_t TStAnTreeMaker::Finish()
{
    //Write histograms to root file etc.
    if(mSaveFile)
    {
	mFile->Write();
	mFile->Close();
    }
    return kStOK;
}

