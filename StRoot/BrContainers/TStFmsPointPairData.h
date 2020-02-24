// Filename: TStFmsPointPairData.h
// Description: Fms pion candidate information. 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Dec 20 13:01:28 2019 (-0500)
// URL: jlab.org/~latif

#ifndef TSTFMSPOINTPAIRDATA_H
#define TSTFMSPOINTPAIRDATA_H

#include <string>
#include <iostream>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

class TStFmsPointPairData : public TObject
{
public:
    Float_t mE;		//
    Float_t mM;		//
    Float_t mPt;	//
    Float_t mEta;	//
    Float_t mPhi;	//
    Float_t mZgg;	//
    Float_t mDgg;	//
    Float_t mX;		//
    Float_t mY;		//

    Float_t GetE(){return mE;}
    Float_t GetM(){return mM;}
    Float_t GetPt(){return mPt;}
    Float_t GetEta(){return mEta;}
    Float_t GetPhi(){return mPhi;}
    Float_t GetZgg(){return mZgg;}
    Float_t GetDgg(){return mDgg;}
    Float_t GetX(){return mX;}
    Float_t GetY(){return mY;}

    void SetE(Float_t e){ mE = e;}
    void SetM(Float_t m){ mM = m;}
    void SetPt(Float_t pt){ mPt = pt;}
    void SetEta(Float_t eta){ mEta = eta;}
    void SetPhi(Float_t phi){ mPhi = phi;}
    void SetZgg(Float_t zgg){ mZgg = zgg;}
    void SetDgg(Float_t dgg){ mDgg = dgg;}
    void SetX(Float_t x){ mX = x;}
    void SetY(Float_t y){ mY = y;}
    
    TStFmsPointPairData()
    {
	Reset();
    }
    ~TStFmsPointPairData()
    {
    }

    void Reset()
    {
	mE	= -999.0;		
	mM	= -999.0;		
	mPt	= -999.0;	
	mEta	= -999.0;	
	mPhi	= -999.0;	
	mZgg	= -999.0;	
	mDgg	= -999.0;	
	mX	= -999.0;		
	mY	= -999.0;		
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
    
    ClassDef( TStFmsPointPairData, 1 )
};

#endif
