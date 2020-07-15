// Filename: EjReadAnalysisTree.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jun 11 13:08:16 2020 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"
#include "BrJetMaker/TStJetSkimEvent.h"
#include <iostream>

using namespace std;

void EjReadAnalysisTree()
{
    TFile *file = new  TFile("emJet.root");
    TTree *tree = (TTree*)file->Get("T");

    if(!tree)
    {
	cout << "Could not retrieve TTree" <<endl;
	return;
    }
    
    TStJetEvent *jetEvent = 0;
    TStJetSkimEvent *event;
    TStJetCandidate *jet;
    TStJetTower *tower;
    TStJetParticle *particle;
    
    tree->SetBranchAddress("jetEvents", &jetEvent);

    TH1D *h1nJets = new TH1D("h1nJets", "Number of Jets", 10, 0, 10);
    TH1D *h1TowerEta = new TH1D("h1TowerEta", "Tower Eta", 100, 0, 0);
    
    for(Int_t evt = 0; evt < tree->GetEntries(); ++evt)
    {
	tree->GetEntry(evt);
	cout << "No of jets: " << jetEvent->GetNumberOfJets() <<endl;
	h1nJets->Fill(jetEvent->GetNumberOfJets());
	
	for(Int_t j = 0; j <  jetEvent->GetNumberOfJets(); ++j)
	{
	    jet = jetEvent->GetJet(j);

	    event = jetEvent->GetEvent();
	    cout << "Jet "<< j <<": Number of Tower: "<< jet->GetNumberOfTowers() <<endl;
	    cout << "Unix time:"<< event->GetUnixTime() <<endl;
	    
	    for(Int_t k = 0; k < jet->GetNumberOfTowers(); ++k)
	    {
		tower = jet->GetTower(k);
		cout << "Tower: "<< k << " Eta: "<< tower->GetEta() <<endl;
		h1TowerEta->Fill(tower->GetEta());
	    }

	    for(Int_t k = 0; k < jet->GetNumberOfParticles(); ++k)
	    {
		particle = jet->GetParticle(k);
		cout << "Particle: "<< k << " Eta: "<< particle->GetEta() <<endl;
	    }	    
	}
    }

    TCanvas *c1 = new TCanvas();
    h1nJets->Draw();
    TCanvas *c2 = new TCanvas();
    h1TowerEta->Draw();
}
