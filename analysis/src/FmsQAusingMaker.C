// Filename: FmsQAusingMaker.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jul 18 01:23:03 2019 (-0400)
// URL: jlab.org/~latif

/*
Note: This script keep printing database status printout for every event unless you set the timestamp (day and year).

 */

void FmsQAusingMaker()
{   
    gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    loadSharedLibraries();
    TFile *f = new TFile("FmsQA.root","recreate"); //Creates a root file that will hold the histograms.
	
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0,0,"","resources/temp/FmsFileList.list","", 1000);
    mMaker->SetStatus("*",0);
    //mMaker->SetStatus("MuEvent*",1); // YOu need to enable this to get time from event info. Currently it prints table for every event, we we disable it and set time-stamp manually for the DB.
    mMaker->SetStatus("Fms*",1);
    //connect to STAR FMS database
    St_db_Maker *stDb = new St_db_Maker("StarDb", "MySQL:StarDb");
    stDb->SetDateTime(20170601, 0); // An arbitrary time is set, since all we care about is the geometry.
    StFmsDbMaker *fmsDBMaker = new StFmsDbMaker("FmsDbMk");
    fmsDBMaker->Init();
    
    StMuDst *mDst =  mMaker->muDst();  //Get the Tree from MuDst.
    TChain *ch = mMaker->chain();
	
    StMuFmsCollection *fmsMuColl;
    StMuFmsHit *hit;                          // Look at StMuFmsHit documentaion.    
    int iEvent = 0; // Event number.
    int channel; 
    int detID;
    const oMaxCh = 571; 
    const iMaxCh = 288;
    TH1F *adcDist[4][oMaxCh]; //Initilize the 1D histogram.
    
    for(int i = 0; i < 4; ++i)
    {
	int MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (int l = 0; l < MaxCh; l++) 
	{
	    TString title = "adcDist_";
	    title += (i + 8);        
	    title += "_";        
	    title += (l + 1);        
	    adcDist[i][l] = new TH1F(title, title, 300, 0.0, 500); //Creating 1D histograms for each channel.
	}   
    }

    chain->Init();
    for(int evt = 0; evt < ch->GetEntries(); ++evt)
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
       
	for(int j = 0; j < fmsMuColl->numberOfHits(); j++) //For loop to fill each channel histogram.
	{	     
	    hit = fmsMuColl->getHit(j);       //Look at documentaion for hit.
	    detID = hit->detectorId();        //Gets detector ID for each event.
	    channel = hit->channel();         //Gets channel for each event. 288 for small/oMaxCh for large.
	    if(detID >= 8 && detID <= 11)     // Exclude detector IDs corresponding to FPOST, FPS, FPD etc.
		adcDist[detID - 8][channel - 1]->Fill(hit->adc()); //fill the histogram with adc data. Note: channel number starts from 1.	    
	}
    }

    //Check if database is working fine
    cout << "------->Max channel for det 8:"<< fmsDBMaker->maxChannel(8) <<endl;
    cout  <<"------->Max channel for det 11:"<< fmsDBMaker->maxChannel(11) <<endl;
    
    f->cd();

    for(int i = 0; i < 4; ++i)
    {
	int MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (int l = 0; l < MaxCh; l++)
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) != 0.0)
		adcDist[i][l]->Write();
	}
    }
    chain->Finish();    
    f->Close();    
}			
	
