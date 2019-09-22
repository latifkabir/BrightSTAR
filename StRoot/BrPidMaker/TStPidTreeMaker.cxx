// Filename: TStPidTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStPidTreeMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"

ClassImp(TStPidTreeMaker)

//_____________________________________________________________________________ 
TStPidTreeMaker::TStPidTreeMaker(const char *name):StMaker(name)
{
    mPidTagger = new TStPidTagger();
    
    eTrait = new TStPidTrait(); 
    piTrait = new TStPidTrait(); 
    muTrait = new TStPidTrait(); 
    kaTrait = new TStPidTrait(); 
    prTrait = new TStPidTrait(); 
}

//_____________________________________________________________________________ 
TStPidTreeMaker::~TStPidTreeMaker()
{
    delete mPidTagger;
    
    delete eTrait;
    delete piTrait;
    delete muTrait;
    delete kaTrait;
    delete prTrait;
}


//_____________________________________________________________________________ 
Int_t TStPidTreeMaker::Init()
{
    mFile = new TFile(mOutName, "RECREATE");
    mTree = new TTree("T", "Pid Tree");
    SetBranches();
    return kStOK;    
}

//_____________________________________________________________________________
void TStPidTreeMaker::SetBranches()
{
    //Event info
    mTree->Branch("event", &mEventNo, "event/I");
    // mTree->Branch("trig_MB", &mMB, "trig_MB/I");
    // mTree->Branch("trig_HT1", &mHT1, "trig_HT1/I");
    // mTree->Branch("trig_HT2", &mHT2, "trig_HT2/I");
    mTree->Branch("vz", &mVz, "vz/D");

    //Electron
    mTree->Branch("eN", &meN, "eN/I");                  // Number of electrons
    mTree->Branch("eQ", eTrait->Q, "eQ[eN]/I");         // Charge 
    mTree->Branch("eP", eTrait->P, "eP[eN][3]/D");      // Momentum Px, Py, Pz
    mTree->Branch("eX", eTrait->X, "eX[eN][3]/D");      // Position of projection on detector EMC, EEMC, FMS: x, y, z

    //Pion
    mTree->Branch("piN", &mpiN, "piN/I");               // Number of pions
    mTree->Branch("piQ", piTrait->Q, "piQ[piN]/I");     // Charge 
    mTree->Branch("piP", piTrait->P, "piP[piN][3]/D");  // Momentum Px, Py, Pz
    mTree->Branch("piX", piTrait->X, "piX[piN][3]/D");  // Position of projection on detector EMC, EEMC, FMS: x, y, z

    //Muon
    mTree->Branch("muN", &mmuN, "muN/I");               // Number of muons
    mTree->Branch("muQ", muTrait->Q, "muQ[muN]/I");     // Charge 
    mTree->Branch("muP", muTrait->P, "muP[muN][3]/D");  // Momentum Px, Py, Pz
    mTree->Branch("muX", muTrait->X, "muX[muN][3]/D");  // Position of projection on detector EMC, EEMC, FMS: x, y, z

    //Kaon
    mTree->Branch("kaN", &mkaN, "kaN/I");               // Number of kaons
    mTree->Branch("kaQ", kaTrait->Q, "kaQ[kaN]/I");     // Charge 
    mTree->Branch("kaP", kaTrait->P, "kaP[kaN][3]/D");  // Momentum Px, Py, Pz
    mTree->Branch("kaX", kaTrait->X, "kaX[kaN][3]/D");  // Position of projection on detector EMC, EEMC, FMS: x, y, z
    
    //Proton
    mTree->Branch("prN", &mprN, "prN/I");               // Number of protons
    mTree->Branch("prQ", prTrait->Q, "prQ[prN]/I");     // Charge 
    mTree->Branch("prP", prTrait->P, "prP[prN][3]/D");  // Momentum Px, Py, Pz
    mTree->Branch("prX", prTrait->X, "prX[prN][3]/D");  // Position of projection on detector EMC, EEMC, FMS: x, y, z
    
}


//_____________________________________________________________________________
void TStPidTreeMaker::ResetBuffer()
{

}

//_____________________________________________________________________________
Int_t TStPidTreeMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");

    if(!mMuDst)
    {
	LOG_ERROR << "TStPidTreeMaker::Make - No MuDst found" <<endm;
	return kStFatal;
    }
    ResetBuffer();
    Int_t pid = -1;
    for(Int_t i = 0; i < mMuDst->numberOfPrimaryTracks(); ++i)
    {
	mTrack = mMuDst->primaryTracks(i);
	mPidTagger->SetTrack(mTrack);

	//--- Electron ----
	pid = mPidTagger->TagElectron();
	if(pid == mPidTagger->kElectronId)
	{
	    eTrait->Q[meN] = mTrack->charge();
	    eTrait->P[meN][0] = mTrack->p().x();
	    eTrait->P[meN][1] = mTrack->p().y();
	    eTrait->P[meN][2] = mTrack->p().z();
	    //---- Position of track projection here --------
	    
	    ++meN;
	    continue;
	}
	//---- Pion -----
	pid = mPidTagger->TagPion();
	if(pid == mPidTagger->kPionId)
	{
	    piTrait->Q[mpiN] = mTrack->charge();
	    piTrait->P[mpiN][0] = mTrack->p().x();
	    piTrait->P[mpiN][1] = mTrack->p().y();
	    piTrait->P[mpiN][2] = mTrack->p().z();
	    //---- Position of track projection here --------
	    
	    ++mpiN;
	    continue;
	}
	//----- Kaon --------
	pid = mPidTagger->TagKaon();
	if(pid == mPidTagger->kKaonId)
	{
	    kaTrait->Q[mkaN] = mTrack->charge();
	    kaTrait->P[mkaN][0] = mTrack->p().x();
	    kaTrait->P[mkaN][1] = mTrack->p().y();
	    kaTrait->P[mkaN][2] = mTrack->p().z();
	    //---- Position of track projection here --------
	    
	    ++mkaN;
	    continue;
	}
	//----- Muon --------
	pid = mPidTagger->TagMuon();
	if(pid == mPidTagger->kMuonId)
	{
	    muTrait->Q[mmuN] = mTrack->charge();
	    muTrait->P[mmuN][0] = mTrack->p().x();
	    muTrait->P[mmuN][1] = mTrack->p().y();
	    muTrait->P[mmuN][2] = mTrack->p().z();
	    //---- Position of track projection here --------
	    
	    ++mmuN;
	    continue;
	}
	//----- Proton --------
	pid = mPidTagger->TagProton();
	if(pid == mPidTagger->kProtonId)
	{
	    prTrait->Q[mprN] = mTrack->charge();
	    prTrait->P[mprN][0] = mTrack->p().x();
	    prTrait->P[mprN][1] = mTrack->p().y();
	    prTrait->P[mprN][2] = mTrack->p().z();
	    //---- Position of track projection here --------
	    
	    ++mprN;	    
	}
    }
	    
    return kStOK;
}

//_____________________________________________________________________________
Int_t TStPidTreeMaker::Finish()
{
    //Write histograms to root file etc.

    return kStOK;
}
