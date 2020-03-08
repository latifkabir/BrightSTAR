// Filename: TStNanoDstMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:28:55 2019 (-0500)
// URL: jlab.org/~latif

#ifndef TSTNANODSTMAKER_H
#define TSTNANODSTMAKER_H

#include <vector>
#include "StMaker.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StFmsDbMaker/StFmsDbMaker.h"
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomDefs.h"
#include "StEEmcPool/StEEmcTreeMaker/StEEmcTreeMaker.h"
#include "StJetMaker/mudst/StMuEmcPosition.h"
#include "StEmcUtil/geometry/StEmcGeom.h"

#include "StarClassLibrary/StLorentzVector.hh"
#include "StarClassLibrary/StLorentzVectorF.hh"
#include "BrEmcMaker/TStEmcTrackMatchingMaker.h"
#include "BrEmcMaker/TStEmcPidTrait.h"

//--- For RP afterburner ---
#include "StMuRpsUtil/StMuRpsUtil.h"
#include "StMuRpsUtil/StMuRpsCollection2.h"

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
#include "BrContainers/TStEmcPointData.h"


class StEvent;
class StMuDstMaker;
class StMuDst;
class StMuDstEvent;
class StMuEvent;
class StFmsPointPair;
class StFmsCollection;
class StSpinDbMaker;
class StEmcCollection;
class StEmcPoint;


class TStNanoDstMaker : public StMaker
{
private:
    StMuDstMaker *mMuDstMaker;
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
    Int_t mEventCount;
    
    Bool_t mUseEvent;
    Bool_t mUseTpc;
    Bool_t mUseEmc;
    Bool_t mUseFms;
    Bool_t mUseRps;
    Bool_t mUseEEmc;

    Bool_t mSyncOnEEmc;
    Bool_t mUseRpsAfterburner;
    
    //--- Event ---
    Int_t  mBunchid7bit;
    Int_t  mSpin4bit;
    vector <Int_t> mTrigIDs;
    vector <Int_t>::iterator mIt;
    StMuTriggerIdCollection *mTrigMuColl;
    Int_t mTrigFlag;
    Int_t mAdcSum[2];
    
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
    // StMuRpsCollection *mRpsMuColl;
    StMuRpsCollection2 *mRpsMuColl; //Use after burner
    StMuRpsUtil* mAfterburner;
    StMuRpsTrack *mRpsTrk;
    Double_t mBeamMom;
    
    Int_t mRpNtracks;
    TClonesArray  *mRpsArray;
    TStRpsTrackData *mRpsTrackData;

    //------- BEMC -------------
    //TStEmcTrackMatchingMaker* mTrkMatchingMkr;
    StEmcCollection *mEmcCollection;
    StEmcPoint *mEmcPoint;
    TClonesArray  *mEmcArray;
    TStEmcPointData *mEmcPointData;
    StThreeVectorF mEmcPointPos;
    StThreeVectorF mEmcMomVec;
    StThreeVectorF mVertex;
    Float_t mEmcPointEng;
    Int_t mEmcNpoints;
    //--------- EEMC -------------
    StEEmcTreeMaker_t *mEEmcTreeMaker;
    Int_t mHasEEmcEvent;
    
protected:
    void SetBranches();
    void InitHist();
    void Reset();
    Bool_t AcceptEvent();
public: 
    TStNanoDstMaker(const char *name  = "TStNanoDstMaker");
    virtual ~TStNanoDstMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    Int_t MakeEmc();
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
    void EnableEvent(Bool_t status){mUseEvent = status;}
    void EnableTpc(Bool_t status){mUseTpc = status;}
    void EnableEmc(Bool_t status){mUseEmc = status;}
    void EnableFms(Bool_t status){mUseFms = status;}
    void EnableRps(Bool_t status){mUseRps = status;}
    void EnableEEmc(Bool_t status){mUseEEmc = status;}
    void SyncOnEEmc(Bool_t status){mSyncOnEEmc = status;}
    void UseRpsAfterburner(Bool_t status){mUseRpsAfterburner = status;}
    void SetMuDstMaker(StMuDstMaker *maker){mMuDstMaker = maker;}
    
    ClassDef(TStNanoDstMaker,1) 
};

#endif



