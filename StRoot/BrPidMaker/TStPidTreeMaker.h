// Filename: TStPidTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTPIDTREEMAKER_H
#define TSTPIDTREEMAKER_H

#include "StMaker.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TStPidTagger.h"
#include "TStPidTrait.h"

class StEvent;
class StMuDst;
class StMuTrack;  

class TStPidTreeMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent* mEvent;
    StMuTrack *mTrack;
    TTree *mTree;
    TFile *mFile;
    TString mOutName = "PidTree.root";
    TStPidTagger *mPidTagger;
    
    //Buffer for the tree
    Int_t mEventNo;
    Double_t mVz;
    
    //Electron
    Int_t meN;                       // Number of electrons
    TStPidTrait *eTrait;             // Electron traits

    //Pion
    Int_t mpiN;                       // Number of pions
    TStPidTrait *piTrait;             // Pion traits

    //Muon
    Int_t mmuN;                       // Number of muons
    TStPidTrait *muTrait;             // Muon traits

    //Kaon
    Int_t mkaN;                       // Number of kanons
    TStPidTrait *kaTrait;             // Kaon traits

    //Proton
    Int_t mprN;                       // Number of proton
    TStPidTrait *prTrait;             // Proton traits

    
protected:
    void SetBranches();
    void ResetBuffer();    
public: 
    TStPidTreeMaker(const char *name  = "PidTreeMaker");
    virtual ~TStPidTreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 
    void SetOutName(TString outName) {mOutName = outName;}

    ClassDef(TStPidTreeMaker,1) 
};

#endif

