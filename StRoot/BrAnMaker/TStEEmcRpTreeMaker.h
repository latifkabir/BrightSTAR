// Filename: TStEEmcRpTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:28:55 2019 (-0500)
// URL: jlab.org/~latif


#ifndef TSTEEMCRPTREEMAKER_H
#define TSTEEMCRPTREEMAKER_H

#include <vector>
#include "StMaker.h"
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"
#include "StEEmcPool/StEEmcTreeMaker/StEEmcTreeMaker.h"
#include "TH1D.h"
#include "TTree.h"
#include "TFile.h"

class StEvent;
class StMuDst;
class StMuEvent;
class StSpinDbMaker;
//class StEEmcTreeMaker_t;

class TStEEmcRpTreeMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent *mEvent;
    StMuEvent *mMuEvent;
    TString mOutName;
    
    StSpinDbMaker *mSpinDbMaker;

    TH1D* mEvtCountHist;

    StMuRpsCollection *mRpsMuColl;
    StMuRpsTrackPoint  *mRpsTrkPoint;
    StMuRpsTrack *mRpsTrk;
    Double_t mBeamMom;

    StEEmcTreeMaker_t *mEEmcTreeMaker;
    
    TFile *mFile;
    TTree *mTree;
    Int_t mEvtCount;
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
    Int_t *mTriggers;
    
    Int_t mBbcADCSum[2];
    Int_t mBbcADCSumLarge[2];
    Int_t mBbcEarliestTDC[2];
    Int_t mBbcEarliestTDCLarge[2];

    Int_t mZdcADCSum[2];
    Int_t mVpdADCSum[2];
    Int_t mTofMultiplicity;
        

    //RP Buffer
    Int_t mRpNtracks;
    Int_t *mRpTrackType;       // 1 for global, 0 otherwise
    Int_t *mRpTrackNplanes;
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
    Bool_t AcceptEvent();
public: 
    TStEEmcRpTreeMaker(const char *name  = "TStEEmcRpTreeMaker");
    virtual ~TStEEmcRpTreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    Int_t MakeRps();
    Int_t MakeEvent();
    virtual Int_t Finish();
    virtual Int_t InitRun  (int runumber); 
    virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 
    void SetTrigIDs(vector<Int_t> trigIDs){ mTrigIDs = trigIDs;}
    void SetBeamMomentum(Double_t beamMom){ mBeamMom = beamMom;}
    void SetOutFileName(TString outFileName){ mOutName = outFileName;}
    TH1D* GetEvtCountHist(){return mEvtCountHist;}
    ClassDef(TStEEmcRpTreeMaker,1) 
};

#endif


