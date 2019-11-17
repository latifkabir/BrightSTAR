// Filename: TStFmsRpTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:28:55 2019 (-0500)
// URL: jlab.org/~latif


#ifndef TSTFMSRPTREEMAKER_H
#define TSTFMSRPTREEMAKER_H

#include <vector>
#include "StMaker.h"
#include "StFmsDbMaker/StFmsDbMaker.h"
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"

class StEvent;
class StMuDst;
class StMuEvent;
class StFmsPointPair;
class StFmsCollection;
class StSpinDbMaker;

class TStFmsRpTreeMaker : public StMaker
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

    TFile *mFile;
    TTree *mTree;
    Bool_t mSaveFile;
    static const Int_t kMaxPairs = 1000;
    static const Int_t kMaxRpTracks = 1000;

    Int_t  mBunchid7bit;
    Int_t  mSpin4bit;
    vector <Int_t> mTrigIDs;
    vector <Int_t>::iterator mIt;

    //Event Buffer
    Int_t mEventId;
    Short_t mBspin;
    Short_t mYspin;
    Int_t mTrigFlag;
    
    Int_t mBbcADCSum[2];
    Int_t mBbcADCSumLarge[2];
    Int_t mBbcEarliestTDC[2];
    Int_t mBbcEarliestTDCLarge[2];

    Int_t mZdcADCSum[2];
    Int_t mVpdADCSum[2];
    Int_t mTofMultiplicity;
        
    //FMS Buffer
    //Not sure if saving points or pointPair (and in Lorentz Vector or separate quantity) would be the best option (to be revisited later)
    Int_t mFmsNpairs;
    Double_t *mFmsPairE;
    Double_t *mFmsPairM;
    Double_t *mFmsPairPt;
    Double_t *mFmsPairEta;
    Double_t *mFmsPairPhi;
    Double_t *mFmsPairZgg;
    Double_t *mFmsPairDgg;
    Double_t *mFmsPairX;
    Double_t *mFmsPairY;

    //RP Buffer
    Int_t mRpNtracks;
    Int_t *mRpTrackBranch;  
    Double_t *mRpTrackTheta;   // In units if mrad
    Double_t *mRpTrackTheta_x; // in mrad
    Double_t *mRpTrackTheta_y; // in mrad
    Double_t *mRpTrackEta;
    Double_t *mRpTrackPhi;
    Double_t *mRpTrackP;
    Double_t *mRpTrackPt;
    Double_t *mRpTrackXi;
    Double_t *mRpTrackMt;      // -t of the RP track

protected:
    void SetBranches();
    void ResetBuffer();
public: 
    TStFmsRpTreeMaker(const char *name  = "TStFmsRpTreeMaker");
    virtual ~TStFmsRpTreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    Int_t MakeFms();
    Int_t MakeRps();
    Int_t MakeEvent();
    virtual Int_t Finish();
    virtual Int_t InitRun  (int runumber); 
    virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 
    void Set1dHist(TH1D *h1d){ mHist1d = h1d;}
    void Set2dHist(TH2D *h2d){ mHist2d = h2d;}
    void SetTrigIDs(vector<Int_t> trigIDs){ mTrigIDs = trigIDs;}
    void SetTree(TTree *tree){mTree = tree; mSaveFile = kFALSE;}
    ClassDef(TStFmsRpTreeMaker,1) 
};

#endif


