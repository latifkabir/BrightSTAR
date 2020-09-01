// Filename: TStFmsPointData.h
// Description: Fms point information. 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Dec 20 13:01:28 2019 (-0500)
// URL: jlab.org/~latif

#ifndef TSTFMSPOINTDATA_H
#define TSTFMSPOINTDATA_H

#include <string>
#include <iostream>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

class TStFmsPointData : public TObject
{
protected:
    Float_t mE;		// Point energy
    Float_t mPt;	// point pt
    Float_t mPx;	// point px
    Float_t mPy;	// point py
    Float_t mPz;	// point pz
    Float_t mEta;	// point Eta
    Float_t mPhi;	// Point phi
    Float_t mX;		// Point position X (in STAR coord)
    Float_t mY;		// Point position Y (in STAR coord)

    
public:    
    Float_t GetE(){return mE;}
    Float_t GetPt(){return mPt;}
    Float_t GetEta(){return mEta;}
    Float_t GetPhi(){return mPhi;}
    Float_t GetPx(){return mPx;}
    Float_t GetPy(){return mPy;}
    Float_t GetPz(){return mPz;}
    Float_t GetX(){return mX;}
    Float_t GetY(){return mY;}

    
    void SetE(Float_t e){ mE = e;}
    void SetPt(Float_t pt){ mPt = pt;}
    void SetEta(Float_t eta){ mEta = eta;}
    void SetPhi(Float_t phi){ mPhi = phi;}
    void SetPx(Float_t px){ mPx = px;}
    void SetPy(Float_t py){ mPy = py;}
    void SetPz(Float_t pz){ mPz = pz;}
    void SetX(Float_t x){ mX = x;}
    void SetY(Float_t y){ mY = y;}
    
    TStFmsPointData()
    {
	Reset();
    }
    ~TStFmsPointData()
    {
    }

    void Reset()
    {
	mE		= -999.0;		
	mPt		= -999.0;	
	mEta		= -999.0;	
	mPhi		= -999.0;	
	mPx		= -999.0;	
	mPy		= -999.0;	
	mPz		= -999.0;	
	mX		= -999.0;		
	mY		= -999.0;
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
    
    ClassDef( TStFmsPointData, 1 )
};

#endif
