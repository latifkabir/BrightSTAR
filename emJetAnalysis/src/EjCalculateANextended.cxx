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
    
    TH2D *bHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    TH2D *yHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    Double_t ptBins[] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0}; //For info only
    Double_t xfBins[] = {0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7};
    Double_t engBins[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0}; //For info only
    Double_t photonBins[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};// Last bin contains 6 or more photons
    
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
    TH2D *bHist3p[kSpinBins]; //3 pthotons or more
    TH2D *bHist2p[kSpinBins]; //Just 2 photons
    TH2D *bHistPtVsXf3p;
    TH2D *bHistPtVsXf2p;
    for(Int_t i = 0; i < kSpinBins; ++i)
    {
	TString title = Form("bHist3p_%i", i);
	bHist3p[i] = (TH2D*)file->Get(title);
	title = Form("bHist2p_%i", i);
	bHist2p[i] = (TH2D*)file->Get(title);
    }
    bHistPtVsXf3p = (TH2D*)file->Get("PtVsXf3p");
    bHistPtVsXf2p = (TH2D*)file->Get("PtVsXf2p");
    
    const Int_t nHalfPhiBins = bHist[0][0][0]->GetNbinsX() / 2; //Phi bins per hemisphere
    const Int_t nPtBins = bHist[0][0][0]->GetNbinsY();

    //Note: left-right is with respect to the beam (here blue beam)

    Int_t phiBins_left[] = {9, 10, 11, 12, 13, 14, 15, 16}; //<----------- Update here if nPhiBins changes
    Int_t phiBins_right[] = {1, 2, 3, 4, 5, 6, 7, 8}; //<----------- Update here if nPhiBins changes
    
    Double_t phiValues[nHalfPhiBins];
    
    Double_t bAnRaw[nHalfPhiBins][kXfBins];
    Double_t bAnRawError[nHalfPhiBins][kXfBins];
    Double_t bAn[kXfBins];
    Double_t bAnError[kXfBins];
    memset(bAn, 0, sizeof(bAn));
    memset(bAnError, 0, sizeof(bAnError));
    
    Double_t yAnRaw[nHalfPhiBins][kXfBins];
    Double_t yAnRawError[nHalfPhiBins][kXfBins];
    Double_t yAn[kXfBins];      // A_N with polarization correction
    Double_t yAnError[kXfBins]; // A_N error with polarization correction
    memset(yAn, 0, sizeof(yAn));
    memset(yAnError, 0, sizeof(yAnError));
    
    Double_t bNu_l;  //b prefix for 2 photons case here
    Double_t bNu_r;
    Double_t bNd_l;
    Double_t bNd_r;

    Double_t yNu_l;  //y prefix for 3 or more photons case here
    Double_t yNu_r;
    Double_t yNd_l;
    Double_t yNd_r;

    Double_t numer;
    Double_t denom;
    Double_t numerErrSq;
    Double_t denomErrSq;
    
    for(Int_t k = 0; k < nHalfPhiBins; ++k)
    {
	for(Int_t l = 0; l < kXfBins; ++l)
	{
	    if(l == 0)
		phiValues[k] = bHist2p[0]->GetXaxis()->GetBinCenter(phiBins_left[k]);

	    bNd_l = bHist2p[0]->GetBinContent(phiBins_left[k], l + 1);
	    bNu_l = bHist2p[1]->GetBinContent(phiBins_left[k], l + 1);
		    
	    bNd_r = bHist2p[0]->GetBinContent((phiBins_right[k]), l + 1);
	    bNu_r = bHist2p[1]->GetBinContent((phiBins_right[k]), l + 1);
		    
	    yNd_l = bHist3p[0]->GetBinContent(phiBins_left[k], l + 1);
	    yNu_l = bHist3p[1]->GetBinContent(phiBins_left[k], l + 1);
		    
	    yNd_r = bHist3p[0]->GetBinContent((phiBins_right[k]), l + 1);
	    yNu_r = bHist3p[1]->GetBinContent((phiBins_right[k]), l + 1);
      		    
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
    TGraphErrors *bGr[kXfBins];
    TGraphErrors *yGr[kXfBins];
    TF1 *bFitFnc[kXfBins];
    TF1 *yFitFnc[kXfBins];
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
    
    bGrPhy = new TGraphErrors(); //b for 2 photons case
    yGrPhy = new TGraphErrors(); // y for 3 or more photons case
    bGrPhy->SetName("An2Photons");
    bGrPhy->SetTitle("No. of Photons = 2; x_{F}; A_{N}");
    yGrPhy->SetName("An3OrMorePhotons");
    yGrPhy->SetTitle("No. of Photons >= 3; x_{F}; A_{N}");
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
	    
    for(Int_t k = 0; k < kXfBins; ++k)
    {
	bGr[k] = new TGraphErrors();
	yGr[k] = new TGraphErrors();
	bGr[k]->SetName(Form("An2_Photons_XfBin%i", k));
	bGr[k]->SetTitle(Form("No. of Photons = 2, %.1f < x_{F} < %.1f ; #phi [rad]; A_{raw}", xfBins[k], xfBins[k + 1]));
	yGr[k]->SetName(Form("An3nPhotons_XfBin%i", k));
	yGr[k]->SetTitle(Form("No. of Photons >= 3, %.1f < x_{F} < %.1f ; #phi [rad]; A_{raw}", xfBins[k], xfBins[k + 1]));

	bGr[k]->SetMaximum(0.1);
	bGr[k]->SetMinimum(-0.1);
	yGr[k]->SetMaximum(0.01);
	yGr[k]->SetMinimum(-0.01);
		
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
    Int_t canvasCount = 1;

    //------------- For FMS --------------------

    TCanvas *c1 = new TCanvas("EMjet_A_N_fms2", "EM Jet A_{N}");
    bGrPhy->Draw("AP");
    
    TCanvas *c2 = new TCanvas("EMjet_A_N_fms3", "EM Jet A_{N}", 600, 400);
    TMultiGraph *mg = new TMultiGraph("mg", "mg");
       
    //yGrPhy->Draw("AP");
    // TLine* L1Temp = new TLine(1.5, 0, 9.5, 0);
    // L1Temp->Draw("same");
    
    //Zhanwen's values for 3 or more photons (Data taken from HepData):
    Double_t x_z3[] = {0.2213, 0.2571, 0.3038, 0.3529, 0.4029, 0.4531, 0.5033, 0.5536, 0.6123}; 
    Double_t y_z3[] = {0.0008, 0.0023, 0.0032, 0.0051, 0.0056, 0.0078, 0.0084, 0.0125, 0.0135}; 
    Double_t y_z3err[] = {0.0013, 0.0005, 0.0005, 0.0007, 0.0009, 0.0011, 0.0014, 0.0017, 0.0018};

    //Zhanwen's values for all photons (Data taken from HepData):
    Double_t x_z[] = {0.22116, 0.25683, 0.30373, 0.35284, 0.40294, 0.45320, 0.50343, 0.55365, 0.61238}; 
    Double_t y_z[] = {0.0044, 0.0053, 0.0066, 0.0079, 0.0074, 0.0099, 0.0096, 0.0141, 0.0161}; 
    Double_t y_zerr[] = {0.0011, 0.0005, 0.0005, 0.0006, 0.0008, 0.0010, 0.0012, 0.0015, 0.0015}; 
    
    TGraphErrors *znGr3 = new TGraphErrors(9, x_z3, y_z3, 0, y_z3err);
    znGr3->SetName("znGr3");

    TGraphErrors *znGr = new TGraphErrors(9, x_z, y_z, 0, y_zerr);
    znGr->SetName("znGrAll");
    
    //znGr->Draw("sameAP");

 
    mg->Add(yGrPhy);
    mg->Add(znGr3);
    mg->Draw("P");

    znGr3->Write();
    znGr->Write();

    mg->Write();
    c1->Write();
    c2->Write();
    
    /*
    //This area is just for plotting final physics result ----------------
    //--- For Fms ---
    //only consider energy ranges 20 -40, 40 - 60, 60 - 80 i.e. bin index 1, 2, 3  and nPhotons = 1 - 5
    TCanvas* c2 = new TCanvas("asym_fms","Asymmetries",1000,600);
    float varMins[5] = { 1.8, 1.8, 1.8, 1.8, 1.8};
    float varMaxs[5] = { 8.2, 8.2, 8.2, 8.2, 8.2};
    const char* xTitles[3] = { "p_{T} [GeV/c]","p_{T} [GeV/c]","p_{T} [GeV/c]" };
    const char* yTitles[5] = { "A_{N}", "A_{N}", "A_{N}", "A_{N}", "A_{N}" };

    PanelPlot* asymPlot = new PanelPlot(c2, 3, 5, 2, "asym_fms", xTitles, yTitles);

    for (int i = 0; i < 5; i++)
    {
	for (int j = 0; j < 3; j++)
	{
	    asymPlot->GetPlot(j,i)->SetXRange( varMins[i], varMaxs[i]);
	    asymPlot->GetPlot(j,i)->SetYRange( -0.05, 0.05);

	    // if(i == 4 && j == 0) // legend causes shift in x axis base for the panel
	    // {
	    //     asymPlot->GetPlot(j,i)->Add(bGrPhy[j+1][4 - i], Plot::Point | Plot::Erry,  0, "x_{F} > 0");
	    //     asymPlot->GetPlot(j,i)->Add(yGrPhy[j+1][4 - i], Plot::Point | Plot::Erry,  8, "x_{F} < 0");
	    // }
	    // else
	    {
		asymPlot->GetPlot(j,i)->Add(bGrPhy[j+1][4 - i], Plot::Point | Plot::Erry, 0);
		asymPlot->GetPlot(j,i)->Add(yGrPhy[j+1][4 - i], Plot::Point | Plot::Erry, 8);
	    }
	    if(i == 0 && j == 0)
		asymPlot->GetPlot(j,i)->AddText(2.5, -0.04, "Preliminary", 0.10);       
	}
    }
    asymPlot->Draw();
    
    c2->Write();   
    */    
    
    /*
      Plot the saved fitted graphs as:
      gStyle->SetOptFit(1)
      bEbin1_PhotonBin0_PtBin0->Draw("AP*")
    */
    
    //outFile->Write();
}