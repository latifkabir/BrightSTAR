// Filename: TStPidTagger.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTPIDTAGGER_H
#define TSTPIDTAGGER_H

#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"

class TStPidTagger : public TObject
{
private:
    const StMuDst *mMuDst;  
    const StMuTrack *mTrack;  

protected:
    Double_t mnSig_e;
    Double_t mMaxM_e;
    Double_t mMinM_e;
    Double_t mMaxP_e;
    Double_t mMinP_e;

    Double_t mnSig_pi;
    Double_t mMaxM_pi;
    Double_t mMinM_pi;
    Double_t mMaxP_pi;
    Double_t mMinP_pi;

    Double_t mnSig_mu;
    Double_t mMaxM_mu;
    Double_t mMinM_mu;
    Double_t mMaxP_mu;
    Double_t mMinP_mu;

    Double_t mnSig_ka;
    Double_t mMaxM_ka;
    Double_t mMinM_ka;
    Double_t mMaxP_ka;
    Double_t mMinP_ka;

    Double_t mnSig_pr;
    Double_t mMaxM_pr;
    Double_t mMinM_pr;
    Double_t mMaxP_pr;
    Double_t mMinP_pr;    
    
public:
    //Particle ID according to PDG MC numbering scheme
    const Int_t kElectronId = 11;
    const Int_t kMuonId = 13;
    const Int_t kPionId = 211;
    const Int_t kKaonId = 321;
    const Int_t kProtonId = 2212;
    
    TStPidTagger();
    TStPidTagger(const StMuDst *mDst);
    virtual ~TStPidTagger();
    virtual Int_t Init();
    void SetTrack(const StMuTrack *track){mTrack = track;}
    Int_t TagElectron();
    Int_t TagPion();
    Int_t TagProton();
    Int_t TagKaon();
    Int_t TagMuon();
    
    ClassDef(TStPidTagger,1) 
};

#endif

