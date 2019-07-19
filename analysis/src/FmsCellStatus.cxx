// Filename: FmsCellStatus.cxx
// Description: Read FMS QA root file and make status table for cells to identify dead, bad, bit shifted and hot channels.
// Author: Latif Kabir < latif@jlab.org >
// Created: Fri Jul 19 01:03:54 2019 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"

void FmsCellStatus(TString inFile)
{
    //--------------------------------------------
    //Need for DB access
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0, 0, "", "resources/temp/FmsFileList.list", "", 1000); 
    mMaker->SetStatus("*",0);
    mMaker->SetStatus("Fms*",1);
    //connect to STAR FMS database
    St_db_Maker *stDb = new St_db_Maker("StarDb", "MySQL:StarDb");
    stDb->SetDateTime(20170601, 0); // An arbitrary time is set, since all we care about is the geometry.
    StFmsDbMaker *fmsDBMaker = new StFmsDbMaker("FmsDbMk");
    fmsDBMaker->Init();
    chain->Init();
    chain->EventLoop(1);
    //Check if database is working fine
    cout << "------->Max channel for det 8:"<< fmsDBMaker->maxChannel(8) <<endl;
    cout  <<"------->Max channel for det 11:"<< fmsDBMaker->maxChannel(11) <<endl;

    //---------------------------------------------
    const Int_t oMaxCh = 571; 
    const Int_t iMaxCh = 288;
    TH1F *adcDist[4][oMaxCh]; //Initilize the 1D histogram.
    TStar::ExistIfInvalid(inFile);
    TFile *file = new TFile(inFile);
    struct DetChPair
    {
	Int_t det;
	Int_t ch;
    } det_ch;
    
    vector < DetChPair > deadChList;
    vector < DetChPair > badChList;
    vector < DetChPair > hotChList;
    vector < DetChPair > bitShChList;
    TH2D *hist2d = new TH2D("FMSMap", "FMS Cell Status Map", 100, -101.0, 101.0, 100, -101.0, 101.0);
    TText *text = new TText();
    text->SetTextSize(0.015);
    StThreeVectorF fmsVec;
    
    for(Int_t i = 0; i < 4; ++i)
    {
	Int_t MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (Int_t l = 0; l < MaxCh; l++) 
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)
		continue;
	    TString hist_name = "adcDist_";
	    hist_name += (i + 8);        
	    hist_name += "_";        
	    hist_name += (l + 1);        
	    adcDist[i][l] = (TH1F*) file->Get(hist_name);
	    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
	    hist2d->Fill(fmsVec.x(), fmsVec.y(), i + l);	    
	}   
    }
    hist2d->Draw("colz");
    //------------------------------------------------------------
    for(Int_t i = 0; i < 4; ++i)
    {
	Int_t MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (Int_t l = 0; l < MaxCh; l++)
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)
		continue;
	    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);

	    if(adcDist[i][l]->GetEntries() == 0)
	    {
		det_ch.det = (i + 8);
		det_ch.ch = (l + 1);
		deadChList.push_back(det_ch);
		text->SetTextColor(kGreen);
		text->DrawText(fmsVec.x(), fmsVec.y(), Form("%i", l + 1));
		continue;
	    }
	    else if(adcDist[i][l]->GetEntries() < 1000 && adcDist[i][l]->GetEntries() > 0)
	    {
		det_ch.det = (i + 8);
		det_ch.ch = (l + 1);
		badChList.push_back(det_ch);
		text->SetTextColor(kRed);
		text->DrawText(fmsVec.x(), fmsVec.y(), Form("%i", l + 1));
		continue;
	    }
	    for(Int_t m = 1; m <= 10; ++m)
	    {
		if(adcDist[i][l]->GetBinContent(m) == 0)
		{
		    det_ch.det = (i + 8);
		    det_ch.ch = (l + 1);
		    bitShChList.push_back();
		    text->SetTextColor(kBlue);
		    text->DrawText(fmsVec.x(), fmsVec.y(), Form("%i", l + 1));
		    continue;
		}
	    }
	    text->SetTextColor(kBlack);
	    text->DrawText(fmsVec.x(), fmsVec.y(), Form("%i", l + 1));
	}
    }
    //-----------------------------------------------------
    vector <DetChPair>::iterator it;    
    cout << "List of dead channels:" <<endl;
    for(it = deadChList.begin(); it != deadChList.end(); ++ it)
	cout << it->det << "\t"<< it->ch<<endl;
    
    cout << "List of bad channels:" <<endl;
    for(it = badChList.begin(); it != badChList.end(); ++ it)
	cout << it->det << "\t"<< it->ch <<endl;

    cout << "List of bit-shifted channels:" <<endl;
    for(it = bitShChList.begin(); it != bitShChList.end(); ++ it)
	cout << it->det << "\t"<< it->ch <<endl;
    
    file->Close();    
}
