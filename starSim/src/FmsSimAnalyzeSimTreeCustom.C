// Filename: FmsSimAnalyzeSimTree.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Jul 31 02:55:40 2020 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"
#include "BrContainers/TStFmsPointData.h"

void FmsSimAnalyzeSimTree(TString fileList, Double_t minE, Double_t maxE) // Use wildcat to combine files
{
    Double_t Emin = minE;//2.0;
    Double_t Emax = maxE; //500.0;
    
    TChain *tree = new TChain("T");
    tree->Add(fileList);

    StPythiaEvent *pyEvent = 0;
    TClonesArray *fmsPointArr = new TClonesArray("TStFmsPointData");
    TStFmsPointData *fmsPointData;
    Int_t trgBit;
    string trgBitStr;
    
    tree->SetBranchAddress("pythiaEvent", &pyEvent);
    tree->SetBranchAddress("fmsPoint", &fmsPointArr);
    tree->SetBranchAddress("trigBit", &trgBit);

    TFile *file = new TFile("PythiaEeventHist.root", "recreate");

    TH1D *h1TrigTypes = new TH1D ("h1TrigTypes", "Trigger Types; Trigger Types", 30, 0, 30);

    h1TrigTypes->GetXaxis()->SetBinLabel(3,"Small BS1");
    h1TrigTypes->GetXaxis()->SetBinLabel(5,"Small BS2");
    h1TrigTypes->GetXaxis()->SetBinLabel(7,"Small BS3");
    h1TrigTypes->GetXaxis()->SetBinLabel(9,"Large BS1");
    h1TrigTypes->GetXaxis()->SetBinLabel(11,"Large BS2");
    h1TrigTypes->GetXaxis()->SetBinLabel(13,"Large BS3");
    h1TrigTypes->GetXaxis()->SetBinLabel(15,"FMS-JP0");
    h1TrigTypes->GetXaxis()->SetBinLabel(17,"FMS-JP1");
    h1TrigTypes->GetXaxis()->SetBinLabel(19,"FMS-JP2");
    h1TrigTypes->GetXaxis()->SetBinLabel(21,"None");

    
    TH1D* h1EngPy = new TH1D("h1EngPy", "Energy; E [GeV]", 120, 0, 120);
    TH1D* h1PtPy = new TH1D("h1PtPy", "Pt; Pt [GeV/c]", 100, 0, 15);
    TH1D* h1EtaPy = new TH1D("h1EtaPy", "Eta; #Eta", 100, 2.5, 4.5);
    TH1D* h1PhiPy = new TH1D("h1PhiPy", "Phi; #phi [rad]", 100, -3.2, 3.2);
    TH1D* h1nPhotonPy = new TH1D("h1nPhotonPy", "Number of Photons; Number of photons", 20, -1, 19);
    Int_t nPhotonPy;
    
    TH1D* h1EngMu = new TH1D("h1EngMu", "Energy; E [GeV]", 120, 0, 120);
    TH1D* h1PtMu = new TH1D("h1PtMu", "Pt; Pt [GeV/c]", 100, 0, 15);
    TH1D* h1EtaMu = new TH1D("h1EtaMu", "Eta; #Eta", 100, 2.5, 4.5);
    TH1D* h1PhiMu = new TH1D("h1PhiMu", "Phi; #phi [rad]", 100, -3.2, 3.2);
    TH1D* h1nPhotonMu = new TH1D("h1nPhotonMu", "Number of Photons; Number of photons", 20, -1, 19);   
    Int_t nPhotonMu;

    TH2D* h2nPhoton = new TH2D("h2nPhoton", "Number of Photons; Number of photons [Generated]; Number of photons [Reconstructed]", 20, -1, 19, 20, -1, 19);   
    
    cout << "Total events:"<< tree->GetEntries() <<endl;

    for(Int_t evt = 0; evt < tree->GetEntries(); ++evt)
    {
	tree->GetEntry(evt);

	if(evt%500 == 0)
	    cout << "Events processed:"<< evt <<endl;


	trgBitStr = to_string(trgBit);
	for(Int_t ii = 1; ii < 10; ++ii)
	{
	    if(trgBitStr[ii] == '1')
		h1TrigTypes->Fill(2*ii);
	}	

	// if(trgBitStr[4] != '1') //Lg-bs1
	//     continue;

	// if(trgBitStr[8] != '1') //Fms-JP1
	//     continue;
	
	//cout << "Event id:" << pyEvent->eventId() <<endl;
	nPhotonPy = 0;
	for(Int_t i = 0; i < pyEvent->numberOfParticles(); ++i)
	{
	    const TParticle *part = pyEvent->particle(i);

	    if(part->GetPdgCode() !=  22)
	    	continue;
	    
	    if(part->Eta() < 2.5 || part->Eta() > 4.5)
		continue;

	    if(part->Energy() < Emin || part->Energy() > Emax)
	    	continue;
	    
	    h1EngPy->Fill(part->Energy());
	    h1PtPy->Fill(part->Pt());
	    h1EtaPy->Fill(part->Eta());
	    h1PhiPy->Fill(part->Phi() - TMath::Pi());

	    ++nPhotonPy;
	}	
	h1nPhotonPy->Fill(nPhotonPy);

	nPhotonMu = 0;
	for(Int_t i = 0; i < fmsPointArr->GetEntriesFast(); ++i)
	{
	    fmsPointData = (TStFmsPointData*)fmsPointArr->At(i);
	    
	    if(fmsPointData->GetEta() < 2.5 || fmsPointData->GetEta() > 4.5)
		continue;

	    if(fmsPointData->GetE() < Emin || fmsPointData->GetE() > Emax)
	    	continue;
	    
	    h1EngMu->Fill(fmsPointData->GetE());
	    h1PtMu->Fill(fmsPointData->GetPt());
	    h1EtaMu->Fill(fmsPointData->GetEta());
	    h1PhiMu->Fill(fmsPointData->GetPhi());
	    ++nPhotonMu;	    
	}
	h1nPhotonMu->Fill(nPhotonMu);

	h2nPhoton->Fill(nPhotonPy, nPhotonMu);
    }

    const Int_t nCanvas = 6;
    TCanvas *c1[nCanvas];
    
    c1[0] = new TCanvas("c0");
    h1EngPy->SetLineColor(kBlue);
    c1[0]->SetLogy();
    h1EngPy->Draw();
    h1EngMu->Draw("samee");

    c1[1] = new TCanvas("c1");
    c1[1]->SetLogy();
    h1PtPy->SetLineColor(kBlue);
    h1PtPy->Draw();
    h1PtMu->Draw("samee");

    c1[2] = new TCanvas("c2");
    h1EtaPy->SetLineColor(kBlue);
    h1EtaPy->Draw();
    h1EtaMu->Draw("samee");

    c1[3] = new TCanvas("c3");
    h1PhiPy->SetLineColor(kBlue);
    h1PhiPy->Draw();
    h1PhiMu->Draw("samee");

    c1[4] = new TCanvas("c4");
    h1nPhotonPy->SetLineColor(kBlue);
    h1nPhotonPy->Draw();
    h1nPhotonMu->Draw("samee");

    c1[5] = new TCanvas("c5");    
    h2nPhoton->Draw("colz");

    for(Int_t i = 0; i < nCanvas; ++i)
	c1[i]->Write();
    
    file->Write();
}


