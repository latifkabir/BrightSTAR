// Filename: TStTrackData.h
// Description: Container for track information. 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Dec 20 13:01:28 2019 (-0500)
// URL: jlab.org/~latif

#ifndef TSTTRACKDATA_H
#define TSTTRACKDATA_H

#include <string>
#include <iostream>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TApplication.h"

class TStTrackData : public TObject
{
protected:
    Float_t 	mPt;			// primary track pt
    Float_t 	mEta;			// primary track Eta
    Float_t 	mPhi;			// primary track phi
    UShort_t 	mId;			// track Id
    Float_t 	mDedx;			// dEdx
    Int_t 	mQ;			// q
    Int_t 	mNHitsFit;		// nHitsFit - TPC
    Int_t 	mNHitsMax;		// nHitsMax - TPC
    Int_t 	mNHitsDedx;		// nHitsDedx - TPC
    Float_t 	mNSigmaPion;		// nsigmaPi
    Float_t 	mNSigmaKaon;		// nsigmaK
    Float_t 	mNSigmaProton;		// nsigmaP
    Float_t 	mNSigmaElectron;	// nsigmaE
    Float_t 	mDCA;			// global track gDCA
	
					// pidTraits
    Short_t 	mBTofPidTraitsIndex;	// index of the BTOF pidTratis in the event
    Short_t 	mMtdPidTraitsIndex;	// index of the MTD pidTratis in the event
    Short_t 	mEmcPidTraitsIndex;	// index of the EMC pidTratis in the event
    Short_t 	mHelixIndex;		// index of associated Track Helix
    Short_t 	mMcIndex;		// index of associated Track Helix

public:    
    Float_t GetPt(){return mPt;}			// primary track pt
    Float_t GetEta(){return mEta;}			// primary track Eta
    Float_t GetPhi(){return mPhi;}			// primary track phi
    UShort_t GetId(){return mId;}			// track Id
    Float_t GetDedx(){return mDedx;}			// dEdx
    Int_t GetQ(){return	mQ;}			        // q

    void SetPt(Float_t pt){mPt = pt;}			// primary track pt
    void SetEta(Float_t eta){mEta = eta;}		// primary track Eta
    void SetPhi(Float_t phi){mPhi = phi;}		// primary track phi
    void SetId(UShort_t id){mId = id;}			// track Id
    void SetDedx(Float_t dedx){mDedx = dedx;}		// dEdx
    void SetQ(Int_t q){mQ = q;}			        // q
    
   
    TStTrackData()
    {
	Reset();
    }
    ~TStTrackData()
    {
    }

    void Reset()
    {
	mPt                 = -999.0;
	mEta                = -999.0;
	mPhi                = -999.0;
	mId                 = 0;
	mDedx               = 0;
	mQ		    = 0;
	mNHitsFit           = 0;
	mNHitsMax           = 0;
	mNHitsDedx          = 0;
	mNSigmaPion         = -999.0;
	mNSigmaKaon         = -999.0;
	mNSigmaProton       = -999.0;
	mNSigmaElectron     = -999.0;
	mDCA                = -999.0;
		
	// pidTraits
	mBTofPidTraitsIndex = -1;
	mMtdPidTraitsIndex  = -1;
	mEmcPidTraitsIndex  = -1;
	mHelixIndex         = -1;
	mMcIndex            = -1;
    }

    TVector3 GetMomentum() { TVector3 v; v.SetPtEtaPhi( mPt, mEta, mPhi ); return v; } 
    void SetMomentum( float pt, float eta, float phi )
    { 
	this->mPt  = pt;
	this->mEta = eta;
	this->mPhi = phi;
    }
    TLorentzVector GetLorentzVec( double m )
    { 
	TLorentzVector lv;
	lv.SetPtEtaPhiM( mPt, mEta, mPhi, m );
	return lv;
    }
	       	
    ClassDef( TStTrackData, 1 )
};

#endif
