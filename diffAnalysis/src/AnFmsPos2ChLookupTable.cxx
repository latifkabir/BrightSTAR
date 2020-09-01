// Filename: AnFmsPos2ChMapper.cxx
// Description: Generate a lookup tab;e 
// Author: Latif Kabir < latif@jlab.org >
// Created: Fri Jul 19 01:03:54 2019 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "TStar.h"

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
    Int_t maxCh;    
    StThreeVectorF fmsVec;
    
    Double_t x, y;
    TFile *f = new TFile("FmsLookupTable.root", "recreate");
    TH2D *hist2d = new TH2D("hist2d", "Position to Channel Id", 120,-100, 100, 120,-100, 100);
    
    for(Int_t i = 0; i < 4; ++i)
    {
	maxCh = (i < 2)? oMaxCh : iMaxCh; 
	for (Int_t l = 0; l < maxCh; l++) 
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)
		continue;
	    
	    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
	    x = fmsVec.x();
	    y = fmsVec.y();
	    hist2d->Fill(x, y, (i + 8)*1000 + (l + 1));
	}   
    }

    hist2d->Draw("colz");

    f->Write();
}


void AnFmsGetChFromPos(Double_t x, Double_t y)
{
    TString inLookupFile = TStar::gConfig->GetStarHome() + "/database/FmsLookupTable.root";
    if(gSystem->AccessPathName(inLookupFile))
    {
	cout << "Input root file not found" <<endl;
	return;
    }
    TFile *f = new TFile(inLookupFile);
    TH2D *hist2d = (TH2D*)f->Get("hist2d");

    Int_t ch = hist2d->GetBinContent(hist2d->FindBin(x, y));

    if(ch > 8000 && ch < 9000)
	cout << "x: "<< x << " y: "<< y << " Det: " << 8 << " ch: " << (ch - 8000) <<endl;
    else if(ch > 9000 && ch < 10000)
	cout << "x: "<< x << " y: "<< y << " Det: " << 9 << " ch: " << (ch - 9000) <<endl;
    else if(ch > 10000 && ch < 11000)
	cout << "x: "<< x << " y: "<< y << " Det: " << 10 << " ch: " << (ch - 10000) <<endl;
    else if(ch > 11000 && ch < 12000)
	cout << "x: "<< x << " y: "<< y << " Det: " << 11 << " ch: " << (ch - 11000) <<endl;
    
    f->Close();
}

// tree->Branch("iCell", &iData[0][0][0], "iCell[2][288][2]/D"); //[detId][channel][x or y]
// tree->Branch("oCell", &oData[0][0][0], "oCell[2][571][2]/D"); //[detId][channel][x or y]

void AnFmsPrintChIdOnHist(TString dummyInFile, TString histFileName, TString histName)
{
    //Need for DB access
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0, 0, "", dummyInFile, "", 1000); 
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
    Int_t maxCh;    
    StThreeVectorF fmsVec;
    
    Double_t x, y;
    TFile *f = new TFile(histFileName);
    TH2D *hist2d = (TH2D*)f->Get(histName);
    if(!hist2d)
    {
	cout << "Found null pointer for histogram. Check hist name" <<endl;
	return;
    }
    TCanvas *c2 = new TCanvas();
    TText *text = new TText();
    text->SetTextSize(0.007);
    text->SetTextColor(kBlack);

    hist2d->Draw("colz");
    
    for(Int_t i = 0; i < 4; ++i)
    {
	maxCh = (i < 2)? oMaxCh : iMaxCh; 
	for (Int_t l = 0; l < maxCh; l++) 
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)
		continue;
	    
	    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
	    x = fmsVec.x();
	    y = fmsVec.y();
	    text->DrawText(x, y, Form("%i, %i", i + 8, l + 1));	    ;
	}   
    }

    c2->Draw();       
}
