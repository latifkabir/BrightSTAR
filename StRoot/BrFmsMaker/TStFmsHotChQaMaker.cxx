// Filename: TStFmsHotChQaMaker.cxx
// Description: Make histrogram of FMS Channel energy using StEvent (i.e. with reconstruction do on the fly)
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif

#include "BrFmsMaker/TStFmsHotChQaMaker.h"
#include "StEvent/StEvent.h"
#include "StEvent/StFmsCollection.h"
#include "StEvent/StFmsHit.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
//#include "StFmsDbMaker/StFmsDbMaker.h"

ClassImp(TStFmsHotChQaMaker)

TStFmsHotChQaMaker::TStFmsHotChQaMaker(const char *name):StMaker(name)
{
    
}


TStFmsHotChQaMaker::~TStFmsHotChQaMaker()
{

}


//_____________________________________________________________________________ 
Int_t TStFmsHotChQaMaker::Init()
{
    // mDbMaker = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    // if (!mDbMaker)
    // {
    // 	LOG_ERROR <<"TStFmsTreeMaker::InitRun - !StFmsDbMaker" <<endl;
    // 	return kStFatal;
    // }
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
	    TString title = "engDist_";
	    title += (i + 8);        
	    title += "_";        
	    title += (l + 1);        
	    mEngDist[i][l] = new TH1D(title, title, 500, 0.0, 50); 
	}   
    }
    mCounter = new TH1D("counter", "run and event counter", 5, 0, 5);
    mCounter->Fill(2);
    
    return kStOK;
}


//_____________________________________________________________________________
Int_t TStFmsHotChQaMaker::Make()
{
    //mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");
    if(!mEvent)
    {
	cout << "TStFmsHotChQaMaker::Make : Unable to get StEvent event" <<endl;
	return kStErr;
    }
    mFmsColl = (StFmsCollection*)mEvent->fmsCollection();
    if(!mFmsColl)
    {
	cout << "No Fms data. SKipping event..." <<endl;
	return kStSkip;
    }

    StSPtrVecFmsHit fmsHitVec = mFmsColl->hits();
    for(Int_t j = 0; j < fmsHitVec.size(); j++) 
    {	     
	mHit = fmsHitVec[j];      
	mDetId = mHit->detectorId();       
	mChannel = mHit->channel();        
	if(mDetId >= 8 && mDetId <= 11)     // Exclude detector IDs corresponding to FPOST, FPS, FPD etc.
	    mEngDist[mDetId - 8][mChannel - 1]->Fill(mHit->energy()); 
    }
    mCounter->Fill(4);
    
    return kStOK; 
}

//_____________________________________________________________________________
Int_t TStFmsHotChQaMaker::Finish()
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
	    //if(mDbMaker->getGain(i + 8, l + 1) != 0.0)  //Exclude unphysical cells
		mEngDist[i][l]->Write();
	}	
    }
    mCounter->Write();
    mFile->Close();
    return kStOK;
}
