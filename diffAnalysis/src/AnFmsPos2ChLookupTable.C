// Filename: AnFmsPos2ChMapper.cxx
// Description: Generate a lookup tab;e 
// Author: Latif Kabir < latif@jlab.org >
// Created: Fri Jul 19 01:03:54 2019 (-0400)
// URL: jlab.org/~latif

// #include "StRootInclude.h"
// #include "RootInclude.h"
// #include "TStar.h"

void AnFmsPos2ChLookupTable(TString inFile)
{
    //Need for DB access
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0, 0, "", inFile, "", 1000); 
    mMaker->SetStatus("*",0);
    mMaker->SetStatus("Fms*",1);
    //connect to STAR FMS database
    St_db_Maker *stDb = new St_db_Maker("StarDb", "MySQL:StarDb");
    stDb->SetDateTime(20150601, 0); // An arbitrary time is set, since all we care about is the geometry.
    StFmsDbMaker *fmsDBMaker = new StFmsDbMaker("FmsDbMk");
    fmsDBMaker->Init();
    chain->Init();
    chain->EventLoop(1);
    //Check if database is working fine
    cout << "------->Max channel for det 8:"<< fmsDBMaker->maxChannel(8) <<endl;
    cout  <<"------->Max channel for det 11:"<< fmsDBMaker->maxChannel(11) <<endl;

    const Int_t oMaxCh = 571; 
    const Int_t iMaxCh = 288;
    
    StThreeVectorF fmsVec;
    Double_t iData[2][288][2];
    Double_t oData[2][571][2];
    
    Double_t x, y;
    TFile *f = new TFile("FmsLookupTable.root", "recreate");
    TH2D *hist2d = new TH2D("hist2d", "Channel id to Position", 120,-100, 100, 120,-100, 100);
    TTree *tree = new TTree("T", "Fms Lookup Tree");
    tree->Branch("iCell", &iData[0][0][0], "iCell[2][288][2]/D"); //[detId][channel][x or y]
    tree->Branch("oCell", &oData[0][0][0], "oCell[2][571][2]/D"); //[detId][channel][x or y]
    
    for(Int_t i = 0; i < 2; ++i)
    {
	for (Int_t l = 0; l < oMaxCh; l++) 
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)
	    {
		x = -100;
		y = -100;		
	    }
	    else
	    {
		fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
		x = fmsVec.x();
		y = fmsVec.y();
	    }
	    oData[i][l][0] = x;
	    oData[i][l][1] = y;
	    hist2d->Fill(x, y);
	}   
    }

    for(Int_t i = 2; i < 4; ++i)
    {	
	for (Int_t l = 0; l < iMaxCh; l++) 
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)
	    {
		x = -100;
		y = -100;		
	    }
	    else
	    {
		fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
		x = fmsVec.x();
		y = fmsVec.y();
	    }
	    iData[i - 2][l][0] = x;
	    iData[i - 2][l][1] = y;
	    hist2d->Fill(x, y);
	}   
    }
    T->Fill();    
    hist2d->Draw("colz");

    f->Write();
}

