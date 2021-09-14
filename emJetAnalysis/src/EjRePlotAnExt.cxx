// Filename: EjRePlotExt.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 16 12:04:11 2021 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"

void EjRePlotAnExt()
{
    TFile *f1Ph = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass5/Run15FmsEmJetAn_1Photon_And_All_photons_with_smbs3.root");
    TFile *f2And3Ph = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass5/Run15FmsEmJetAn_3Ph_with_smbs3_20-100GeV.root");
    
    TGraphErrors* gr1Ph = (TGraphErrors*)f1Ph->Get("An2Photons"); //1 photon case was saved as 2 photon. Do not be confused about name
    gr1Ph->SetName("An1Photon");
    TGraphErrors* gr2Ph = (TGraphErrors*)f2And3Ph->Get("An2Photons");
    TGraphErrors* gr3PhOrMore = (TGraphErrors*)f2And3Ph->Get("AnNphotons");

    gr1Ph->SetMarkerColor(kRed);
    gr2Ph->SetMarkerColor(kBlack);
    gr3PhOrMore->SetMarkerColor(kBlue);

    gr1Ph->SetMarkerSize(3);
    gr2Ph->SetMarkerSize(3);
    gr3PhOrMore->SetMarkerSize(3);
    
    gr1Ph->SetLineColor(kRed);
    gr2Ph->SetLineColor(kBlack);
    gr3PhOrMore->SetLineColor(kBlue);

    //Copy Graphs for systematic error bars
    TGraphErrors *gr1Ph_sys = (TGraphErrors*)gr1Ph->Clone("An1Photon_sys");
    TGraphErrors *gr2Ph_sys = (TGraphErrors*)gr2Ph->Clone("An2Photons_sys");
    TGraphErrors *gr3PhOrMore_sys = (TGraphErrors*)gr3PhOrMore->Clone("AnNphotons_sys");

    gr1Ph_sys->SetFillStyle(3001);
    gr2Ph_sys->SetFillStyle(3001);
    gr3PhOrMore_sys->SetFillStyle(3001);
    
    gr1Ph_sys->SetFillColor(kRed);
    gr2Ph_sys->SetFillColor(kBlack);
    gr3PhOrMore_sys->SetFillColor(kBlue);
    
    Double_t x, y;
    for(Int_t i = 0; i < gr1Ph_sys->GetN(); ++i)
    {
	gr1Ph_sys->GetPoint(i, x, y);
	gr1Ph_sys->SetPointError(i, x*0.03, y*0.35); // 3% sys error for x and 35% for y sys error
    }

    for(Int_t i = 0; i < gr2Ph_sys->GetN(); ++i)
    {
	gr2Ph_sys->GetPoint(i, x, y);
	gr2Ph_sys->SetPointError(i, x*0.03, y*0.35);
    }

    for(Int_t i = 0; i < gr3PhOrMore_sys->GetN(); ++i)
    {
	gr3PhOrMore_sys->GetPoint(i, x, y);
	gr3PhOrMore_sys->SetPointError(i, x*0.03, y*0.20);
    }
    
    // gr1Ph_sys->Draw();
    //gr1Ph->Draw("same");
    gr1Ph->Draw("AP"); // Fow without Sys only

    // gr2Ph_sys->Draw("same");
    gr2Ph->Draw("same");

    // gr3PhOrMore_sys->Draw("same");
    gr3PhOrMore->Draw("same");

    // gr1Ph_sys->SetDrawOption("a2");
    // gr1Ph->SetDrawOption("p same");

    // gr2Ph_sys->SetDrawOption("2");
    // gr2Ph->SetDrawOption("p same");

    // gr3PhOrMore_sys->SetDrawOption("2");
    // gr3PhOrMore->SetDrawOption("p same");    
}
