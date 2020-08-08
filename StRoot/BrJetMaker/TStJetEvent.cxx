// Filename: TStJetEvent.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun 14 15:55:40 2020 (-0400)
// URL: jlab.org/~latif

#include "TStJetEvent.h"

#include "StSpinPool/StJetEvent/StJetElement.h"
#include "StSpinPool/StJetEvent/StJetParticle.h"
#include "StSpinPool/StJetEvent/StJetTower.h"


ClassImp(TStJetEvent)

//--------------------------------------------------------------------------------------------
void TStJetEvent::CopyTower(StJetTower*  t, TStJetTower* tower) //Copy from t to tower
{
    tower->SetId(t->id()); 
    tower->SetDetectorId(t->detectorId());
    tower->SetStatus(t->status());
	
    tower->SetAdc(t->adc());
    tower->SetPedestal(t->pedestal());
    tower->SetRms(t->rms());
	
    tower->SetEnergy(t->energy());
    tower->SetPt(t->pt());
    tower->SetEta(t->eta());
    tower->SetPhi(t->phi());	
    tower->SetJt(t->jt());
}

//---------------------------------------------------------------------------------------------
void TStJetEvent::CopyParticle(StJetParticle* t, TStJetParticle* particle)
{
    particle->SetId(t->id());
    particle->SetPt(t->pt());
    particle->SetEta(t->eta());
    particle->SetPhi(t->phi());
    particle->SetM(t->m());
    particle->SetE(t->e());
    particle->SetPdg(t->pdg());
    particle->SetStatus(t->status());
    particle->SetFirstMother(t->firstMother());
    particle->SetLastMother(t->lastMother());
    particle->SetFirstDaughter(t->firstDaughter());
    particle->SetLastDaughter(t->lastDaughter());
}

//-----------------------------------------------------------------------------------------------
// void TStJetEvent::CopyTrack(StJetTrack* t, TStJetTrack* track)
// {
//   track->mId             = t->id();
//   track->mDetectorId     = t->detectorId();
//   track->mFlag           = t->flag();
//   track->mCharge         = t->charge();
//   track->mNHits          = t->nHits();
//   track->mNHitsFit       = t->nHitsFit();
//   track->mNHitsPoss      = t->nHitsPoss();
//   track->mNHitsDedx      = t->nHitsDedx();
//   track->mDedx           = t->dEdx();
//   track->mBeta           = t->beta();
//   track->mFirstPoint     = t->firstPoint();
//   track->mLastPoint      = t->lastPoint();
//   track->mExitTowerId    = t->exitTowerId();
//   track->mExitDetectorId = t->exitDetectorId();
//   track->mExitPoint.SetPtEtaPhi(t->bemcRadius(),t->etaext(),t->phiext());
//   track->mDca.SetXYZ(t->dcaX(),t->dcaY(),t->dcaZ());
//   track->mDcaD           = t->dcaD();
//   track->mChi2           = t->chi2();
//   track->mChi2Prob       = t->chi2prob();
//   TVector3 mom(t->px(),t->py(),t->pz());
//   track->mPt             = mom.Pt();
//   track->mEta            = mom.Eta();
//   track->mPhi            = mom.Phi();
//   track->mNSigmaPion     = t->nSigmaPion();
//   track->mNSigmaKaon     = t->nSigmaKaon();
//   track->mNSigmaProton   = t->nSigmaProton();
//   track->mNSigmaElectron = t->nSigmaElectron();
// }
