// Filename: TStFmsQAmaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "BrFmsMaker/TStFmsQAMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StFmsDbMaker/StFmsDbMaker.h"
#include "StMuDSTMaker/COMMON/StMuFmsCollection.h"
#include "StMuDSTMaker/COMMON/StMuFmsHit.h"
#include "StMuDSTMaker/COMMON/StMuFmsPoint.h"


ClassImp(TStFmsQAMaker)

TStFmsQAMaker::TStFmsQAMaker(const char *name):StMaker(name)
{
    
}


TStFmsQAMaker::~TStFmsQAMaker()
{

}


//_____________________________________________________________________________ 
Int_t TStFmsQAMaker::Init()
{
    mDbMaker = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    if (!mDbMaker)
    {
    	LOG_ERROR <<"TStFmsTreeMaker::InitRun - !StFmsDbMaker" <<endl;
    	return kStFatal;
    }
    mFile = new TFile(mOutName, "RECREATE");

    for(Int_t i = 0; i < 4; ++i)
    {
	Int_t MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (Int_t l = 0; l < MaxCh; l++) 
	{
	    TString title = "adcDist_";
	    title += (i + 8);        
	    title += "_";        
	    title += (l + 1);        
	    mAdcDist[i][l] = new TH1D(title, title, 300, 0.0, 500); 
	}   
    }
 
   return kStOK;
}


//_____________________________________________________________________________
Int_t TStFmsQAMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");
    if(!mMuDst)
    {
	cout << "Unable to get MuDst event" <<endl;
	return kStErr;
    }
    mFmsMuColl = mMuDst->muFmsCollection();
    if(!mFmsMuColl)
    {
	cout << "No Fms data. SKipping event..." <<endl;
	return kStSkip;
    }

    for(Int_t j = 0; j < mFmsMuColl->numberOfHits(); j++) 
    {	     
	mHit = mFmsMuColl->getHit(j);      
	mDetId = mHit->detectorId();       
	mChannel = mHit->channel();        
	if(mDetId >= 8 && mDetId <= 11)     // Exclude detector IDs corresponding to FPOST, FPS, FPD etc.
	    mAdcDist[mDetId - 8][mChannel - 1]->Fill(mHit->adc()); 
    }
    
    return kStOK; 
}

//_____________________________________________________________________________
Int_t TStFmsQAMaker::Finish()
{
    if(!mFile)
    {
	cout << "Unable to create output root file" <<endl;
	return kStErr;
    }
    mFile->cd();
    for(Int_t i = 0; i < 4; ++i)
    {
	Int_t MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (Int_t l = 0; l < MaxCh; l++)
	{
	    if(mDbMaker->getGain(i + 8, l + 1) != 0.0)  //Exclude unphysical cells
		mAdcDist[i][l]->Write();
	}	
    }
    mFile->Close();
    return kStOK;
}
