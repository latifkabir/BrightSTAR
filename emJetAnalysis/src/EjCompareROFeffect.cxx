// Filename: EjCompareROFeffect.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Aug 17 02:30:08 2021 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include "cppInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"
#include "Hists.h"

using namespace std;


void EjCompareROFeffect(TString fWithSmBs3, TString fWoSmBs3)    
{

    if(gSystem->AccessPathName(fWithSmBs3) || gSystem->AccessPathName(fWoSmBs3))
    {
	cout << "Could not find the input file" <<endl;
	return;
    }
    TFile *fW = new TFile(fWithSmBs3);
    TFile *fWo = new TFile(fWoSmBs3);

    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;

    TGraphErrors *bGrPhy[kEnergyBins][kPhotonBins];
    TGraphErrors *yGrPhy[kEnergyBins][kPhotonBins];

    
    for(Int_t i = 0; i < kEnergyBins; ++i)
    {
	for(Int_t j = 0; j < kPhotonBins; ++j)
	{
	    TString name = Form("bEbin%i_PhotonBin%i", i, j);
	    bGrPhy[i][j] = (TGraphErrors*)fW->Get(name);
	    yGrPhy[i][j] = (TGraphErrors*)fWo->Get(name);
	   
	    bGrPhy[i][j]->SetTitle("");
	    yGrPhy[i][j]->SetTitle("");
	}
    }
    fW->Close();
    fWo->Close();


    //--- For Fms ---
    //only consider energy ranges 20 -40, 40 - 60, 60 - 80 i.e. bin index 1, 2, 3  and nPhotons = 1 - 5
    TCanvas* c2 = new TCanvas("asym_fms","Asymmetries",1000,600);
    float varMins[5] = { 1.8, 1.8, 1.8, 1.8, 1.8};
    float varMaxs[5] = { 8.2, 8.2, 8.2, 8.2, 8.2};
    const char* xTitles[3] = { "p_{T} [GeV/c]","p_{T} [GeV/c]","p_{T} [GeV/c]" };
    const char* yTitles[5] = { "A_{N}", "A_{N}", "A_{N}", "A_{N}", "A_{N}" };

    PanelPlot* asymPlot = new PanelPlot(c2, 3, 5, 4, "asym_fms", xTitles, yTitles);

    for (int i = 0; i < 5; i++)
    {
	for (int j = 0; j < 3; j++)
	{
	    asymPlot->GetPlot(j,i)->SetXRange( varMins[i], varMaxs[i]);	
	    asymPlot->GetPlot(j,i)->SetYRange( -0.1, 0.1); // Run 15
		    
	    asymPlot->GetPlot(j,i)->Add(bGrPhy[j+1][4 - i], Plot::Point | Plot::Erry, 0); //With SmBs3 trigger
	    asymPlot->GetPlot(j,i)->Add(yGrPhy[j+1][4 - i], Plot::Point | Plot::Erry, 8); //Without SmBs3 trigger	
	}
    }  
    asymPlot->Draw();	    
}
