// Filename: TStTemplateMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStTemplateMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"

ClassImp(TStTemplateMaker)

//_____________________________________________________________________________ 
TStTemplateMaker::TStTemplateMaker(const char *name):StMaker(name)
{
    //
}

//_____________________________________________________________________________ 
TStTemplateMaker::~TStTemplateMaker()
{
    //
}


//_____________________________________________________________________________ 
Int_t TStTemplateMaker::Init()
{
    // Create tables
    
    // Create Histograms
    
    // Get pointer to database
    // mDbMaker = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    // if (!mDbMaker)
    // {
    // 	LOG_ERROR <<"TStFmsTreeMaker::InitRun - !StFmsDbMaker" <<endl;
    // 	return kStFatal;
    // }
    return kStOK;
}


//_____________________________________________________________________________
Int_t TStTemplateMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");
    // Get St<det>MuCollection
    // Or Get St<det>Colelction
    // Check if mMuDst or mEvent is valid
    if(!mMuDst)
    {
	LOG_ERROR << "TSt<Template>Maker::Make - No MuDst found" <<endm;
	return kStFatal;
    }
      
    return kStOK;
}

//_____________________________________________________________________________
Int_t TStTemplateMaker::Finish()
{
    //Write histograms to root file etc.

    return kStOK;
}
