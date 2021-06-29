// Filename: EjRooUnfolding.C
// Description: EM-jet unfolding based on RooUnfoldExample.cxx example from RooUnfolding
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Mar 24 16:04:07 2021 (-0400)
// URL: jlab.org/~latif

#include <iostream>
using std::cout;
using std::endl;

#include "TRandom.h"
#include "TH1D.h"
#include "TCanvas.h"

//#include "RooUnfoldResponse.h"
//#include "RooUnfoldBayes.h"
#include "/star/u/kabir/GIT/BrightSTAR/RooUnfold/src/RooUnfoldSvd.h"
//#include "RooUnfoldTUnfold.h"
//#include "RooUnfoldIds.h" 
  
void EjRooUnfolding(TString respMatFile, TString  foldedResultFile)
{
   gROOT->SetStyle("Plain");
   gStyle->SetOptStat(0);

   if(gSystem->AccessPathName(respMatFile) || gSystem->AccessPathName(foldedResultFile))
   {
       cout << "Input file not found" <<endl;
       return;
   }
   
   TFile *fRespMat = new TFile(respMatFile);
   TFile *fFoldedRes = new TFile(foldedResultFile);

   //<----- Later you should get response matrix for each energy and pt bin. ------->
   TH1D *h1nPhPart = (TH1D*)fRespMat->Get("h1nPhotonsPart"); //Particle level jet  photon multiplicity
   TH1D *h1nPhDet = (TH1D*)fRespMat->Get("h1nPhotonsDet");   //Detector level jet photon multiplicity
   TH2D *h2RespMat = (TH2D*)fRespMat->Get("h2nPhResMat");    // <------ Should be det along x and part along y
   // ---> should be normalized to 1 for all 
   ///*   
   //Normalize all rows of the response matrix
   Double_t sumBinCont = 0;
   // for(Int_t j = 1; j <= h2RespMat->GetNbinsY(); ++j) //along y-axis
   // {
   //     sumBinCont = 0;
   //     for(Int_t i = 1; i <= h2RespMat->GetNbinsX(); ++i) // along x-axis
   //     {
   // 	   sumBinCont += h2RespMat->GetBinContent(i, j);	  
   //     }
   //     for(Int_t i = 1; i <= h2RespMat->GetNbinsX(); ++i) // along x-axis
   //     {
   // 	   h2RespMat->SetBinContent(i, j, h2RespMat->GetBinContent(i, j) / sumBinCont);	  
   //     }
   // }

   // for(Int_t i = 1; i <= h2RespMat->GetNbinsX(); ++i) //along x-axis
   // {
   //     sumBinCont = 0;
   //     for(Int_t j = 1; j <= h2RespMat->GetNbinsX(); ++j) // along y-axis
   //     {
   // 	   sumBinCont += h2RespMat->GetBinContent(i, j);	  
   //     }
   //     for(Int_t j = 1; j <= h2RespMat->GetNbinsX(); ++j) // along y-axis
   //     {
   // 	   h2RespMat->SetBinContent(i, j, h2RespMat->GetBinContent(i, j) / sumBinCont);	  
   //     }
   // }

   
   //h1nPhPart->Scale(1.0 / h1nPhPart->Integral());
   //h1nPhDet->Scale(1.0 / h1nPhDet->Integral());
   //*/
   TH1D *h1FoldedRes[3][9]; // max three energy bins and 9 pt bins   
   TH1D *h1UnfoldedRes[3][9]; // max three energy bins and 9 pt bins   
   for(Int_t i = 0; i < 9; ++i)
   {
       TString hName1 = Form("emJetPtBin1_%i", i);
       TString hName2 = Form("emJetPtBin2_%i", i);
       TString hName3 = Form("emJetPtBin3_%i", i);
       h1FoldedRes[0][i] = (TH1D*) fFoldedRes->Get(hName1);
       h1UnfoldedRes[0][i] = new TH1D("Unf" + hName1, hName1, h1FoldedRes[0][i]->GetNbinsX(), 0,  h1FoldedRes[0][i]->GetNbinsX());
       
       h1FoldedRes[1][i] = (TH1D*) fFoldedRes->Get(hName2);
       h1UnfoldedRes[1][i] = new TH1D("Unf" + hName2, hName2, h1FoldedRes[1][i]->GetNbinsX(), 0,  h1FoldedRes[1][i]->GetNbinsX());
       
       h1FoldedRes[2][i] = (TH1D*) fFoldedRes->Get(hName3);
       h1UnfoldedRes[2][i] = new TH1D("Unf" + hName3, hName3, h1FoldedRes[2][i]->GetNbinsX(), 0,  h1FoldedRes[2][i]->GetNbinsX());
   }
   

   //<-------- Lets consider one particular energy and pt bin for now ---------------->
   
   TH1D* foldedres = h1FoldedRes[1][2];
   Int_t nbins = foldedres->GetNbinsX();
   // Statistical covariance matrix
   TH2D *statcov = new TH2D("statcov", "covariance matrix", nbins, 0, nbins, nbins, 0, nbins);

   
   // Fill the data covariance matrix
   for (int i=1; i<=foldedres->GetNbinsX(); i++)
   {
       statcov->SetBinContent(i,i,foldedres->GetBinError(i)*foldedres->GetBinError(i));
   }
 
   // ----------------------------
   // Here starts the actual unfolding
   //
   RooUnfoldResponse response (h1nPhDet, h1nPhPart, h2RespMat);
   RooUnfoldSvd unfold (&response, foldedres, 5);   // OR 
   TH1D* hReco= (TH1D*) unfold.Hreco();
   TH1D* unfres = hReco;
   
   TCanvas* c1= new TCanvas("canvas","canvas");

   //unfold.PrintTable (cout, hTrue);
   hReco->SetLineColor(8);
   hReco->Draw();
   foldedres->Draw("SAME");
 

   cout << "------------------------------------------------------------------------------------" <<endl;
   for(Int_t i = 1; i <= 5; ++i)
   {
       cout << foldedres->GetBinContent(i) << "\t" << unfres->GetBinContent(i) <<endl;       
   }

   TCanvas *can3 = new TCanvas();
   h2RespMat->Draw("colz");   
}
