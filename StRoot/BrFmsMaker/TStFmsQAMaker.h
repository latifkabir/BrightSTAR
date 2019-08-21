// Filename: TStFmsQAmaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTFMSQAMAKER_H
#define TSTFMSQAMAKER_H

#include "StMaker.h"
#include "TFile.h"
#include "TH1D.h"

class StEvent;
class StMuDst;
class StFmsDbMaker;
class StMuFmsCollection;
class StMuFmsHit;

class TStFmsQAMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent* mEvent;
    TFile *mFile;
    StFmsDbMaker* mDbMaker;
    StMuFmsCollection *mFmsMuColl;
    StMuFmsHit *mHit;
    TH1D *mAdcDist[4][571];
    const Int_t oMaxCh = 571; 
    const Int_t iMaxCh = 288;
    Int_t mDetId;
    Int_t mChannel;
    TString mOutName = "FmsQA.root";
protected:

    
public: 
    TStFmsQAMaker(const char *name = "FmsQAMaker");
    virtual ~TStFmsQAMaker();
    virtual Int_t Init();
    virtual Int_t  Make();
    virtual Int_t Finish();
    void SetOutName(TString name){ mOutName = name;}
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 

    ClassDef(TStFmsQAMaker,1) 
};

#endif

