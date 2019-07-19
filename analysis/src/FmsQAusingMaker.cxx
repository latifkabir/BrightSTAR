// Filename: FmsQAusingMaker.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jul 18 01:23:03 2019 (-0400)
// URL: jlab.org/~latif

/*
Note: This script keep printing database status printout for every event unless you set the timestamp (day and year).

 */
#include "StRootInclude.h"
#include "RootInclude.h"
#include "cppInclude.h"

void FmsQAusingMaker(TString fileList, TString outFileName)
{   
    TStar::ExitIfInvalid(fileList);
    
    TFile *f = new TFile(outFileName,"recreate"); 
	
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);
    mMaker->SetStatus("*",0);
    mMaker->SetStatus("Fms*",1);
    //connect to STAR FMS database
    St_db_Maker *stDb = new St_db_Maker("StarDb", "MySQL:StarDb");
    stDb->SetDateTime(20170601, 0); // An arbitrary time is set, since all we care about is the geometry.
    StFmsDbMaker *fmsDBMaker = new StFmsDbMaker("FmsDbMk");
    fmsDBMaker->Init();
    
    StMuDst *mDst =  mMaker->muDst(); 
    TChain *ch = mMaker->chain();
	
    StMuFmsCollection *fmsMuColl;
    StMuFmsHit *hit;                         
    Int_t iEvent = 0; 
    Int_t channel; 
    Int_t detID;
    const oMaxCh = 571; 
    const iMaxCh = 288;
    TH1F *adcDist[4][oMaxCh]; 
    
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
	    adcDist[i][l] = new TH1F(title, title, 300, 0.0, 500); 
	}   
    }

    chain->Init();
    for(Int_t evt = 0; evt < ch->GetEntries(); ++evt)
    {
	if(evt % 1000 == 0)
	    cout << "Events processed:"<< evt <<endl;
	chain->Clear();
	chain->Make(evt);
	
	fmsMuColl = mDst->muFmsCollection();

	if(!fmsMuColl)
	{
	    cout<<"No FMS data for this event"<<endl;
	    continue;
	}
       
	for(Int_t j = 0; j < fmsMuColl->numberOfHits(); j++) 
	{	     
	    hit = fmsMuColl->getHit(j);      
	    detID = hit->detectorId();       
	    channel = hit->channel();        
	    if(detID >= 8 && detID <= 11)     // Exclude detector IDs corresponding to FPOST, FPS, FPD etc.
		adcDist[detID - 8][channel - 1]->Fill(hit->adc()); 
	}
    }

    //Check if database is working fine
    cout << "------->Max channel for det 8:"<< fmsDBMaker->maxChannel(8) <<endl;
    cout  <<"------->Max channel for det 11:"<< fmsDBMaker->maxChannel(11) <<endl;
    
    f->cd();

    for(Int_t i = 0; i < 4; ++i)
    {
	Int_t MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (Int_t l = 0; l < MaxCh; l++)
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) != 0.0)  //Exclude unphysical cells
		adcDist[i][l]->Write();
	}
    }
    chain->Finish();    
    f->Close();    
}			
	
