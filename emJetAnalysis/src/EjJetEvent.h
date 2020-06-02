// Filename: EjJetCandidate.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Dec 20 13:01:28 2019 (-0500)
// URL: jlab.org/~latif

#ifndef TSTEMCPOINTDATA_H
#define TSTEMCPOINTDATA_H

#include <string>
#include <iostream>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

class EjJetCandidate : public TObject
{
protected:
    Int_t nPhotons;    
    Double_t eta;
    Double_t phi;
    Double_t eng;
    Double_t pt;
    Double_t jetX;
    Double_t jetY;
    Double_t rt;
    Double_t phEng[kMaxPhotons];

    
public:       
    Float_t GetE(){return mE;}
    Float_t GetPx(){return mPx;}
    Float_t GetPy(){return mPy;}
    Float_t GetPz(){return mPz;}
    Float_t GetX(){return mX;}
    Float_t GetY(){return mY;}
    Float_t GetZ(){return mZ;}
    Int_t GetQuality(){return mQ;}
    Int_t GetNtracks(){return mNtracks;}

    void SetE(Float_t e){ mE = e;}
    void SetPx(Float_t px){mPx = px;}
    void SetPy(Float_t py){mPy = py;}
    void SetPz(Float_t pz){mPz = pz;}
    void SetX(Float_t x){mX = x;}
    void SetY(Float_t y){mY = y;}
    void SetZ(Float_t z){mZ = z;}
    void SetQuality(Int_t q){mQ = q;}
    void SetNtracks(Int_t n){mNtracks = n;}
    
    EjJetCandidate()
    {
	Reset();
    }
    ~EjJetCandidate()
    {
    }

    void Reset()
    {
	mE = -1.0;	   //
	mPx = -1.0;        //
	mPy = -1.0;        //
	mPz = -1.0;        //
	mX = -1.0;         //
	mY = -1.0;         //
	mZ = -1.0;         //
	mQ = -1;           //
	mNtracks = -1;     //		
    }

    
    ClassDef( EjJetCandidate, 1 )
};

#endif
