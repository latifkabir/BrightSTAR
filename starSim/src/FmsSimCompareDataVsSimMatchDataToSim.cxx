// Filename: FmsSimCompareDataVsSim.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Sep  2 13:47:32 2020 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "StRootInclude.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"
#include "BrJetMaker/TStJetSkimEvent.h"
#include <iostream>

using namespace std;

/*

In practice, you are expected to generate as many events is simulated pt bin as many events you have in data pt bins. In FMS simulation, since we have less events in simulation, so we are matching number of events in data to that of simulation for different pt bins. This is just to check how data compares with simulation in this approach. 

*/

void FmsSimCompareDataVsSimMatchDataToSim(TString simFile, TString dataFile, TString trigger)
{
    gStyle->SetOptStat(10);

    if(!(trigger == "sm-bs1" || trigger == "lg-bs1" || trigger == "jp0" || trigger == "jp1"))
    {
	cout << "Invalid trigger" <<endl;
	return;
    }
    
    TFile *outFile = new TFile("fmsJetDataVsSim.root", "recreate");

    TString det = "fms";
    Double_t etaMin;
    Double_t etaMax;
    Double_t detZ; //For FMS
    if(det == "fms")
    {

	// etaMin = 2.8;
	// etaMax = 3.9;
	
	etaMin = 2.0;
	etaMax = 4.5;
	detZ = 735.; //For FMS
    }
    else if(det == "eemc")
    {
	etaMin = 0.8;
	etaMax = 2.5;
	detZ = kEEmcZSMD; //For EEMC
    }
    else
    {
	cout << "Invalid detector" <<endl;
	return;
    }
    
    //------------------------------------------- Simulation ----------------------------------------
    TChain* chSim = new TChain("jet");

    Int_t nFiles_jet;
    nFiles_jet = chSim->Add(simFile);

    Int_t nEvents = chSim->GetEntries();

    cout << "Number of sim files added: "<<nFiles_jet <<endl;
    cout << "Total sim Entries to be processed: "<< nEvents <<endl;
    chSim->ls();

    // Set jet buffer
    StJetEvent* jetEventSim = 0;
    chSim->SetBranchAddress("AntiKtR070NHits12",&jetEventSim);

    
    //-- All Triggers -------
    TH1D *h1TrigTypes_s = new TH1D ("h1TrigTypes_s", "Trigger Types; Trigger Types", 30, 0, 30);

    h1TrigTypes_s->GetXaxis()->SetBinLabel(3,"Small BS1");
    h1TrigTypes_s->GetXaxis()->SetBinLabel(5,"Small BS2");
    h1TrigTypes_s->GetXaxis()->SetBinLabel(7,"Small BS3");
    h1TrigTypes_s->GetXaxis()->SetBinLabel(9,"Large BS1");
    h1TrigTypes_s->GetXaxis()->SetBinLabel(11,"Large BS2");
    h1TrigTypes_s->GetXaxis()->SetBinLabel(13,"Large BS3");
    h1TrigTypes_s->GetXaxis()->SetBinLabel(15,"FMS-JP0");
    h1TrigTypes_s->GetXaxis()->SetBinLabel(17,"FMS-JP1");
    h1TrigTypes_s->GetXaxis()->SetBinLabel(19,"FMS-JP2");
    h1TrigTypes_s->GetXaxis()->SetBinLabel(21,"None");
    
    TH1D *h1nJets_s = new TH1D ("h1nJets_s", "Number of Jets; No. of Jets", 10, -1, 9);

    TH1D *h1nVtx_s = new TH1D("h1nVtx_s", "Number of Vertices", 5, 0, 5);
    TH1D* h1Vtxz_s = new TH1D("h1Vtxz_s", "Vtx z Distribution [All Sources]; Vertex z [cm]",100, -200, 200);
    
    TH1D *h1JetEta_s = new TH1D ("h1JetEta_s", "EM Jet Eta; Jet #eta", 100, 1.0, 5.0);
    TH1D *h1JetPhi_s = new TH1D ("h1JetPhi_s", "EM Jet Phi; Jet #phi [rad]", 100, -3.2, 3.2);
    TH1D *h1JetE_s = new TH1D ("h1JetE_s", "EM Jet E; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetE_sm_s = new TH1D ("h1JetE_sm_s", "EM Jet E [small cells]; Jet E [GeV]", 100, 0.0, 85.0);
    TH1D *h1JetE_lg_s = new TH1D ("h1JetE_lg_s", "EM Jet E [large cells]; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetPt_s = new TH1D ("h1JetPt_s", "Jet Pt; Jet Pt [GeV/c]", 20, 0.0, 20.0);
    TH1D *h1JetVtxZ_s = new TH1D ("h1JetVtxZ_s", "Jet Vtx z; Jet vtx z [cm]", 100, -200.0, 200.0);
    TH1D *h1nPhotons_s = new TH1D("h1nPhotons_s", "number of photons in EM jets; Number of photons", 20, 0, 20);
        
    Double_t eta;
    Double_t phi;
    Double_t E;
    Double_t pt;
    Double_t z_vtx;
    Double_t jetX;
    Double_t jetY;
    Double_t theta;
    Int_t trigBit;
    string trgBitStr;
    StJetParticle *part;
    Int_t vtxType = -1;
    Double_t vtxZ = -999;
    //----- Event loop ----------
    //Loads entries from Jet Tree and Skim Tree simultaneously
    for (int iEntry = 0; iEntry < nEvents; ++iEntry)
    {
	if (chSim->GetEvent(iEntry) <= 0) break;

	// Should not be null
	assert(jetEventSim);

	if (iEntry % 1000 == 0) cout << iEntry << endl;
	
	// if(jetEvent->numberOfJets() == 0)
	//     continue;

	StJetVertex* vertex = jetEventSim->vertex(0);
	if (!vertex) continue;
	
	// if(vertex->ranking() < 0) continue; //Not applicable for EM jet	
	// if(fabs(vertex->position().z()) > 80.0) continue;

	//Trigger Types
	trigBit = jetEventSim->runId();

	trgBitStr = to_string(trigBit);
	for(Int_t ii = 1; ii < 10; ++ii)
	{
	    if(trgBitStr[ii] == '1')
		h1TrigTypes_s->Fill(2*ii);
	}	
	
	if(trigBit == 1000000000)
	{
	    h1TrigTypes_s->Fill(2*10);
	    continue;
	}

	if(trigger == "sm-bs1")
	{
	    if(trgBitStr[1] != '1') //sm-bs1
		continue;
	}
	else if(trigger == "lg-bs1")
	{
	    if(trgBitStr[4] != '1') //Lg-bs1
		continue;
	}
	else if(trigger == "jp0")
	{
	    if(trgBitStr[7] != '1') //Fms-JP0
		continue;
	}
	else if(trigger == "jp1")
	{
	    if(trgBitStr[8] != '1') //Fms-JP1
		continue;
	}

	
	if(jetEventSim->numberOfJets() == 0)
	    continue;
	
	vtxType = -1;
	vtxZ = vertex->position().z();
	h1nVtx_s->Fill(jetEventSim->numberOfVertices());		
	h1nJets_s->Fill(jetEventSim->numberOfJets());

	//It seems most people loop over vertices. Find the justification.
	//I am looping over number of jets
	for(Int_t i = 0; i < jetEventSim->numberOfJets(); ++i) 
	{
	    eta = jetEventSim->jet(i)->eta();
	    phi = jetEventSim->jet(i)->phi();
	    E = jetEventSim->jet(i)->E();
	    pt = jetEventSim->jet(i)->pt();
	    z_vtx = jetEventSim->jet(i)->vertex()->position().z();
	    theta =  2 * atan( exp(-eta) );
	    jetX = (735. - z_vtx) * tan(theta) * cos(phi);
	    jetY = (735. - z_vtx) * tan(theta) * sin(phi);

	    if(eta < etaMin  || eta > etaMax) //Consider FMS EM Jets only (for trigger specific plots)
		continue;
	    if(pt < 2)
		continue;
	    h1JetEta_s->Fill(eta);
	    h1JetPhi_s->Fill(phi);
	    h1JetPt_s->Fill(pt);
	    h1JetVtxZ_s->Fill(z_vtx); // no vtx found case i.e. vtx = 0 will dominate if all types filled together

	    h1JetE_s->Fill(E);
	    if(eta > 2.0 && eta < 3.15) //large cells
		h1JetE_lg_s->Fill(E); 
	    if(eta > 3.15 && eta < 4.0)  //small cells
		h1JetE_sm_s->Fill(E);

	    if(jetEventSim->jet(i)->numberOfTowers()>0) //<------------------------TEST only
		h1nPhotons_s->Fill(jetEventSim->jet(i)->numberOfTowers());
	
	}	
    } // End event loop


    //-------------------------------- Data --------------------------------------------------
    TChain *chData = new TChain("T");
    chData->Add(dataFile);
    
    TStJetEvent *jetEventData = 0;
    TStJetSkimEvent *skimEventData;
    TStJetCandidate *jet;
    TStJetTower *tower;
    TStJetParticle *particle;
    
    chData->SetBranchAddress("jetEvents", &jetEventData);
    
    TH1D *h1nJets_all_d = new TH1D("h1nJets_all_d", "Number of Jets from All [TPC/BEMC + EEMC + FMS]", 10, 0, 10);
    TH1D *h1nJets_d  = new TH1D("h1nJets_d ", "Number of Jets [FMS or EEMC]", 10, 0, 10);
    TH1D *h1TrigType_d = new TH1D("h1TrigType_d", "Trigger Type; Trigger Type", 10, 0, 10);
    
    TH1D *h1TowerEta_d = new TH1D("h1TowerEta_d", "Tower Eta", 100, 1.0, 4.5);
    TH1D *h1TowerPhi_d = new TH1D("h1TowerPhi_d", "Tower Phi", 100, -3.2, 3.2);
    TH1D *h1TowerE_d = new TH1D("h1TowerE_d", "Tower E", 100, 0, 100);

    TH1D *h1JetEta_d = new TH1D ("h1JetEta_d", "EM Jet Eta; Jet #eta", 100, 1.0, 5.0);
    TH1D *h1JetPhi_d = new TH1D ("h1JetPhi_d", "EM Jet Phi; Jet #phi [rad]", 100, -3.2, 3.2);
    TH1D *h1JetE_d = new TH1D ("h1JetE_d", "EM Jet E; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetE_sm_d = new TH1D ("h1JetE_sm_d", "EM Jet E [small cells]; Jet E [GeV]", 100, 0.0, 85.0);
    TH1D *h1JetE_lg_d = new TH1D ("h1JetE_lg_d", "EM Jet E [large cells]; Jet E [GeV]", 100, 0.0, 70.0);    
    TH1D *h1JetPt_d = new TH1D ("h1JetPt_d", "Jet Pt; Jet Pt [GeV/c]", 20, 0.0, 20.0);
    TH1D *h1nPhotons_d = new TH1D("h1nPhotons_d", "number of photons in EM jets; Number of Photons", 20, 0, 20);
    TH1D *h1vtxZ_d = new TH1D("h1vtxZ_d", "Jet vetrex z; Jet vertex z [cm]", 100, -200, 200);

    TH1D *h1JetPt_dummy = new TH1D ("h1JetPt_dummy", "Jet Pt; Jet Pt [GeV/c]", 20, 0.0, 20.0);
    
    Double_t eng;
    Int_t nJets = 0;
    Bool_t doSkipEvent;
    
    cout << "Total Entries to be processed: "<< chData->GetEntries() <<endl;

    for(Int_t evt = 0; evt < chData->GetEntries(); ++evt)
    {
	h1JetPt_dummy->Reset();
	for(Int_t r = 1; r <= h1JetPt_dummy->GetNbinsX(); ++r)
	    h1JetPt_dummy->SetBinContent(r, h1JetPt_d->GetBinContent(r));
	
	doSkipEvent = kFALSE;
	
	chData->GetEntry(evt);

	if(evt % 1000 == 0)
	    cout << "Data Events Processed: "<< evt <<endl;
	
	skimEventData = jetEventData->GetEvent();
		
	for(Int_t t = 0; t < 9; ++t)
	{
	    if(skimEventData->GetTrigFlag(t))
		h1TrigType_d->Fill(t);
	}

	if(trigger == "sm-bs1")
	{
	    if(!skimEventData->GetTrigFlag(3)) //sm-bs1 Trigger
		continue;
	}
	else if(trigger == "lg-bs1")
	{
	    if(!skimEventData->GetTrigFlag(6)) //lg-bs1 Trigger
		continue;
	}
	else if(trigger == "jp0")
	{
	    if(!skimEventData->GetTrigFlag(0)) //JP0 Trigger
		continue;
	}
	else if(trigger == "jp1")
	{
	    if(!skimEventData->GetTrigFlag(1)) //JP1 Trigger
		continue;
	}
	
	nJets = 0;	
	for(Int_t j = 0; j <  jetEventData->GetNumberOfJets(); ++j)
	{
	    jet = jetEventData->GetJet(j);
	   	    
	    eta = jet->GetEta();
	    phi = jet->GetPhi();
	    theta =  2 * atan( exp(-eta) );

	    if(eta < etaMin || eta > etaMax) //Conside only EEMC and FMS coverage
		continue;
	    
	    eng = jet->GetE();
	    pt = jet->GetPt();
	    if(pt< 2)
		continue;
	    
	    h1JetPt_dummy->Fill(pt);
	    for(Int_t r = 1; r <= h1JetPt_dummy->GetNbinsX(); ++r)
	    {
		if(h1JetPt_dummy->GetBinContent(r) > h1JetPt_s->GetBinContent(r))
		{
		    doSkipEvent = kTRUE;
		    break;
		}		
	    }
	    if(doSkipEvent)
		continue;

	    ++nJets;
	    if(j == 0)
		h1vtxZ_d->Fill(vtxZ);
	    
	    h1nPhotons_d->Fill(jet->GetNumberOfTowers());
	    h1JetEta_d->Fill(eta);
	    h1JetPhi_d->Fill(phi);
	    h1JetPt_d->Fill(pt);
	    h1JetE_d->Fill(eng);
	           
	    jetX = (detZ - vtxZ) * tan(theta) * cos(phi);
	    jetY = (detZ - vtxZ) * tan(theta) * sin(phi);
	    
	    if(det == "fms")  //actual range 2.65 - 3.9
	    {	
		if(eta > 2.0 && eta < 3.15) //FMS large cells
		    h1JetE_lg_d->Fill(eng); 
		if(eta > 3.15 && eta < 4.0)  //FMS small cells
		    h1JetE_sm_d->Fill(eng);		
	    }
	    	    
	    for(Int_t k = 0; k < jet->GetNumberOfTowers(); ++k)
	    {
		tower = jet->GetTower(k);
	
		h1TowerEta_d->Fill(tower->GetEta());
		h1TowerPhi_d->Fill(tower->GetPhi());
		h1TowerE_d->Fill(tower->GetEnergy());
	    }
	}
	//Note: For data, we save events only if there is non-zero number of jets
	if(nJets > 0)
	    h1nJets_d->Fill(nJets); //this is must for EEMC
    }

    //------------------------------- Comparison ----------------------------------------------    
    TCanvas *c[10];

    TLegend *legend = new TLegend(0.1,0.7,0.48,0.9);
    legend->AddEntry(h1nJets_d,"Data","lep");
    legend->AddEntry(h1nJets_s,"Simulation","l");
    
    c[0] = new TCanvas("c1");
    h1nJets_d->SetLineColor(kBlue);
    h1nJets_d->SetMarkerColor(kBlue);
    h1nJets_d->Draw("e");
    h1nJets_s->Draw("same");
    legend->Draw();
    
    c[1] = new TCanvas("c2");
    h1JetE_d->SetLineColor(kBlue);
    h1JetE_d->SetMarkerColor(kBlue);
    h1JetE_s->Draw();
    h1JetE_d->Draw("samee");
    legend->Draw();
    
    c[2] = new TCanvas("c3");
    h1JetPt_d->SetLineColor(kBlue);
    h1JetPt_d->SetMarkerColor(kBlue);
    h1JetPt_s->Draw();
    h1JetPt_d->Draw("samee");
    legend->Draw();
    
    c[3] = new TCanvas("c4");
    h1JetEta_d->SetLineColor(kBlue);
    h1JetEta_d->SetMarkerColor(kBlue);
    h1JetEta_s->Draw();
    h1JetEta_d->Draw("samee");
    legend->Draw();
    
    c[4] = new TCanvas("c5");
    h1JetPhi_d->SetLineColor(kBlue);
    h1JetPhi_d->SetMarkerColor(kBlue);
    h1JetPhi_s->Draw();
    h1JetPhi_d->Draw("samee");
    legend->Draw();
    
    c[5] = new TCanvas("c6");
    h1JetE_sm_d->SetLineColor(kBlue);
    h1JetE_sm_d->SetMarkerColor(kBlue);
    h1JetE_sm_s->Draw();
    h1JetE_sm_d->Draw("samee");
    legend->Draw();
    
    c[6] = new TCanvas("c7");
    h1JetE_lg_d->SetLineColor(kBlue);
    h1JetE_lg_d->SetMarkerColor(kBlue);
    h1JetE_lg_s->Draw();
    h1JetE_lg_d->Draw("samee");
    legend->Draw();
    
    c[7] = new TCanvas("c8");
    h1nPhotons_d->SetLineColor(kBlue);
    h1nPhotons_d->SetMarkerColor(kBlue);
    h1nPhotons_s->Draw();
    h1nPhotons_d->Draw("samee");
    legend->Draw();
    
    for(Int_t i = 0; i < 8; ++i)
    	c[i]->Write();
    
    outFile->Write();    
}
