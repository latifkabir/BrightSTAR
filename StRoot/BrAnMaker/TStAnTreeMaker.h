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
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomDefs.h"
#include "StJetMaker/mudst/StMuEmcPosition.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"

#include "BrContainers/TStEventData.h"
#include "BrContainers/TStTrackData.h"
#include "BrContainers/TStChargedPidData.h"
#include "BrPidMaker/TStPidTagger.h"
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
    TString mOutName = "AnTree.root";
    
    StFmsDbMaker *mFmsDbMk;
    StSpinDbMaker *mSpinDbMaker;
    
    TH1D *mHist1d;
    TH2D *mHist2d;
    
    TFile *mFile;
    TTree *mTree = 0;
    Bool_t mSaveFile;

    //--- Event ---
    Int_t  mBunchid7bit;
    Int_t  mSpin4bit;
    vector <Int_t> mTrigIDs;
    vector <Int_t>::iterator mIt;
    StMuTriggerIdCollection *mTrigMuColl;
    Int_t mTrigFlag;
    
    TStEventData *mEventData;
    
    //--- TPC Track ----
    StMuTrack *mTrack;
    TClonesArray  *mTrackArray;
    TStTrackData *mTrackData;

    //--- Charged PID ---
    TStChargedPidData *mChargedPidData;
    TStPidTagger  *mPidTagger;
    Int_t mNe;  // Number of electrons
    Int_t mNpi; // Number of pions
    Int_t mNmu; // Number of muons
    Int_t mNka; // Number of kanons
    Int_t mNpr; // Number of proton
    Int_t mNuk; // Number of unknown PID
    
    TClonesArray  *mElArray;
    TClonesArray  *mPiArray;
    TClonesArray  *mPrArray;
    TClonesArray  *mKaArray;
    TClonesArray  *mMuArray;
    TClonesArray  *mUkArray; //Unknown group

    Double_t mMom;
    Double_t mM2;
    Double_t mBeta;   
    Bool_t mFillHist = kTRUE; //kFALSE;
    StThreeVectorD mMomentum_proj;
    StThreeVectorD mPosition_proj;
    StMuEmcPosition mEmcPosition;
    Double_t mField;
    Double_t mEmcRadius; 
    Double_t mEEmcZSMD = kEEmcZSMD;
    Double_t mProjX;
    Double_t mProjY;
    Double_t mProjZ;
    
    TH2D *mDedxVsQp;
    TH2D *mM2VsQp;
    TH2D *mDedxVsQp_e;
    TH2D *mM2VsQp_e;
    TH2D *mDedxVsQp_pi;
    TH2D *mM2VsQp_pi;
    TH2D *mDedxVsQp_pr;
    TH2D *mM2VsQp_pr;
    TH2D *mDedxVsQp_ka;
    TH2D *mM2VsQp_ka;
    TH2D *mDedxVsQp_mu;
    TH2D *mM2VsQp_mu;
    
    //--- FMS ---
    //Not sure if saving points or pointPair (and in Lorentz Vector or separate quantity) would be the best option (to be revisited later)
    StFmsCollection *mFmsColl;    
    StFmsPointPair *mPair;
    vector<StFmsPointPair*> mPointPairs;
    
    Int_t mFmsNpairs;
    TClonesArray  *mFmsArray;
    TStFmsPointPairData *mFmsPointPairData;
    
    //--- RP ---
    StMuRpsCollection *mRpsMuColl;
    StMuRpsTrack *mRpsTrk;
    Double_t mBeamMom;
    
    Int_t mRpNtracks;
    TClonesArray  *mRpsArray;
    TStRpsTrackData *mRpsTrackData;
            
protected:
    void SetBranches();
    void InitHist();
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
    Int_t MakeChargedPid();
    void ProjectTrack();
    void FillHist(Int_t particleId);
    virtual Int_t Finish();
    virtual Int_t InitRun  (int runumber); 
    virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun
    void Set1dHist(TH1D *h1d){ mHist1d = h1d;}
    void Set2dHist(TH2D *h2d){ mHist2d = h2d;}
    void SetTrigIDs(vector<Int_t> trigIDs){ mTrigIDs = trigIDs;}
    void SetTree(TTree *tree){mTree = tree; mSaveFile = kFALSE;}
    void SetBeamMomentum(Double_t beamMom){ mBeamMom = beamMom;}
    void SetOutFileName(TString out_name){mOutName = out_name;}
    ClassDef(TStAnTreeMaker,1) 
};

#endif


