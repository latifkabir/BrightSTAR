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
    TStChargedPidData::Class()->IgnoreTObjectStreamer();
    
    //Event Buffer
    mEventData =  new TStEventData();	

    //Track Buffer
    mTrackArray = new TClonesArray("TStTrackData");

    //Charged PID
    mPidTagger = new TStPidTagger();
    mElArray = new TClonesArray("TStChargedPidData");
    mPiArray = new TClonesArray("TStChargedPidData");
    mKaArray = new TClonesArray("TStChargedPidData");
    mPrArray = new TClonesArray("TStChargedPidData");
    mMuArray = new TClonesArray("TStChargedPidData");
    mUkArray = new TClonesArray("TStChargedPidData");
    
    //FMS Buffer
    mFmsArray = new TClonesArray("TStFmsPointPairData");

    //RP Buffer
    mRpsArray = new TClonesArray("TStRpsTrackData");
}

//_____________________________________________________________________________ 
TStAnTreeMaker::~TStAnTreeMaker()
{
    //Track buffer
    delete mEventData;
    
    //Track buffer
    delete mTrackArray;

    //Charged PID
    delete mElArray;
    delete mPiArray;
    delete mPrArray;
    delete mKaArray;
    delete mMuArray;
    delete mUkArray;
    
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
	mFile = new TFile(mOutName, "RECREATE");
	mTree = new TTree("T", "An Tree");
	mSaveFile = kTRUE;
    }
    else
	mSaveFile = kFALSE;
    SetBranches();
    InitHist();
    mEmcRadius = (Double_t)StEmcGeom::instance("bemc")->Radius();
    
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
    mTree->Branch("event", mEventData, 256000, 99);

    //TPC Track branch
    //mTree->Branch("track", &mTrackArray, 256000, 99);

    //Charged PID branches
    mTree->Branch("e", &mElArray, 256000, 99);
    mTree->Branch("pi", &mPiArray, 256000, 99);
    mTree->Branch("pr", &mPrArray, 256000, 99);
    mTree->Branch("ka", &mKaArray, 256000, 99);
    mTree->Branch("mu", &mMuArray, 256000, 99);
    mTree->Branch("uk", &mUkArray, 256000, 99);
    
    //FMS branches
    mTree->Branch("fmsPointPair", &mFmsArray, 256000, 99);

    //RP branches
    mTree->Branch("rpTrack", &mRpsArray, 256000, 99);

    cout << "Done setting branches..." <<endl;
}

//_____________________________________________________________________________
void TStAnTreeMaker::InitHist()
{
    mDedxVsQp = new TH2D("hDedxVsQp", "dE/dx vs qx|p| (No cut); qx|p| [GeV/c]; dE/dx [keV/cm]", 100, -3.0, 3.0, 100, 0, 10);
    mM2VsQp = new TH2D("hM2VsQp", "m^{2} vs qx|p| (No cut); qx|p| [GeV/c]; m^{2} [(GeV/c^{2})^{2}]", 100, -3.0, 3.0, 100, -0.4, 2.0);
    mDedxVsQp_e = new TH2D("hDedxVsQp_e", "dE/dx vs qx|p| for electron; qx|p| [GeV/c]; dE/dx [keV/cm]", 100, -3.0, 3.0, 100, 0, 10);
    mM2VsQp_e = new TH2D("hM2VsQp_e", "m^{2} vs qx|p| for electron; qx|p| [GeV/c]; m^{2} [(GeV/c^{2})^{2}]", 100, -3.0, 3.0, 100, -0.05, 0.05);
    mDedxVsQp_pi = new TH2D("hDedxVsQp_pi", "dE/dx vs qx|p| for Pion; qx|p| [GeV/c]; dE/dx [keV/cm]", 100, -3.0, 3.0, 100, 0, 10);
    mM2VsQp_pi = new TH2D("hM2VsQp_pi", "m^{2} vs qx|p| for Pion; qx|p| [GeV/c]; m^{2} [(GeV/c^{2})^{2}]", 100, -3.0, 3.0, 100, -0.1, 0.3);
    mDedxVsQp_pr = new TH2D("hDedxVsQp_pr", "dE/dx vs qx|p| for Proton; qx|p| [GeV/c]; dE/dx [keV/cm]", 100, -3.0, 3.0, 100, 0, 10);
    mM2VsQp_pr = new TH2D("hM2VsQp_pr", "m^{2} vs qx|p| for Proton; qx|p| [GeV/c]; m^{2} [(GeV/c^{2})^{2}]", 100, -3.0, 3.0, 100, 0.0, 2.0);
    mDedxVsQp_ka = new TH2D("hDedxVsQp_ka", "dE/dx vs qx|p| for Kaon; qx|p| [GeV/c]; dE/dx [keV/cm]", 100, -3.0, 3.0, 100, 0, 10);
    mM2VsQp_ka = new TH2D("hM2VsQp_ka", "m^{2} vs qx|p| for Kaon; qx|p| [GeV/c]; m^{2} [(GeV/c^{2})^{2}]", 100, -3.0, 3.0, 100, 0.0, 0.5);
    mDedxVsQp_mu = new TH2D("hDedxVsQp_mu", "dE/dx vs qx|p| for Muon; qx|p| [GeV/c]; dE/dx [keV/cm]", 100, -3.0, 3.0, 100, 0, 10);
    mM2VsQp_mu = new TH2D("hM2VsQp_mu", "m^{2} vs qx|p| for Muon; qx|p| [GeV/c]; m^{2} [(GeV/c^{2})^{2}]", 100, -3.0, 3.0, 100, -0.4, 0.4);   
}

//_____________________________________________________________________________
void TStAnTreeMaker::Reset()
{
    mEventData->Reset();
    
    mTrackArray->Clear();

    mElArray->Clear();
    mPiArray->Clear();
    mPrArray->Clear();
    mMuArray->Clear();
    mKaArray->Clear();
    mUkArray->Clear();
    mNpi = 0;
    mNe = 0;
    mNpr = 0;
    mNmu = 0;
    mNka = 0;
    mNuk = 0;
    
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

    //------ Reset Buffer --------
    Reset();
    
    Int_t status = kStOK;
    status = MakeEvent();
    if(status != kStOK)
	return status;
    status = MakeFms();
    if(status != kStOK)
	return status;
    status = MakeRps();
    //status = MakeTrack();
    status = MakeChargedPid();
    
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
    //Run no. and event no.     
    mEventData->mRunNumber = mMuEvent->runNumber();
    mEventData->mEventId = mMuEvent->eventId();

    //Trigger ids
    mTrigMuColl = &mMuEvent->triggerIdCollection();
    if(mTrigMuColl)
    {
	const StTriggerId trgIDs = mTrigMuColl->nominal();
	mEventData->mNtrig = trgIDs.triggerIds().size();

	if(mEventData->mNtrig > mEventData->mMaxTrigs)
	{
	    std::cout << "TStAnTreeMaker::MakeEvent - The trigger buffer is out of limit. You must adjust it." << std::endl;
	    return kStErr;
	}
	
	for(Int_t i = 0; i < mEventData->mNtrig; i++)
	    mEventData->mTriggers[i] = trgIDs.triggerIds().at(i);
    }
    // BBC, ZDC, VPD branches <------- To be verified/revisited
    for(Int_t ew = 0; ew < 2; ew++)
    {
	// BBC
	mEventData->mBbcADCSum[ew] = mMuEvent->triggerData()->bbcADCSum((StBeamDirection)ew);
	mEventData->mBbcADCSumLarge[ew] = mMuEvent->triggerData()->bbcADCSumLargeTile((StBeamDirection)ew);
	mEventData->mBbcEarliestTDC[ew] = mMuEvent->triggerData()->bbcEarliestTDC((StBeamDirection)ew);
	mEventData->mBbcEarliestTDCLarge[ew] = mMuEvent->triggerData()->bbcEarliestTDCLarge((StBeamDirection)ew);
	// ZDC
	mEventData->mZdcADCSum[ew] = 0;
	for(Int_t pmt = 1; pmt <= 3; pmt++)
	{
	    mEventData->mZdcADCSum[ew] += mMuEvent->triggerData()->zdcADC((StBeamDirection)ew,pmt); // is this correct?
	}
	// VPD
	mEventData->mVpdADCSum[ew] = 0;
	for(Int_t pmt = 1; pmt <= 16; pmt++)
	{
	    mEventData->mVpdADCSum[ew] += mMuEvent->triggerData()->vpdADC((StBeamDirection)ew,pmt); // is this correct?
	}
    } 
    mEventData->mTofMultiplicity = mMuEvent->triggerData()->tofMultiplicity();

    // if(mMuDst->primaryVertex())
    // {
    // 	mEventData->mVx = mMuDst->primaryVertex()->position().x();
    // 	mEventData->mVy = mMuDst->primaryVertex()->position().y();
    // 	mEventData->mVz = mMuDst->primaryVertex()->position().z();

    // 	cout << "----->Vertex:"<< mMuDst->primaryVertex()->position().z() <<"\t"<< mEventData->mVz<<endl;

    // }

    mEventData->mVx = mMuDst->event()->primaryVertexPosition().x();
    mEventData->mVy = mMuDst->event()->primaryVertexPosition().y();
    mEventData->mVz = mMuDst->event()->primaryVertexPosition().z();
    
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
	mEventData->mBspin = -1;
	mEventData->mYspin = -1;
	break;
    }
    case 6:
    {
	mEventData->mBspin = -1;
	mEventData->mYspin = +1;	
	break;
    }
    case 9:
    {
	mEventData->mBspin = +1;
	mEventData->mYspin = -1;	
	break;
    }
    case 10:
    {
	mEventData->mBspin = +1;
	mEventData->mYspin = +1;
	break;
    }
    default: //Undefined
	mEventData->mBspin = 0;
	mEventData->mYspin = 0;      
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
	mFmsPointPairData->mZgg = mPair->zgg();
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
	mRpsTrackData =  new((*mRpsArray)[i])TStRpsTrackData();	
	mRpsTrk = mRpsMuColl->track(i);
	
	mRpsTrackData->mType = (mRpsTrk->type() == StMuRpsTrack::rpsGlobal) ? 1 : 0;
	mRpsTrackData->mNplanes = mRpsTrk->planesUsed();
	mRpsTrackData->mBranch = mRpsTrk->branch();	
	mRpsTrackData->mTheta = 1000.0*mRpsTrk->theta();	
	mRpsTrackData->mTheta_x = 1000.0*mRpsTrk->theta(0);	
	mRpsTrackData->mTheta_y = 1000.0*mRpsTrk->theta(1);	
	mRpsTrackData->mEta = mRpsTrk->eta();
	mRpsTrackData->mPhi = mRpsTrk->phi();
	mRpsTrackData->mPt = mRpsTrk->pt();		
	mRpsTrackData->mP = mRpsTrk->p();		
	mRpsTrackData->mPx = mRpsTrk->pVec().x();		
	mRpsTrackData->mPy = mRpsTrk->pVec().y();		
	mRpsTrackData->mPz = mRpsTrk->pVec().z();		
	mRpsTrackData->mXi = mRpsTrk->xi(mBeamMom); // Beam momentum is approximate		
	mRpsTrackData->mMt = -1.0*mRpsTrk->t(mBeamMom);	
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
void TStAnTreeMaker::ProjectTrack()
{
    Bool_t hasProj = kFALSE;
    
    hasProj = mEmcPosition.trackOnEmc(&mPosition_proj, &mMomentum_proj, mTrack, mField, mEmcRadius);    
    if (!hasProj)
	hasProj = mEmcPosition.trackOnEEmc(&mPosition_proj, &mMomentum_proj, mTrack, mField, mEEmcZSMD);

    if(hasProj)
    {
	mProjX = mPosition_proj.x();	
	mProjY = mPosition_proj.y();	
	mProjZ = mPosition_proj.z();	
    }
    else
    {
	mProjX = -999;
	mProjY = -999;
	mProjZ = -999;
    }
}
//_____________________________________________________________________________
Int_t TStAnTreeMaker::MakeChargedPid()
{
    mField = StMuDst::event()->magneticField() / 10.0;
    Int_t pid = -1;
    
    for(Int_t i = 0; i < mMuDst->numberOfPrimaryTracks(); ++i)
    {
	mTrack = mMuDst->primaryTracks(i);
	mPidTagger->SetTrack(mTrack);
	ProjectTrack();
	if(mFillHist)
	    FillHist(0); //Histogram before PID cut
	
	//---------The particle tagging order is very important and to be finalized later ----------------
	//---- Pion -----
	pid = mPidTagger->TagPion();
	if(pid == mPidTagger->kPionId)
	{
	    mChargedPidData =  new((*mPiArray)[mNpi])TStChargedPidData();
	    mChargedPidData->mQ = mTrack->charge();
	    mChargedPidData->mPt = mTrack->pt();
	    mChargedPidData->mEta = mTrack->eta();
	    mChargedPidData->mPhi = mTrack->phi();
	    mChargedPidData->mX = mProjX;
	    mChargedPidData->mY = mProjY;
	    mChargedPidData->mZ = mProjZ;

	    if(mFillHist)
		FillHist(mPidTagger->kPionId);	    
	    ++mNpi;
	    continue;
	}
	//----- Proton --------
	pid = mPidTagger->TagProton();
	if(pid == mPidTagger->kProtonId)
	{
	    mChargedPidData =  new((*mPrArray)[mNpr])TStChargedPidData();
	    mChargedPidData->mQ = mTrack->charge();
	    mChargedPidData->mPt = mTrack->pt();
	    mChargedPidData->mEta = mTrack->eta();
	    mChargedPidData->mPhi = mTrack->phi();
	    mChargedPidData->mX = mProjX;
	    mChargedPidData->mY = mProjY;
	    mChargedPidData->mZ = mProjZ;
	    
	    if(mFillHist)
		FillHist(mPidTagger->kProtonId);	    
	    ++mNpr;
	    continue;
	}
	//----- Kaon --------
	pid = mPidTagger->TagKaon();
	if(pid == mPidTagger->kKaonId)
	{
	    mChargedPidData =  new((*mKaArray)[mNka])TStChargedPidData();
	    mChargedPidData->mQ = mTrack->charge();
	    mChargedPidData->mPt = mTrack->pt();
	    mChargedPidData->mEta = mTrack->eta();
	    mChargedPidData->mPhi = mTrack->phi();
	    mChargedPidData->mX = mProjX;
	    mChargedPidData->mY = mProjY;
	    mChargedPidData->mZ = mProjZ;

	    if(mFillHist)
		FillHist(mPidTagger->kKaonId);
	    ++mNka;
	    continue;
	}
	//--- Electron ----
	pid = mPidTagger->TagElectron();
	if(pid == mPidTagger->kElectronId)
	{
	    mChargedPidData =  new((*mElArray)[mNe])TStChargedPidData();
	    mChargedPidData->mQ = mTrack->charge();
	    mChargedPidData->mPt = mTrack->pt();
	    mChargedPidData->mEta = mTrack->eta();
	    mChargedPidData->mPhi = mTrack->phi();
	    mChargedPidData->mX = mProjX;
	    mChargedPidData->mY = mProjY;
	    mChargedPidData->mZ = mProjZ;

	    if(mFillHist)
		FillHist(mPidTagger->kElectronId);	    
	    ++mNe;
	    continue;
	}
	//----- Muon --------
	pid = mPidTagger->TagMuon();
	if(pid == mPidTagger->kMuonId)
	{
	    mChargedPidData =  new((*mMuArray)[mNmu])TStChargedPidData();
	    mChargedPidData->mQ = mTrack->charge();
	    mChargedPidData->mPt = mTrack->pt();
	    mChargedPidData->mEta = mTrack->eta();
	    mChargedPidData->mPhi = mTrack->phi();
	    mChargedPidData->mX = mProjX;
	    mChargedPidData->mY = mProjY;
	    mChargedPidData->mZ = mProjZ;

	    if(mFillHist)
		FillHist(mPidTagger->kMuonId);
	    ++mNmu;
	    continue;
	}
	//-------- Unknown PID ----------
	mChargedPidData =  new((*mUkArray)[mNuk])TStChargedPidData();
	mChargedPidData->mQ = mTrack->charge();
	mChargedPidData->mPt = mTrack->pt();
	mChargedPidData->mEta = mTrack->eta();
	mChargedPidData->mPhi = mTrack->phi();
	mChargedPidData->mX = mProjX;
	mChargedPidData->mY = mProjY;
	mChargedPidData->mZ = mProjZ;
	++mNuk;
    }
    return kStOK;
}

//_____________________________________________________________________________
void TStAnTreeMaker::FillHist(Int_t particleId)
{
    switch(particleId)
    {
        case 0:	      
	    mBeta = mTrack->btofPidTraits().beta();
	    mM2 = mTrack->p().mag2()*(1 - pow(mBeta, 2)) / pow(mBeta, 2);
	    mDedxVsQp->Fill(mTrack->p().mag()*mTrack->charge(), mTrack->dEdx()*1.0e6);
	    mM2VsQp->Fill(mTrack->p().mag()*mTrack->charge(), mM2);
	    // cout <<mBeta<<"\t"<<mM2<<"\t"<<mTrack->p().mag()<<"\t"<<mTrack->nSigmaPion()<<endl;
	    break;
        case 11:
	    mDedxVsQp_e->Fill(mTrack->p().mag()*mTrack->charge(), mTrack->dEdx()*1.0e6);
	    mM2VsQp_e->Fill(mTrack->p().mag()*mTrack->charge(), mPidTagger->GetM2()); 
	    break;
        case 211:
	    mDedxVsQp_pi->Fill(mTrack->p().mag()*mTrack->charge(), mTrack->dEdx()*1.0e6);
	    mM2VsQp_pi->Fill(mTrack->p().mag()*mTrack->charge(), mPidTagger->GetM2()); 
	    break;
        case 2212:
	    mDedxVsQp_pr->Fill(mTrack->p().mag()*mTrack->charge(), mTrack->dEdx()*1.0e6);
	    mM2VsQp_pr->Fill(mTrack->p().mag()*mTrack->charge(), mPidTagger->GetM2()); 
	    break;
        case 321:
	    mDedxVsQp_ka->Fill(mTrack->p().mag()*mTrack->charge(), mTrack->dEdx()*1.0e6);
	    mM2VsQp_ka->Fill(mTrack->p().mag()*mTrack->charge(), mPidTagger->GetM2()); 
	    break;
        case 13:
	    mDedxVsQp_mu->Fill(mTrack->p().mag()*mTrack->charge(), mTrack->dEdx()*1.0e6);
	    mM2VsQp_mu->Fill(mTrack->p().mag()*mTrack->charge(), mPidTagger->GetM2mu()); 
	    break;		
        // defaut:
	//     break;
    }
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

