// Filename: EjUnfolding.C
// Description: EM-jet unfolding based on TSVDUnfoldExample.C example from root
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Mar 24 16:04:07 2021 (-0400)
// URL: jlab.org/~latif

#include <iostream>
 
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TString.h"
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TLine.h"
 
#include "TSVDUnfold.h"
 
  
void EjUnfolding(TString respMatFile, TString  foldedResultFile)
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
   TH2D *h2RespMat = (TH2D*)fRespMat->Get("h2nPhResMat"); // <------ Should be det along x and part along y
   // ---> should be normalized to 1 for all 
   
   TH1D *h1FoldedRes[3][9]; // max three energy bins and 9 pt bins   
   TH1D *h1UnfoldedRes[3][9]; // max three energy bins and 9 pt bins   
   for(Int_t i = 0; i < 9; ++i)
   {
       TString hName1 = Form("emJetPtBin1-%i", i);
       TString hName2 = Form("emJetPtBin2-%i", i);
       TString hName3 = Form("emJetPtBin3-%i", i);
       h1FoldedRes[0][i] = (TH1D*) fFoldedRes->Get(hName1);
       h1UnfoldedRes[0][i] = new TH1D(hName1, hName1, h1FoldedRes[0][i]->GetGetNbinsX(), 0,  h1FoldedRes[0][i]->GetGetNbinsX());
       
       h1FoldedRes[1][i] = (TH1D*) fFoldedRes->Get(hName2);
       h1UnfoldedRes[1][i] = new TH1D(hName2, hName2, h1FoldedRes[1][i]->GetGetNbinsX(), 0,  h1FoldedRes[1][i]->GetGetNbinsX());
       
       h1FoldedRes[2][i] = (TH1D*) fFoldedRes->Get(hName3);
       h1UnfoldedRes[2][i] = new TH1D(hName3, hName3, h1FoldedRes[2][i]->GetGetNbinsX(), 0,  h1FoldedRes[2][i]->GetGetNbinsX());
   }
   

   TH1D* foldedres = h1FoldedRes[0][0];
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
   // Create TSVDUnfold object and initialise
   TSVDUnfold *tsvdunf = new TSVDUnfold(foldedres, statcov, h1nPhDet, h1nPhPart, h2RespMat );
 
   // It is possible to normalise unfolded spectrum to unit area
   tsvdunf->SetNormalize( kFALSE ); // no normalisation here
 
   // Perform the unfolding with regularisation parameter kreg = 13
   // - the larger kreg, the finer grained the unfolding, but the more fluctuations occur
   // - the smaller kreg, the stronger is the regularisation and the bias
   TH1D* unfres = tsvdunf->Unfold( 13 );
 
   // Get the distribution of the d to cross check the regularization
   // - choose kreg to be the point where |d_i| stop being statistically significantly >>1
   TH1D* ddist = tsvdunf->GetD();
 
   // Get the distribution of the singular values
   TH1D* svdist = tsvdunf->GetSV();
 
   // Compute the error matrix for the unfolded spectrum using toy MC
   // using the measured covariance matrix as input to generate the toys
   // 100 toys should usually be enough
   // The same method can be used for different covariance matrices separately.
   TH2D* ustatcov = tsvdunf->GetUnfoldCovMatrix( statcov, 100 );
 
   // Now compute the error matrix on the unfolded distribution originating
   // from the finite detector matrix statistics
   TH2D* uadetcov = tsvdunf->GetAdetCovMatrix( 100 );
 
   // Sum up the two (they are uncorrelated)
   ustatcov->Add( uadetcov );
 
   //Get the computed regularized covariance matrix (always corresponding to total uncertainty passed in constructor) and add uncertainties from finite MC statistics.
   TH2D* utaucov = tsvdunf->GetXtau();
   utaucov->Add( uadetcov );
 
   //Get the computed inverse of the covariance matrix
   TH2D* uinvcov = tsvdunf->GetXinv();
 
 
   // ---------------------------------
   // Only plotting stuff below
   
   for (int i=1; i<=unfres->GetNbinsX(); i++)
   {
      unfres->SetBinError(i, TMath::Sqrt(utaucov->GetBinContent(i,i)));
   }
  
   TLegend *leg = new TLegend(0.58,0.60,0.99,0.88);
   leg->SetBorderSize(0);
   leg->SetFillColor(0);
   leg->SetFillStyle(0);
   leg->AddEntry(unfres,"Unfolded Data","p");
   leg->AddEntry(foldedres,"Folded Data","l");
 
   TCanvas *c1 = new TCanvas( "c1", "Unfolding toy example with TSVDUnfold", 1000, 900 );
 
   c1->Divide(1,2);
   TVirtualPad * c11 = c1->cd(1);
 
   TH1D* frame = new TH1D( *unfres );
   frame->SetTitle( "Unfolding toy example with TSVDUnfold" );
   frame->GetXaxis()->SetTitle( "x variable" );
   frame->GetYaxis()->SetTitle( "Events" );
   frame->GetXaxis()->SetTitleOffset( 1.25 );
   frame->GetYaxis()->SetTitleOffset( 1.29 );
   frame->Draw();
 
   foldedres->SetLineStyle(2);
   foldedres->SetLineColor(4);
   foldedres->SetLineWidth(2);
   unfres->SetMarkerStyle(20);

   // ------------------------------------------------------------
 
   // add histograms
   unfres->Draw("same");
   foldedres->Draw("same");
   
   leg->Draw();
 
   // covariance matrix
   TVirtualPad * c12 = c1->cd(2);
   c12->Divide(2,1);
   TVirtualPad * c2 = c12->cd(1);
   c2->SetRightMargin   ( 0.15         );
 
   TH2D* covframe = new TH2D( *ustatcov );
   covframe->SetTitle( "TSVDUnfold covariance matrix" );
   covframe->GetXaxis()->SetTitle( "x variable" );
   covframe->GetYaxis()->SetTitle( "x variable" );
   covframe->GetXaxis()->SetTitleOffset( 1.25 );
   covframe->GetYaxis()->SetTitleOffset( 1.29 );
   covframe->Draw();
 
   ustatcov->SetLineWidth( 2 );
   ustatcov->Draw( "colzsame" );
 
   // distribution of the d quantity
   TVirtualPad * c3 = c12->cd(2);
   c3->SetLogy();
 
   TLine *line = new TLine( 0.,1.,40.,1. );
   line->SetLineStyle(2);
 
   TH1D* dframe = new TH1D( *ddist );
   dframe->SetTitle( "TSVDUnfold |d_{i}|" );
   dframe->GetXaxis()->SetTitle( "i" );
   dframe->GetYaxis()->SetTitle( "|d_{i}|" );
   dframe->GetXaxis()->SetTitleOffset( 1.25 );
   dframe->GetYaxis()->SetTitleOffset( 1.29 );
   dframe->SetMinimum( 0.001 );
   dframe->Draw();
 
   ddist->SetLineWidth( 2 );
   ddist->Draw( "same" );
   line->Draw();
}
