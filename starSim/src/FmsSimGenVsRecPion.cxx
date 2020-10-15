// Filename: FmsSimGenVsRecPion.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Jul 31 02:55:40 2020 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"

//Use this script to compare generated vs reconstructed pions
void FmsSimGenVsRecPion(TString simFileName, TString dataFileName, Double_t minE, Double_t maxE) // Use wildcat to combine files
{
    gStyle->SetOptStat(0);
    
    Double_t Emin = minE;//2.0;
    Double_t Emax = maxE; //500.0;

    //The range of eta was taken to have some fedutial cut, so that pythia particles would not be missed at the edges.
    Double_t etaMin = 2.5;
    Double_t etaMax = 4.4;
    
    TChain *tree = new TChain("PythiaTree");
    tree->Add(simFileName);

    StPythiaEvent *pyEvent = 0;
    TClonesArray *fmsPointArr = new TClonesArray("StMuFmsPoint");
    StMuFmsPoint *fmsPoint1;
    StMuFmsPoint *fmsPoint2;
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

    
    TH1D *h1PionMpy = new TH1D("h1PionMpy", "Diphoton Mass Distribution; M [GeV/c]", 100, 0.05, 0.4);
    TH1D *h1PionMrec = new TH1D("h1PionMrec", "Diphoton Mass Distribution; M [GeV/c]", 100, 0.05, 0.4);
    TH1D *h1PionMdat = new TH1D("h1PionMdat", "Diphoton Mass Distribution; M [GeV/c]", 100, 0.05, 0.4);
    Int_t nPionPy;
    Int_t nPionMu;
    TLorentzVector LV1;
    TLorentzVector LV2;
    
    cout << "Total events:"<< tree->GetEntries() <<endl;

    for(Int_t evt = 0; evt < tree->GetEntries(); ++evt)
    {
	tree->GetEntry(evt);

	if(evt%500 == 0)
	    cout << "Events processed:"<< evt <<endl;


	// trgBitStr = to_string(trgBit);
	// for(Int_t ii = 1; ii < 10; ++ii)
	// {
	//     if(trgBitStr[ii] == '1')
	// 	h1TrigTypes->Fill(2*ii);
	// }	

	// if(trgBitStr[1] != '1') //sm-bs1
	//     continue;
	
	// if(trgBitStr[4] != '1') //Lg-bs1
	//     continue;

	// if(trgBitStr[7] != '1') //Fms-JP0
	//     continue;
	
	// if(trgBitStr[8] != '1') //Fms-JP1
	//     continue;
	
	//cout << "Event id:" << pyEvent->eventId() <<endl;

	//----------------- Generated Pions --------------------------------	
	nPionPy = 0;
	for(Int_t i = 0; i < pyEvent->numberOfParticles(); ++i)
	{
	    const TParticle *part1 = pyEvent->particle(i);

	    if(part1->GetPdgCode() !=  22)
	    	continue;
	    
	    if(part1->Eta() < etaMin || part1->Eta() > etaMax)
		continue;

	    if(part1->Energy() < Emin || part1->Energy() > Emax)
	    	continue;

	    LV1.SetPtEtaPhiE(part1->Pt(), part1->Eta(), part1->Phi(), part1->Energy());
	    
	    for(Int_t j = 0; j < pyEvent->numberOfParticles(); ++j)
	    {
		if(i == j)
		    continue;
		
		const TParticle *part2 = pyEvent->particle(j);

		if(part2->GetPdgCode() !=  22)
		    continue;
	    
		if(part2->Eta() < etaMin || part2->Eta() > etaMax)
		    continue;

		if(part2->Energy() < Emin || part2->Energy() > Emax)
		    continue;


		LV2.SetPtEtaPhiE(part2->Pt(), part2->Eta(), part2->Phi(), part2->Energy());
		
		h1PionMpy->Fill((LV1 + LV2).M());
	    }
	}	


	//----------------- Reconstructed Pions ----------------------------------
	nPionMu = 0;
	Float_t zOff = 0.0; //90.0; //<--------------- This offset can be set from StFmsPointMaker 
	for(Int_t i = 0; i < fmsPointArr->GetEntriesFast(); ++i)
	{
	    fmsPoint1 = (StMuFmsPoint*)fmsPointArr->At(i);
	    
	    if(fmsPoint1->xyz().pseudoRapidity() < etaMin || fmsPoint1->xyz().pseudoRapidity() > etaMax)
		continue;

	    if(fmsPoint1->energy() < Emin || fmsPoint1->energy() > Emax)
	    	continue;

	    //LV1.SetPtEtaPhiE(fmsPoint1->fourMomentum().perp(), fmsPoint1->xyz().pseudoRapidity(), fmsPoint1->xyz().phi(), fmsPoint1->energy()); // This assumes vertex is 0 which is not necessarily correct
	    LV1.SetPxPyPzE(fmsPoint1->fourMomentum(0.0, zOff).px(), fmsPoint1->fourMomentum(0.0, zOff).py(), fmsPoint1->fourMomentum(0.0, zOff).pz(), fmsPoint1->fourMomentum(0.0, zOff).e()); 
	    
	    for(Int_t j = 0; j < fmsPointArr->GetEntriesFast(); ++j)
	    {
		if(i == j)
		    continue;
		
		fmsPoint2 = (StMuFmsPoint*)fmsPointArr->At(j);
	    
		if(fmsPoint2->xyz().pseudoRapidity() < etaMin || fmsPoint2->xyz().pseudoRapidity() > etaMax)
		    continue;

		if(fmsPoint2->energy() < Emin || fmsPoint2->energy() > Emax)
		    continue;

		//LV2.SetPtEtaPhiE(fmsPoint2->fourMomentum().perp(), fmsPoint2->xyz().pseudoRapidity(), fmsPoint2->xyz().phi(), fmsPoint2->energy()); // This assumes vertex is 0 which is not necessarily correct
		LV2.SetPxPyPzE(fmsPoint2->fourMomentum(0.0, zOff).px(), fmsPoint2->fourMomentum(0.0, zOff).py(), fmsPoint2->fourMomentum(0.0, zOff).pz(), fmsPoint2->fourMomentum(0.0, zOff).e()); 
		h1PionMrec->Fill((LV1 + LV2).M());
	    
	    }	    
	}
    }

    //------------------------------------- Pion mass from Jet tower of simulated data ---------------------------------------
    /*
    TChain *simCh = new TChain("jet");
    simCh->Add(dataFileName);
    StJetEvent* jetEvent = 0;
    simCh->SetBranchAddress("AntiKtR070NHits12",&jetEvent);

    StJetCandidate *jet;
    StJetTower *tower;
    Int_t nTowers = 0;
    cout << "Processing data files. No of events to be processed: "<< simCh->GetEntries() <<endl;

    for(Int_t evt = 0; evt < simCh->GetEntries(); ++evt)
    {
	if(evt %1000 == 0)
	    cout << "Events Processed: "<< evt <<endl;
	
	simCh->GetEntry(evt);

	StJetVertex* vertex = jetEvent->vertex(0);
	if (!vertex) continue;
	if(vertex->numberOfJets() == 0)
	    continue;
	
	jet = vertex->jet(0); //We mostly have single jet. Consider single jet cases
	nTowers = jet->numberOfTowers();
	for(Int_t t = 0; t < nTowers; ++t)
	{
	    tower = jet->tower(t);
	    if(tower->energy() < minE || tower->energy() > maxE)
		continue;

	    LV1.SetPtEtaPhiE(tower->pt(), tower->eta(), tower->phi(), tower->energy()); //This is not exactly correct as it considers vertex at 0

	    for(Int_t k = 0; k < nTowers; ++k)
	    {
		if(t == k)
		    continue;
		
		tower = jet->tower(k);
		if(tower->energy() < minE || tower->energy() > maxE)
		    continue;

		LV2.SetPtEtaPhiE(tower->pt(), tower->eta(), tower->phi(), tower->energy()); //This is not exactly correct as it considers vertex at 0

		h1PionMdat->Fill((LV1 + LV2).M());
	    }	   
	}	
    }
    */
        //------------------------------------- Pion mass from Jet tower of real data ---------------------------------------
    TChain *dataCh = new TChain("T");
    dataCh->Add(dataFileName);
    TStJetEvent* jetEventData = 0;
    dataCh->SetBranchAddress("jetEvents",&jetEventData);

    TStJetCandidate *jetData;
    TStJetTower *towerData;
    Int_t nTowersData = 0;
    cout << "Processing data files. No of events to be processed: "<< dataCh->GetEntries() <<endl;

    for(Int_t evt = 0; evt < dataCh->GetEntries(); ++evt)
    {
	if(evt %1000 == 0)
	    cout << "Events Processed: "<< evt <<endl;
	
	dataCh->GetEntry(evt);

	if(jetEventData->GetNumberOfJets() == 0)
	    continue;
	
	jetData = jetEventData->GetJet(0); //We mostly have single jet. Consider single jet cases
	nTowersData = jetData->GetNumberOfTowers();
	for(Int_t t = 0; t < nTowersData; ++t)
	{
	    towerData = jetData->GetTower(t);
	    if(towerData->GetEnergy() < minE || towerData->GetEnergy() > maxE)
		continue;

	    LV1.SetPtEtaPhiE(towerData->GetPt(), towerData->GetEta(), towerData->GetPhi(), towerData->GetEnergy()); //This is not exactly correct as it considers vertex at 0

	    for(Int_t k = 0; k < nTowersData; ++k)
	    {
		if(t == k)
		    continue;
		
		towerData = jetData->GetTower(k);
		if(towerData->GetEnergy() < minE || towerData->GetEnergy() > maxE)
		    continue;

		LV2.SetPtEtaPhiE(towerData->GetPt(), towerData->GetEta(), towerData->GetPhi(), towerData->GetEnergy()); //This is not exactly correct as it considers vertex at 0

		h1PionMdat->Fill((LV1 + LV2).M());
	    }	   
	}	
    }

    
    const Int_t nCanvas = 1;
    TCanvas *c1[nCanvas];
    TLegend *legend = new TLegend(0.1,0.7,0.48,0.9);
    // legend->AddEntry(h1PionMpy,"Generated","l");
    legend->AddEntry(h1PionMrec,"Reconstructed","lep");
    legend->AddEntry(h1PionMdat,"Data","ep");
    
    c1[0] = new TCanvas("c0");
    h1PionMpy->SetLineColor(kBlue);
    h1PionMdat->SetLineColor(kRed);
    // h1PionMpy->Draw();
    // h1PionMrec->DrawNormalized("samee", h1PionMpy->GetEntries());
    // h1PionMdat->DrawNormalized("samee", h1PionMpy->GetEntries());
    
    h1PionMrec->DrawNormalized();
    h1PionMdat->DrawNormalized("samee");
    
    legend->Draw();
    
    
    for(Int_t i = 0; i < nCanvas; ++i)
	c1[i]->Write();
    
    file->Write();
}


