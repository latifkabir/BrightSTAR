// Filename: EjCreateBinnedHist.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri May  8 15:08:25 2020 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include "RootInclude.h"
#include "cppInclude.h"

using namespace std;

void EjCreateBinnedHist(TString inFileName, TString outName)
{
    TChain *ch = new TChain("T");
    ch->Add(inFileName);

    TFile *file = new TFile(outName, "recreate");
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
        
    ch->SetBranchAddress("eng", &eng);
    ch->SetBranchAddress("pt", &pt);
    ch->SetBranchAddress("eta", &eta);
    ch->SetBranchAddress("phi", &phi);
    ch->SetBranchAddress("vtxZ", &vtxZ);
    ch->SetBranchAddress("jetX", &jetX);
    ch->SetBranchAddress("jetY", &jetY);
    ch->SetBranchAddress("nPhotons", &nPhotons);
    ch->SetBranchAddress("spinB", &spinB);
    ch->SetBranchAddress("spinY", &spinY);
    //ch->SetBranchAddress("rt", &rt);
    ch->SetBranchAddress("phiBin", &phiBin);

    /*
      We need to bin in: energy (5), number of photons (6), phi (16), spin (2), pt(6).
      Let's create TH2D histograms of array size [2(spin)][4(energy)][#photon(5)]. The 2D histogram to be filled with phi bins along x and pt bins along y.
      We need another similar array for yellow beam as well.
    */

    const Int_t kSpinBins = 2;
    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;
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
		bHist[i][j][k] = new TH2D(bTitle, bTitle, 16, 1, 17, nPtBins, ptBins);
		yHist[i][j][k] = new TH2D(yTitle, yTitle, 16, 1, 17, nPtBins, ptBins);
	    }
	}
    }

    Int_t bSpin_i;
    Int_t ySpin_i;
    Int_t nPhotons_i;
    Int_t phi_i;
    Int_t eng_i;
    
    Int_t nEntries = ch->GetEntries();
    cout << "Total events to be processed: "<< nEntries <<endl;

    for(Int_t evt = 0; evt < nEntries; ++evt)
    {

	if(evt%1000 == 0)
	    cout << "Events processed: "<< evt <<endl;
	
	ch->GetEntry(evt);
	
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
	    nPhotons_i = kPhotonBins -1;
	else
	    continue;

	eng_i = (Int_t)(eng / 20.0); 

	if(eng_i >= kEnergyBins)
	    continue;

	bHist[bSpin_i][eng_i][nPhotons_i]->Fill(phiBin, pt);
	yHist[ySpin_i][eng_i][nPhotons_i]->Fill(phiBin, pt);
	
    }

    file->Write();    
}
