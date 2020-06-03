// Filename: EjCalculateAN.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri May  8 15:08:25 2020 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include "RootInclude.h"
#include "cppInclude.h"
#include "BrightStInclude.h"
using namespace std;

void EjCalculateAN(Int_t fillNo, TString fileNamePrefix)
{
    /*
      We need to bin in: energy (5), number of photons (6), phi (16), spin (2), pt(6).
      Let's create TH2D histograms of array size [2(spin)][4(energy)][#photon(5)]. The 2D histogram to be filled with phi bins along x and pt bins along y.
      We need another similar array for yellow beam as well.
    */
    TString inFile = (TString)"BinnedHist_" + to_string(fillNo) + (TString)".root";
    if(gSystem->AccessPathName(inFile))
    {
	cout << "Input file not found: "<< inFile <<endl;
	return;
    }
    
    TFile *file = new TFile(inFile);
    const Int_t kSpinBins = 2;
    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;

    TH2D *bHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    TH2D *yHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    Double_t ptBins[] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0}; //For info only
    Double_t engBins[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0}; //For info only
    //Int_t nPtBins = sizeof(ptBins) / sizeof(Double_t) - 1;
    
    for(Int_t i = 0; i < kSpinBins; ++i)
    {
	for(Int_t j = 0; j < kEnergyBins; ++j)
	{
	    for(Int_t k = 0; k < kPhotonBins; ++k)
	    {
		TString bTitle = Form("bHist_%i_%i_%i", i, j, k);
		TString yTitle = Form("yHist_%i_%i_%i", i, j, k);
		bHist[i][j][k] = (TH2D*)file->Get(bTitle);
		yHist[i][j][k] = (TH2D*)file->Get(yTitle);
	    }
	}
    }

    const Int_t nHalfPhiBins = bHist[0][0][0]->GetNbinsX() / 2; //Phi bins per hemisphere
    const Int_t nPtBins = bHist[0][0][0]->GetNbinsY();

    //Note: left-right is with respect to the beam (here blue beam)

    Int_t phiBins_right[] = {9, 10, 11, 12, 13, 14, 15, 16}; //<----------- Update here if nPhiBins changes
    Int_t phiBins_left[] = {1, 2, 3, 4, 5, 6, 7, 8}; //<----------- Update here if nPhiBins changes
    
    // Int_t phiBins_right[] = {13, 14, 15, 16, 1, 2, 3, 4}; //<----------- Update here if nPhiBins changes
    // Int_t phiBins_left[] = {5, 6, 7, 8, 9, 10, 11, 12}; //<----------- Update here if nPhiBins changes

    Double_t phiValues[nHalfPhiBins];
    
    Double_t bAnRaw[kEnergyBins][kPhotonBins][nHalfPhiBins][nPtBins];
    Double_t bAnRawError[kEnergyBins][kPhotonBins][nHalfPhiBins][nPtBins];
    Double_t bAn[kEnergyBins][kPhotonBins][nPtBins];
    Double_t bAnError[kEnergyBins][kPhotonBins][nPtBins];
    memset(bAn, 0, sizeof(yAn));
    memset(bAnError, 0, sizeof(yAn));
    
    Double_t yAnRaw[kEnergyBins][kPhotonBins][nHalfPhiBins][nPtBins];
    Double_t yAnRawError[kEnergyBins][kPhotonBins][nHalfPhiBins][nPtBins];
    Double_t yAn[kEnergyBins][kPhotonBins][nPtBins];
    Double_t yAnError[kEnergyBins][kPhotonBins][nPtBins];
    memset(yAn, 0, sizeof(yAn));
    memset(yAnError, 0, sizeof(yAn));
    
    Double_t bNu_l;
    Double_t bNu_r;
    Double_t bNd_l;
    Double_t bNd_r;

    Double_t yNu_l;
    Double_t yNu_r;
    Double_t yNd_l;
    Double_t yNd_r;

    Double_t numer;
    Double_t denom;
    Double_t numerErrSq;
    Double_t denomErrSq;
    
    for(Int_t i = 0; i < kEnergyBins; ++i)
    {
	for(Int_t j = 0; j < kPhotonBins; ++j)
	{
	    for(Int_t k = 0; k < nHalfPhiBins; ++k)
	    {
		for(Int_t l = 0; l < nPtBins; ++l)
		{
		    if(i == 0 && j == 0 && l == 0)
			phiValues[k] = bHist[0][i][j]->GetXaxis()->GetBinCenter(phiBins_right[k]);

		    bNd_r = bHist[0][i][j]->GetBinContent((phiBins_right[k]), l + 1);
		    bNu_r = bHist[1][i][j]->GetBinContent((phiBins_right[k]), l + 1);
		    
		    bNd_l = bHist[0][i][j]->GetBinContent(phiBins_left[k], l + 1);
		    bNu_l = bHist[1][i][j]->GetBinContent(phiBins_left[k], l + 1);
		    
		    yNd_r = yHist[0][i][j]->GetBinContent((phiBins_right[k]), l + 1);
		    yNu_r = yHist[1][i][j]->GetBinContent((phiBins_right[k]), l + 1);

		    yNd_l = yHist[0][i][j]->GetBinContent(phiBins_left[k], l + 1);
		    yNu_l = yHist[1][i][j]->GetBinContent(phiBins_left[k], l + 1);

      		    
		    //You need to ensure that the Left-Right pairing is done exactly as in the formula
		    //----- Blue beam measured asymmetry ------------
		    numer = sqrt(bNu_r*bNd_l) - sqrt(bNd_r*bNu_l);
		    denom = sqrt(bNu_r*bNd_l) + sqrt(bNd_r*bNu_l);

		    numerErrSq = (bNd_l + bNu_r + bNu_l + bNd_r) / 4.0;
		    denomErrSq = (bNd_l + bNu_r + bNu_l + bNd_r) / 4.0;
		    
		    if(denom == 0)
		    {
			bAnRaw[i][j][k][l] = -999;
			bAnRawError[i][j][k][l] = -999;
		    }
		    else
		    {
			bAnRaw[i][j][k][l] = numer / denom;
			bAnRawError[i][j][k][l] = fabs( bAnRaw[i][j][k][l]) * sqrt( numerErrSq / pow(numer, 2) + denomErrSq / pow(denom, 2)); //See error propagation in the analysis note
		    }

		    //cout << "Raw Asym:"<< bAnRaw[i][j][k][l] << " +- "<< bAnRawError[i][j][k][l]  <<endl;

		    
		    //----- Yellow beam measured asymmetry ------------
		    numer = sqrt(yNu_r*yNd_l) - sqrt(yNd_r*yNu_l);
		    denom = sqrt(yNu_r*yNd_l) + sqrt(yNd_r*yNu_l);

		    numerErrSq = (yNd_l + yNu_r + yNu_l + yNd_r) / 4.0;
		    denomErrSq = (yNd_l + yNu_r + yNu_l + yNd_r) / 4.0;
		    
		    if(denom == 0)
		    {
			yAnRaw[i][j][k][l] = -999;
			yAnRawError[i][j][k][l] = -999;
		    }
		    else
		    {
			yAnRaw[i][j][k][l] = numer / denom;
			yAnRawError[i][j][k][l] = fabs(yAnRaw[i][j][k][l]) * sqrt( numerErrSq / pow(numer, 2) + denomErrSq / pow(denom, 2)); //See error propagation in the analysis note
		    }
		}
	    }
	}
    }

    gROOT->SetBatch(kTRUE);
    TFile *outFile = new TFile(Form("RawAN_%i.root", fillNo), "recreate");
    TGraphErrors *bGr[kEnergyBins][kPhotonBins][nPtBins];
    TGraphErrors *yGr[kEnergyBins][kPhotonBins][nPtBins];
    TF1 *bFitFnc[kEnergyBins][kPhotonBins][nPtBins];
    TF1 *yFitFnc[kEnergyBins][kPhotonBins][nPtBins];
    TGraphErrors *bGrPhy[kEnergyBins][kPhotonBins];
    TGraphErrors *yGrPhy[kEnergyBins][kPhotonBins];
    Int_t nPointsB;
    Int_t nPointsY;
    
    for(Int_t i = 0; i < kEnergyBins; ++i)
    {
	for(Int_t j = 0; j < kPhotonBins; ++j)
	{
	    bGr[i][j] = new TGraphErrors();
	    yGr[i][j] = new TGraphErrors();
	    bGr[i][j]->SetName(Form("bEbin%i_PhotonBin%i_PtBin%i", i, j, k));
	    bGr[i][j]->SetTitle(Form("bEbin%i_PhotonBin%i_PtBin%i", i, j, k));
	    yGr[i][j]->SetName(Form("yEbin%i_PhotonBin%i_PtBin%i", i, j, k));
	    yGr[i][j]->SetTitle(Form("yEbin%i_PhotonBin%i_PtBin%i", i, j, k));
	    
	    for(Int_t k = 0; k < nPtBins; ++k)
	    {
		bGr[i][j][k] = new TGraphErrors();
		yGr[i][j][k] = new TGraphErrors();
		bGr[i][j][k]->SetName(Form("bEbin%i_PhotonBin%i_PtBin%i", i, j, k));
		bGr[i][j][k]->SetTitle(Form("bEbin%i_PhotonBin%i_PtBin%i", i, j, k));
		yGr[i][j][k]->SetName(Form("yEbin%i_PhotonBin%i_PtBin%i", i, j, k));
		yGr[i][j][k]->SetTitle(Form("yEbin%i_PhotonBin%i_PtBin%i", i, j, k));

		bFitFnc[i][j][k] = new TF1(Form("bFitFnc_%i_%i_%i", i, j, k), "[0]*cos(x) + [1]");
		yFitFnc[i][j][k] = new TF1(Form("yFitFnc_%i_%i_%i", i, j, k), "[0]*cos(x) + [1]");

		nPointsB = 0;
		nPointsY = 0;
		for(Int_t l = 0; l < nHalfPhiBins; ++l)
		{
		    if(bAnRaw[i][j][l][k] != -999)
		    {
			bGr[i][j][k]->SetPoint(nPointsB, phiValues[l], bAnRaw[i][j][l][k]);
			bGr[i][j][k]->SetPointError(nPointsB, 0, bAnRawError[i][j][l][k]);
			++nPointsB;
		    }

		    if(yAnRaw[i][j][l][k] != -999)
		    {
			yGr[i][j][k]->SetPoint(nPointsY, phiValues[l], yAnRaw[i][j][l][k]);
			yGr[i][j][k]->SetPointError(nPointsY, 0, yAnRawError[i][j][l][k]);
			++nPointsY;
		    }
		}
		
		bGr[i][j][k]->Fit(Form("bFitFnc_%i_%i_%i", i, j, k));
		yGr[i][j][k]->Fit(Form("yFitFnc_%i_%i_%i", i, j, k));

		if(bGr[i][j][k]->GetN() >= 0.5*nHalfPhiBins)
		{
		    bAn[i][j][k] = bFitFnc[i][j][k]->GetParameter(0);
		    bAnError[i][j][k] = bFitFnc[i][j][k]->GetParError(0);
		}

		if(bGr[i][j][k]->GetN() >= 0.5*nHalfPhiBins)
		{
		    yAn[i][j][k] = yFitFnc[i][j][k]->GetParameter(0);
		    yAnError[i][j][k] = yFitFnc[i][j][k]->GetParError(0);
		}
		bGr[i][j][k]->Write();
		yGr[i][j][k]->Write();

		// bCanvas[i][j][k]->Write();
		// yCanvas[i][j][k]->Write();		
	    }
	}
    }

    //------------------ Plot physics AN --------------------

    
    /*
     Plot the saved fitted graphs as:
     gStyle->SetOptFit(1)
     bEbin1_PhotonBin0_PtBin0->Draw("AP*")
    */
    
    //outFile->Write();
}
