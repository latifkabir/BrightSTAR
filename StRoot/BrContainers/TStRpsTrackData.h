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

    Int_t GetType(){return mType;}
    Int_t GetNplanes(){return mNplanes;}
    Int_t GetBranch(){return mBranch;}
    Double_t GetTheta(){return mTheta;}
    Double_t GetThetaX(){return mTheta_x;}
    Double_t GetThetaY(){return mTheta_y;}
    Double_t GetEta(){return mEta;}
    Double_t GetPhi(){return mPhi;}
    Double_t GetP(){return mP;}
    Double_t GetPx(){return mPx;}
    Double_t GetPy(){return mPy;}
    Double_t GetPz(){return mPz;}
    Double_t GetPt(){return mPt;}
    Double_t GetXi(){return mXi;}
    Double_t GetMt(){return mMt;}

    void SetType(Int_t type){ mType = type;}
    void SetNplanes(Int_t nplanes){ mNplanes = nplanes;}
    void SetBranch(Int_t branch){ mBranch = branch;}
    void SetTheta(Double_t theta){ mTheta = theta;}
    void SetThetaX(Double_t theta_x){ mTheta_x = theta_x;}
    void SetThetaY(Double_t theta_y){ mTheta_y = theta_y;}
    void SetEta(Double_t eta){ mEta = eta;}
    void SetPhi(Double_t phi){ mPhi = phi;}
    void SetP(Double_t p){ mP = p;}
    void SetPx(Double_t px){ mPx = px;}
    void SetPy(Double_t py){ mPy = py;}
    void SetPz(Double_t pz){ mPz = pz;}
    void SetPt(Double_t pt){ mPt = pt;}
    void SetXi(Double_t xi){ mXi = xi;}
    void SetMt(Double_t mt){ mMt = mt;}

    
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
