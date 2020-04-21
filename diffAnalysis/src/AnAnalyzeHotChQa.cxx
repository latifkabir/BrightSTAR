// Filename: AnAnalyzeHotChQa.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Apr 19 01:20:38 2020 (-0400)
// URL: jlab.org/~latif

/*
FMS Hot Channel Masking Work Flow
-----------------------------------

1. Genearte Max (571 + 299) histograms of all active cells having the energy distribution using RunFmsHotChQaMaker and Scheduler for a specific fill number 

2. Save all histograms in a merged pdf for your refernce to check visually once you have final hot channel list.

3. Use the output from step-1 in the script AnAnalyseHotChQa

4. Find hot channels using the following criteria:

 - i) If any cell has 5 times higher entires than the average number of entries within a specific energy intrgeal range (exclude near zero energy entries)
 - ii) compare any cell's nEntries with other cell's nEntries which are in the vicinity. If significantly higher, mark it as hot. This step may be done algorithmcally or visually with a 2D plot with det and cell id printed.

5. Make a list of bit shifted channels that did not get fixed based on the energy distribution. Effect of these chnnels on the physics result should be examined. This can be done by  checking number of empty bins in an interval.

*/

#include "StRootInclude.h"
#include "RootInclude.h"
#include <vector>
using namespace std;

void AnAnalyzeHotChQa(TString inHistFile)
{
    //For hot channel 
    Int_t timesHigher = 3;
    Double_t energyTh = 0.5;
    //For problematic bit shifted channels
    Double_t firstBinE = 1.0; // 1 GeV
    Double_t lastBinE = 5.0;  // 5 GeV
    Int_t maxEmptyBins = 10;   //Maximum number of empty bins allowed
    
    //----------- Need for DB access -----------------------------------------------
    StChain *chain = new StChain;
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
    //--------------------------------------------------------------------------------
    
    TFile *file = new TFile(inHistFile);
    if(!file)
    {
	cout << "Input Histpgram File NOT found" <<endl;
	return;
    }
    TCanvas *c1 = new TCanvas();
    c1->Print("FmsHotChQaAnalysis.pdf(", "pdf");
    
    TH1D *mEngDist[4][571];

    Int_t nActiveCells = 0;
    Int_t nEntries = 0;
    Int_t accuEntries = 0;
    Double_t avgEntries = 0;
    const Int_t oMaxCh = 571; 
    const Int_t iMaxCh = 288;
    StThreeVectorF fmsVec;
    
    TH2D *hist2d_before = new TH2D("hist2d_before", "Fms Cell Activity; Cell Energy [GeV]; No. of Entries", 120,-100, 100, 120,-100, 100);
    
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
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)  //Exclude unphysical cells
		continue;
	    
	    mEngDist[i][l] = (TH1D*)file->Get(title);
	    //nEntries = mEngDist[i][l]->GetEntries();
	    nEntries = mEngDist[i][l]->Integral(mEngDist[i][l]->FindBin(energyTh), mEngDist[i][l]->GetNbinsX() + 1);
	    accuEntries += nEntries;
	    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
	    hist2d_before->Fill(fmsVec.x(), fmsVec.y(), nEntries);
	    ++nActiveCells;
	}   
    }
    avgEntries = (Double_t) (accuEntries / nActiveCells);

    cout << "Number of active channels: "<< nActiveCells <<endl;
    cout << "Average Entries: " << avgEntries <<endl;

    hist2d_before->Draw("colz");
    c1->Print("FmsHotChQaAnalysis.pdf", "pdf");
    hist2d_before->Draw("lego");
    c1->Print("FmsHotChQaAnalysis.pdf", "pdf");

    //--------------- Identifying hot channels based on the the number of entries in interval -----------
    struct DetChPair
    {
	Int_t det;
	Int_t ch;
    } det_ch;
    vector < DetChPair > hotChList;
    vector < DetChPair > badBsChList;
    TH2D *hist2d_after = new TH2D("hist2d_after", "Fms Cell Activity", 120,-100, 100, 120,-100, 100);
    Int_t firstBin;
    Int_t lastBin;
    Int_t nEmptyBins;
    
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
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)  //Exclude unphysical cells
		continue;

	    det_ch.det = (i + 8);
	    det_ch.ch = (l + 1);
	    	       
	    //nEntries = mEngDist[i][l]->GetEntries();
	    nEntries = mEngDist[i][l]->Integral(mEngDist[i][l]->FindBin(energyTh), mEngDist[i][l]->GetNbinsX() + 1);
	    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
	    if(nEntries >= timesHigher*avgEntries)
	    {
		hotChList.push_back(det_ch);
		hist2d_after->Fill(fmsVec.x(), fmsVec.y(), 0);
	    }
	    else
		hist2d_after->Fill(fmsVec.x(), fmsVec.y(), nEntries);

	    //-------------- Find problematic bit shifted channels --------------------------
	    nEmptyBins = 0;
	    firstBin = mEngDist[i][l]->FindBin(firstBinE); 
	    lastBin = mEngDist[i][l]->FindBin(lastBinE);  
	    for(Int_t b = firstBin; b <= lastBin; ++b)
	    {
		if(mEngDist[i][l]->GetBinContent(b) == 0)
		   ++ nEmptyBins;
	    }
	    if(nEmptyBins >= maxEmptyBins)
		badBsChList.push_back(det_ch);
	}   
    }
    hist2d_after->Draw("colz");
    c1->Print("FmsHotChQaAnalysis.pdf", "pdf");
    hist2d_after->Draw("lego");
    c1->Print("FmsHotChQaAnalysis.pdf)", "pdf");
    
    vector <DetChPair>::iterator it;
    cout << "Number Hot Channels:"<< hotChList.size() <<endl;
    cout << "Hot Channels:" <<endl;
    for(it = hotChList.begin(); it != hotChList.end(); ++ it)
	cout << it->det << "\t"<< it->ch<<endl;

    cout << "\n\n\nNumber Problematic Bit Shifted Channels:"<< badBsChList.size() <<endl;
    cout << "Problematic Bit Shifted Channels:" <<endl;
    for(it = badBsChList.begin(); it != badBsChList.end(); ++ it)
	cout << it->det << "\t"<< it->ch<<endl;
    
}

void AnHotChQaSaveAsPdf(TString inHistFile)
{
    gROOT->SetBatch(kTRUE);
    const Int_t oMaxCh = 571; 
    const Int_t iMaxCh = 288;
    //----------- Need for DB access -----------------------------------------------
    StChain *chain = new StChain;
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
    //--------------------------------------------------------------------------------

    TFile *file = new TFile(inHistFile);
    if(!file)
    {
	cout << "Input Histpgram File NOT found" <<endl;
	return;
    }
    TH1D *mEngDist[4][571];
    TCanvas *c1 = new TCanvas();
    c1->Print("FmsHotChQa.pdf(", "pdf");
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
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)  //Exclude unphysical cells
		continue;
	    
	    mEngDist[i][l] = (TH1D*)file->Get(title);
	    mEngDist[i][l]->Draw();
	    c1->Print("FmsHotChQa.pdf", "pdf");
	}   
    }
    c1->Print("FmsHotChQa.pdf)", "pdf");    
}
