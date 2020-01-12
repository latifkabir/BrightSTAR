// Filename: TStChargedPidData.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep 22 01:04:16 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTCHARGEDPIDDATA_H
#define TSTCHARGEDPIDDATA_H

#include <string>
#include <iostream>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TApplication.h"

class TStChargedPidData : public TObject
{
public:
    Int_t    mQ;	// Charge
    Double_t mPt;	// Primary track Pt
    Double_t mEta;	// Primary track eta
    Double_t mPhi;	// Primary track phi
    Double_t mX;	// Position of projection on detector EMC, EEMC, FMS: x
    Double_t mY;	// Position of projection on detector EMC, EEMC, FMS: y
    Double_t mZ;	// Position of projection on detector EMC, EEMC, FMS: z
    
    TStChargedPidData()
    {
	Reset();
    }
    ~TStChargedPidData()
    {
    }

    void Reset()
    {
	mPt     = -999.0;
	mEta    = -999.0;
	mPhi    = -999.0;
	mQ	= -9;
	mX	= -999.0;
	mY	= -999.0;
	mZ	= -999.0;
    }

    TVector3 GetMomentum()
    {
	TVector3 v;
	v.SetPtEtaPhi( mPt, mEta, mPhi );
	return v;
    } 
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
	       	
    ClassDef( TStChargedPidData, 1 )
};

#endif
