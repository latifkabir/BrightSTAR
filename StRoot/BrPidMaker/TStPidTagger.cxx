// Filename: TStPidTagger.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStPidTagger.h"


ClassImp(TStPidTagger)

//_____________________________________________________________________________ 
TStPidTagger::TStPidTagger()
{
    Init();
}

//_____________________________________________________________________________ 
TStPidTagger::TStPidTagger(const StMuDst *mDst)
{
    mMuDst = mDst;
    Init();
}

//_____________________________________________________________________________ 
TStPidTagger::~TStPidTagger()
{

}


//_____________________________________________________________________________ 
Int_t TStPidTagger::Init()
{    
    mnSig_e = 2.0;
    mMaxM2_e = 0.01;
    mMinM2_e = -0.01;
    mMaxP_e = 2.0;
    mMinP_e = 0.0;

    mnSig_pi = 2.0;
    mMaxM2_pi = 0.15;
    mMinM2_pi = 0.0;
    mMaxP_pi = 2.0;
    mMinP_pi = 0.0;

    mnSig_mu = 2.0;
    mMaxM2_mu = 0.1;
    mMinM2_mu = -0.1;
    mMaxP_mu = 2.0;
    mMinP_mu = 0.0;

    mnSig_ka = 2.0;
    mMaxM2_ka = 0.36;
    mMinM2_ka = 0.16;
    mMaxP_ka = 2.0;
    mMinP_ka = 0.0;

    mnSig_pr = 2.0;
    mMaxM2_pr = 1.5;
    mMinM2_pr = 0.5;
    mMaxP_pr = 2.0;
    mMinP_pr = 0.0;    
   
    return 0;
}

//_____________________________________________________________________________
void TStPidTagger::SetTrack(StMuTrack *track)
{
	mTrack = track;
	mMom = mTrack->p().mag();
	mBtofPidTraits = mTrack->btofPidTraits();
	mBeta = mBtofPidTraits.beta();
	if(mBeta != -999)
	    mM2 = mTrack->p().mag2()*(1 - pow(mBeta, 2)) / pow(mBeta, 2);
	else
	    mM2 = -999;
	
	mMtdPidTraits = mTrack->mtdPidTraits();
	mBeta_mu = mMtdPidTraits.beta();
	if(mBeta_mu != -999)
	    mM2_mu = mTrack->p().mag2()*(1 - pow(mBeta_mu, 2)) / pow(mBeta_mu, 2);
	else
	    mM2_mu = -999;
}

//_____________________________________________________________________________ 
Int_t TStPidTagger::TagElectron()
{
    if(mMom >= mMinP_e && mMom <= mMaxP_e
       && fabs(mTrack->nSigmaElectron()) < mnSig_e
       && mM2 > mMinM2_e && mM2 < mMaxM2_e)    
	return kElectronId;    
    else
	return -1;
}
//_____________________________________________________________________________ 
Int_t TStPidTagger::TagPion()
{
    //cout << mMom <<"\t"<< mTrack->nSigmaPion() <<"\t"<<mM2<<endl;    
     if(mMom >= mMinP_pi && mMom <= mMaxP_pi
	&& fabs(mTrack->nSigmaPion()) < mnSig_pi
       && mM2 > mMinM2_pi && mM2 < mMaxM2_pi)    
	return kPionId;    
    else
	return -1;   
}
//_____________________________________________________________________________ 
Int_t TStPidTagger::TagKaon()
{
    if(mMom >= mMinP_ka && mMom <= mMaxP_ka
       && fabs(mTrack->nSigmaKaon()) < mnSig_ka
       && mM2 > mMinM2_ka && mM2 < mMaxM2_ka)    
	return kKaonId;    
    else
	return -1;      
}
//_____________________________________________________________________________ 
Int_t TStPidTagger::TagProton()
{
    if(mMom >= mMinP_pr && mMom <= mMaxP_pr
       && fabs(mTrack->nSigmaProton()) < mnSig_pr
       && mM2 > mMinM2_pr && mM2 < mMaxM2_pr)    
	return kProtonId;    
    else
	return -1;       
}
//_____________________________________________________________________________ 
Int_t TStPidTagger::TagMuon()
{
    //---> To be investigated <----------
    // if(mM2_mu != -999)
    // cout<< mMom <<" >= "<< mMinP_mu <<" && "<< mMom <<" <= "<< mMaxP_mu
    // 	<<" && "<< fabs(mMtdPidTraits.sigmaMuon())<<" < "<< mnSig_mu<<
    // 	" && "<< mM2<< " > " <<mMinM2_mu <<" && "<< mM2_mu <<" < "<< mMaxM2_mu<<endl;
    if(mMom >= mMinP_mu && mMom <= mMaxP_mu
       && fabs(mMtdPidTraits.sigmaMuon()) < mnSig_mu
       && mM2_mu > mMinM2_mu && mM2_mu < mMaxM2_mu)    
	return kMuonId;    
    else
	return -1;
}
