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
#include "EjRePlotAn.h"

using namespace std;

void EjRePlotAn(TString fileName, TString det, Int_t isMerged)
{
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
	    name = Form("yEbin%i_PhotonBin%i", i, j);
	    yGrPhy[i][j] = (TGraphErrors*)file->Get(name);

	    
	    name = Form("Sys_bEbin%i_PhotonBin%i", i, j);
	    bGrPhy_sys[i][j] = (TGraphErrors*)file->Get(name);
	    name = Form("Sys_yEbin%i_PhotonBin%i", i, j);
	    yGrPhy_sys[i][j] = (TGraphErrors*)file->Get(name);

	    bGrPhy[i][j]->SetTitle(""); //Remove title 
	    yGrPhy[i][j]->SetTitle("");
	    bGrPhy_sys[i][j]->SetTitle("");
	    yGrPhy_sys[i][j]->SetTitle("");
	}
    }
    file->Close();

    Double_t yMax = 0.08;	
    Int_t canvasCount = 1;
    TCanvas *c1 = new TCanvas("EMjet_A_N_fms", "EM Jet A_{N}");
    c1->Divide(kEnergyBins -1, kPhotonBins -1);
    for(Int_t i = 0; i < kPhotonBins - 1; ++i)
    {
	for(Int_t j = 1; j < kEnergyBins; ++j)
	{
	    bGrPhy_sys[j][i]->SetFillColor(1);
	    bGrPhy_sys[j][i]->SetFillStyle(3001);
	    yGrPhy_sys[j][i]->SetFillColor(2);
	    yGrPhy_sys[j][i]->SetFillStyle(3001);

	    bGrPhy[j][i]->SetMaximum(yMax);
	    bGrPhy[j][i]->SetMinimum(-1.0*yMax);
	    bGrPhy_sys[j][i]->SetMaximum(yMax);
	    bGrPhy_sys[j][i]->SetMinimum(-1.0*yMax);

	    yGrPhy[j][i]->SetMaximum(yMax);
	    yGrPhy[j][i]->SetMinimum(-1.0*yMax);
	    yGrPhy_sys[j][i]->SetMaximum(yMax);
	    yGrPhy_sys[j][i]->SetMinimum(-1.0*yMax);
	    
	    c1->cd(canvasCount);

	    bGrPhy_sys[j][i]->Draw();
	    bGrPhy[j][i]->Draw("same");
	    yGrPhy_sys[j][i]->Draw("same");
	    yGrPhy[j][i]->Draw("same");
	   
	    bGrPhy_sys[j][i]->SetDrawOption("a2");	    
	    bGrPhy[j][i]->SetDrawOption("p same");

	    yGrPhy_sys[j][i]->SetDrawOption("2 same");
	    yGrPhy[j][i]->SetDrawOption("p same");
	    	    
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

   TFile *xfFile;
   
   Int_t Kx;
   Int_t Ky;
   if(det == "fms")
   {
       Kx = 3;
       Ky = (isMerged)? 4 : 5;
       yMax = 0.08;
       if(gSystem->AccessPathName("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass5/Fms_PtVsAvgXf.root")) return;
       xfFile = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass5/Fms_PtVsAvgXf.root");
   }
   else if(det == "eemc")
   {
       Kx = 1;
       Ky = (isMerged)? 4 : 5;
       yMax = (isMerged)? 0.03 : 0.05;
       if(gSystem->AccessPathName("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass4/EEmc_PtVsAvgXf.root")) return;
       xfFile = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass4/EEmc_PtVsAvgXf.root");
   }
   else
   {
       cout << "Invalid detector" <<endl;
       return;
   }
   
   TGraphErrors *grXf[3];
   for(Int_t i = 0; i < Kx; ++i)
   {
       TString name = Form("grAvgXfVsPt%i", i);
       grXf[i] = (TGraphErrors*)xfFile->Get(name);
   }
   Int_t s = (isMerged)? -1 : 0;
   
   // Number of PADS
   const Int_t Nx = Kx;
   const Int_t Ny = Ky;
   Int_t maxPhI = (isMerged)? 2 : 4; //Maximum photon multiplicity index
   // Margins
   Float_t lMargin = 0.12;
   Float_t rMargin = 0.05;
   Float_t bMargin = 0.15;
   Float_t tMargin = 0.05;

   // Canvas setup
   EjCanvasPartition(C,Nx,Ny,lMargin,rMargin,bMargin,tMargin);

   TPad *pad[Nx][Ny];
   
   for (Int_t j = 0; j < Ny; j++)
   {
       for (Int_t i = 0; i < Nx; i++)
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
	   
	   TGraphErrors *hFrame;
	   if(j == 0 && isMerged)
	       hFrame = grXf[i];
	   else
	   {
	       if(det == "eemc" && i == 0) i = -1; //For EEMC, energy bin starts from 0, while for FMS it starts from 1.
	       hFrame  = bGrPhy_sys[i+1][maxPhI - j - s];	    
	   }
	  
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

	   //-------------------------------------------------------------
	   if(j == 0 && isMerged)
	   {
	       if(det == "fms")
	       {
		   grXf[i]->SetMaximum(0.85);
		   grXf[i]->SetMinimum(0.2);
	       }
	       else if(det == "eemc")
	       {
		   grXf[i]->SetMaximum(0.16);
		   grXf[i]->SetMinimum(0.04);
	       }
	       grXf[i]->GetYaxis()->SetNdivisions(505);
	   
	       grXf[i]->Draw("AP");	       
	       continue;
	   }
	   //---------------------------------------------------------------	  
	   //Note: The pad is filled from bottom to top. i.e. Higher photon multiplicity is plotted first.
	   bGrPhy_sys[i+1][maxPhI - j - s]->SetFillColor(1);
	   bGrPhy_sys[i+1][maxPhI - j - s]->SetFillStyle(3001);
	   yGrPhy_sys[i+1][maxPhI - j - s]->SetFillColor(2);
	   yGrPhy_sys[i+1][maxPhI - j - s]->SetFillStyle(3001);

	   bGrPhy[i+1][maxPhI - j - s]->SetMarkerSize(1.3);
	   bGrPhy[i+1][maxPhI - j - s]->SetMaximum(yMax);
	   bGrPhy[i+1][maxPhI - j - s]->SetMinimum(-1.0*yMax);
	   bGrPhy_sys[i+1][maxPhI - j - s]->SetMarkerSize(1.3);
	   bGrPhy_sys[i+1][maxPhI - j - s]->SetMaximum(yMax);
	   bGrPhy_sys[i+1][maxPhI - j - s]->SetMinimum(-1.0*yMax);

	   yGrPhy[i+1][maxPhI - j - s]->SetMarkerSize(1.3);
	   yGrPhy[i+1][maxPhI - j - s]->SetMaximum(yMax);
	   yGrPhy[i+1][maxPhI - j - s]->SetMinimum(-1.0*yMax);
	   bGrPhy_sys[i+1][maxPhI - j - s]->SetMarkerSize(1.3);
	   yGrPhy_sys[i+1][maxPhI - j - s]->SetMaximum(yMax);
	   yGrPhy_sys[i+1][maxPhI - j - s]->SetMinimum(-1.0*yMax);
	    
	   bGrPhy_sys[i+1][maxPhI - j - s]->Draw();
	   bGrPhy[i+1][maxPhI - j - s]->Draw("same");
	   yGrPhy_sys[i+1][maxPhI - j - s]->Draw("same");
	   yGrPhy[i+1][maxPhI - j - s]->Draw("same");
	   
	   bGrPhy_sys[i+1][maxPhI - j - s]->SetDrawOption("a2");	    
	   bGrPhy[i+1][maxPhI - j - s]->SetDrawOption("p same");

	   yGrPhy_sys[i+1][maxPhI - j - s]->SetDrawOption("2 same");
	   yGrPhy[i+1][maxPhI - j - s]->SetDrawOption("p same");

	   TLine* L1Temp = new TLine(1.95, 0, 9.7, 0);
	   L1Temp->SetLineStyle(7);
	   L1Temp->Draw("same");
	   //------------------------------------------------
	   if(det == "eemc" && i == -1) i = 0;
       }
   }
   
   C->cd();
}

void EjCanvasPartition(TCanvas *C,const Int_t Nx, const Int_t Ny,
                     Float_t lMargin, Float_t rMargin,
                     Float_t bMargin, Float_t tMargin)
{
   if (!C) return;

   // Setup Pad layout:
   Float_t vSpacing = 0.0;
   Float_t vStep  = (1.- bMargin - tMargin - (Ny-1) * vSpacing) / Ny;

   Float_t hSpacing = 0.0;
   Float_t hStep  = (1.- lMargin - rMargin - (Nx-1) * hSpacing) / Nx;

   Float_t vposd,vposu,vmard,vmaru,vfactor;
   Float_t hposl,hposr,hmarl,hmarr,hfactor;

   for (Int_t i=0;i<Nx;i++) {

      if (i==0) {
         hposl = 0.0;
         hposr = lMargin + hStep;
         hfactor = hposr-hposl;
         hmarl = lMargin / hfactor;
         hmarr = 0.0;
      } else if (i == Nx-1) {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep + rMargin;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = rMargin / (hposr-hposl);
      } else {
         hposl = hposr + hSpacing;
         hposr = hposl + hStep;
         hfactor = hposr-hposl;
         hmarl = 0.0;
         hmarr = 0.0;
      }

      for (Int_t j=0;j<Ny;j++) {

         if (j==0) {
            vposd = 0.0;
            vposu = bMargin + vStep;
            vfactor = vposu-vposd;
            vmard = bMargin / vfactor;
            vmaru = 0.0;
         } else if (j == Ny-1) {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep + tMargin;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = tMargin / (vposu-vposd);
         } else {
            vposd = vposu + vSpacing;
            vposu = vposd + vStep;
            vfactor = vposu-vposd;
            vmard = 0.0;
            vmaru = 0.0;
         }

         C->cd(0);

         char name[16];
         sprintf(name,"pad_%i_%i",i,j);
         TPad *pad = (TPad*) gROOT->FindObject(name);
         if (pad) delete pad;
         pad = new TPad(name,"",hposl,vposd,hposr,vposu);
         pad->SetLeftMargin(hmarl);
         pad->SetRightMargin(hmarr);
         pad->SetBottomMargin(vmard);
         pad->SetTopMargin(vmaru);

         pad->SetFrameBorderMode(0);
         pad->SetBorderMode(0);
         pad->SetBorderSize(0);

         pad->Draw();
      }
   }
}
