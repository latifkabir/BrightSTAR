// Filename: EjCalculateANextended.cxx
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

void EjCalculateANextended(TString inFileName, TString outName, TString det)
{
    /*
      We need to bin in: energy (5), number of photons (6), phi (16), spin (2), pt(6).
      Let's create TH2D histograms of array size [2(spin)][4(energy)][#photon(5)]. The 2D histogram to be filled with phi bins along x and pt bins along y.
      We need another similar array for yellow beam as well.
    */
    TString inFile = inFileName;
    if(gSystem->AccessPathName(inFile))
    {
	cout << "Input file not found: "<< inFile <<endl;
	return;
    }
    
    TFile *file = new TFile(inFile);
    const Int_t kSpinBins = 2;
    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;
    const Int_t kXfBins = 10;
    
    Double_t ptBins[] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0}; //For info only
    Double_t xfBins[] = {0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7};
    Double_t engBins[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0}; //For info only
    Double_t photonBins[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};// Last bin contains 6 or more photons
    
    //Int_t nPtBins = sizeof(ptBins) / sizeof(Double_t) - 1;
    
    TH2D *hist4p[kSpinBins]; //N pthotons or more, by default N = 4
    TH2D *hist1p[kSpinBins]; //Just 1 or 2 photons
    TH2D *hist3p[kSpinBins]; //Just 3 photons
    TH2D *histPtVsXf4p;
    TH2D *histPtVsXf1p;
    TH2D *histPtVsXf3p;
    for(Int_t i = 0; i < kSpinBins; ++i)
    {
	TString title = Form("bHistNp_%i", i); // Np is really 4p
	hist4p[i] = (TH2D*)file->Get(title);
	title = Form("bHist2p_%i", i); // 2p is actually 1 or 2-photon. Kept for backward compatibility 
	hist1p[i] = (TH2D*)file->Get(title);
	title = Form("bHist3p_%i", i);
	hist3p[i] = (TH2D*)file->Get(title);
    }
    histPtVsXf4p = (TH2D*)file->Get("PtVsXfNp"); // Np is really 4p
    histPtVsXf1p = (TH2D*)file->Get("PtVsXf2p");
    histPtVsXf3p = (TH2D*)file->Get("PtVsXf3p");
    
    const Int_t nHalfPhiBins = hist1p[0]->GetNbinsX() / 2; //Phi bins per hemisphere
    const Int_t nPtBins = hist1p[0]->GetNbinsY();

    //Note: left-right is with respect to the beam (here blue beam)

    Int_t phiBins_left[] = {9, 10, 11, 12, 13, 14, 15, 16}; //<----------- Update here if nPhiBins changes
    Int_t phiBins_right[] = {1, 2, 3, 4, 5, 6, 7, 8}; //<----------- Update here if nPhiBins changes
    
    Double_t phiValues[nHalfPhiBins];
    
    Double_t AnRaw_1p[nHalfPhiBins][kXfBins];
    Double_t AnRaw_1pError[nHalfPhiBins][kXfBins];
    Double_t An_1p[kXfBins];
    Double_t An_1pError[kXfBins];
    memset(An_1p, 0, sizeof(An_1p));
    memset(An_1pError, 0, sizeof(An_1pError));
    
    Double_t AnRaw_4p[nHalfPhiBins][kXfBins];
    Double_t AnRaw_4pError[nHalfPhiBins][kXfBins];
    Double_t An_4p[kXfBins];      // A_N with polarization correction
    Double_t An_4pError[kXfBins]; // A_N error with polarization correction
    memset(An_4p, 0, sizeof(An_4p));
    memset(An_4pError, 0, sizeof(An_4pError));

    Double_t AnRaw_3p[nHalfPhiBins][kXfBins];
    Double_t AnRaw_3pError[nHalfPhiBins][kXfBins];
    Double_t An_3p[kXfBins];      // A_N with polarization correction
    Double_t An_3pError[kXfBins]; // A_N error with polarization correction
    memset(An_3p, 0, sizeof(An_3p));
    memset(An_3pError, 0, sizeof(An_3pError));
    
    Double_t p1Nu_l;  // for 1 or 2 photons case
    Double_t p1Nu_r;
    Double_t p1Nd_l;
    Double_t p1Nd_r;

    Double_t p4Nu_l;  // for at least 4 photons case 
    Double_t p4Nu_r;
    Double_t p4Nd_l;
    Double_t p4Nd_r;

    Double_t p3Nu_l;  // for 3 photons case 
    Double_t p3Nu_r;
    Double_t p3Nd_l;
    Double_t p3Nd_r;
    
    Double_t numer;
    Double_t denom;
    Double_t numerErrSq;
    Double_t denomErrSq;
    
    for(Int_t k = 0; k < nHalfPhiBins; ++k)
    {
	for(Int_t l = 0; l < kXfBins; ++l)
	{
	    if(l == 0)
		phiValues[k] = hist1p[0]->GetXaxis()->GetBinCenter(phiBins_left[k]);

	    p1Nd_l = hist1p[0]->GetBinContent(phiBins_left[k], l + 1);
	    p1Nu_l = hist1p[1]->GetBinContent(phiBins_left[k], l + 1);
		    
	    p1Nd_r = hist1p[0]->GetBinContent((phiBins_right[k]), l + 1);
	    p1Nu_r = hist1p[1]->GetBinContent((phiBins_right[k]), l + 1);
		    
	    p4Nd_l = hist4p[0]->GetBinContent(phiBins_left[k], l + 1);
	    p4Nu_l = hist4p[1]->GetBinContent(phiBins_left[k], l + 1);
		    
	    p4Nd_r = hist4p[0]->GetBinContent((phiBins_right[k]), l + 1);
	    p4Nu_r = hist4p[1]->GetBinContent((phiBins_right[k]), l + 1);

	    p3Nd_l = hist3p[0]->GetBinContent(phiBins_left[k], l + 1);
	    p3Nu_l = hist3p[1]->GetBinContent(phiBins_left[k], l + 1);
		    
	    p3Nd_r = hist3p[0]->GetBinContent((phiBins_right[k]), l + 1);
	    p3Nu_r = hist3p[1]->GetBinContent((phiBins_right[k]), l + 1);
      		    
	    //You need to ensure that the Left-Right pairing is done exactly as in the formula
	    //----- 1 0r 2-photon EM-jet measured asymmetry ------------
	    numer = sqrt(p1Nu_l*p1Nd_r) - sqrt(p1Nd_l*p1Nu_r);
	    denom = sqrt(p1Nu_l*p1Nd_r) + sqrt(p1Nd_l*p1Nu_r);

	    numerErrSq = p1Nu_l * p1Nd_r * (p1Nd_l + p1Nu_r) + p1Nd_l * p1Nu_r * (p1Nu_l + p1Nd_r);
	    denomErrSq = pow(denom, 4);
		    
	    if(denom == 0)
	    {
		AnRaw_1p[k][l] = -999;
		AnRaw_1pError[k][l] = -999;
	    }
	    else
	    {
		AnRaw_1p[k][l] = numer / denom;
		AnRaw_1pError[k][l] = sqrt(numerErrSq / denomErrSq); //See error propagation in the analysis note
	    }

	    //cout << "Raw Asym:"<< AnRaw_1p[i][j][k][l] << " +- "<< AnRaw_1pError[i][j][k][l]  <<endl;
	    
	    //----- 3-photon EM-jet measured asymmetry ------------
	    numer = sqrt(p3Nu_l*p3Nd_r) - sqrt(p3Nd_l*p3Nu_r);
	    denom = sqrt(p3Nu_l*p3Nd_r) + sqrt(p3Nd_l*p3Nu_r);

	    numerErrSq = p3Nu_l * p3Nd_r * (p3Nd_l + p3Nu_r) + p3Nd_l * p3Nu_r * (p3Nu_l + p3Nd_r);
	    denomErrSq = pow(denom, 4);
		    
	    if(denom == 0)
	    {
		AnRaw_3p[k][l] = -999;
		AnRaw_3pError[k][l] = -999;
	    }
	    else
	    {
		AnRaw_3p[k][l] = numer / denom;
		AnRaw_3pError[k][l] = sqrt(numerErrSq / denomErrSq); //See error propagation in the analysis note
	    }

		    
	    //----- 4-photon EM-jet measured asymmetry ------------
	    numer = sqrt(p4Nu_l*p4Nd_r) - sqrt(p4Nd_l*p4Nu_r);
	    denom = sqrt(p4Nu_l*p4Nd_r) + sqrt(p4Nd_l*p4Nu_r);

	    numerErrSq = p4Nu_l * p4Nd_r * (p4Nd_l + p4Nu_r) + p4Nd_l * p4Nu_r * (p4Nu_l + p4Nd_r);
	    denomErrSq =  pow(denom, 4);
		    
	    if(denom == 0)
	    {
		AnRaw_4p[k][l] = -999;
		AnRaw_4pError[k][l] = -999;
	    }
	    else
	    {
		AnRaw_4p[k][l] = numer / denom;
		AnRaw_4pError[k][l] = sqrt(numerErrSq / denomErrSq); //See error propagation in the analysis note
	    }
	}
    }

    gROOT->SetBatch(kTRUE);
    TFile *outFile = new TFile(outName, "recreate");
    TGraphErrors *p1Gr[kXfBins];
    TGraphErrors *p4Gr[kXfBins];
    TGraphErrors *p3Gr[kXfBins];
    TF1 *p1FitFnc[kXfBins];
    TF1 *p4FitFnc[kXfBins];
    TF1 *p3FitFnc[kXfBins];
    TGraphErrors *p1GrPhy;
    TGraphErrors *p4GrPhy;
    TGraphErrors *p3GrPhy;
    Int_t nPoints_p1;
    Int_t nPoints_p4;
    Int_t nPoints_p3;
    Int_t nPointsPhy_p1;
    Int_t nPointsPhy_p4;
    Int_t nPointsPhy_p3;
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
    
    p1GrPhy = new TGraphErrors(); //for 1 or 2 photons case
    p4GrPhy = new TGraphErrors(); //for 4 or more photons case
    p3GrPhy = new TGraphErrors(); //for 3-photon case
    p1GrPhy->SetName("An1photons");
    p1GrPhy->SetTitle("No. of Photons = 1 or 2; x_{F}; A_{N}");
    p4GrPhy->SetName("An4photons");
    p4GrPhy->SetTitle("No. of Photons >= 4; x_{F}; A_{N}");
    p3GrPhy->SetName("An3photons");
    p3GrPhy->SetTitle("No. of Photons = 3; x_{F}; A_{N}");
    p1GrPhy->SetMarkerColor(kBlack);
    p1GrPhy->SetLineColor(kBlack);
    p1GrPhy->SetMarkerStyle(kFullCircle);
	    
    p1GrPhy->SetMaximum(0.1);
    p1GrPhy->SetMinimum(-0.1);

    p4GrPhy->SetMaximum(0.1);
    p4GrPhy->SetMinimum(-0.1);

    p3GrPhy->SetMaximum(0.1);
    p3GrPhy->SetMinimum(-0.1);
	    
    p4GrPhy->SetMarkerColor(kRed);
    p4GrPhy->SetLineColor(kRed);
    p4GrPhy->SetMarkerStyle(kOpenCircle);

    p3GrPhy->SetMarkerColor(kBlue);
    p3GrPhy->SetLineColor(kBlue);
    p3GrPhy->SetMarkerStyle(kOpenCircle);
    
    nPointsPhy_p1 = 0;
    nPointsPhy_p4 = 0;
    nPointsPhy_p3 = 0;
	    
    for(Int_t k = 0; k < kXfBins; ++k)
    {
	p1Gr[k] = new TGraphErrors();
	p4Gr[k] = new TGraphErrors();
	p3Gr[k] = new TGraphErrors();
	
	p1Gr[k]->SetName(Form("An1photon_XfBin%i", k));
	p1Gr[k]->SetTitle(Form("No. of Photons = 1 or 2, %.1f < x_{F} < %.1f ; #phi [rad]; A_{raw}", xfBins[k], xfBins[k + 1]));
	p4Gr[k]->SetName(Form("An4photons_XfBin%i", k));
	p4Gr[k]->SetTitle(Form("No. of Photons >= 4, %.1f < x_{F} < %.1f ; #phi [rad]; A_{raw}", xfBins[k], xfBins[k + 1]));
	p3Gr[k]->SetName(Form("An3photons_XfBin%i", k));
	p3Gr[k]->SetTitle(Form("No. of Photons = 3, %.1f < x_{F} < %.1f ; #phi [rad]; A_{raw}", xfBins[k], xfBins[k + 1]));
	
	p1Gr[k]->SetMaximum(0.1);
	p1Gr[k]->SetMinimum(-0.1);
	p4Gr[k]->SetMaximum(0.01);
	p4Gr[k]->SetMinimum(-0.01);
	p3Gr[k]->SetMaximum(0.01);
	p3Gr[k]->SetMinimum(-0.01);
	
 	p1FitFnc[k] = new TF1(Form("p1FitFnc_%i", k), "[0]*cos(x) + [1]");
	p4FitFnc[k] = new TF1(Form("p4FitFnc_%i", k), "[0]*cos(x) + [1]");
	p3FitFnc[k] = new TF1(Form("p3FitFnc_%i", k), "[0]*cos(x) + [1]");

	nPoints_p1 = 0;
	nPoints_p4 = 0;
	nPoints_p3 = 0;
	for(Int_t l = 0; l < nHalfPhiBins; ++l)
	{
	    if(AnRaw_1p[l][k] != -999)
	    {
		p1Gr[k]->SetPoint(nPoints_p1, phiValues[l], AnRaw_1p[l][k]);
		p1Gr[k]->SetPointError(nPoints_p1, 0, AnRaw_1pError[l][k]);
		++nPoints_p1;
	    }

	    if(AnRaw_4p[l][k] != -999)
	    {
		p4Gr[k]->SetPoint(nPoints_p4, phiValues[l], AnRaw_4p[l][k]);
		p4Gr[k]->SetPointError(nPoints_p4, 0, AnRaw_4pError[l][k]);
		++nPoints_p4;
	    }

	    if(AnRaw_3p[l][k] != -999)
	    {
		p3Gr[k]->SetPoint(nPoints_p3, phiValues[l], AnRaw_3p[l][k]);
		p3Gr[k]->SetPointError(nPoints_p3, 0, AnRaw_3pError[l][k]);
		++nPoints_p3;
	    }
	}
		
	p1Gr[k]->Fit(Form("p1FitFnc_%i",k));
	p4Gr[k]->Fit(Form("p4FitFnc_%i",k));
	p3Gr[k]->Fit(Form("p3FitFnc_%i",k));

	if(p1Gr[k]->GetN() >= 0.5*nHalfPhiBins)
	{
	    An_1p[k] = p1FitFnc[k]->GetParameter(0) / polB;
	    An_1pError[k] = p1FitFnc[k]->GetParError(0) / polB;

	    p1GrPhy->SetPoint(nPointsPhy_p1, (xfBins[k] + xfBins[k+1])*0.5 , An_1p[k]);
	    p1GrPhy->SetPointError(nPointsPhy_p1, 0, An_1pError[k]);
	    ++nPointsPhy_p1;		    
	}

	if(p4Gr[k]->GetN() >= 0.5*nHalfPhiBins)
	{
	    An_4p[k] = p4FitFnc[k]->GetParameter(0) / polB;
	    An_4pError[k] = p4FitFnc[k]->GetParError(0) / polB;

	    p4GrPhy->SetPoint(nPointsPhy_p4, (xfBins[k] + xfBins[k+1])*0.5 , An_4p[k]);
	    p4GrPhy->SetPointError(nPointsPhy_p4, 0, An_4pError[k]);
	    ++nPointsPhy_p4;		    
	}

	if(p3Gr[k]->GetN() >= 0.5*nHalfPhiBins)
	{
	    An_3p[k] = p3FitFnc[k]->GetParameter(0) / polB;
	    An_3pError[k] = p3FitFnc[k]->GetParError(0) / polB;

	    p3GrPhy->SetPoint(nPointsPhy_p3, (xfBins[k] + xfBins[k+1])*0.5 , An_3p[k]);
	    p3GrPhy->SetPointError(nPointsPhy_p3, 0, An_3pError[k]);
	    ++nPointsPhy_p3;		    
	}
	
	p1Gr[k]->Write();
	p4Gr[k]->Write();	
	p3Gr[k]->Write();	
    }
    p1GrPhy->Write();
    p4GrPhy->Write();
    p3GrPhy->Write();
    
}
