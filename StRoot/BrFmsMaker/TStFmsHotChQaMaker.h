// Filename: TStFmsQAmaker.h
// Description: Make histrogram of FMS Channel energy using StEvent (i.e. with reconstruction do on the fly)
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTFMSHOTCHQAMAKER_H
#define TSTFMSHOTCHQAMAKER_H

#include "StMaker.h"
#include "TFile.h"
#include "TH1D.h"

class StEvent;
class StMuDst;
class StFmsDbMaker;
class StFmsCollection;
class StFmsHit;

class TStFmsHotChQaMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent* mEvent;
    TFile *mFile;
    StFmsDbMaker* mDbMaker;
    StFmsCollection *mFmsColl;
    StFmsHit *mHit;
    TH1D *mEngDist[4][571];
    TH1D *mCounter;
    const Int_t oMaxCh = 571; 
    const Int_t iMaxCh = 288;
    Int_t mDetId;
    Int_t mChannel;
    TString mOutName = "FmsHotChQa.root";
protected:

    
public: 
    TStFmsHotChQaMaker(const char *name = "FmsHotChQaMaker");
    virtual ~TStFmsHotChQaMaker();
    virtual Int_t Init();
    virtual Int_t  Make();
    virtual Int_t Finish();
    void SetOutName(TString name){ mOutName = name;}
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 

    ClassDef(TStFmsHotChQaMaker,1) 
};

#endif

