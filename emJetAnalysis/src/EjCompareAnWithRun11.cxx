// Filename: EjRePlotAn.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Jul 24 13:31:34 2021 (-0400)
// URL: jlab.org/~latif

// Based on the example: $ROOTSYS/tutorials/graphics/canvas2.C

#include <iostream>
#include "RootInclude.h"
#include "cppInclude.h"
#include "BrightStInclude.h"
#include "EjCompareAnWithRun11.h"
#include "EjRePlotAn.h"

using namespace std;

void EjCompareAnWithRun11(TString fileName)
{
    //40 -60 GeV EM-jet Energy
    Double_t x11e1[8] = {2.3, 2.8, 3.175, 3.4, 3.825, 4.325, 5.025, 6.15};
    //[#photons][#pt]
    Double_t y11e1[5][8] = {{0.010, 0.011, 0.025, 0.006, 0.009, 0.015, 0.022, 0.012}, 
			 {0.011, 0.005, 0.011, 0.016, 0.015, 0.010, 0.012, 0.012},
			 {0.004, 0.004, -0.000, 0.004, 0.009, 0.007, 0.006, 0.006},
			 {-0.004, -0.003, -0.004, 0.003, 0.002, -0.007, 0.003, -0.000},
			 {-0.009, 0.004, -0.000, 0.001, 0.004, -0.003, -0.003, -0.008}};

    Double_t y11err1[5][8] = { {0, 0, 0.0046, 0, 0, 0.0046, 0.0052, 0.0088},
			       {0.0031, 0, 0, 0, 0, 0, 0, 0.0047},
			       {0, 0, 0, 0, 0, 0, 0, 0},
			       {0, 0, 0.0053, 0, 0, 0, 0, 0},
			       {0.0112, 0.0079, 0.0084, 0.0056, 0, 0, 0, 0}
	
                             };
    
    //60 - 80 GeV EM-jet Energy
    Double_t x11e2[9] = {2.426, 2.847, 3.128, 3.408, 3.724, 4.250, 4.951, 6.285, 8.07};
     //[#photons][#pt]   
    Double_t y11e2[5][9] = {{ 0.044, 0.009, 0.021, 0.02, 0.013, 0.021, 0.027, 0.02, -0.005},
			  {0.019, 0.016, 0.013, 0.013, 0.018, 0.025, 0.026, 0.022, 0.013},
			  {0.015, 0.015, 0.006, 0.015, 0.006, 0.009, 0.013, 0.001, 0.002},
			  {0.030, 0.008, 0.004, 0.006, -0.007, 0.001, 0.001, 0.002, -0.010},
			  {-1.0, 0.013, 0.008, 0.003, 0.001, -0.01, 0.001, -0.002, 0.016}};

    Double_t y11err2[5][9] = {{0.0126, 0.0063, 0.0068, 0.0057, 0.0042, 0.0068, 0.0079, 0.0089, 0.0189},
			      {0.00625, 0.00417, 0.00469, 0.00312, 0, 0, 0, 0, 0.0099},
			      {0.0083, 0.0052, 0, 0, 0, 0, 0, 0, 0.0078},	 
			      {0.016, 0.0068, 0.0068, 0.0047, 0.0026, 0, 0, 0, 0.0078},
			      {0, 0.0101, 0.0123, 0.0084, 0, 0.0062, 0, 0, 0.0079}			     	
                            };
    
    //80 - 100 GeV EM-jet Energy
    Double_t x11e3[7] = {3.090, 3.304, 3.724, 4.179, 4.808, 6.207, 8.690};
    //[#photons][#pt]   
    Double_t y11e3[5][7] = {{-1.0, 0.036, 0.016, 0.030, 0.024, 0.015, 0.002},
			  {0.004, 0.029, 0.026, 0.024, 0.031, 0.022, 0.005},
			  {0.005, 0.010, 0.004, 0.01, 0.015, 0.016, -0.006},
			  {-0.002, -0.015, -0.009, 0.001, 0.005, 0.005, 0.001},
			  {-1.0, 0.030, -0.002, 0.002, -0.004, -0.008, -0.004}};
    Double_t y11err3[5][7] = { {0, 0.0174, 0.0074, 0.0074,0.0074, 0.0153, 0.021},
			       {0.0232, 0.011, 0.0057, 0.0031, 0, 0.0077, 0.0129},
			       {0.022, 0.014, 0.0073, 0.0042, 0.0042, 0.00469, 0.0104},
			       {0.0031, 0.0141, 0.0057, 0.0036, 0, 0.0042, 0.0094},
			       {0, 0.0225, 0.0101, 0.007, 0.0062, 0, 0.0084}
                             };

    
    if(gSystem->AccessPathName(fileName))
    {
	cout << "Could not find the input file" <<endl;
	return;
    }
    TFile *file = new TFile(fileName);

    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;

    TGraphErrors *bGrPhy[kEnergyBins][kPhotonBins];
    TGraphErrors *yGrPhy[kEnergyBins][kPhotonBins];
    //For systematic error bars: Duplicate all graphs
    TGraphErrors *bGrPhy_sys[kEnergyBins][kPhotonBins];
    TGraphErrors *yGrPhy_sys[kEnergyBins][kPhotonBins];
    
    for(Int_t i = 0; i < kEnergyBins; ++i)
    {
	for(Int_t j = 0; j < kPhotonBins; ++j)
	{
	    TString name = Form("bEbin%i_PhotonBin%i", i, j);
	    bGrPhy[i][j] = (TGraphErrors*)file->Get(name);

	    // if(i == 0)
	    // 	yGrPhy[i][j] = new TGraphErrors(8, x11e1, y11e1[j], 0, y11err1[j]);
	    // if(i == 1)
	    // 	yGrPhy[i][j] = new TGraphErrors(9, x11e2, y11e1[j], 0, y11err2[j]);
	    // if(i == 2)
	    // 	yGrPhy[i][j] = new TGraphErrors(7, x11e3, y11e1[j], 0, y11err3[j]);

	    bGrPhy[i][j]->SetTitle("");

	}
    }
    file->Close();

    ifstream inFile("/star/u/kabir/GIT/BrightSTAR/database/Run11ResultsByPhotonMult.txt");
    if(!inFile)
    {
	cout << "Unable to open text file" <<endl;
	return;
    }
    Int_t l = 8;
    Double_t x, y, ex1, ex2, ey1, ey2;
    for(Int_t i = 0; i < 3; ++i)
    {
	if(i == 0)
	    l = 8;
	else if(i == 1)
	    l = 9;
	else if(i == 2)
	    l = 7;
    	for(Int_t j = 0; j < 5; ++j)
    	{
    	    yGrPhy[i][j] = new TGraphErrors();
    	    for(Int_t k = 0; k < l; ++k)
    	    {
    		inFile >> x >> y >> ex1 >> ex2 >> ey1 >> ey2;
		
		if(inFile.eof())
		    break;
		
		yGrPhy[i][j]->SetPoint(k, x, y); 
		yGrPhy[i][j]->SetPointError(k, ex1, ey1); 		
    	    }
    	}
    }
    inFile.close();
    
    Int_t canvasCount = 1;
    TCanvas *c1 = new TCanvas("EMjet_A_N_fms", "EM Jet A_{N}");
    c1->Divide(3, 5);
    //TCanvas *c1[3][5];
    for(Int_t i = 0; i < 5; ++i)
    {
	for(Int_t j = 0; j < 3; ++j)
	{
	    bGrPhy[j+2][i]->SetMaximum(0.05);
	    bGrPhy[j+2][i]->SetMinimum(-0.05);

	    yGrPhy[j][i]->SetMaximum(0.05);
	    yGrPhy[j][i]->SetMinimum(-0.05);
	    yGrPhy[j][i]->SetMarkerColor(kRed);
	    yGrPhy[j][i]->SetLineColor(kRed);
	    yGrPhy[j][i]->SetMarkerStyle(kFullCircle);
	    
	    c1->cd(canvasCount);
	    // c1[i][j] = new TCanvas(Form("PhotonBin_%i_EnergyBin_%i", i, j));
	    bGrPhy[j+2][i]->Draw("AP");  //My energy bin starts from 0 - 20 GeV
	    yGrPhy[j][i]->Draw("psame"); //Run 11 result is from 40 - 60 GeV
	   	    	    
	    TLine* L1Temp = new TLine(1.5, 0, 9.5, 0);
	    L1Temp->Draw("same");
	    ++canvasCount;
	}
    }

   
   gStyle->SetOptStat(0);

   TCanvas *C = (TCanvas*) gROOT->FindObject("C");
   if (C) delete C;
   C = new TCanvas("C","canvas",1024,640);
   C->SetFillStyle(4000);

   // Number of PADS
   const Int_t Nx = 3;
   const Int_t Ny = 5;

   // Margins
   Float_t lMargin = 0.12;
   Float_t rMargin = 0.05;
   Float_t bMargin = 0.15;
   Float_t tMargin = 0.05;

   // Canvas setup
   EjCanvasPartition(C,Nx,Ny,lMargin,rMargin,bMargin,tMargin);

   TPad *pad[Nx][Ny];
   
   for (Int_t j=0;j<Ny;j++)
   {
       for (Int_t i=0;i<Nx;i++)
       {
	   C->cd(0);

	   // Get the pads previosly created.
	   char pname[16];
	   sprintf(pname,"pad_%i_%i",i,j);
	   pad[i][j] = (TPad*) gROOT->FindObject(pname);
	   pad[i][j]->Draw();
	   pad[i][j]->SetFillStyle(4000);
	   pad[i][j]->SetFrameFillStyle(4000);
	   pad[i][j]->cd();
	   
	   // Size factors
	   Float_t xFactor = pad[0][0]->GetAbsWNDC()/pad[i][j]->GetAbsWNDC();
	   Float_t yFactor = pad[0][0]->GetAbsHNDC()/pad[i][j]->GetAbsHNDC();
	   
	   TGraphErrors *hFrame = bGrPhy[i+1][4 - j];
	  
	   // Format for y axis
	   hFrame->GetYaxis()->SetLabelFont(43);
	   hFrame->GetYaxis()->SetLabelSize(26);
	   hFrame->GetYaxis()->SetLabelOffset(0.02);
	   hFrame->GetYaxis()->SetTitleFont(43);
	   hFrame->GetYaxis()->SetTitleSize(16);
	   hFrame->GetYaxis()->SetTitleOffset(5);

	   hFrame->GetYaxis()->CenterTitle();
	   hFrame->GetYaxis()->SetNdivisions(505);

	   // TICKS Y Axis
	   hFrame->GetYaxis()->SetTickLength(xFactor*0.08/yFactor);

	   // Format for x axis
	   hFrame->GetXaxis()->SetLabelFont(43);
	   hFrame->GetXaxis()->SetLabelSize(36);
	   hFrame->GetXaxis()->SetLabelOffset(0.02);
	   hFrame->GetXaxis()->SetTitleFont(43);
	   hFrame->GetXaxis()->SetTitleSize(16);
	   hFrame->GetXaxis()->SetTitleOffset(5);
	   hFrame->GetXaxis()->CenterTitle();
	   hFrame->GetXaxis()->SetNdivisions(505);

	   // TICKS X Axis
	   hFrame->GetXaxis()->SetTickLength(yFactor*0.06/xFactor);
	   
	   //---------------------------------------------------------------
	   yGrPhy[i][4 - j]->SetMarkerColor(kRed);
	   yGrPhy[i][4 - j]->SetLineColor(kRed);
	   yGrPhy[i][4 - j]->SetMarkerStyle(kFullCircle);

	   bGrPhy[i+2][4 - j]->SetMarkerSize(1.3);
	   bGrPhy[i+2][4 - j]->SetMaximum(0.07);
	   bGrPhy[i+2][4 - j]->SetMinimum(-0.03);

	   yGrPhy[i][4 - j]->SetMarkerSize(1.3);
	   yGrPhy[i][4 - j]->SetMaximum(0.07);
	   yGrPhy[i][4 - j]->SetMinimum(-0.03);

	   bGrPhy[i+2][4 - j]->Draw("ap"); //My energy bin starts from 0 - 20 GeV
	   yGrPhy[i][4 - j]->Draw("psame"); //Run 11 result energy bin is from 40 - 60 GeV
	   
	   TLine* L1Temp = new TLine(1.8, 0, 9.5, 0);
	   L1Temp->SetLineStyle(7);
	   L1Temp->Draw("same");
	   //---------------------------------------------------------------	 
       }
   }
   C->cd();        
}

