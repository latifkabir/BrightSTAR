// Filename: TStPidTagger.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTPIDTAGGER_H
#define TSTPIDTAGGER_H

#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuBTofPidTraits.h"
#include "StMuDSTMaker/COMMON/StMuMtdPidTraits.h"

class TStPidTagger : public TObject
{
private:
    const StMuDst *mMuDst;  
    StMuTrack *mTrack;
    Double_t mMom;
    Double_t mM2;
    Double_t mBeta;
    StMuBTofPidTraits mBtofPidTraits;
    StMuMtdPidTraits mMtdPidTraits;
    Double_t mM2_mu;
    Double_t mBeta_mu;
protected:
    Double_t mnSig_e;
    Double_t mMaxM2_e;
    Double_t mMinM2_e;
    Double_t mMaxP_e;
    Double_t mMinP_e;

    Double_t mnSig_pi;
    Double_t mMaxM2_pi;
    Double_t mMinM2_pi;
    Double_t mMaxP_pi;
    Double_t mMinP_pi;

    Double_t mnSig_mu;
    Double_t mMaxM2_mu;
    Double_t mMinM2_mu;
    Double_t mMaxP_mu;
    Double_t mMinP_mu;

    Double_t mnSig_ka;
    Double_t mMaxM2_ka;
    Double_t mMinM2_ka;
    Double_t mMaxP_ka;
    Double_t mMinP_ka;

    Double_t mnSig_pr;
    Double_t mMaxM2_pr;
    Double_t mMinM2_pr;
    Double_t mMaxP_pr;
    Double_t mMinP_pr;    
    
public:
    //Particle ID according to PDG MC numbering scheme
    const Int_t kElectronId = 11;
    const Int_t kMuonId = 13;
    const Int_t kPionId = 211;
    const Int_t kKaonId = 321;
    const Int_t kProtonId = 2212;
    static const Int_t kMaxTracks = 1000;
    
    TStPidTagger();
    TStPidTagger(const StMuDst *mDst);
    virtual ~TStPidTagger();
    virtual Int_t Init();
    void SetTrack(StMuTrack *track);
    Int_t TagElectron();
    Int_t TagPion();
    Int_t TagProton();
    Int_t TagKaon();
    Int_t TagMuon();
    Double_t GetM2(){return mM2;}
    Double_t GetM2mu(){return mM2_mu;}
    
    ClassDef(TStPidTagger,1) 
};

#endif

