#include "TChain.h"
#include "TMath.h"
#include "TSystem.h"
#include <iostream>
#include <fstream>
using namespace std;

//------------------------------------------------
void addFilesToChain(TChain& chain, string inList)
{
	int nFiles = 0;

	ifstream in;
	in.open(inList.c_str());
	while (in.is_open())
	{
		string inFile;
		in >> inFile;
		if (!in.good()) { break; in.close(); }

		//cout <<"Adding... " <<inFile.c_str() <<endl;
		chain.Add(inFile.c_str());
		nFiles++;
	}
	cout <<Form("Total # of files/events: %i/%i ", nFiles, chain.GetEntries());
	cout <<chain.GetName() <<endl;

	return;
}//addFileToChain

//-------------------
void readJetNew(void)
{
	gSystem->Load("StJetEvent.so");
	gSystem->Load("StJetSkimEvent.so");
	gSystem->Load("StUeEvent.so");

	TChain iChain[3] = {"jet", "jetSkimTree", "ue"};
	string inList[3];
	inList[0] = "inList_jet.txt";
	inList[1] = "inList_skim.txt";
	inList[2] = "inList_ue.txt";

	for (int i=0; i<3; i++) addFilesToChain(iChain[i], inList[i]);
	if ( (iChain[0].GetEntries() != iChain[1].GetEntries()) ||
		 (iChain[1].GetEntries() != iChain[2].GetEntries()) ||
		 (iChain[0].GetEntries() != iChain[2].GetEntries())	) { cout <<"Mismatching # of entries!" <<endl; return; }

	StJetEvent* evtJet  = new StJetEvent;
	StJetSkimEvent* evtSkim = new StJetSkimEvent;
	StUeOffAxisConesEvent* evtUe = new StUeOffAxisConesEvent;
	iChain[0].SetBranchAddress("AntiKtR070NHits12", &evtJet);
	iChain[1].SetBranchAddress("skimEventBranch", &evtSkim);
	iChain[2].SetBranchAddress("AntiKtR070NHits12OffAxisConesR070", &evtUe);

	//-------------------------------------------

	TFile* outFile = new TFile("out_fmsEmjet.root", "RECREATE");
	outFile->SetCompressionLevel(9);

	TH1F* H1_jEta    = new TH1F("jEta", ";eta", 55, -1.0, 4.5); H1_jEta->Sumw2();
	TH1F* H1_nJets   = new TH1F("nJets", ";nJet", 11, -0.5, 10.5); H1_nJets->Sumw2();
	TH1F* H1_nPoints = new TH1F("nPoints", ";# of points/jet", 26, -0.5, 25.5); H1_nPoints->Sumw2();
	TH1F* H1_vtxType = new TH1F("vtxType", ";Type of zVtx", 5, -0.5, 4.5); H1_vtxType->Sumw2();
	TH2F* H2_jetUePhi[2];
	TH1F* histVtxZ = new TH1F("histVtxZ", "Vtx Distribution",100, -200, 200);
	TH1F* histVtx0 = new TH1F("histVtx0", "Vtx Distribution",100, -200, 200);
	TH1F* histVtx1 = new TH1F("histVtx1", "Vtx Distribution",100, -200, 200);
	TH1F* histVtx2 = new TH1F("histVtx2", "Vtx Distribution",100, -200, 200);
	TH1F* histVtx3 = new TH1F("histVtx3", "Vtx Distribution",100, -200, 200);
	for (int i=0; i<2; i++)
	{
		const float PI = TMath::Pi();
		H2_jetUePhi[i] = new TH2F(Form("jetUePhi_%i", i), ";phi (jet);phi (ue)", 314,-PI,PI, 314,-PI,PI);
		H2_jetUePhi[i]->Sumw2();
	}

	//-------------------------------------------

	const int nEvents = iChain[0].GetEntries();
	for (int a=0; a<nEvents; a++)
	{
		if (a>1.e6) break;
		if (a>0 && a%(int)1.e5==0) cout <<"Processing... " <<a <<endl;
		for (int b=0; b<3; b++) iChain[b].GetEntry(a);

		//Check event sync by runId and eventId
		if ( evtJet->runId() != evtSkim->runId() ) //No runId in UE
		{
			cout <<Form("Mismatching runId! %i %i", evtJet->runId(), evtSkim->runId()) <<endl;
		}
		if ( (evtJet ->eventId() != evtSkim->eventId()) ||
			 (evtJet ->eventId() != evtUe  ->eventId()) ||
			 (evtSkim->eventId() != evtUe  ->eventId()) ) 
		{
			cout <<Form("Mismatching eventId! %i %i %i\n", evtJet->eventId(), evtSkim->eventId(), evtUe->eventId());
		}

		//zVtx match check btw jet/ue
		StJetVertex* vtxJet = evtJet->vertex(0);
		StUeVertex*  vtxUe  = evtUe ->vertex(0);
		if (fabs(vtxJet->position().z() - vtxUe->position().z()) > 1.e-5) cout <<"Mismatching zVtx!" <<endl;

		//---------------------------------------

		//Get spin info
		const int bunch7  = evtSkim->bx7();
		const int bunch48 = evtSkim->bx48();
		const int spin4   = evtSkim->spin4usingBx48();

		//---------------------------------------

		//TPC (0), VPD (1), BBC (2), and None (3)
		int zVtxType = -1;
		if (evtJet->numberOfVertices()>1 || (vtxJet->position().x()!=0. && vtxJet->position().y()!=0.)) zVtxType = 0;
		else if (evtSkim->vpdZvertex() != -999.) zVtxType = 1;
		else if (vtxJet->position().z() != 0.) zVtxType = 2;
		else zVtxType = 3;

		H1_vtxType->Fill(zVtxType);
		histVtxZ->Fill(vtxJet->position().z());

		if(zVtxType == 0) histVtx0->Fill(vtxJet->position().z());
		if(zVtxType == 1) histVtx1->Fill(vtxJet->position().z());
		if(zVtxType == 2) histVtx2->Fill(vtxJet->position().z());
		if(zVtxType == 3) histVtx3->Fill(vtxJet->position().z());
		
		if (zVtxType == 0) continue; //Exclude TPC vertex events

		//---------------------------------------

		int nJetsActual = 0;
		const int nJets = vtxJet->numberOfJets();
		for (int b=0; b<nJets; b++)
		{
			StJetCandidate* jetCand = vtxJet->jet(b);

			const float jEta = jetCand->eta();
			//if (jEta<2.3 || jEta>4.2) continue;

			//Check # of towers (points)
			const int nTowers = jetCand->numberOfTowers();

			//-----------------------------------

			StUeOffAxisConesJet* ueCand = vtxUe->ueJet(b); //StUeOffAxisCOnesJet is inherited from StUeJet
			for (int c=0; c<2; c++) //Always 2 UE reconstructed WRT original jet: cw (-90) and ccw (+90)
			{
				StUeOffAxisCones* ueCone = ueCand->cone(c);
				H2_jetUePhi[c]->Fill(jetCand->phi(), ueCone->phi());
			}

			nJetsActual++;
			H1_jEta->Fill(jEta);
			H1_nPoints->Fill(nTowers);
		}//b, jets
		H1_nJets->Fill(nJetsActual);

	}//a, events

	outFile->Write();
	outFile->Close();
	return;
}//Main
