// Filename: EjCreateBinnedHist.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri May  8 15:08:25 2020 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include "RootInclude.h"
#include "cppInclude.h"
#include "BrightStInclude.h"
using namespace std;

void EjCreateBinnedHist(Int_t fillNo, TString fileNamePrefix)
{
    /*
      We need to bin in: energy (5), number of photons (6), phi (16), spin (2), pt(6).
      Let's create TH2D histograms of array size [2(spin)][4(energy)][#photon(5)]. The 2D histogram to be filled with phi bins along x and pt bins along y.
      We need another similar array for yellow beam as well.
    */
    TString outName = (TString)"BinnedHist_" + to_string(fillNo) + (TString)".root";
    TFile *file = new TFile(outName, "recreate");
    const Int_t kSpinBins = 2;
    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;
    const Int_t kPhiBins = 16;
    TH2D *bHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    TH2D *yHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    Double_t ptBins[] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0};
    Double_t engBins[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0}; //For info only
    Int_t nPtBins = sizeof(ptBins) / sizeof(Double_t) - 1;
    
    for(Int_t i = 0; i < kSpinBins; ++i)
    {
	for(Int_t j = 0; j < kEnergyBins; ++j)
	{
	    for(Int_t k = 0; k < kPhotonBins; ++k)
	    {
		TString bTitle = Form("bHist_%i_%i_%i", i, j, k);
		TString yTitle = Form("yHist_%i_%i_%i", i, j, k);
		bHist[i][j][k] = new TH2D(bTitle, bTitle, kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), nPtBins, ptBins);
		yHist[i][j][k] = new TH2D(yTitle, yTitle, kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), nPtBins, ptBins);
	    }
	}
    }

    Int_t bSpin_i;
    Int_t ySpin_i;
    Int_t nPhotons_i;
    Int_t phi_i;
    Int_t eng_i;

    TH1D *h1bSpinI = new TH1D("h1bSpinI", "Blue Spin index", 10, -1, 4);
    TH1D *h1ySpinI = new TH1D("h1ySpinI", "Yellow Spin index", 10, -1, 4);
    TH1D *h1nPhotonsI = new TH1D("h1nPhotonsI", "Number of photons index", 11, -1, 10);    
    TH1D *h1EngI = new TH1D("h1EngI", "Energy bin index", 7, -1, 6);
    TH1D *h1PhiB = new TH1D("h1PhiB", "Phi [Blue beam]", kPhiBins, -1.0*TMath::Pi(), TMath::Pi());
    TH1D *h1PhiY = new TH1D("h1PhiY", "Phi [Yellow blue]", kPhiBins, -1.0*TMath::Pi(), TMath::Pi());

    Int_t nPhotons;
    Int_t spinB;
    Int_t spinY;
    Int_t phiBin;
    Double_t eng;
    Double_t pt;
    Double_t eta;
    Double_t phi;
    Double_t jetX;
    Double_t jetY;
    Double_t vtxZ;
    Double_t rt;

    Double_t phi_y;
    Double_t phi_b;

    TStFillNoDB fillDb;
    TStRunList runListDb;
    //vector <int> runList = fillDb.GetRunsWithFill(fillNo);
    TEntryList *runList = runListDb.GetRunList();
    
    Int_t runNumber;
    TString fileName;
    //for(Int_t r = 0; r < runList.size(); ++r)
    for(Int_t r = 0; r < runList->GetN(); ++r)
    {
	//runNumber = runList[r];
	runNumber = runList->GetEntry(r);
	fileName = fileNamePrefix + to_string(runNumber) + ".root";
	if(gSystem->AccessPathName(fileName))
	{
	    cout << "File Not found:" << fileName <<endl;
	    cout << "SKIPPED" <<endl;
	    continue;
	}
	TFile *tFile = new TFile(fileName);
	TTree *tree = (TTree*)tFile->Get("T");
	if(!tree)
	{
	    cout << "No Tree found in"<< fileName <<" ... SKIPPED"<<endl;
	    tFile->Close();
	    delete tFile;	    
	    continue;
	}
	
	tree->SetBranchAddress("eng", &eng);
	tree->SetBranchAddress("pt", &pt);
	tree->SetBranchAddress("eta", &eta);
	tree->SetBranchAddress("phi", &phi);
	tree->SetBranchAddress("vtxZ", &vtxZ);
	tree->SetBranchAddress("jetX", &jetX);
	tree->SetBranchAddress("jetY", &jetY);
	tree->SetBranchAddress("nPhotons", &nPhotons);
	tree->SetBranchAddress("spinB", &spinB);
	tree->SetBranchAddress("spinY", &spinY);
	tree->SetBranchAddress("rt", &rt);
    
	Int_t nEntries = tree->GetEntries();
	cout << "Processing run number: "<< runNumber <<endl;
	cout << "Total events to be processed: "<< nEntries <<endl;

	for(Int_t evt = 0; evt < nEntries; ++evt)
	{
	    if(evt%5000 == 0)
		cout << "Events processed: "<< evt <<endl;
	
	    tree->GetEntry(evt);
	
	    // if(rt != 1)
	    //     continue;

	    if(fabs(vtxZ) > 80)
		continue;

	    //Exclude hot spot here
	
	    if(spinB == -1)
		bSpin_i = 0;
	    else if(spinB == 1)
		bSpin_i = 1;
	    else
		continue;
	
	    if(spinY == -1)
		ySpin_i = 0;
	    else if(spinY == 1)
		ySpin_i = 1;
	    else
		continue;

	    if(nPhotons > 0 && nPhotons < kPhotonBins)
		nPhotons_i = nPhotons - 1;
	    else if(nPhotons >= kPhotonBins)
		nPhotons_i = kPhotonBins - 1;
	    else
		continue;

	    eng_i = (Int_t)(eng / 20.0); 

	    if(eng_i >= kEnergyBins)
		continue;

	    if(phi >= 0)
	    {
	    	phi_b = phi;
	    	phi_y = TMath::Pi() - phi;
	    }
	    else
	    {
	    	phi_b = phi;
	    	phi_y = -1.0*TMath::Pi() - phi;		
	    }
	    
	    bHist[bSpin_i][eng_i][nPhotons_i]->Fill(phi_b, pt);
	    yHist[ySpin_i][eng_i][nPhotons_i]->Fill(phi_y, pt);

	    h1bSpinI->Fill(bSpin_i);
	    h1ySpinI->Fill(ySpin_i);
	    h1nPhotonsI->Fill(nPhotons_i);
	    h1EngI->Fill(eng_i);
	    h1PhiB->Fill(phi_b);
	    h1PhiY->Fill(phi_y);
	}
	tFile->Close();
	delete tFile;
    }
    file->Write();    
}
