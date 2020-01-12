// Filename: TStEventData.h
// Description: Container for Event Data.
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Dec 20 12:58:02 2019 (-0500)
// URL: jlab.org/~latif


#ifndef TSTEVENTDATA_H
#define TSTEVENTDATA_H

#include <iostream>
#include "TObject.h"


class TStEventData : public TObject
{

public:
    Int_t mRunNumber;			//
    Int_t mEventId;			//
    Short_t mBspin;			//
    Short_t mYspin;			//
    Int_t mTrigFlag;			//
    static const Int_t mMaxTrigs = 30;	//
    Int_t mNtrig;			//
    Int_t mTriggers[mMaxTrigs];		//
    
    Int_t mBbcADCSum[2];		//
    Int_t mBbcADCSumLarge[2];		//
    Int_t mBbcEarliestTDC[2];		//
    Int_t mBbcEarliestTDCLarge[2];	//

    Int_t mZdcADCSum[2];		// 
    Int_t mVpdADCSum[2];		//
    Int_t mTofMultiplicity;		//
    Double_t mVx;                       //
    Double_t mVy;                       //
    Double_t mVz;                       //
    
    TStEventData()
    {
	Reset();	
    }
    ~TStEventData() {}

    void Reset()
    {
	mRunNumber = -1;
	mEventId = -1;
	mBspin = 0;
	mYspin = 0;
	mTrigFlag = -1;
	mNtrig = 0;
	for(Int_t i = 0; i < mMaxTrigs; ++i)
	    mTriggers[i] = 0;
    
	mBbcADCSum[0] = 0;
	mBbcADCSum[1] = 0;
	
	mBbcADCSumLarge[0] = 0;
	mBbcADCSumLarge[1] = 0;
	
	mBbcEarliestTDC[0] = 0;
	mBbcEarliestTDC[1] = 0;
	
	mBbcEarliestTDCLarge[0] = 0;
	mBbcEarliestTDCLarge[1] = 0;

	mZdcADCSum[0] = 0;
	mZdcADCSum[1] = 0;
	
	mVpdADCSum[0] = 0;
	mVpdADCSum[1] = 0;
	mTofMultiplicity = -1;

	mVx = -999.0;
	mVy = -999.0;
	mVz = -999.0;
    }
    
    // Int_t GetYear() const { return (mRunId / 1000000) - 1 + 2000; }
	
    // void SetVertex( Float_t x, Float_t y, Float_t z )
    // {
    // 	this->mPrimaryVertex_mX1 = x;
    // 	this->mPrimaryVertex_mX2 = y;
    // 	this->mPrimaryVertex_mX3 = z;
    // }

    ClassDef( TStEventData, 2 )
};


#endif
