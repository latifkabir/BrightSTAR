// Filename: FmsCellStatus.cxx
// Description: Read FMS QA root file and make status table for cells to identify dead, bad, bit shifted and hot channels.
// Author: Latif Kabir < latif@jlab.org >
// Created: Fri Jul 19 01:03:54 2019 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "TStar.h"

void FmsCellStatus(TString inFile)
{
    gROOT->SetBatch(kTRUE);
    //gStyle->SetOptLogy(1);
    //--------------------------------------------
    //Need for DB access
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0, 0, "", "resources/temp/temp.list", "", 1000); 
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
    TStar::ExitIfInvalid(inFile);
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
    TH2D *hist2d = new TH2D("FMSMap", "FMS Cell Status Map", 100, -101.0, 101.0, 150, -101.0, 101.0);
    TText *text = new TText();
    text->SetTextSize(0.015);
    TCanvas *c0 = new TCanvas("c0", "FMS Cell Status Map");
    TCanvas *c1 = new TCanvas("c1", "Dead");
    TCanvas *c2 = new TCanvas("c2", "Bad");
    TCanvas *c3 = new TCanvas("c3", "Bit Shifted");
    
    StThreeVectorF fmsVec;
    Double_t x, y;
    Double_t x_offset = -1.5;
    Double_t y_offset = -1.5;
    
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
	    hist2d->Fill(fmsVec.x(), fmsVec.y(), i +8);
	}   
    }
    Int_t nBins = adcDist[0][459-1]->GetNbinsX();
    Int_t bin_low = (Int_t)(nBins*0.05);
    Int_t bin_hi = (Int_t)(nBins*0.50);
    Int_t refIntegral1 = (Int_t)(0.15*adcDist[0][459-1]->Integral(bin_low, bin_hi)); // 15% entries of reference histogram in the specified window
    Int_t refIntegral2 = 10;
    Double_t refRMS = 2.0;
    
    c0->cd();
    hist2d->Draw("colz");
    Int_t eBinCounter = 0;
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
	    det_ch.det = (i + 8);
	    det_ch.ch = (l + 1);
	    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
	    x = fmsVec.x() + x_offset;
	    y = fmsVec.y() + y_offset;
	    if(adcDist[i][l]->GetEntries() == 0)
	    {
		deadChList.push_back(det_ch);
		text->SetTextColor(kGreen);
		text->DrawText(x, y, Form("%i", l + 1));
		continue;
	    }
	    else if((adcDist[i][l]->Integral(bin_low, bin_hi) < refIntegral1 && adcDist[i][l]->GetEntries() > 0)
		    || (adcDist[i][l]->GetRMS() < refRMS)
		    || adcDist[i][l]->Integral(150, 250) < refIntegral2)
	    {	
		badChList.push_back(det_ch);
		text->SetTextColor(kRed);
		text->DrawText(x, y, Form("%i", l + 1));
		continue;
	    }
	    else
	    {
		eBinCounter = 0;
		for(Int_t m = 1; m <= 10; ++m)
		{
		    if(adcDist[i][l]->GetBinContent(m) == 0)
			++eBinCounter;
		    if(eBinCounter >= 3)
		    {
			bitShChList.push_back(det_ch);
			text->SetTextColor(kBlue);
			text->DrawText(x, y, Form("%i", l + 1));
			break;
		    }
		    else if(m == 10)
		    {
			//text->SetTextColor(kBlack);
			//text->DrawText(x, y, Form("%i", l + 1));
		    }
		}
	    }
	}
    }
    //-----------------------------------------------------
    vector <DetChPair>::iterator it;
    ofstream deadList("results/pdf/6.FMS_QA/deadList.txt");
    ofstream badList("results/pdf/6.FMS_QA/badList.txt");
    ofstream bitShList("results/pdf/6.FMS_QA/bitShList.txt");
    if(!badList || !bitShList)
    {
	cout<< "Unable to create channel list"<<endl;
	return;
    }
    c0->Print("results/pdf/FMS_Channel_Status_Map.pdf", "pdf");	 
    c1->Print("results/pdf/FMS_DeadChannel.pdf(", "pdf");
    c2->Print("results/pdf/FMS_BadChannel.pdf(", "pdf");
    c3->Print("results/pdf/FMS_BitShChannel.pdf(", "pdf");

    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
    cout<< "Total dead channels: "<< deadChList.size() << endl;
    cout<< "Total bad channels: "<< badChList.size() << endl;
    cout<< "Total bit-shifted channels: "<< bitShChList.size() << endl;
    cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< endl;
    
    cout << "List of dead channels:" <<endl;
    for(it = deadChList.begin(); it != deadChList.end(); ++ it)
    {
	cout << it->det << "\t"<< it->ch<<endl;
	deadList << it->det << "\t"<< it->ch<<endl;
	c1->cd();
	adcDist[it->det - 8][it->ch -1]->Draw();
	c1->Print("results/pdf/FMS_DeadChannel.pdf", "pdf");
    }
    cout << "List of bad channels:" <<endl;
    for(it = badChList.begin(); it != badChList.end(); ++ it)
    {
	cout << it->det << "\t"<< it->ch <<endl;
	badList << it->det << "\t"<< it->ch <<endl;
	c2->cd();
	adcDist[it->det - 8][it->ch -1]->Draw();
	c2->Print("results/pdf/FMS_BadChannel.pdf", "pdf");
    }
    cout << "List of bit-shifted channels:" <<endl;
    for(it = bitShChList.begin(); it != bitShChList.end(); ++ it)
    {
	cout << it->det << "\t"<< it->ch <<endl;
	bitShList << it->det << "\t"<< it->ch <<endl;
	c3->cd();
	adcDist[it->det - 8][it->ch -1]->Draw();
	c3->Print("results/pdf/FMS_BitShChannel.pdf", "pdf");
    }
    c1->Print("results/pdf/FMS_DeadChannel.pdf)", "pdf");
    c2->Print("results/pdf/FMS_BadChannel.pdf)", "pdf");
    c3->Print("results/pdf/FMS_BitShChannel.pdf)", "pdf");
    
    //fmsDBMaker->dumpFmsBitShiftGain();
    deadList.close();
    badList.close();
    bitShList.close();
}
