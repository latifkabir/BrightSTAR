// Filename: TStRpsTrackData.h
// Description: Container for RP track information. 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Dec 20 13:01:28 2019 (-0500)
// URL: jlab.org/~latif

#ifndef TSTRPSTRACKDATA_H
#define TSTRPSTRACKDATA_H

#include <string>
#include <iostream>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

class TStRpsTrackData : public TObject
{
public:
    Int_t mType;       // 1 for global, 0 otherwise
    Int_t mNplanes;    //  
    Int_t mBranch;     //
    Double_t mTheta;   // In units if mrad
    Double_t mTheta_x; // in mrad
    Double_t mTheta_y; // in mrad
    Double_t mEta;     //
    Double_t mPhi;     //
    Double_t mP;       //
    Double_t mPx;      //
    Double_t mPy;      //
    Double_t mPz;      //
    Double_t mPt;      //
    Double_t mXi;      //
    Double_t mMt;      // -t of the RP track

    TStRpsTrackData()
    {
	Reset();
    }
    ~TStRpsTrackData()
    {
    }
    
    void Reset()
    {
	mType		= -1;      
	mNplanes	= -1;   
	mBranch		= -1;    
	mTheta		= -999.0;  
	mTheta_x	= -999.0; 
	mTheta_y	= -999.0; 
	mEta		= -999.0;     
	mPhi		= -999.0;     
	mP		= -999.0;       
	mPx		= -999.0;      
	mPy		= -999.0;      
	mPz		= -999.0;      
        mPt		= -999.0;      
	mXi		= -999.0;      
	mMt		= -999.0;   
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
	       	
    ClassDef( TStRpsTrackData, 1 )
};

#endif
