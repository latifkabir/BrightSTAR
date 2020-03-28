// Filename: HotChCut.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Mar 25 14:19:45 2020 (-0400)
// URL: jlab.org/~latif

void HotChCut()
{
 
    TFile *fex = new TFile("/star/u/kabir/GIT/BrightSTAR/results/root/6.AnFmsRpCorr/v3/AnFmsRpCorrUpbPionMassCutHotChCutExtreme.root");
    TFile *forg = new TFile("/star/u/kabir/GIT/BrightSTAR/results/root/6.AnFmsRpCorr/v3/AnFmsRpCorrUpbPionMassCut.root");
    TFile *fmod = new TFile("/star/u/kabir/GIT/BrightSTAR/results/root/6.AnFmsRpCorr/v3/AnFmsRpCorrUpbHotChMaskedPionMcut.root");
    TH1D *org = (TH1D*)forg->Get("sumE_west");
    TH1D *mod = (TH1D*)fmod->Get("sumE_west");
    TH1D *ext = (TH1D*)fex->Get("sumE_west");

    org->SetLineColor(kBlack);
    mod->SetLineColor(kRed);
    ext->SetLineColor(kBlue);
    
    org->Draw();
    ext->Draw("same");
    mod->Draw("same");
     
}
