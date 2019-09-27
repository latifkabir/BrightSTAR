// Filename: TStPidTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif

#include <algorithm>
#include "TStPidTreeMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

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

    delete mDedxVsQp;
    delete mM2VsQp;
    delete mDedxVsQp_e;
    delete mM2VsQp_e;
    delete mDedxVsQp_pi;
    delete mM2VsQp_pi;
    delete mDedxVsQp_pr;
    delete mM2VsQp_pr;
    delete mDedxVsQp_ka;
    delete mM2VsQp_ka;
    delete mDedxVsQp_mu;
    delete mM2VsQp_mu;
}


//_____________________________________________________________________________ 
Int_t TStPidTreeMaker::Init()
{
    mFile = new TFile(mOutName, "RECREATE");
    mTree = new TTree("T", "Pid Tree");
    SetBranches();

    mEmcRadius = (Double_t)StEmcGeom::instance("bemc")->Radius(); 
    
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
    mEventNo = -1;
    mVz = -999;
    
    meN = 0;
    mpiN = 0;
    mmuN = 0;
    mkaN = 0;
    mprN = 0;

    std::fill_n(eTrait->Q, TStPidTagger::kMaxTracks, -999);
    std::fill_n(&eTrait->P[0][0], 3*TStPidTagger::kMaxTracks, -999);
    std::fill_n(&eTrait->X[0][0], 3*TStPidTagger::kMaxTracks, -999);
    std::fill_n(piTrait->Q, TStPidTagger::kMaxTracks, -999);
    std::fill_n(&piTrait->P[0][0], 3*TStPidTagger::kMaxTracks, -999);
    std::fill_n(&piTrait->X[0][0], 3*TStPidTagger::kMaxTracks, -999);
    std::fill_n(prTrait->Q, TStPidTagger::kMaxTracks, -999);
    std::fill_n(&prTrait->P[0][0], 3*TStPidTagger::kMaxTracks, -999);
    std::fill_n(&prTrait->X[0][0], 3*TStPidTagger::kMaxTracks, -999);
    std::fill_n(kaTrait->Q, TStPidTagger::kMaxTracks, -999);
    std::fill_n(&kaTrait->P[0][0], 3*TStPidTagger::kMaxTracks, -999);
    std::fill_n(&kaTrait->X[0][0], 3*TStPidTagger::kMaxTracks, -999);    
    std::fill_n(muTrait->Q, TStPidTagger::kMaxTracks, -999);
    std::fill_n(&muTrait->P[0][0], 3*TStPidTagger::kMaxTracks, -999);
    std::fill_n(&muTrait->X[0][0], 3*TStPidTagger::kMaxTracks, -999);
        
}
//_____________________________________________________________________________
void TStPidTreeMaker::ProjectTrack()
{
    Bool_t hasProj = kFALSE;
    
    hasProj = mEmcPosition.trackOnEmc(&mPosition_proj, &mMomentum_proj, mTrack, mField, mEmcRadius);    
    if (!hasProj)
	hasProj = mEmcPosition.trackOnEEmc(&mPosition_proj, &mMomentum_proj, mTrack, mField, mEEmcZSMD);

    if(!hasProj)
	return;    
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
    mField = StMuDst::event()->magneticField() / 10.0;
    Int_t pid = -1;

    mEventNo = mMuDst->event()->eventNumber();
    if(mMuDst->primaryVertex())
	mVz = mMuDst->primaryVertex()->position().z();
    
    for(Int_t i = 0; i < mMuDst->numberOfPrimaryTracks(); ++i)
    {
	mTrack = mMuDst->primaryTracks(i);
	mPidTagger->SetTrack(mTrack);
	ProjectTrack();
	if(mFillHist)
	    FillHist(0); //Histogram before PID cut

	if(mpiN >= TStPidTagger::kMaxTracks || meN >= TStPidTagger::kMaxTracks || mprN >= TStPidTagger::kMaxTracks || mmuN >= TStPidTagger::kMaxTracks || mkaN >= TStPidTagger::kMaxTracks)
	{
	    cout << "TStPidMaker::Make - Array size out of range"<<endl;
	    return -1;
	}
	
	//---------The particle tagging order is very important and to be finalized later ----------------
	//---- Pion -----
	pid = mPidTagger->TagPion();
	if(pid == mPidTagger->kPionId)
	{
	    piTrait->Q[mpiN] = mTrack->charge();
	    piTrait->P[mpiN][0] = mTrack->p().x();
	    piTrait->P[mpiN][1] = mTrack->p().y();
	    piTrait->P[mpiN][2] = mTrack->p().z();
	    piTrait->X[mpiN][0] = mPosition_proj.x();
	    piTrait->X[mpiN][1] = mPosition_proj.y();
	    piTrait->X[mpiN][2] = mPosition_proj.z();

	    if(mFillHist)
		FillHist(mPidTagger->kPionId);	    
	    ++mpiN;
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
	    prTrait->X[mprN][0] = mPosition_proj.x();
	    prTrait->X[mprN][1] = mPosition_proj.y();
	    prTrait->X[mprN][2] = mPosition_proj.z();
	    
	    if(mFillHist)
		FillHist(mPidTagger->kProtonId);	    
	    ++mprN;
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
	    kaTrait->X[mkaN][0] = mPosition_proj.x();
	    kaTrait->X[mkaN][1] = mPosition_proj.y();
	    kaTrait->X[mkaN][2] = mPosition_proj.z();	    

	    if(mFillHist)
		FillHist(mPidTagger->kKaonId);
	    ++mkaN;
	    continue;
	}
	//--- Electron ----
	pid = mPidTagger->TagElectron();
	if(pid == mPidTagger->kElectronId)
	{
	    eTrait->Q[meN] = mTrack->charge();
	    eTrait->P[meN][0] = mTrack->p().x();
	    eTrait->P[meN][1] = mTrack->p().y();
	    eTrait->P[meN][2] = mTrack->p().z();
	    eTrait->X[meN][0] = mPosition_proj.x();
	    eTrait->X[meN][1] = mPosition_proj.y();
	    eTrait->X[meN][2] = mPosition_proj.z();	    

	    if(mFillHist)
		FillHist(mPidTagger->kElectronId);	    
	    ++meN;
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
	    muTrait->X[mmuN][0] = mPosition_proj.x();
	    muTrait->X[mmuN][1] = mPosition_proj.y();
	    muTrait->X[mmuN][2] = mPosition_proj.z();

	    if(mFillHist)
		FillHist(mPidTagger->kMuonId);
	    ++mmuN;
	    //continue;
	}
    }
    mTree->Fill();	    
    return kStOK;
}

//_____________________________________________________________________________
Bool_t TStPidTreeMaker::AcceptTrigger()
{
    //Trigger cut here
    //Trigger should be added only from Run Macros.
    
    return kTRUE;
}
//_____________________________________________________________________________
Bool_t TStPidTreeMaker::AcceptTrack()
{
    //Track quality cut here

    return kTRUE;
}

//_____________________________________________________________________________
Bool_t TStPidTreeMaker::AcceptVertex()
{
    //Vertex quality cut here

    return kTRUE;
}

//_____________________________________________________________________________
void TStPidTreeMaker::FillHist(Int_t particleId)
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
Int_t TStPidTreeMaker::Finish()
{
    //Write histograms to root file etc.
    mFile->Write();
    return kStOK;
}
