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
protected:
    Float_t mE;		// Pion energy
    Float_t mM;		// pion mass
    Float_t mPt;	// pion pt
    Float_t mEta;	// pion Eta
    Float_t mPhi;	// Pion phi
    Float_t mZgg;	// pion Zgg
    Float_t mDgg;	// pion Dgg
    Float_t mX;		// Pion position X (in STAR coord)
    Float_t mY;		// Pion position Y (in STAR coord)
    Float_t mX1;        // Photon 1 position X (in STAR coord)
    Float_t mX2;        // Photon 2 position X (in STAR coord)
    Float_t mY1;        // Photon 1 position Y (in STAR coord)
    Float_t mY2;        // Photon 2 position X (in STAR coord)
    Int_t mFpsPid1;     // Photon 1 FPS pid
    Int_t mFpsPid2;     // Photon 2 FPS pid
    
public:    
    Float_t GetE(){return mE;}
    Float_t GetM(){return mM;}
    Float_t GetPt(){return mPt;}
    Float_t GetEta(){return mEta;}
    Float_t GetPhi(){return mPhi;}
    Float_t GetZgg(){return mZgg;}
    Float_t GetDgg(){return mDgg;}
    Float_t GetX(){return mX;}
    Float_t GetY(){return mY;}
    Float_t GetX1(){return mX1;}
    Float_t GetX2(){return mX2;}
    Float_t GetY1(){return mY1;}
    Float_t GetY2(){return mY2;}
    Int_t GetFpsPid1(){return mFpsPid1;}
    Int_t GetFpsPid2(){return mFpsPid2;}
    
    void SetE(Float_t e){ mE = e;}
    void SetM(Float_t m){ mM = m;}
    void SetPt(Float_t pt){ mPt = pt;}
    void SetEta(Float_t eta){ mEta = eta;}
    void SetPhi(Float_t phi){ mPhi = phi;}
    void SetZgg(Float_t zgg){ mZgg = zgg;}
    void SetDgg(Float_t dgg){ mDgg = dgg;}
    void SetX(Float_t x){ mX = x;}
    void SetY(Float_t y){ mY = y;}
    void SetX1(Float_t x){mX1 = x;}
    void SetX2(Float_t x){mX2 = x;}
    void SetY1(Float_t y){mY1 = y;}
    void SetY2(Float_t y){mY2 = y;}
    void SetFpsPid1(Int_t pid){mFpsPid1 = pid;}
    void SetFpsPid2(Int_t pid){mFpsPid2 = pid;}
    
    TStFmsPointPairData()
    {
	Reset();
    }
    ~TStFmsPointPairData()
    {
    }

    void Reset()
    {
	mE		= -999.0;		
	mM		= -999.0;		
	mPt		= -999.0;	
	mEta		= -999.0;	
	mPhi		= -999.0;	
	mZgg		= -999.0;	
	mDgg		= -999.0;	
	mX		= -999.0;		
	mY		= -999.0;
	mX1		= -999.0;		
	mY1		= -999.0;
	mX2		= -999.0;		
	mY2		= -999.0;
	mFpsPid1	= -1;
	mFpsPid2	= -1;
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
