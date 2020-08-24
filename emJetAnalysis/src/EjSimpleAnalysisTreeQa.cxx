// Filename: EjSimpleAnalysisTreeQa.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri May  8 15:08:25 2020 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "cppInclude.h"

//Use wildcat in the file name to combine many files.
void EjSimpleAnalysisTreeQa(TString inFileName, TString outName)
{

    TChain *ch = new TChain("T");
    ch->Add(inFileName);
    
    TFile *file = new TFile(outName, "recreate");
    Int_t nPhotons;
    Int_t spinB;
    Int_t spinY;
    Double_t eng;
    Double_t pt;
    Double_t eta;
    Double_t phi;
    Double_t jetX;
    Double_t jetY;
    Double_t vtxZ;
    Double_t rt;
        
    ch->SetBranchAddress("eng", &eng);
    ch->SetBranchAddress("pt", &pt);
    ch->SetBranchAddress("eta", &eta);
    ch->SetBranchAddress("phi", &phi);
    ch->SetBranchAddress("vtxZ", &vtxZ);
    ch->SetBranchAddress("jetX", &jetX);
    ch->SetBranchAddress("jetY", &jetY);
    ch->SetBranchAddress("nPhotons", &nPhotons);
    ch->SetBranchAddress("spinB", &spinB);
    ch->SetBranchAddress("spinY", &spinY);
    ch->SetBranchAddress("rt", &rt);

    TH1D *h1Eta = new TH1D ("h1Eta", "EM Jet Eta; Jet #eta", 100, -1.5, 5.0);
    TH1D *h1Phi = new TH1D ("h1Phi", "EM Jet Phi; Jet #phi [rad]", 100, -3.3, 3.3);
    TH1D *h1E = new TH1D ("h1E", "EM Jet E; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1Pt = new TH1D ("h1Pt", "Jet Pt; Jet Pt [GeV/c]", 100, 0.0, 50.0);
    TH1D *h1nPhotons = new TH1D("h1nPhotons", "number of photons in EM jets; Number of Photons", 20, 0, 20);
    TH1D *h1vtxZ = new TH1D("h1vtxZ", "Jet vetrex z; Jet vertex z [cm]", 100, -200, 200);
    TH1D *h1spinB = new TH1D("h1spinB", "Blue beam spin; Blue beam spin", 5, -2, 2);
    TH1D *h1spinY = new TH1D("h1spinY", "Yellow beam spin; Yellow beam spin", 5, -2, 2);
    
    
    TH2D *h2EvsPt = new TH2D("h2EvsPt", "Eng vs Pt; Pt [GeV/C]; E [GeV]", 100, 0, 20, 100, 0, 100);
    TH2D *h2nPhVsEng = new TH2D("h2nPhVsEng", "Number of photons vs Eng; E [Gev]; No. of Photons", 100, 0, 100, 20, 0, 20);
    //TH2D *h2JetXY = new TH2D ("h2JetXY", "Jet Position [FMS]; Jet X [cm]; Jet Y [cm]", 100, -100, 100, 100, -100, 100);
    TH2D *h2JetXY = new TH2D ("h2JetXY", "Jet Position [FMS]; Jet X [cm]; Jet Y [cm]", 100, -250, 250, 100, -250, 250);
    //TH2D *h2EtaPhi = new TH2D ("h2EtaPhi", "Jet Eta Phi [FMS]; Jet #eta; Jet #phi [rad]", 100, 2.5, 4.5, 100, -3.5, 3.5);
    TH2D *h2EtaPhi = new TH2D ("h2EtaPhi", "Jet Eta Phi [FMS]; Jet #eta; Jet #phi [rad]", 100, 0.8, 2.5, 100, -3.5, 3.5);
    
    Int_t nEntries = ch->GetEntries();

    for(Int_t i = 0; i < nEntries; ++i)
    {
	ch->GetEntry(i);

	// if(rt != 1)
	//     continue;
	
	h1Eta->Fill(eta);
	h1Phi->Fill(phi);
	h1E->Fill(eng);
	h1Pt->Fill(pt);
	h1vtxZ->Fill(vtxZ);
	h1nPhotons->Fill(nPhotons);
	h1spinB->Fill(spinB);
	h1spinY->Fill(spinY);

	h2JetXY->Fill(jetX, jetY);
	h2EtaPhi->Fill(eta, phi);
	h2nPhVsEng->Fill(eng, nPhotons);
	h2EvsPt->Fill(pt, eng);
    }


    file->Write();
    
}
