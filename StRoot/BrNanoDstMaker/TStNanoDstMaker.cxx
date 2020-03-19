// Filename: TStNanoDstMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif

#include "TStNanoDstMaker.h"
#include "StEvent/StFmsPointPair.h"
#include "StEvent/StFmsCollection.h"
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"
#include "StEvent/StTriggerId.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"
#include "StEvent/StEmcPoint.h"
#include "StEvent/StEmcCollection.h"

ClassImp(TStNanoDstMaker)

//_____________________________________________________________________________ 
TStNanoDstMaker::TStNanoDstMaker(const char *name):StMaker(name)
{
    mBeamMom = 100.0; //Overwrite from runMacro
    mEventCount = 0;
    mSyncOnEEmc = kFALSE;
    mUseRpsAfterburner = kFALSE;
    
    // Turn Off TObject streamer to save space on the file.
    // see: https://root.cern.ch/doc/master/classTClass.html#a606b0442d6fec4b1cd52f43bca73aa51
    TStEventData::Class()->IgnoreTObjectStreamer();
    TStTrackData::Class()->IgnoreTObjectStreamer();
    TStRpsTrackData::Class()->IgnoreTObjectStreamer();
    TStFmsPointPairData::Class()->IgnoreTObjectStreamer();
    TStChargedPidData::Class()->IgnoreTObjectStreamer();
    TStEmcPointData::Class()->IgnoreTObjectStreamer();
    
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
    
    //EMC Buffer
    mEmcArray = new TClonesArray("TStEmcPointData");

    mUseEvent = kFALSE;
    mUseTpc = kFALSE;
    mUseEmc = kFALSE;
    mUseFms = kFALSE;
    mUseRps = kFALSE;
    mUseEEmc = kFALSE;
}

//_____________________________________________________________________________ 
TStNanoDstMaker::~TStNanoDstMaker()
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

    //EMC Buffer    
    delete mEmcArray;
}


//_____________________________________________________________________________ 
Int_t TStNanoDstMaker::Init()
{
    if(mTrigIDs.size() > 0)
	cout << "-------->Triggers Included:" <<endl;
    for(mIt = mTrigIDs.begin(); mIt != mTrigIDs.end(); ++mIt)
	cout <<"------->: "<<*mIt <<endl;
    
    mFmsDbMk = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    if (!mFmsDbMk)
    {
	LOG_ERROR <<"TStNanoDstMaker::InitRun - !StFmsDbMaker" <<endl;
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

    //mMuDstMaker = (StMuDstMaker*)GetInputDS("MuDst");
    if(!mMuDstMaker)
    {
	cout << "TStNanoDstMaker::Init - Unable to to retrieve MuDstMaker pointer" <<endl;
	return kStErr;
    }
    mAfterburner = new StMuRpsUtil(mMuDstMaker); // RP afterburner
    
    return kStOK;
}

//_____________________________________________________________________________ 
Int_t TStNanoDstMaker::InitRun(int runnumber)
{
    mSpinDbMaker = static_cast<StSpinDbMaker*>(GetMaker("spinDb"));
    mSpinDbMaker->print();

    return kStOK;
}

//_____________________________________________________________________________ 
void TStNanoDstMaker::SetBranches()
{
    //Event branches
    if(mUseEvent)
	mTree->Branch("event", mEventData, 256000, 99);

    //TPC Track branch
    //mTree->Branch("track", &mTrackArray, 256000, 99);

    //Charged PID branches
    if(mUseTpc)
    {
	mTree->Branch("e", &mElArray, 256000, 99);
	mTree->Branch("pi", &mPiArray, 256000, 99);
	mTree->Branch("pr", &mPrArray, 256000, 99);
	mTree->Branch("ka", &mKaArray, 256000, 99);
	mTree->Branch("mu", &mMuArray, 256000, 99);
	mTree->Branch("uk", &mUkArray, 256000, 99);
    }
    //FMS branches
    if(mUseFms)
	mTree->Branch("fmsPointPair", &mFmsArray, 256000, 99);

    //RP branches
    if(mUseRps)
	mTree->Branch("rpTrack", &mRpsArray, 256000, 99);

    //EMC branches
    if(mUseEmc)
	mTree->Branch("emcPoint", &mEmcArray, 256000, 99);

    //EEMC Flag
    if(mUseEEmc)
	mTree->Branch("eemcStatus", &mHasEEmcEvent, "eemcStatus/I");
    
    cout << "Done setting branches..." <<endl;
}

//_____________________________________________________________________________
void TStNanoDstMaker::InitHist()
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
void TStNanoDstMaker::Reset()
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

    mEmcArray->Clear();    
}
    
//_____________________________________________________________________________
Int_t TStNanoDstMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");

    if (!mMuDst || !mEvent)
    {
	cout <<"TStNanoDstMaker::Make - !MuDst or !StEvent" <<endl;
	return kStErr;
    }
    mMuEvent = mMuDst->event();

    // Event filtering has been moved to TStFmsRpFilterMaker class.
    // if(!AcceptEvent())
    // 	return kStSkip;

    //------ Reset Buffer --------
    Reset();

    //------- EEmc tree is separate.  But it can be synchronized here. If synced, it keeps event only if there is a vlaid. If not synced, it has a flag indicating if corresponding EEMC event is available. Trees will have different number of entries in the later case and you need to use the flag to synchronize. 
    //EEMC event
    if(mUseEEmc)
    {
	mEEmcTreeMaker = (StEEmcTreeMaker_t*)GetMaker("EEmcTreeMaker");
	if(!mEEmcTreeMaker)
	{
	    cout << "TStNanoDstMaker::Make !StEEmcTreeMaker_t" <<endl;
	    return kStErr;
	}
	//Synchronize RP tree with EEMC part1 tree. This will keep event only if EEMC tree has an entry for this event.
	if(mSyncOnEEmc)
	{
	    if(mEEmcTreeMaker->getNumPart1EventsWritten() != (mEventCount + 1))
		return kStSkip;
	}
	if(mEEmcTreeMaker->getNumPart1EventsWritten() != (mEventCount + 1))
	    mHasEEmcEvent = 1;
	else
	    mHasEEmcEvent = 0;
    }
    
    if(mUseEvent)
	MakeEvent();
    if(mUseFms)
	MakeFms();
    if(mUseRps)
	MakeRps();
    if(mUseTpc)
        //MakeTrack();
	MakeChargedPid();
    if(mUseEmc)
	MakeEmc();
    
    mTree->Fill();
    ++mEventCount;
    
    return kStOk;    
}
//_____________________________________________________________________________
Bool_t TStNanoDstMaker::AcceptEvent()
{
    /*
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
    */
    return kTRUE;
}
//_____________________________________________________________________________
Int_t TStNanoDstMaker::MakeEvent()
{    
    //Run no. and event no.     
    mEventData->SetRunNumber(mMuEvent->runNumber());
    mEventData->SetEventId(mMuEvent->eventId());

    //Trigger ids
    mTrigMuColl = &mMuEvent->triggerIdCollection();
    if(mTrigMuColl)
    {
	const StTriggerId trgIDs = mTrigMuColl->nominal();
	mEventData->SetNtrigs(trgIDs.triggerIds().size());

	if(mEventData->GetNtrigs() > mEventData->mMaxTrigs)
	{
	    std::cout << "TStNanoDstMaker::MakeEvent - The trigger buffer is out of limit. You must adjust it." << std::endl;
	    return kStErr;
	}
	
	for(Int_t i = 0; i < mEventData->GetNtrigs(); i++)
	    mEventData->SetTrigger(i, trgIDs.triggerIds().at(i));
    }
    // BBC, ZDC, VPD branches <------- To be verified/revisited
    for(Int_t ew = 0; ew < 2; ew++)
    {
	// BBC
	mEventData->SetBbcSumSmall(ew, mMuEvent->triggerData()->bbcADCSum((StBeamDirection)ew));
	mEventData->SetBbcSumLarge(ew, mMuEvent->triggerData()->bbcADCSumLargeTile((StBeamDirection)ew));
	mEventData->SetEarliestTdcSmall(ew, mMuEvent->triggerData()->bbcEarliestTDC((StBeamDirection)ew));
	mEventData->SetEarliestTdcLarge(ew, mMuEvent->triggerData()->bbcEarliestTDCLarge((StBeamDirection)ew));
	// ZDC
	mAdcSum[ew] = 0;
	for(Int_t pmt = 1; pmt <= 3; pmt++)
	{
	   mAdcSum[ew] += mMuEvent->triggerData()->zdcADC((StBeamDirection)ew,pmt); 
	}
	mEventData->SetZdcSum(ew, mAdcSum[ew]);
	// VPD
	mAdcSum[ew] = 0;
	for(Int_t pmt = 1; pmt <= 16; pmt++)
	{
	   mAdcSum[ew] += mMuEvent->triggerData()->vpdADC((StBeamDirection)ew,pmt); // replace by vpdADCSum if same
	}
	mEventData->SetVpdSum(ew, mAdcSum[ew]);
    } 
    mEventData->SetTofMultiplicity(mMuEvent->triggerData()->tofMultiplicity());
    mEventData->SetNprimaryTracks(mMuDst->numberOfPrimaryTracks());
    mEventData->SetNvertices(mMuDst->primaryVertices()->GetEntries());
    if(mMuDst->primaryVertex())
	mEventData->SetRanking(mMuDst->primaryVertex()->ranking());
    else
	mEventData->SetRanking(-1.0);
    
    // This approach leads to craches for some events
    // if(mMuDst->primaryVertex())
    // {
    // 	mEventData->mVx = mMuDst->primaryVertex()->position().x();
    // 	mEventData->mVy = mMuDst->primaryVertex()->position().y();
    // 	mEventData->mVz = mMuDst->primaryVertex()->position().z();

    // 	cout << "----->Vertex:"<< mMuDst->primaryVertex()->position().z() <<"\t"<< mEventData->mVz<<endl;

    // }

    mEventData->SetVx(mMuDst->event()->primaryVertexPosition().x());
    mEventData->SetVy(mMuDst->event()->primaryVertexPosition().y());
    mEventData->SetVz(mMuDst->event()->primaryVertexPosition().z());
    
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
	mEventData->SetBlueSpin(-1);
	mEventData->SetYellowSPin(-1);
	break;
    }
    case 6:
    {
	mEventData->SetBlueSpin(-1);
	mEventData->SetYellowSPin(+1);	
	break;
    }
    case 9:
    {
	mEventData->SetBlueSpin(+1);
	mEventData->SetYellowSPin(-1);	
	break;
    }
    case 10:
    {
	mEventData->SetBlueSpin(+1);
	mEventData->SetYellowSPin(+1);
	break;
    }
    default: //Undefined
	mEventData->SetBlueSpin(0);
	mEventData->SetYellowSPin(0);      
    }
  
    return kStOk;    
}
//_____________________________________________________________________________
Int_t TStNanoDstMaker::MakeFms()
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

	mFmsPointPairData->SetE(mPair->energy());
	mFmsPointPairData->SetM(mPair->mass());	      
	mFmsPointPairData->SetPt(mPair->pT());
	mFmsPointPairData->SetEta(mPair->eta());
	mFmsPointPairData->SetPhi(mPair->phi());
	mFmsPointPairData->SetZgg(mPair->zgg());
	mFmsPointPairData->SetDgg(mPair->dgg());
	mFmsPointPairData->SetX(mPair->x());
	mFmsPointPairData->SetY(mPair->y());	
	mFmsPointPairData->SetX1(mPair->point(0)->XYZ().x());	
	mFmsPointPairData->SetX2(mPair->point(1)->XYZ().x());
	mFmsPointPairData->SetY1(mPair->point(0)->XYZ().y());	
	mFmsPointPairData->SetY2(mPair->point(1)->XYZ().y());	
	mFmsPointPairData->SetFpsPid1(mPair->point(0)->fpsPid());	
	mFmsPointPairData->SetFpsPid2(mPair->point(1)->fpsPid());	
    }
    
    return kStOk;    
}
//_____________________________________________________________________________
Int_t TStNanoDstMaker::MakeRps()
{
    //------ Using afterburner ----
    // if(mUseRpsAfterburner)
    // {    
	mAfterburner->updateVertex(0.000415, 0.000455, 0.0); // specific to run 15 pp200 trans !!! To be set with set method
	mRpsMuColl = mAfterburner->process(); // executes afterburner
    // }
    //------
    
    //mRpsMuColl = mMuDst->RpsCollection();  //No afterburner
    if(!mRpsMuColl)
    {
	cout<<"No RP data for this event"<<endl;
	return kStSkip;
    }
    
    mRpNtracks = mRpsMuColl->numberOfTracks();
    Int_t trkType;
    for(Int_t i = 0; i < mRpNtracks; ++i)
    {	
	mRpsTrackData =  new((*mRpsArray)[i])TStRpsTrackData();	
	mRpsTrk = mRpsMuColl->track(i);
	trkType = (mRpsTrk->type() == StMuRpsTrack::rpsGlobal) ? 1 : 0;
	
	mRpsTrackData->SetType(trkType);
	mRpsTrackData->SetNplanes(mRpsTrk->planesUsed());
	mRpsTrackData->SetBranch(mRpsTrk->branch());	
	mRpsTrackData->SetTheta(1000.0*mRpsTrk->theta());	
	mRpsTrackData->SetThetaX(1000.0*mRpsTrk->theta(0));	
	mRpsTrackData->SetThetaY(1000.0*mRpsTrk->theta(1));	
	mRpsTrackData->SetEta(mRpsTrk->eta());
	mRpsTrackData->SetPhi(mRpsTrk->phi());
	mRpsTrackData->SetPt(mRpsTrk->pt());		
	mRpsTrackData->SetP(mRpsTrk->p());		
	mRpsTrackData->SetPx(mRpsTrk->pVec().x());		
	mRpsTrackData->SetPy(mRpsTrk->pVec().y());		
	mRpsTrackData->SetPz(mRpsTrk->pVec().z());		
	mRpsTrackData->SetXi(mRpsTrk->xi(mBeamMom)); // Beam momentum is approximate		
	mRpsTrackData->SetMt(-1.0*mRpsTrk->t(mBeamMom));	
    }

    //afterburner
    mAfterburner->clear(); //Critical!!!
    
    return kStOk;    
}

//_____________________________________________________________________________
Int_t TStNanoDstMaker::MakeTrack()
{
    if(!mMuDst)
    {
	LOG_ERROR << "TStNanoDstMaker::MakeTrack - No MuDst found" <<endm;
	return kStFatal;
    }

    for(Int_t i = 0; i < mMuDst->numberOfPrimaryTracks(); ++i)
    {
	mTrack = mMuDst->primaryTracks(i);
	mTrackData =  new((*mTrackArray)[i])TStTrackData();
	
	mTrackData->SetQ(mTrack->charge());
	mTrackData->SetEta(mTrack->eta());
	mTrackData->SetPhi(mTrack->phi());
	mTrackData->SetPt(mTrack->pt());	      	
    }

    return kStOK;
}
//_____________________________________________________________________________
void TStNanoDstMaker::ProjectTrack()
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
Int_t TStNanoDstMaker::MakeChargedPid()
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
	    mChargedPidData->SetQ(mTrack->charge());
	    mChargedPidData->SetPt(mTrack->pt());
	    mChargedPidData->SetEta(mTrack->eta());
	    mChargedPidData->SetPhi(mTrack->phi());
	    mChargedPidData->SetX(mProjX);
	    mChargedPidData->SetY(mProjY);
	    mChargedPidData->SetZ(mProjZ);

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
	    mChargedPidData->SetQ(mTrack->charge());
	    mChargedPidData->SetPt(mTrack->pt());
	    mChargedPidData->SetEta(mTrack->eta());
	    mChargedPidData->SetPhi(mTrack->phi());
	    mChargedPidData->SetX(mProjX);
	    mChargedPidData->SetY(mProjY);
	    mChargedPidData->SetZ(mProjZ);
	    
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
	    mChargedPidData->SetQ(mTrack->charge());
	    mChargedPidData->SetPt(mTrack->pt());
	    mChargedPidData->SetEta(mTrack->eta());
	    mChargedPidData->SetPhi(mTrack->phi());
	    mChargedPidData->SetX(mProjX);
	    mChargedPidData->SetY(mProjY);
	    mChargedPidData->SetZ(mProjZ);

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
	    mChargedPidData->SetQ(mTrack->charge());
	    mChargedPidData->SetPt(mTrack->pt());
	    mChargedPidData->SetEta(mTrack->eta());
	    mChargedPidData->SetPhi(mTrack->phi());
	    mChargedPidData->SetX(mProjX);
	    mChargedPidData->SetY(mProjY);
	    mChargedPidData->SetZ(mProjZ);

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
	    mChargedPidData->SetQ(mTrack->charge());
	    mChargedPidData->SetPt(mTrack->pt());
	    mChargedPidData->SetEta(mTrack->eta());
	    mChargedPidData->SetPhi(mTrack->phi());
	    mChargedPidData->SetX(mProjX);
	    mChargedPidData->SetY(mProjY);
	    mChargedPidData->SetZ(mProjZ);

	    if(mFillHist)
		FillHist(mPidTagger->kMuonId);
	    ++mNmu;
	    continue;
	}
	//-------- Unknown PID ----------
	mChargedPidData =  new((*mUkArray)[mNuk])TStChargedPidData();
	mChargedPidData->SetQ(mTrack->charge());
	mChargedPidData->SetPt(mTrack->pt());
	mChargedPidData->SetEta(mTrack->eta());
	mChargedPidData->SetPhi(mTrack->phi());
	mChargedPidData->SetX(mProjX);
	mChargedPidData->SetY(mProjY);
	mChargedPidData->SetZ(mProjZ);
	++mNuk;
    }
    return kStOK;
}

//_____________________________________________________________________________
void TStNanoDstMaker::FillHist(Int_t particleId)
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
//_____________________________________________________________________________
Int_t TStNanoDstMaker::MakeEmc()
{
    // mTrkMatchingMkr = (TStEmcTrackMatchingMaker*)GetMaker("TStEmcTrackMatchingMaker");
    // if(!mTrkMatchingMkr)
    // {
    // 	cout << "TStEmcTreeMaker::Make- Unable to retrieve EmcTrackMatchingMaker. Can Not proceed" <<endl;
    // 	return kStFatal;
    // }
    
    mVertex = mMuDst->event()->primaryVertexPosition();

    mEmcCollection = mMuDst->emcCollection();
    if(!mEmcCollection)
    {
	cout<<"No EMC data for this event"<<endl;
	return kStSkip;
    }
    StSPtrVecEmcPoint& mEmcPoints = mEmcCollection->barrelPoints();
    mEmcNpoints = mEmcPoints.size();
   
    for(Int_t i = 0; i < mEmcNpoints; ++i)
    {
	mEmcPointData =  new((*mEmcArray)[i])TStEmcPointData();	
	mEmcPoint = mEmcPoints[i];
	mEmcPointPos = mEmcPoint->position();  
	mEmcPointEng = mEmcPoint->energy();
	mEmcMomVec = (mEmcPointPos - mVertex).unit();
	mEmcMomVec *= mEmcPointEng;
	
	mEmcPointData->SetE(mEmcPointEng);
	mEmcPointData->SetQuality(mEmcPoint->quality());
	mEmcPointData->SetX(mEmcPointPos.x());
	mEmcPointData->SetY(mEmcPointPos.y());
	mEmcPointData->SetZ(mEmcPointPos.z());		
	mEmcPointData->SetPx(mEmcMomVec.x()); 
	mEmcPointData->SetPy(mEmcMomVec.y()); 
	mEmcPointData->SetPz(mEmcMomVec.z()); 
	mEmcPointData->SetNtracks(mEmcPoint->nTracks());		
    }

    return kStOK;
}


//_____________________________________________________________________________
Int_t TStNanoDstMaker::Finish()
{
    //Write histograms to root file etc.
    if(mSaveFile)
    {
	mFile->Write();
	mFile->Close();
    }
    return kStOK;
}

