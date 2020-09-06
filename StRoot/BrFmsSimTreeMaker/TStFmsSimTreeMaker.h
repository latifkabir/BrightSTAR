// Filename: TStFmsSimTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTFMSSIMTREEMAKER_H
#define TSTFMSSIMTREEMAKER_H

#include "StMaker.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TClonesArray.h"
#include "BrContainers/TStFmsPointData.h"

class StEvent;
class StMuDst;
class StMuFmsCollection;  
class StMuFmsPoint;

class TStFmsSimTreeMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent* mEvent;
    //StFmsDbMaker* mDbMaker; // Example DB Maker, replace with desired detector.

    TString mOutName = "FmsSimTree.root";
    TFile *mFile;
    TTree *mTree = 0;
    Int_t mNevents;
    
    StMuFmsCollection *mFmsMuColl;

    Int_t mRunId;
    Int_t mEventId;
    Int_t mTriggerBit;
    TClonesArray *mFmsPointArray;
    StMuFmsPoint *mFmsPoint;
    //StPythiaEvent *pythiaEvent;
    TStFmsPointData *mFmsPointData;    

    
    TH2D *mPointXY_trg;
    TH2D *mPointXY_notrg;
    TH1D *mPointPhi_trg;
    TH1D *mPointPhi_notrg;
    
protected:

    
public: 
    TStFmsSimTreeMaker(const char *name  = "FmsSimTreeMaker");
    virtual ~TStFmsSimTreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 


    ClassDef(TStFmsSimTreeMaker,1) 
};

#endif

