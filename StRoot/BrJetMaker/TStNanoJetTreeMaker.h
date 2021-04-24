// Filename: TStNanoJetTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTNANOJETTREEMAKER_H
#define TSTNANOJETTREEMAKER_H

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "StMaker.h"

class StEvent;
class StMuDst;
class StMuEvent;
class StJetMaker2015;
class StJetEvent;
class StJetTower;
class StJetParticle;
class StJetVertex;
class StJetSkimEvent;
class StJetSkimTrig;
class StJetSkimEventMaker;

class TStJetEvent;
class TStJetSkimEvent;
class TStJetCandidate;
class TStJetTower;
class TStJetParticle;

class TStNanoJetTreeMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent* mEvent;
    StMuEvent *mMuEvent;
    
    StJetMaker2015 *mJetMaker;
    StJetSkimEventMaker *mSkimEventMaker;
    StJetEvent* mInJetEvent;
    StJetSkimEvent* mInSkimEvent;
    StJetParticle *mInParticle;
    StJetTower *mInTower;
    StJetVertex *mInVertex;
    
    TFile *mOutFile;
    TTree *mTree;
    TString mOutName = "NanoJetTree.root";
    TString mBranchName = "AntiKtR070NHits12";
    TStJetEvent *mOutJetEvent;    
    TStJetSkimEvent *mOutSkimEvent;    
    TStJetCandidate *mOutJet;
    TStJetParticle *mOutParticle;
    TStJetTower *mOutTower;

    static const Int_t mMaxTriggers = 9;
    StJetSkimTrig* mJetTrig[mMaxTriggers];
    Int_t mTrigIds[mMaxTriggers];
    Double_t mEtaMax;
    Double_t mEtaMin;
    Double_t mZdist;
    Double_t mR = 0.7;
        
public: 
    TStNanoJetTreeMaker(StJetMaker2015* jetMaker, StJetSkimEventMaker* skimMaker, const char *name  = "NanoJetTreeMaker");
    virtual ~TStNanoJetTreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    void SetOutFileName(TString fileName){mOutName = fileName;}
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 
    void SetEtaMax(Double_t etaMax){mEtaMax = etaMax;}
    void SetEtaMin(Double_t etaMin){mEtaMin = etaMin;}
    void SetDetZdist(Double_t z){mZdist = z;}
    void SetTrigIds(Int_t *trigIds);
    void SetR(Double_t R){mR = R;}
    void SetBranchName(TString branchName){mBranchName = branchName;} //Branch name in main jet tree (to be copied)
    
    ClassDef(TStNanoJetTreeMaker,1) 
};

#endif

