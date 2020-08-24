// Filename: ReadPythiaEvent.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Jul 31 02:55:40 2020 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"

void ReadPythiaEvent(TString fileList) // Use wildcat to combine files
{
    TChain *tree = new TChain("PythiaTree");
    tree->Add(fileList);

    StPythiaEvent *pyEvent = 0;
    tree->SetBranchAddress("PythiaBranch", &pyEvent);

    TFile *file = new TFile("PythiaEeventHist.root", "recreate");
    TH1D* h1Eng = new TH1D("h1Eng", "Energy", 120, 0, 120);
    TH1D* h1Pt = new TH1D("h1Pt", "Pt", 100, 0, 25);
    TH1D* h1Eta = new TH1D("h1Eta ", "Eta", 100, 2.5, 4.5);
    TH1D* h1nPhoton = new TH1D("h1nPhoton", "Number of Photon", 20, -1, 19);
    Int_t nPhoton;

    cout << "Total events:"<< tree->GetEntries() <<endl;

    for(Int_t evt = 0; evt < tree->GetEntries(); ++evt)
    {
	tree->GetEntry(evt);
	if(!pyEvent)
	    continue;

	if(evt%500 == 0)
	    cout << "Events processed:"<< evt <<endl;
	
	//cout << "Event id:" << pyEvent->eventId() <<endl;
	nPhoton = 0;
	for(Int_t i = 0; i < pyEvent->numberOfParticles(); ++i)
	{
	    const TParticle *part = pyEvent->particle(i);

	    if(part->GetPdgCode() !=  22)
		continue;
	    
	    if(part->Eta() < 2.5 || part->Eta() > 4.5)
		continue;

	    if(part->Energy() < 2)
	    	continue;
	    
	    h1Eng->Fill(part->Energy());
	    h1Pt->Fill(part->Pt());
	    h1Eta->Fill(part->Eta());

	    ++nPhoton;
	}
	if(nPhoton)
	    h1nPhoton->Fill(nPhoton);
    }

    // TCanvas *c1 = new TCanvas();
    // h1Eng->Draw();
    // TCanvas *c2 = new TCanvas();
    // h1Eta->Draw();
    // TCanvas *c3 = new TCanvas();
    // h1nPhoton->Draw();
    
    file->Write();
}



void ReadMuDstEvent(TString fileList) //Use wildcat to combine files
{
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "");
    muDstMaker->SetStatus("*",0);
    muDstMaker->SetStatus("Fms*",1);
    
    StMuDst *mDst = muDstMaker->muDst();   // Get StMuDst
    StMuFmsCollection *fmsMuColl;
    StMuFmsHit *fmsHit;
    StMuFmsCluster *fmsCluster;
    StMuFmsPoint *fmsPoint;
    Int_t nPoints;

    TFile *file = new TFile("MuDstEeventHist.root", "recreate");
    TH1D* h1Eng = new TH1D("h1Eng", "Energy", 120, 0, 120);
    TH1D* h1Pt = new TH1D("h1Pt", "Pt", 100, 0, 25);
    TH1D* h1Eta = new TH1D("h1Eta ", "Eta", 100, 2.5, 4.5);
    TH1D* h1nPhoton = new TH1D("h1nPhoton", "Numbr of Photon", 20, -1, 19);
    Int_t nPhoton;
    
    TChain *ch = muDstMaker->chain();

    chain->Init();
    
    Int_t nEvents = ch->GetEntries(); 
    cout << "Total events:"<< nEvents <<endl;
    for (Int_t iev = 0; iev < nEvents; iev++)
    {
	if(iev % 500 == 0)
	    cout << "Events processed:"<< iev <<endl;
	
	chain->Clear();
	Int_t iret = chain->Make(iev);
	fmsMuColl = mDst->muFmsCollection();

	if(!fmsMuColl)
	{
	    cout<<"No FMS data for this event"<<endl;
	    continue;
	}
	nPhoton = 0;
	nPoints = fmsMuColl->numberOfPoints();
	for(Int_t i = 0; i < nPoints; ++i)
	{
	    fmsPoint = fmsMuColl->getPoint(i);

	    if(fmsPoint->xyz().pseudoRapidity() < 2.5 || fmsPoint->xyz().pseudoRapidity() > 4.5)
		continue;

	    if(fmsPoint->energy() < 2)
	    	continue;
	    
	    h1Eng->Fill(fmsPoint->energy());
	    h1Pt->Fill(fmsPoint->fourMomentum().perp());
	    h1Eta->Fill(fmsPoint->xyz().pseudoRapidity());
	    ++nPhoton;
	}
	if(nPhoton)
	    h1nPhoton->Fill(nPhoton);
    }
    chain->Finish();
    
    // TCanvas *c1 = new TCanvas();
    // h1Eta->Draw();
    // TCanvas *c2 = new TCanvas();
    // h1Eng->Draw();
    // TCanvas *c3 = new TCanvas();
    // h1nPhoton->Draw();

    file->Write();
    
}			


void FmsSimReadSimData(TString fileList)
{
    fileList.ReplaceAll("MuDst", "pythia");
    ReadPythiaEvent(fileList);
    fileList.ReplaceAll("pythia", "MuDst");
    ReadMuDstEvent(fileList);    
}
