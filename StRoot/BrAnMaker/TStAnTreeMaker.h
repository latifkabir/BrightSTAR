// Filename: TStAnTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:28:55 2019 (-0500)
// URL: jlab.org/~latif


#ifndef TSTANTREEMAKER_H
#define TSTANTREEMAKER_H

#include <vector>
#include "StMaker.h"
#include "StFmsDbMaker/StFmsDbMaker.h"
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"

#include "BrContainers/TStEventData.h"
#include "BrContainers/TStTrackData.h"
#include "BrContainers/TStFmsPointPairData.h"
#include "BrContainers/TStRpsTrackData.h"

class StEvent;
class StMuDst;
class StMuEvent;
class StFmsPointPair;
class StFmsCollection;
class StSpinDbMaker;

class TStAnTreeMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent *mEvent;
    StMuEvent *mMuEvent;
    TString mName;
    
    StFmsDbMaker *mFmsDbMk;
    StSpinDbMaker *mSpinDbMaker;
    
    StFmsCollection *mFmsColl;    
    StFmsPointPair *mPair;
    TH1D *mHist1d;
    TH2D *mHist2d;
    vector<StFmsPointPair*> mPointPairs;

    StMuRpsCollection *mRpsMuColl;
    StMuRpsTrackPoint  *mRpsTrkPoint;
    StMuRpsTrack *mRpsTrk;
    Double_t mBeamMom;
    
    TFile *mFile;
    TTree *mTree;
    Bool_t mSaveFile;
    static const Int_t kMaxPairs = 1000;
    static const Int_t kMaxRpTracks = 1000;
    static const Int_t kMaxTriggers = 200;

    Int_t  mBunchid7bit;
    Int_t  mSpin4bit;
    vector <Int_t> mTrigIDs;
    vector <Int_t>::iterator mIt;

    StMuTriggerIdCollection *mTrigMuColl;
    //Event Buffer
    Int_t mRunNumber;
    Int_t mEventId;
    Short_t mBspin;
    Short_t mYspin;
    Int_t mTrigFlag;
    Int_t mNtrig;

    
    Int_t mBbcADCSum[2];
    Int_t mBbcADCSumLarge[2];
    Int_t mBbcEarliestTDC[2];
    Int_t mBbcEarliestTDCLarge[2];

    Int_t mZdcADCSum[2];
    Int_t mVpdADCSum[2];
    Int_t mTofMultiplicity;

    //Event
    TClonesArray *mEventArray;
    TStEventData *mEventData;
    
    //TPC Track
    StMuTrack *mTrack;
    TClonesArray  *mTrackArray;
    TStTrackData *mTrackData;
    
    //FMS Buffer
    //Not sure if saving points or pointPair (and in Lorentz Vector or separate quantity) would be the best option (to be revisited later)
    Int_t mFmsNpairs;
    TClonesArray  *mFmsArray;
    TStFmsPointPairData * mFmsPointPairData;
    
    //RP Buffer
    Int_t mRpNtracks;
    TClonesArray  *mRpsArray;
    TStRpsTrackData *mRpsTrackData;

    
protected:
    void SetBranches();
    void Reset();
    Bool_t AcceptEvent();
public: 
    TStAnTreeMaker(const char *name  = "TStAnTreeMaker");
    virtual ~TStAnTreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    Int_t MakeFms();
    Int_t MakeRps();
    Int_t MakeEvent();
    Int_t MakeTrack();
    virtual Int_t Finish();
    virtual Int_t InitRun  (int runumber); 
    virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 
    void Set1dHist(TH1D *h1d){ mHist1d = h1d;}
    void Set2dHist(TH2D *h2d){ mHist2d = h2d;}
    void SetTrigIDs(vector<Int_t> trigIDs){ mTrigIDs = trigIDs;}
    void SetTree(TTree *tree){mTree = tree; mSaveFile = kFALSE;}
    void SetBeamMomentum(Double_t beamMom){ mBeamMom = beamMom;}
    ClassDef(TStAnTreeMaker,1) 
};

#endif


