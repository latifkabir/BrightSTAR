// Filename: AnFmsHotChFinder.C
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

6. Scan through pdf output for initial hot channels and other bad (bit shifted for example) channels and manually add channels to the json file that seem obvious.

7. Plot 2D sell activty distribution masking the channels from the json database and ensure the overall distribution is satisfactory. Repeat untill you are fully satisfied.

-------> Lot better idea: Divide all cells in many eta bins. Calculate mean number of entries of the cells for each eta bin. To decide if a cell is hot or not, compare the cells nEnries with the nEntries from that eta bin. You will be able to raise the threhold in this case and the keep many more cells than current case.

*/

#include "BrightStInclude.h"
#include "StRootInclude.h"
#include "RootInclude.h"
#include <vector>
using namespace std;

struct DetChPair
{
    Int_t det;
    Int_t ch;
    Int_t entries;
};

Bool_t AnComparisonFnc(DetChPair a, DetChPair b)
{
    return (a.entries > b.entries);    
}

//---------------------------------------------------------------------------------------------------------
void AnFmsHotChFinder(Int_t fillNoFirst, Int_t fillNoLast, Int_t iteration)
{
    //For hot channel 
    Int_t timesHigher = 2.0;
    Double_t upperTh = 1.5;
    Double_t lowerTh = 0.2;
    Double_t energyTh = 0.5;
    //For problematic bit shifted channels
    Double_t firstBinE = 1.0; // 1 GeV
    Double_t lastBinE = 5.0;  // 5 GeV
    Int_t maxEmptyBins = 45;  //35 Maximum number of empty bins allowed

    if(!TStar::gBrDebug)
	gROOT->SetBatch(1);
    //----------- Need for DB access -----------------------------------------------
    StChain *chain = new StChain;
    //connect to STAR FMS database
    St_db_Maker *stDb = new St_db_Maker("StarDb", "MySQL:StarDb");
    stDb->SetDateTime(20160301, 0); // An arbitrary time is set, since all we care about is the geometry.
    StFmsDbMaker *fmsDBMaker = new StFmsDbMaker("FmsDbMk");
    fmsDBMaker->Init();
    chain->Init();
    chain->EventLoop(1);
    //Check if database is working fine
    cout << "------->Max channel for det 8:"<< fmsDBMaker->maxChannel(8) <<endl;
    cout  <<"------->Max channel for det 11:"<< fmsDBMaker->maxChannel(11) <<endl;
    //--------------------------------------------------------------------------------
    TH2D *hist2d_before = new TH2D("hist2d_before", "FMS Cell Activity [Before Masking]; X [cm]; Y [cm]", 120,-100, 100, 120,-100, 100);
    TH2D *hist2d_odd = new TH2D("hist2d_odd", "FMS Cell Activity [Before Masking]; X [cm]; Y [cm]", 120,-100, 100, 120,-100, 100);
    TH2D *hist2d_even = new TH2D("hist2d_even", "FMS Cell Activity [Before Masking]; X [cm]; Y [cm]", 120,-100, 100, 120,-100, 100);
    TH2D *hist2d_after = new TH2D("hist2d_after", "FMS Cell Activity [After Masking]; X [cm]; Y [cm]", 120,-100, 100, 120,-100, 100);
    
    for(Int_t fillNo = fillNoFirst; fillNo <= fillNoLast; ++fillNo)
    {
	hist2d_before->Reset();
	hist2d_odd->Reset();
	hist2d_even->Reset();
	hist2d_after->Reset();
	
	TString inHistFile(Form("dst/R15FmsHotChQa/R15FmsHotChQa_Fill_%i.root", fillNo));
	if(gSystem->AccessPathName(inHistFile))
	{
	    if(TStar::gBrDebug)
		cout << "Input Histogram File NOT found" <<endl;
	    continue;
	}
	
	TFile *file = new TFile(inHistFile);
	if(!file)
	    continue;
	TCanvas *c1 = new TCanvas();
	if(TStar::gBrDebug)       
	    c1->Print(Form("FmsHotChFinder_%i.pdf(", fillNo), "pdf");
	
	DetChPair det_ch;
	const Int_t radialDiv = 15;
	vector < DetChPair > chList[radialDiv];
	Double_t averages[radialDiv] = {0};
	Int_t numCells = 0;
    
	TH1D *mEngDist[4][571];
	Bool_t isHotCh[4][571] = {0};
	TStFmsHotChDB *fmsHotChDb = new TStFmsHotChDB();
	TStFillNoDB *fillDb = new TStFillNoDB();    
	Int_t runNumber = fillDb->GetRunsWithFill(fillNo)[0];
	fmsHotChDb->GetHotChList(runNumber, isHotCh);
    
	Int_t nActiveCells = 0;
	Int_t nEntries = 0;
	Int_t accuEntries = 0;
	Double_t avgEntries = 0;
	const Int_t oMaxCh = 571; 
	const Int_t iMaxCh = 288;
	StThreeVectorF fmsVec;
	Double_t radius;
	Double_t width = (115.0 - 20.0) / radialDiv;
	Int_t radialBin = -1;
    
	gStyle->SetOptStat(0);
    
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
		fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
		radius = sqrt(fmsVec.x()*fmsVec.x() + fmsVec.y()*fmsVec.y());

		radialBin = -1;
		for(Int_t k = 0; k < radialDiv; k++)
		{

		    if(radius > (20 + k*width) && radius <= 20 + (k + 1)*width)
		    {
			radialBin = k;
			break;
		    }
		}

		if(radialBin == -1)
		{
		    if(TStar::gBrDebug)       
			cout << "Invalid radial bin number for det:" << (i+ 8) << "ch: "<< (l + 1) <<endl;
		    continue;
		}

		det_ch.det = i + 8;
		det_ch.ch = (l + 1);
		det_ch.entries = nEntries;
	    
		if(iteration == 0)
		{
		    accuEntries += nEntries;
		    chList[radialBin].push_back(det_ch); 
		    hist2d_before->Fill(fmsVec.x(), fmsVec.y(), nEntries);
		    ++nActiveCells;
		}
		else if(iteration > 0 && !isHotCh[i][l])
		{
		    accuEntries += nEntries;
		    chList[radialBin].push_back(det_ch); 
		    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
		    hist2d_before->Fill(fmsVec.x(), fmsVec.y(), nEntries);
		    ++nActiveCells;
		}
	    }   
	}
	avgEntries = (Double_t) (accuEntries / nActiveCells);

	if(TStar::gBrDebug)
	{
	    cout << "Number of active channels: "<< nActiveCells <<endl;
	    cout << "Average Entries: " << avgEntries <<endl;
	}
	if(TStar::gBrDebug)
	{
	    hist2d_before->Draw("colz");
	    c1->Print(Form("FmsHotChFinder_%i.pdf", fillNo), "pdf");
	    hist2d_before->Draw("lego");
	    c1->Print(Form("FmsHotChFinder_%i.pdf", fillNo), "pdf");
	}

	for(Int_t i = 0; i < radialDiv; ++i)
	{
	    if(TStar::gBrDebug)
		cout << "Radial bin: "<< i << " number of cells: " << chList[i].size() <<endl;

	    std::sort(chList[i].begin(), chList[i].end(), AnComparisonFnc);

	    for(Int_t j = 0; j < chList[i].size(); ++j)
	    {
		if(TStar::gBrDebug)
		    cout << "Redial bin: " << i << " det-ch pair: "<< j <<" (" << chList[i][j].det << ", " << chList[i][j].ch << ") "<< " entries:" << chList[i][j].entries <<endl;

		fmsVec = fmsDBMaker->getStarXYZ(chList[i][j].det, chList[i][j].ch);
		if(i % 2 == 1)	     
		    hist2d_odd->Fill(fmsVec.x(), fmsVec.y(), chList[i][j].entries);
		else
		    hist2d_even->Fill(fmsVec.x(), fmsVec.y(), chList[i][j].entries);	    
	    }
	    if(TStar::gBrDebug)       
		cout << "------------------------------------------------------------------------------------------\n" <<endl;
	}
	if(TStar::gBrDebug)
	{
	    hist2d_odd->Draw("colz");
	    c1->Print(Form("FmsHotChFinder_%i.pdf", fillNo), "pdf");
	    hist2d_even->Draw("colz");
	    c1->Print(Form("FmsHotChFinder_%i.pdf", fillNo), "pdf");
	}
	//------------- Calculate Average of each radial band --------------------------
	for(Int_t i = 0; i < radialDiv; ++i)
	{
	    if(TStar::gBrDebug)
		cout << "Calculating Average Activity for Radial Bin: "<< i << " Number of Cells: " << chList[i].size() <<endl;
	    if(chList[i].size() <= 10)
	    {
		averages[i] = 999999;
		if(TStar::gBrDebug)
		    cout << "Radial bin has too few entries. Setting default very high average "<< averages[i] <<endl;
		continue;
	    }
	    for(Int_t j = 5; j < chList[i].size() - 5 ; ++j)
		averages[i] += chList[i][j].entries;	    
	    averages[i] = averages[i] / (chList[i].size() - 10);
	    if(TStar::gBrDebug)
		cout << "Average entries (calculated excluding too big/small):" << averages[i] <<endl;
	}

	//--------------- Identifying hot channels based on the number of entries in interval -----------
	vector < DetChPair > hotChList;
	vector < DetChPair > badBsChList;
	vector <DetChPair>::iterator it;
               
	for(Int_t i = 0; i < radialDiv; ++i)
	{
	    for(Int_t j = 0; j < chList[i].size(); ++j)
	    {
		fmsVec = fmsDBMaker->getStarXYZ(chList[i][j].det, chList[i][j].ch);
		det_ch.det = chList[i][j].det;
		det_ch.ch = chList[i][j].ch;
		if(chList[i][j].entries > upperTh*averages[i])
		{
		    hotChList.push_back(det_ch);
		    hist2d_after->Fill(fmsVec.x(), fmsVec.y(), 0);
		}	
		else if(chList[i][j].entries < lowerTh*averages[i])
		{
		    badBsChList.push_back(det_ch);
		    hist2d_after->Fill(fmsVec.x(), fmsVec.y(), 0);
		}
		else
		    hist2d_after->Fill(fmsVec.x(), fmsVec.y(), chList[i][j].entries);	    
	    }
	}

	//---------- Find problematic bit shifted channels --------------
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
		if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)  //Exclude unphysical cells
		    continue;
		det_ch.det = (i + 8);
		det_ch.ch = (l + 1);
		nEmptyBins = 0;
		firstBin = mEngDist[i][l]->FindBin(firstBinE); 
		lastBin = mEngDist[i][l]->FindBin(lastBinE);  
		for(Int_t b = firstBin; b <= lastBin; ++b)
		{
		    if(mEngDist[i][l]->GetBinContent(b) == 0)
			++ nEmptyBins;
		}
		if(nEmptyBins >= maxEmptyBins)
		{
		    badBsChList.push_back(det_ch);
		    hist2d_after->Fill(fmsVec.x(), fmsVec.y(), 0); //This might not overwrite previous filled value
		}
	    }
	}
	if(TStar::gBrDebug)
	{
	    hist2d_after->Draw("colz");
	    c1->Print(Form("FmsHotChFinder_%i.pdf", fillNo), "pdf");
	    hist2d_after->Draw("lego");
	    c1->Print(Form("FmsHotChFinder_%i.pdf", fillNo), "pdf");
    
	    //--------- Print det id and channel id on the 2d histogram -----------------------
	    TCanvas *c2 = new TCanvas();
	    TText *text = new TText();
	    text->SetTextSize(0.007);
	    text->SetTextColor(kBlack);

	    if(TStar::gBrDebug)    
		hist2d_after->Draw("colz");
	    for(Int_t i = 0; i < 4; ++i)
	    {
		Int_t MaxCh;
		if(i == 0 || i == 1)
		    MaxCh = oMaxCh;
		else
		    MaxCh = iMaxCh;
		for (Int_t l = 0; l < MaxCh; l++) 
		{
		    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)  //Exclude unphysical cells
			continue;
		    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
		    text->DrawText(fmsVec.x() - 1.0, fmsVec.y() - 2.5, Form("%i, %i", i + 8, l + 1));	    
		}
	    }    

	    for(it = hotChList.begin(); it != hotChList.end(); ++ it)
	    {
		fmsVec = fmsDBMaker->getStarXYZ(it->det, it->ch);
		text->DrawText(fmsVec.x(), fmsVec.y(), "X");	    
	    }
	    for(it = badBsChList.begin(); it != badBsChList.end(); ++ it)
	    {
		fmsVec = fmsDBMaker->getStarXYZ(it->det, it->ch);
		text->DrawText(fmsVec.x(), fmsVec.y(), "X");	    
	    }

	    c2->Draw();
	    c2->Print(Form("FmsHotChFinder_%i.pdf)", fillNo), "pdf");
    
	    gStyle->SetOptStat(1);
	    TCanvas *c3 = new TCanvas();
	    c3->SetLogy(1);
	    c3->Print(Form("FmsHotCh_%i.pdf(", fillNo), "pdf");
	    for(it = hotChList.begin(); it != hotChList.end(); ++ it)
	    {
		mEngDist[it->det - 8][it->ch - 1]->Draw();
		c3->Print(Form("FmsHotCh_%i.pdf", fillNo), "pdf");
	    }
	    c3->Print(Form("FmsHotCh_%i.pdf)", fillNo), "pdf");

	    TCanvas *c4 = new TCanvas();
	    c4->SetLogy(1);
	    c4->Print(Form("FmsBitShiftCh_%i.pdf(", fillNo), "pdf");
	    for(it =  badBsChList.begin(); it !=  badBsChList.end(); ++ it)
	    {
		mEngDist[it->det - 8][it->ch - 1]->Draw();
		c4->Print(Form("FmsBitShiftCh_%i.pdf", fillNo), "pdf");
	    }
	    c4->Print(Form("FmsBitShiftCh_%i.pdf)", fillNo), "pdf");
	}
	Int_t nHotChCount = 0;
	if(TStar::gBrDebug)
	{
	    cout << "Number Hot Channels:"<< hotChList.size() <<endl;
	    cout << "Hot Channels:\n" <<endl;
	}
	//Hot channel list    
	cout << "\t{\n\t\t\"fill\":"<< fillNo <<", \n\t\t\"hot\": [";
	for(it = hotChList.begin(); it != hotChList.end(); ++ it)
	{
	    ++nHotChCount;
	    cout <<"{\"det\":"<<it->det << ", \"ch\":"<< it->ch<<"}";
	    if(nHotChCount != hotChList.size())
		cout << ", ";
	}
	cout << "],\n" <<endl;
	//Other (e.g. bit shifted) problematic channels
	Int_t nBadBsChCount = 0;
	if(TStar::gBrDebug)
	{
	    cout << "\n\n\nNumber of Problematic Bit Shifted Channels:"<< badBsChList.size() <<endl;
	    cout << "Problematic Bit Shifted Channels:\n" <<endl;
	}
	cout <<"\t\"bad\": [";
	for(it = badBsChList.begin(); it != badBsChList.end(); ++it)
	{
	    ++nBadBsChCount;
	    cout <<"{\"det\":"<<it->det << ", \"ch\":"<< it->ch<<"}";
	    if(nBadBsChCount != badBsChList.size())
		cout << ", ";
	}
	cout << "]\n" <<endl;
	cout << "\t},\n" <<endl;

	//delete file;
	file->Close();
    }
}





//---------------------------------------------------------------------------------------------------------
void AnFmsCellActivity(Int_t fillNo)
{
    //For hot channel 
    Int_t timesHigher = 3;
    Double_t energyTh = 0.5;
    //----------- Need for DB access -----------------------------------------------
    StChain *chain = new StChain;
    //connect to STAR FMS database
    St_db_Maker *stDb = new St_db_Maker("StarDb", "MySQL:StarDb");
    stDb->SetDateTime(20160301, 0); // An arbitrary time is set, since all we care about is the geometry.
    StFmsDbMaker *fmsDBMaker = new StFmsDbMaker("FmsDbMk");
    fmsDBMaker->Init();
    chain->Init();
    chain->EventLoop(1);
    //Check if database is working fine
    cout << "------->Max channel for det 8:"<< fmsDBMaker->maxChannel(8) <<endl;
    cout  <<"------->Max channel for det 11:"<< fmsDBMaker->maxChannel(11) <<endl;
    //--------------------------------------------------------------------------------
    TString inHistFile(Form("dst/R15FmsHotChQa/R15FmsHotChQa_Fill_%i.root", fillNo));
    TFile *file = new TFile(inHistFile);
    if(!file)
    {
    	cout << "Input Histogram File NOT found" <<endl;
    	return;
    }
    Bool_t isHotCh[4][571] = {0};
    TStFmsHotChDB *fmsHotChDb = new TStFmsHotChDB();
    TStFillNoDB *fillDb = new TStFillNoDB();    
    Int_t runNumber = fillDb->GetRunsWithFill(fillNo)[0];
    fmsHotChDb->GetHotChList(runNumber, isHotCh);
    
    TCanvas *c1 = new TCanvas();       
    TH1D *mEngDist[4][571];

    Int_t nActiveCells = 0;
    Int_t nEntries = 0;
    Int_t accuEntries = 0;
    Double_t avgEntries = 0;
    const Int_t oMaxCh = 571; 
    const Int_t iMaxCh = 288;
    StThreeVectorF fmsVec;
    
    TH2D *hist2d = new TH2D("hist2d", "FMS Cell Activity With Hot/Bad Channels Masked; X [cm]; Y [cm]", 120,-100, 100, 120,-100, 100);
    TH2D *hist2dep = new TH2D ("hist2dEtaPhis", "Cell Eta Phi [FMS];  #eta;  #phi", 100, 2.5, 4.5, 100, -3.2, 3.2);
    
    TText *text = new TText();
    text->SetTextSize(0.007);
    text->SetTextColor(kBlack);
    
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
	    if(isHotCh[i][l])
	    {
		hist2d->Fill(fmsVec.x(), fmsVec.y(), 0);
		hist2dep->Fill(fmsVec.pseudoRapidity(), fmsVec.phi(), 0);
	    }
	    else
	    {
		hist2d->Fill(fmsVec.x(), fmsVec.y(), nEntries);
		hist2dep->Fill(fmsVec.pseudoRapidity(), fmsVec.phi(), nEntries);
		++nActiveCells;
	    }
	}   
    }
    hist2d->Draw("lego");
    
    TCanvas *c2 = new TCanvas();
    gStyle->SetOptStat(0);
    hist2d->Draw("colz");
    for(Int_t i = 0; i < 4; ++i)
    {
	Int_t MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (Int_t l = 0; l < MaxCh; l++) 
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)  //Exclude unphysical cells
		continue;
	    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
	    text->SetTextSize(0.007);
	    text->DrawText(fmsVec.x() - 1.0, fmsVec.y() - 2.5, Form("%i, %i", i + 8, l + 1));	    
	    if(!isHotCh[i][l])
		continue;
	    text->SetTextSize(0.01);
	    text->DrawText(fmsVec.x(), fmsVec.y() - 1.0, "X");	    	    
	}
    }


    TCanvas *c3 = new TCanvas();
    hist2dep->Draw("colz");
    for(Int_t i = 0; i < 4; ++i)
    {
	Int_t MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (Int_t l = 0; l < MaxCh; l++) 
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0)  //Exclude unphysical cells
		continue;
	    fmsVec = fmsDBMaker->getStarXYZ(i + 8, l + 1);
	    text->SetTextSize(0.007);
	    text->DrawText(fmsVec.pseudoRapidity(), fmsVec.phi(), Form("%i, %i", i + 8, l + 1));	    
	    if(!isHotCh[i][l])
		continue;
	    text->SetTextSize(0.01);
	    text->DrawText(fmsVec.pseudoRapidity(), fmsVec.phi(), "X");	    	    
	}
    }

    
    avgEntries = (Double_t) (accuEntries / nActiveCells);

    cout << "Number of active channels: "<< nActiveCells <<endl;
    cout << "Average Entries: " << avgEntries <<endl;
}


void AnHotChQaSaveAsPdf(Int_t fillNo)
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
    TString inHistFile(Form("dst/R15FmsHotChQa/R15FmsHotChQa_Fill_%i.root", fillNo));
    TFile *file = new TFile(inHistFile);
    if(!file)
    {
	cout << "Input Histpgram File NOT found" <<endl;
	return;
    }
    TH1D *mEngDist[4][571];
    Bool_t isHotCh[4][571] = {0};
    TStFmsHotChDB *fmsHotChDb = new TStFmsHotChDB();
    TStFillNoDB *fillDb = new TStFillNoDB();    
    Int_t runNumber = fillDb->GetRunsWithFill(fillNo)[0];
    fmsHotChDb->GetHotChList(runNumber, isHotCh);
    
    TCanvas *c1 = new TCanvas();
    c1->SetLogy(1);
    c1->Print("FmsGoodCh.pdf(", "pdf");
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
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0 || isHotCh[i][l])  //Exclude unphysical cells or hot/bad cells
		continue;
	    
	    mEngDist[i][l] = (TH1D*)file->Get(title);
	    mEngDist[i][l]->Draw();
	    c1->Print("FmsGoodCh.pdf", "pdf");
	}   
    }
    c1->Print("FmsGoodCh.pdf)", "pdf");

    TCanvas *c2 = new TCanvas();
    c2->SetLogy(1);
    c2->Print("FmsExcludedCh.pdf(", "pdf");
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
	    if(fmsDBMaker->getGain(i + 8, l + 1) == 0.0 || !isHotCh[i][l])  //Exclude unphysical cells or good cells
		continue;
	    
	    mEngDist[i][l] = (TH1D*)file->Get(title);
	    mEngDist[i][l]->Draw();
	    c2->Print("FmsExcludedCh.pdf", "pdf");
	}   
    }
    c2->Print("FmsExcludedCh.pdf)", "pdf");    
    
}

