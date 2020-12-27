// Filename: AnCalculateAN.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri May  8 15:08:25 2020 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include "RootInclude.h"
#include "cppInclude.h"
#include "BrightStInclude.h"
#include "Hists.h"
using namespace std;

void AnCalculateAN(TString inFileName, TString outName, TString det)
{
    TString inFile = inFileName;
    if(gSystem->AccessPathName(inFile))
    {
	cout << "Input file not found: "<< inFile <<endl;
	return;
    }
    
    TFile *file = new TFile(inFile);

    TH2D *bXfVsPhiUp = (TH2D*)file->Get("bXfVsPhiUp");
    TH2D *bXfVsPhiDn = (TH2D*)file->Get("bXfVsPhiDn");

    TH2D *yXfVsPhiUp = (TH2D*)file->Get("yXfVsPhiUp");
    TH2D *yXfVsPhiDn = (TH2D*)file->Get("yXfVsPhiDn");
    
    if(!bXfVsPhiUp || !bXfVsPhiDn || !yXfVsPhiUp || !yXfVsPhiDn)
    {
	cout << "Unable to retrieve histogram" <<endl;
	return;
    }
    
    const Int_t nHalfPhiBins = bXfVsPhiUp->GetNbinsX() / 2; //Phi bins per hemisphere
    const Int_t nXfBins = bXfVsPhiUp->GetNbinsY();
    Double_t xfBins[] = {0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7}; //<-------- Ensure this matches histogram in the file
    
    //Note: left-right is with respect to the beam (here blue beam)

    Int_t phiBins_left[] = {9, 10, 11, 12, 13, 14, 15, 16}; //<----------- Update here if nPhiBins changes
    Int_t phiBins_right[] = {1, 2, 3, 4, 5, 6, 7, 8}; //<----------- Update here if nPhiBins changes
    
    Double_t phiValues[nHalfPhiBins];
    
    Double_t bAnRaw[nHalfPhiBins][nXfBins];
    Double_t bAnRawError[nHalfPhiBins][nXfBins];
    Double_t bAn[nXfBins];
    Double_t bAnError[nXfBins];
    memset(bAn, 0, sizeof(bAn));
    memset(bAnError, 0, sizeof(bAnError));
    
    Double_t yAnRaw[nHalfPhiBins][nXfBins];
    Double_t yAnRawError[nHalfPhiBins][nXfBins];
    Double_t yAn[nXfBins];      // A_N with polarization correction
    Double_t yAnError[nXfBins]; // A_N error with polarization correction
    memset(yAn, 0, sizeof(yAn));
    memset(yAnError, 0, sizeof(yAnError));
    
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
    
    for(Int_t k = 0; k < nHalfPhiBins; ++k)
    {
	for(Int_t l = 0; l < nXfBins; ++l)
	{
	    if(l == 0)
		phiValues[k] = bXfVsPhiUp->GetXaxis()->GetBinCenter(phiBins_left[k]);
	    
	    bNd_l = bXfVsPhiDn->GetBinContent(phiBins_left[k], l + 1);
	    bNu_l = bXfVsPhiUp->GetBinContent(phiBins_left[k], l + 1);
		    
	    bNd_r = bXfVsPhiDn->GetBinContent((phiBins_right[k]), l + 1);
	    bNu_r = bXfVsPhiUp->GetBinContent((phiBins_right[k]), l + 1);
		    
	    yNd_l = yXfVsPhiDn->GetBinContent(phiBins_left[k], l + 1);
	    yNu_l = yXfVsPhiUp->GetBinContent(phiBins_left[k], l + 1);
		    
	    yNd_r = yXfVsPhiDn->GetBinContent((phiBins_right[k]), l + 1);
	    yNu_r = yXfVsPhiUp->GetBinContent((phiBins_right[k]), l + 1);
      		    
	    //You need to ensure that the Left-Right pairing is done exactly as in the formula
	    //----- Blue beam measured asymmetry ------------
	    numer = sqrt(bNu_l*bNd_r) - sqrt(bNd_l*bNu_r);
	    denom = sqrt(bNu_l*bNd_r) + sqrt(bNd_l*bNu_r);

	    numerErrSq = (bNd_l + bNu_r + bNu_l + bNd_r) / 4.0;
	    denomErrSq = (bNd_l + bNu_r + bNu_l + bNd_r) / 4.0;
		    
	    if(denom == 0)
	    {
		bAnRaw[k][l] = -999;
		bAnRawError[k][l] = -999;
	    }
	    else
	    {
		bAnRaw[k][l] = numer / denom;
		bAnRawError[k][l] = fabs( bAnRaw[k][l]) * sqrt( numerErrSq / pow(numer, 2) + denomErrSq / pow(denom, 2)); //See error propagation in the analysis note
	    }

	    //cout << "Raw Asym:"<< bAnRaw[i][j][k][l] << " +- "<< bAnRawError[i][j][k][l]  <<endl;

		    
	    //----- Yellow beam measured asymmetry ------------
	    numer = sqrt(yNu_l*yNd_r) - sqrt(yNd_l*yNu_r);
	    denom = sqrt(yNu_l*yNd_r) + sqrt(yNd_l*yNu_r);

	    numerErrSq = (yNd_l + yNu_r + yNu_l + yNd_r) / 4.0;
	    denomErrSq = (yNd_l + yNu_r + yNu_l + yNd_r) / 4.0;
		    
	    if(denom == 0)
	    {
		yAnRaw[k][l] = -999;
		yAnRawError[k][l] = -999;
	    }
	    else
	    {
		yAnRaw[k][l] = numer / denom;
		yAnRawError[k][l] = fabs(yAnRaw[k][l]) * sqrt( numerErrSq / pow(numer, 2) + denomErrSq / pow(denom, 2)); //See error propagation in the analysis note
	    }
	}
    }

    gROOT->SetBatch(kTRUE);
    TFile *outFile = new TFile(outName, "recreate");
    TGraphErrors *bGr[nXfBins];
    TGraphErrors *yGr[nXfBins];
    TF1 *bFitFnc[nXfBins];
    TF1 *yFitFnc[nXfBins];
    TGraphErrors *bGrPhy;
    TGraphErrors *yGrPhy;
    Int_t nPointsB;
    Int_t nPointsY;
    Int_t nPointsPhyB;
    Int_t nPointsPhyY;
    Double_t polB = 0.570;   // RMS: 0.0371 in fraction
    Double_t polY = 0.5795;  // RMS: 0.0366 

    if(det == "fms")
    {
	polB = 0.570; 
	polY = 0.5795;  
    }
    else if(det == "eemc")
    {
	polB = 0.570; 
	polY = 0.5795;  
    }
    else
    {
	cout << "Invalid detector" <<endl;
	return;
    }
    
    bGrPhy = new TGraphErrors(); // b for blue beam
    yGrPhy = new TGraphErrors(); // y for yellow beam
    bGrPhy->SetName("AnDiffPionB");
    bGrPhy->SetTitle("Diff Pion A_{N} [Blue]; x_{F}; A_{N}");
    yGrPhy->SetName("AnDiffPionY");
    yGrPhy->SetTitle("Diff Pion A_{N} [Yellow]; x_{F}; A_{N}");
    bGrPhy->SetMarkerColor(kBlack);
    bGrPhy->SetLineColor(kBlack);
    bGrPhy->SetMarkerStyle(kFullCircle);
	    
    bGrPhy->SetMaximum(0.5);
    bGrPhy->SetMinimum(-0.5);

    yGrPhy->SetMaximum(0.5);
    yGrPhy->SetMinimum(-0.5);
	    
    yGrPhy->SetMarkerColor(kRed);
    yGrPhy->SetLineColor(kRed);
    yGrPhy->SetMarkerStyle(kOpenCircle);
	    	    
    nPointsPhyB = 0;
    nPointsPhyY = 0;
	    
    for(Int_t k = 0; k < nXfBins; ++k)
    {
	bGr[k] = new TGraphErrors();
	yGr[k] = new TGraphErrors();
	bGr[k]->SetName(Form("bAnDiffPion_XfBin%i", k));
	bGr[k]->SetTitle(Form("Diffractive Pion A_{N} [Blue], %.2f < x_{F} < %.2f ; #phi [rad]; A_{raw}", xfBins[k], xfBins[k + 1]));
	yGr[k]->SetName(Form("yAnDiffPion_XfBin%i", k));
	yGr[k]->SetTitle(Form("Diffractive Pion A_{N} [Yellow], %.2f < x_{F} < %.2f ; #phi [rad]; A_{raw}", xfBins[k], xfBins[k + 1]));

	bGr[k]->SetMaximum(0.3);
	bGr[k]->SetMinimum(-0.3);
	yGr[k]->SetMaximum(0.1);
	yGr[k]->SetMinimum(-0.1);
		
	bFitFnc[k] = new TF1(Form("bFitFnc_%i", k), "[0]*cos(x) + [1]");
	yFitFnc[k] = new TF1(Form("yFitFnc_%i", k), "[0]*cos(x) + [1]");

	nPointsB = 0;
	nPointsY = 0;
	for(Int_t l = 0; l < nHalfPhiBins; ++l)
	{
	    if(bAnRaw[l][k] != -999)
	    {
		bGr[k]->SetPoint(nPointsB, phiValues[l], bAnRaw[l][k]);
		bGr[k]->SetPointError(nPointsB, 0, bAnRawError[l][k]);
		++nPointsB;
	    }

	    if(yAnRaw[l][k] != -999)
	    {
		yGr[k]->SetPoint(nPointsY, phiValues[l], yAnRaw[l][k]);
		yGr[k]->SetPointError(nPointsY, 0, yAnRawError[l][k]);
		++nPointsY;
	    }
	}
		
	bGr[k]->Fit(Form("bFitFnc_%i",k));
	yGr[k]->Fit(Form("yFitFnc_%i",k));

	if(bGr[k]->GetN() >= 0.5*nHalfPhiBins)
	{
	    bAn[k] = bFitFnc[k]->GetParameter(0) / polB;
	    bAnError[k] = bFitFnc[k]->GetParError(0) / polB;

	    bGrPhy->SetPoint(nPointsPhyB, (xfBins[k] + xfBins[k+1])*0.5 , bAn[k]);
	    bGrPhy->SetPointError(nPointsPhyB, 0, bAnError[k]);
	    ++nPointsPhyB;		    
	}

	if(yGr[k]->GetN() >= 0.5*nHalfPhiBins)
	{
	    yAn[k] = yFitFnc[k]->GetParameter(0) / polB;
	    yAnError[k] = yFitFnc[k]->GetParError(0) / polB;

	    yGrPhy->SetPoint(nPointsPhyY, (xfBins[k] + xfBins[k+1])*0.5 , yAn[k]);
	    yGrPhy->SetPointError(nPointsPhyY, 0, yAnError[k]);
	    ++nPointsPhyY;		    
	}
	bGr[k]->Write();
	yGr[k]->Write();	
    }
    bGrPhy->Write();
    yGrPhy->Write();
    
    //------------------ Plot physics A_N --------------------

    TCanvas *c1 = new TCanvas("DiffPion_AN_blue", "Diffractive #pi^{0} A_{N}");
    bGrPhy->Draw("AP");
    
    TCanvas *c2 = new TCanvas("DiffPion_AN_yellow", "Diffractive #pi^{0} A_{N}", 600, 400);
    yGrPhy->Draw("AP");
    
    c1->Write();
    c2->Write();
    
}
