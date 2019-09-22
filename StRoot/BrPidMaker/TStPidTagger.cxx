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

}

//_____________________________________________________________________________ 
TStPidTagger::TStPidTagger(const StMuDst *mDst)
{
    mMuDst = mDst;
}

//_____________________________________________________________________________ 
TStPidTagger::~TStPidTagger()
{
    //
}


//_____________________________________________________________________________ 
Int_t TStPidTagger::Init()
{

    return 0;
}

//_____________________________________________________________________________ 
Int_t TStPidTagger::TagElectron()
{
    
}
