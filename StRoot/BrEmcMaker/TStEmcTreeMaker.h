// Filename: TStEmcTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSEMCTREEMAKER_H
#define TSEMCTREEMAKER_H

#include <vector>

#include "StMaker.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "StarClassLibrary/StLorentzVector.hh"
#include "StarClassLibrary/StLorentzVectorF.hh"
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"

class StMuDst;
class StMuDstEvent;
class StEmcCollection;
class StEmcPoint;

class TStEmcTreeMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEmcCollection *mEmcCollection;
    StEmcPoint *mPoint1;
    StEmcPoint *mPoint2;

    Double_t mPairM;
    Double_t mZgg;
    Double_t mTheta;
    StThreeVectorF mV1;
    StThreeVectorF mV2;
    StThreeVectorF mPV;
    StThreeVectorF mVertex;
    Double_t mE1;
    Double_t mE2;
    Double_t mPx2;
    Double_t mPy2;
    Double_t mPz2;
    Int_t mQ1;
    Int_t mQ2;
    TLorentzVector mLV1;
    TLorentzVector mLV2;
    TLorentzVector mLV;
    TString mOutName = "EmcTree.root";
    TFile *mFile;
    TTree *mTree;
    static const Int_t kMaxPoints = 1000;
    static const Int_t kMaxPi0 = 1000;

    std::vector <int> mTrigIDs;
    StMuTriggerIdCollection mTrigMuColl;
    
    //Buffer for the tree
    Int_t mEvtNo;
    Int_t mHT1;
    Int_t mHT2;
    Int_t mMB;
    
    Int_t mNpoints;
    Double_t *mX;
    Double_t *mY;
    Double_t *mZ;
    Double_t *mPx;
    Double_t *mPy;
    Double_t *mPz;
    Double_t *mE;
    Int_t *mQ;
    Int_t *mNtracks;
    
    Int_t mNpi0;
    Double_t *mPi0X;
    Double_t *mPi0Y;
    Double_t *mPi0Z;
    Double_t *mPi0Px;
    Double_t *mPi0Py;
    Double_t *mPi0Pz;
    Double_t *mPi0E;
    Double_t *mPi0Pt;
    Double_t *mPi0M;
    Double_t *mPi0theta;
    Double_t *mPi0zgg;
    Double_t *mPi0dgg;
    Int_t *mPi0Q1;
    Int_t *mPi0Q2;
    Int_t *mPi0nTracks1;
    Int_t *mPi0nTracks2;
    
protected:
    void SetBranches();
    void ResetBuffer();
    Bool_t Accept(StMuEvent *muEvent);
    Bool_t HasTrigger(StMuEvent *muEvent, Int_t triggerId);
public: 
    TStEmcTreeMaker(const char *name  = "EmcTreeMaker");
    virtual ~TStEmcTreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    void AddTrigger(Int_t triggerId);
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 
    void SetOutName(TString outName);

    ClassDef(TStEmcTreeMaker, 1) 
};

#endif

