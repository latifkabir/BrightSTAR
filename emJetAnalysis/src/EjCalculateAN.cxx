// Filename: EjCalculateAN.cxx
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

void EjCalculateAN(TString inFileName, TString outName, TString det)
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

    TH2D *bHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    TH2D *yHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    Double_t ptBins[] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0}; //For info only
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

    const Int_t nHalfPhiBins = bHist[0][0][0]->GetNbinsX() / 2; //Phi bins per hemisphere
    const Int_t nPtBins = bHist[0][0][0]->GetNbinsY();

    TH1D *h1FoldedRes[3][9]; // max three energy bins and 9 pt bins   
    for(Int_t i = 0; i < 9; ++i)
    {
	TString hName1 = Form("emJetPtBin1-%i", i);
	TString hName2 = Form("emJetPtBin2-%i", i);
	TString hName3 = Form("emJetPtBin3-%i", i);

	h1FoldedRes[0][i] = new TH1D(hName1, hName1, kPhotonBins, 0,  kPhotonBins);       
	h1FoldedRes[1][i] = new TH1D(hName2, hName2, kPhotonBins, 0,  kPhotonBins);
	h1FoldedRes[2][i] = new TH1D(hName3, hName3, kPhotonBins, 0,  kPhotonBins);
    }
    
    //Note: left-right is with respect to the beam (here blue beam)
    Int_t phiBins_left[] = {9, 10, 11, 12, 13, 14, 15, 16}; //<----------- Update here if nPhiBins changes
    Int_t phiBins_right[] = {1, 2, 3, 4, 5, 6, 7, 8}; //<----------- Update here if nPhiBins changes
    
    Double_t phiValues[nHalfPhiBins];
    
    Double_t bAnRaw[kEnergyBins][kPhotonBins][nHalfPhiBins][nPtBins];
    Double_t bAnRawError[kEnergyBins][kPhotonBins][nHalfPhiBins][nPtBins];
    Double_t bAn[kEnergyBins][kPhotonBins][nPtBins];
    Double_t bAnError[kEnergyBins][kPhotonBins][nPtBins];
    memset(bAn, 0, sizeof(bAn));
    memset(bAnError, 0, sizeof(bAnError));
    
    Double_t yAnRaw[kEnergyBins][kPhotonBins][nHalfPhiBins][nPtBins];
    Double_t yAnRawError[kEnergyBins][kPhotonBins][nHalfPhiBins][nPtBins];
    Double_t yAn[kEnergyBins][kPhotonBins][nPtBins];      // A_N with polarization correction
    Double_t yAnError[kEnergyBins][kPhotonBins][nPtBins]; // A_N error with polarization correction
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
    
    for(Int_t i = 0; i < kEnergyBins; ++i)
    {
	for(Int_t j = 0; j < kPhotonBins; ++j)
	{
	    for(Int_t k = 0; k < nHalfPhiBins; ++k)
	    {
		for(Int_t l = 0; l < nPtBins; ++l)
		{
		    if(i == 0 && j == 0 && l == 0)
			phiValues[k] = bHist[0][i][j]->GetXaxis()->GetBinCenter(phiBins_left[k]);

		    bNd_l = bHist[0][i][j]->GetBinContent(phiBins_left[k], l + 1);
		    bNu_l = bHist[1][i][j]->GetBinContent(phiBins_left[k], l + 1);
		    
		    bNd_r = bHist[0][i][j]->GetBinContent((phiBins_right[k]), l + 1);
		    bNu_r = bHist[1][i][j]->GetBinContent((phiBins_right[k]), l + 1);
		    
		    yNd_l = yHist[0][i][j]->GetBinContent(phiBins_left[k], l + 1);
		    yNu_l = yHist[1][i][j]->GetBinContent(phiBins_left[k], l + 1);
		    
		    yNd_r = yHist[0][i][j]->GetBinContent((phiBins_right[k]), l + 1);
		    yNu_r = yHist[1][i][j]->GetBinContent((phiBins_right[k]), l + 1);
      		    
		    //You need to ensure that the Left-Right pairing is done exactly as in the formula
		    //----- Blue beam measured asymmetry ------------
		    numer = sqrt(bNu_l*bNd_r) - sqrt(bNd_l*bNu_r);
		    denom = sqrt(bNu_l*bNd_r) + sqrt(bNd_l*bNu_r);

		    numerErrSq = bNu_l * bNd_r * (bNd_l + bNu_r) + bNd_l * bNu_r * (bNu_l + bNd_r);
		    denomErrSq = pow(denom, 4);
		    
		    if(denom == 0)
		    {
			bAnRaw[i][j][k][l] = -999;
			bAnRawError[i][j][k][l] = -999;
		    }
		    else
		    {
			bAnRaw[i][j][k][l] = numer / denom;
			bAnRawError[i][j][k][l] = sqrt(numerErrSq / denomErrSq); //See error propagation in the analysis note
		    }

		    //cout << "Raw Asym:"<< bAnRaw[i][j][k][l] << " +- "<< bAnRawError[i][j][k][l]  <<endl;
		    
		    //----- Yellow beam measured asymmetry ------------
		    numer = sqrt(yNu_l*yNd_r) - sqrt(yNd_l*yNu_r);
		    denom = sqrt(yNu_l*yNd_r) + sqrt(yNd_l*yNu_r);

		    numerErrSq = yNu_l * yNd_r * (yNd_l + yNu_r) + yNd_l * yNu_r * (yNu_l + yNd_r);
		    denomErrSq =  pow(denom, 4);
		    
		    if(denom == 0)
		    {
			yAnRaw[i][j][k][l] = -999;
			yAnRawError[i][j][k][l] = -999;
		    }
		    else
		    {
			yAnRaw[i][j][k][l] = numer / denom;
			yAnRawError[i][j][k][l] = sqrt(numerErrSq / denomErrSq); //See error propagation in the analysis note
		    }
		}
	    }
	}
    }

    gROOT->SetBatch(kTRUE);
    TFile *outFile = new TFile(outName, "recreate");
    TGraphErrors *bGr[kEnergyBins][kPhotonBins][nPtBins];
    TGraphErrors *yGr[kEnergyBins][kPhotonBins][nPtBins];
    TF1 *bFitFnc[kEnergyBins][kPhotonBins][nPtBins];
    TF1 *yFitFnc[kEnergyBins][kPhotonBins][nPtBins];
    TGraphErrors *bGrPhy[kEnergyBins][kPhotonBins];
    TGraphErrors *yGrPhy[kEnergyBins][kPhotonBins];
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
    
    for(Int_t i = 0; i < kEnergyBins; ++i)
    {
	for(Int_t j = 0; j < kPhotonBins; ++j)
	{
	    bGrPhy[i][j] = new TGraphErrors();
	    yGrPhy[i][j] = new TGraphErrors();
	    bGrPhy[i][j]->SetName(Form("bEbin%i_PhotonBin%i", i, j));
	    bGrPhy[i][j]->SetTitle(Form("%.1f GeV < E < %.1f GeV, No. of Photons %i; P_{T} [GeV/c]; A_{N}", engBins[i], engBins[i + 1], j + 1));
	    yGrPhy[i][j]->SetName(Form("yEbin%i_PhotonBin%i", i, j));
	    yGrPhy[i][j]->SetTitle(Form(" %.1f GeV < E < %.1f GeV, No. of Photons %i; P_{T} [GeV/c]; A_{N}", engBins[i], engBins[i + 1], j + 1));
	    bGrPhy[i][j]->SetMarkerColor(kBlack);
	    bGrPhy[i][j]->SetLineColor(kBlack);
	    bGrPhy[i][j]->SetMarkerStyle(kFullCircle);
	    
	    bGrPhy[i][j]->SetMaximum(0.1);
	    bGrPhy[i][j]->SetMinimum(-0.1);

	    yGrPhy[i][j]->SetMaximum(0.1);
	    yGrPhy[i][j]->SetMinimum(-0.1);
	    
	    yGrPhy[i][j]->SetMarkerColor(kRed);
	    yGrPhy[i][j]->SetLineColor(kRed);
	    yGrPhy[i][j]->SetMarkerStyle(kOpenCircle);
	    	    
	    nPointsPhyB = 0;
	    nPointsPhyY = 0;
	    
	    for(Int_t k = 0; k < nPtBins; ++k)
	    {
		bGr[i][j][k] = new TGraphErrors();
		yGr[i][j][k] = new TGraphErrors();
		bGr[i][j][k]->SetName(Form("bEbin%i_PhotonBin%i_PtBin%i", i, j, k));
		bGr[i][j][k]->SetTitle(Form("Blue Beam, %.1f GeV < E < %.1f GeV,  No. of Photons %i, %.1f GeV/c < Pt < %.1f GeV/c; #phi [rad]; A_{raw}", engBins[i], engBins[i + 1] , j + 1, ptBins[k], ptBins[k + 1]));
		yGr[i][j][k]->SetName(Form("yEbin%i_PhotonBin%i_PtBin%i", i, j, k));
		yGr[i][j][k]->SetTitle(Form("Yellow Beam, %.1f GeV < E < %.1f GeV,  No. of Photons %i, %.1f GeV/c < Pt < %.1f GeV/c; #phi [rad]; A_{raw}", engBins[i], engBins[i + 1] , j + 1, ptBins[k], ptBins[k + 1]));

		bGr[i][j][k]->SetMaximum(0.1);
		bGr[i][j][k]->SetMinimum(-0.1);
		yGr[i][j][k]->SetMaximum(0.1);
		yGr[i][j][k]->SetMinimum(-0.1);
		
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
		    bAn[i][j][k] = bFitFnc[i][j][k]->GetParameter(0) / polB;
		    bAnError[i][j][k] = bFitFnc[i][j][k]->GetParError(0) / polB;

		    bGrPhy[i][j]->SetPoint(nPointsPhyB, (ptBins[k] + ptBins[k+1])*0.5 , bAn[i][j][k]);
		    bGrPhy[i][j]->SetPointError(nPointsPhyB, 0, bAnError[i][j][k]);
		    ++nPointsPhyB;

		    h1FoldedRes[i][k]->SetBinContent(j + 1, bAn[i][j][k]);
		}

		if(yGr[i][j][k]->GetN() >= 0.5*nHalfPhiBins)
		{
		    yAn[i][j][k] = yFitFnc[i][j][k]->GetParameter(0) / polY;
		    yAnError[i][j][k] = yFitFnc[i][j][k]->GetParError(0) / polY;

		    yGrPhy[i][j]->SetPoint(nPointsPhyY, (ptBins[k] + ptBins[k+1])*0.5 , yAn[i][j][k]);
		    yGrPhy[i][j]->SetPointError(nPointsPhyY, 0, yAnError[i][j][k]);
		    ++nPointsPhyY;		    
		}
		bGr[i][j][k]->Write();
		yGr[i][j][k]->Write();	
	    }
	    bGrPhy[i][j]->Write();
	    yGrPhy[i][j]->Write();
	}
    }

    //------------------ Plot physics A_N --------------------
    Int_t canvasCount = 1;

    //------------- For FMS --------------------
    if(det == "fms")
    {
	TCanvas *c1 = new TCanvas("EMjet_A_N_fms", "EM Jet A_{N}");
	c1->Divide(kEnergyBins -1, kPhotonBins -1);
	for(Int_t i = 0; i < kPhotonBins - 1; ++i)
	{
	    for(Int_t j = 1; j < kEnergyBins; ++j)
	    {
		c1->cd(canvasCount);
		bGrPhy[j][i]->Draw("AP");
		yGrPhy[j][i]->Draw("P");
		TLine* L1Temp = new TLine(1.5, 0, 9.5, 0);
		L1Temp->Draw("same");
		++canvasCount;
	    }
	}
	c1->Write();

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
		//asymPlot->GetPlot(j,i)->SetYRange( -0.05, 0.05);
		asymPlot->GetPlot(j,i)->SetYRange( -0.1, 0.1);

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
    } 
    //---------- For EEMC Jet -----------------------
    if(det == "eemc")
    {
	TCanvas *c3 = new TCanvas("EMjet_A_N_eemc", "EM Jet A_{N}");
	canvasCount = 1;
	c3->Divide(5, 2);
	for(Int_t i = 0; i < 2; ++i)
	{
	    for(Int_t j = 0; j < 5; ++j)
	    {
		c3->cd(canvasCount);
		bGrPhy[i][j]->Draw("AP");
		yGrPhy[i][j]->Draw("P");
		TLine* L1Temp = new TLine(2.5, 0, 9.5, 0);
		L1Temp->Draw("same");
		++canvasCount;
	    }
	}    
	c3->Write();
    
	//--- For Eemc ---
	//only consider energy ranges 0 - 20, i.e. bin index 0, 1  and nPhotons = 1 - 5
	TCanvas* c4 = new TCanvas("asym_eemc","Asymmetries", 1000, 600);
	float varMins_e[] = { 1.8, 4.5, 1.8, 1.8, 1.8};
	float varMaxs_e[] = { 9.2, 9.2, 9.2, 9.2, 9.2};
	const char* xTitles_e[] = { "p_{T} [GeV/c]","p_{T} [GeV/c]"};
	const char* yTitles_e[] = { "A_{N}", "A_{N}", "A_{N}", "A_{N}", "A_{N}" };

	PanelPlot* asymPlot_e = new PanelPlot(c4, 1, 5, 2, "asym_eemc", xTitles_e, yTitles_e);

	for(int i = 0; i < 5; i++)
	{
	    for(int j = 0; j < 1; j++)
	    {
		asymPlot_e->GetPlot(j,i)->SetXRange( varMins_e[j], varMaxs_e[j]);
		//asymPlot_e->GetPlot(j,i)->SetYRange( -0.005, 0.005);
		asymPlot_e->GetPlot(j,i)->SetYRange( -0.03, 0.03);

		// if(i == 4 && j == 0)
		// {
		//     asymPlot_e->GetPlot(j,i)->Add(bGrPhy[j][4 - i], Plot::Point | Plot::Erry,  0, "x_{F} > 0");
		//     asymPlot_e->GetPlot(j,i)->Add(yGrPhy[j][4 - i], Plot::Point | Plot::Erry,  8, "x_{F} < 0");
		// }
		// else
		{
		    asymPlot_e->GetPlot(j,i)->Add(bGrPhy[j][4 - i], Plot::Point | Plot::Erry, 0);
		    asymPlot_e->GetPlot(j,i)->Add(yGrPhy[j][4 - i], Plot::Point | Plot::Erry, 8);
		}
		if(i == 0 && j == 0)
		    asymPlot_e->GetPlot(j,i)->AddText(2.5, -0.004, "Preliminary", 0.10);       
	    }
	}
	asymPlot_e->Draw();

	c4->Write();    
    }

    /*
      Plot the saved fitted graphs as:
      gStyle->SetOptFit(1)
      bEbin1_PhotonBin0_PtBin0->Draw("AP*")
    */
    
    //outFile->Write();
}
