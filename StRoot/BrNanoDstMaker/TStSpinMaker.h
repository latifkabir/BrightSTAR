// Filename: TStSpinMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Apr 12 03:48:48 2021 (-0400)
// URL: jlab.org/~latif


#ifndef TST_SPIN_MAKER_H
#define TST_SPIN_MAKER_H

#include "TFile.h"
#include "TH1D.h"
#include "TString.h"

#include "StMaker.h"

class StMuDst;
class StMuEvent;
class StSpinDbMaker;

class TStSpinMaker: public StMaker
{
    StMuDst *mDst;   // Get StMuDst
    StMuEvent *mMuEvent;
    StSpinDbMaker *mSpinDbMaker;
    Int_t  mSpin4Bit;
    Int_t  mBunchid7Bit;
    Int_t mSpinY;
    Int_t mSpinB;

public:
    TStSpinMaker(TString name);
    ~TStSpinMaker();
    Int_t Init();
    Int_t InitRun(Int_t runNumber);
    Int_t Make();
    Int_t Finish();

    Int_t GetSpinB(){return mSpinB;}
    Int_t GetSpinY(){return mSpinY;}

    ClassDef(TStSpinMaker,1);
    
};

#endif
