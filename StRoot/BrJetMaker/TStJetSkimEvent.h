// Filename: TStJetSkimEvent.h
// Description: Container for track information. 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Dec 20 13:01:28 2019 (-0500)
// URL: jlab.org/~latif

#ifndef TSTJETSKIMEVENT_H
#define TSTJETSKIMEVENT_H

#include <string>
#include <iostream>
#include "TObject.h"


class TStJetSkimEvent : public TObject
{
protected:
    const static Int_t mMaxTrigs = 9;
    Int_t mRunNumber;
    Int_t mEventId;
    Int_t mTrigFlags[mMaxTrigs];
    Int_t mSpinB;
    Int_t mSpinY;
    Double_t mVertexZ;
    UInt_t mUnixTime;
    Float_t mBbcWestRate;
    Float_t mBbcEastRate;
public:    
    Int_t GetRunNumber(){return mRunNumber;}
    Int_t GetEventId(){return mEventId;}
    Int_t GetTrigFlag(Int_t index){if(index >= 0 && index < mMaxTrigs) return mTrigFlags[index]; return -1;}
    Int_t GetSpinB(){return mSpinB;}
    Int_t GetSpinY(){return mSpinY;}
    Double_t GetVertexZ(){return mVertexZ;}
    UInt_t GetUnixTime(){return mUnixTime;}
    Float_t GetBbcWestRate(){return mBbcWestRate;}
    Float_t GetBbcEastRate(){return mBbcEastRate;}
    
    void SetRunNumber(Int_t run){mRunNumber = run;}
    void SetEventId(Int_t id){ mEventId = id;}
    void SetTrigFlag(Int_t index, Int_t flag){if(index >= 0 && index < mMaxTrigs) mTrigFlags[index] = flag;}
    void SetSpinB(Int_t spin){ mSpinB = spin;}
    void SetSpinY(Int_t spin){ mSpinY = spin;}
    void SetVertexZ(Double_t z){ mVertexZ = z;}
    void SetUnixTime(UInt_t time){ mUnixTime = time;}
    void SetBbcWestRate(Float_t rate){mBbcWestRate = rate;}
    void SetBbcEastRate(Float_t rate){mBbcEastRate = rate;}
    
    TStJetSkimEvent()
    {
	Reset();
    }
    ~TStJetSkimEvent()
    {
    }

    void Reset()
    {
	mRunNumber = -1;
	mEventId = -1;
	std::fill_n(mTrigFlags, mMaxTrigs, 0);
	mSpinB = -999;
	mSpinY = -999;
	mVertexZ = -999;
	mUnixTime = -1;
	mBbcWestRate = -1;
	mBbcEastRate = -1;
    }

	       	
    ClassDef(TStJetSkimEvent, 2 )
};

#endif
