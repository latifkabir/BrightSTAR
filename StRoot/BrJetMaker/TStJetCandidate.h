// Filename: TStJetCandidate.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Dec 20 13:01:28 2019 (-0500)
// URL: jlab.org/~latif

#ifndef TSTJETCANDIDATE_H
#define TSTJETCANDIDATE_H

#include <string>
#include <iostream>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TRefArray.h"
#include "TObjArray.h"
#include "TClonesArray.h"

//--------------------------------------------------------
class TStJetParticle : public TObject
{
protected:
    Short_t mId;
    Int_t mPdg;
    Int_t mStatus;
    Int_t mFirstMother;
    Int_t mLastMother;
    Int_t mFirstDaughter;
    Int_t mLastDaughter;

    Float_t mPt;
    Float_t mEta;
    Float_t mPhi;
    Float_t mM;
    Float_t mE;    

public:
    Short_t GetId(){return mId;}
    Int_t GetPdg(){return mPdg;}
    Int_t GetStatus(){return mStatus;}
    Int_t GetFirstMother(){return mFirstMother;}
    Int_t GetLastMother(){return mLastMother;}
    Int_t GetFirstDaughter(){return mFirstDaughter;}
    Int_t GetLastDaughter(){return mLastDaughter;}

    Float_t GetPt(){return mPt;}
    Float_t GetEta(){return mEta;}
    Float_t GetPhi(){return mPhi;}
    Float_t GetM(){return mM;}
    Float_t GetE(){return mE;}

    TLorentzVector GetFourMomentum(){TLorentzVector lv; lv.SetPtEtaPhiE(mPt, mEta, mPhi, mE); return lv;}

    void SetId(Short_t id){mId = id;}
    void SetPdg(Int_t pdg){mPdg = pdg;}
    void SetStatus(Int_t status){mStatus = status;}
    void SetFirstMother(Int_t mother){mFirstMother = mother;}
    void SetLastMother(Int_t mother){mLastMother = mother;}
    void SetFirstDaughter(Int_t daughter){mFirstDaughter = daughter;}
    void SetLastDaughter(Int_t daughter){mLastDaughter = daughter;}

    void SetPt(Float_t pt){mPt = pt;}
    void SetEta(Float_t eta){mEta = eta;}
    void SetPhi(Float_t phi){mPhi = phi;}
    void SetM(Float_t m){mM = m;}
    void SetE(Float_t e){mE = e;}
    
    TStJetParticle()
    {
	Reset();
    }
    ~TStJetParticle()
    {
    }

    void Reset()
    {
	mId		= -1;	//
	mPdg		= -1;	//
	mStatus		= -1;	//
	mFirstMother	= -1;	//
	mLastMother	= -1;	//
	mFirstDaughter	= -1;	//
	mLastDaughter	= -1;	//

	mPt		= -1;	//
	mEta		= -999; //
	mPhi		= -999; //
	mM		= -1;	//
	mE		= -1;   // 
    }

    ClassDef(TStJetParticle, 1 )
};

//--------------------------------------------------------
class TStJetTower : public TObject
{
protected:
    Float_t mEnergy;
    Float_t mPt;
    Float_t mEta;
    Float_t mPhi;
    Float_t mJt;

    Short_t mId;
    Short_t mDetectorId;
    Short_t mStatus;

    Short_t mAdc;
    Float_t mPedestal;
    Float_t mRms;


public:
    Float_t GetEnergy(){return GetMomentum().Mag();}
    Float_t GetPt(){return mPt;}
    Float_t GetEta(){return mEta;}
    Float_t GetPhi(){return mPhi;}
    Float_t GetJt(){return mJt;}
    Short_t GetId(){return mId;}
    Short_t GetDetectorId(){return mDetectorId;}
    Short_t GetAdc(){return mAdc;}
    Float_t GetPedestal(){return mPedestal;}
    Float_t GetRms(){return mRms;}
    Short_t GetStatus(){return mStatus;}
    TVector3 GetMomentum(){TVector3 mom; mom.SetPtEtaPhi(mPt, mEta, mPhi); return mom;}
    
    void SetEnergy(Float_t energy){mEnergy = energy;}
    void SetPt(Float_t pt){mPt = pt;}
    void SetEta(Float_t eta){mEta = eta;}
    void SetPhi(Float_t phi){mPhi = phi;}
    void SetJt(Float_t jt){mJt = jt;}
    void SetId(Short_t id){mId = id;}
    void SetDetectorId(Short_t id){mDetectorId = id;}
    void SetAdc(Short_t adc){mAdc = adc;}
    void SetPedestal(Float_t ped){mPedestal = ped;}
    void SetRms(Float_t rms){mRms = rms;}
    void SetStatus(Short_t status){mStatus = status;}
    
    TStJetTower()
    {
	Reset();
    }
    ~TStJetTower()
    {
    }

    void Reset()
    {
	mEnergy = -1;		//
	mPt = -1;		//
	mEta = -999;		//
	mPhi = -999;		//
	mJt = -1;		//
	mId = -1;		//
	mDetectorId = -1;	//
	mAdc = -1;		//
	mPedestal = -999;	//
	mRms = -999;		//
	mStatus = -1;		//
    }

    ClassDef(TStJetTower, 1 )
};

//--------------------------------------------------------
class TStJetCandidate : public TObject
{
protected:
    Int_t mNphotons;		//
    Double_t mEta;		//
    Double_t mPhi;		//
    Double_t mE;		//
    Double_t mPt;		//
    Double_t mX;		//
    Double_t mY;		//
    Double_t mRt;		//   
    TRefArray mTowers;		//
    TRefArray mParticles;	// Used for embedding only
    //TRefArray mTracks;	        //  Currently Not used
    
public:       
    Int_t GetNphotons(){return mNphotons;}
    Double_t GetEta(){return mEta;}
    Double_t GetPhi(){return mPhi;}
    Double_t GetE(){return mE;}
    Double_t GetPt(){return mPt;}
    Double_t GetX(){return mX;}
    Double_t GetY(){return mY;}
    Double_t GetRt(){return mRt;}

    void SetNphotons(Int_t nPhotons){mNphotons = nPhotons;}
    void SetEta(Double_t eta){mEta = eta;}
    void SetPhi(Double_t phi){mPhi = phi;}
    void SetE(Double_t e){mE = e;}
    void SetPt(Double_t pt){mPt = pt;}
    void SetX(Double_t x){mX = x;}
    void SetY(Double_t y){mY = y;}
    void SetRt(Double_t rt){mRt = rt;} 

    //TStJetTrack* AddTrack(TStJetTrack* track) { mTracks.Add((TObject*)track); return (TStJetTrack*)mTracks.Last(); }
    TStJetTower* AddTower(TStJetTower* tower) { mTowers.Add((TObject*)tower); return (TStJetTower*)mTowers.Last(); }
    TStJetParticle* AddParticle(TStJetParticle* particle) { mParticles.Add((TObject*)particle); return (TStJetParticle*)mParticles.Last(); }

    //Int_t NumberOfTracks() const { return mTracks.GetEntriesFast(); }
    Int_t GetNumberOfTowers() const { return mTowers.GetEntriesFast(); }
    Int_t GetNumberOfParticles() const { return mParticles.GetEntriesFast(); }
    
    //TStJetTrack* GetTrack(int i) const { return (TStJetTrack*)mTracks.At(i); }
    TStJetTower* GetTower(int i) const { return (TStJetTower*)mTowers.At(i); }
    TStJetParticle* GetParticle(int i) const { return (TStJetParticle*)mParticles.At(i); }

    TStJetCandidate()
    {
	Reset();
    }
    ~TStJetCandidate()
    {
	
    }

    void Reset()
    {
	mNphotons	= -1;		//
	mEta		= -999;		//
	mPhi		= -999;		//
	mE		= -1;		//
	mPt		= -1;		//
	mX		= -999;		//
	mY		= -999;		//
	mRt		= -999;		//

	mTowers.Clear();
	mParticles.Clear();
    }
    
    ClassDef(TStJetCandidate, 3)
};

#endif



