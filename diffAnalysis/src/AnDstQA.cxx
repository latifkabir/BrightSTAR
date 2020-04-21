// Filename: AnDstQA.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Nov 21 13:00:15 2019 (-0500)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "TStRunList.h"
#include "BrContainers/TStFmsPointPairData.h"
#include "BrContainers/TStEventData.h"
#include "BrContainers/TStRpsTrackData.h"

using namespace std;


void AnDstQA(Int_t firstRun, Int_t lastRun, TString outName, TString inFilePrefix)
{
    //--------------------------------- Naive way (Geometric cut) to get rid of hot channels -----------------
   TCutG *cutg1 = new TCutG("CUTG1",6);
   cutg1->SetPoint(0,-17.0798,36.0455);
   cutg1->SetPoint(1,16.0218,36.6951);
   cutg1->SetPoint(2,16.3241,8.43687);
   cutg1->SetPoint(3,-5.44135,8.43687);
   cutg1->SetPoint(4,-17.3821,36.6951);
   cutg1->SetPoint(5,-17.0798,36.0455);


   TCutG *cutg2 = new TCutG("CUTG2",8);
   cutg2->SetPoint(0,-65.1451,62.3549);
   cutg2->SetPoint(1,-39.7521,37.3447);
   cutg2->SetPoint(2,-46.1004,29.2245);
   cutg2->SetPoint(3,-68.7727,56.8331);
   cutg2->SetPoint(4,-64.5405,61.7052);
   cutg2->SetPoint(5,-64.8428,61.7052);
   cutg2->SetPoint(6,-64.8428,61.7052);
   cutg2->SetPoint(7,-65.1451,62.3549);
   //-------------------------------------
    
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList(firstRun, lastRun);
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    TString fileName;
    TString filePrefix = inFilePrefix;

    //Histograms
    TFile *outFile = new TFile(outName, "RECREATE");
    TH1D *hist1West = new TH1D("trkPwest", "West RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2West = new TH1D("trkPtWest", "West RP trk Pt; RP track P_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3West = new TH1D("trkEtaWest", "West RP trk Eta; RP Track #Eta", 200, -10, 10);
    TH1D *hist4West = new TH1D("trkPhiWest", "West RP trk Phi; RP track #phi [mrad]", 200, -3, 3);
    TH1D *hist5West = new TH1D("trkXiWest", "West RP trk Xi; RP track #xi", 200, -100, 20);

    TH1D *hist1East = new TH1D("trkPeast", "East RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2East = new TH1D("trkPtEast", "East RP trk Pt; RP track P_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3East = new TH1D("trkEtaEast", "East RP trk Eta; RP Track #eta", 200, -10, 10);
    TH1D *hist4East = new TH1D("trkPhiEast", "East RP trk Phi; RP track #phi [mrad]", 200, -3, 3);
    TH1D *hist5East = new TH1D("trkXiEast", "East RP trk Xi; RP track #xi", 200, -100, 20);
    
    TH1D *hist9 = new TH1D("trkP", " RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist10 = new TH1D("trkPt", " RP trk Pt; RP track Pt [GeV/c]", 200, 0, 2);

    TH1D *hist6 = new TH1D("pi0M_max", "Diphoton mass with highest energy pair; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);
    TH1D *hist11 = new TH1D("pi0M", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);
    TH1D *hist12 = new TH1D("pi0E", "Diphoton Energy; E_{#gamma#gamma} [GeV]", 200, 10, 100);
    TH2D *hist2d5 = new TH2D("pionXY", "#pi^{0} position; X [cm]; Y[cm]", 100, -100, 100, 100, -100, 100);
    TH2D *hist2d6 = new TH2D("pionXY_max", "#pi^{0} position with highest energy pair; X [cm]; Y[cm]", 100, -100, 100, 100, -100, 100);
    
    TH1D* hist13 = new TH1D("tofMult", "TOF Multiplicity", 250, 0, 500);
    TH1D* hist14 = new TH1D("bbcSumL_west", "West Large BBC ADC Sum", 250, 0, 8000);
    TH1D* hist15 = new TH1D("bbcSumS_west", "West Small BBC ADC Sum", 250, 0, 45000);
    TH1D* hist16 = new TH1D("trigger", "Trigger Distribution", 1000, 480000, 481000);
        
    //Input data     
    TTree *tree;
    Int_t nEntries = 0;
    TClonesArray *fmsArr = new TClonesArray("TStFmsPointPairData");
    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");
    TStEventData *event = new TStEventData();
    TStFmsPointPairData *pion;
    TStRpsTrackData *rpsTrack;
    Int_t nPions = 0;
    Int_t nRpsTracks = 0;
        
    cout << "Total number of files to be processed: "<< maxRuns <<endl;

    //------------ Loop over runs --------------------------
    for (Int_t run_i = 0; run_i < maxRuns; ++run_i)
    {
	run = runList->GetEntry(run_i);
	fileName = filePrefix;
	fileName += run;
	fileName += ".root";

	if(gSystem->AccessPathName(fileName))
	{
	    cout << "No DST file for run number "<< run <<" ... SKIPPED."<<endl;
	    continue;
	}
	cout << "Processing run number: "<< run <<endl;
	cout << "Percent completed: "<< (Int_t)(nRunsDone*100.0 / maxRuns) << "%"<<endl;
	
	TFile *file = new TFile(fileName);
	tree = (TTree*)file->Get("T");
	nEntries = tree->GetEntries();
	cout << "Total number of entries: "<< nEntries <<endl;

	tree->SetBranchAddress("event", &event);
	tree->SetBranchAddress("fmsPointPair", &fmsArr);
	tree->SetBranchAddress("rpTrack", &rpsArr);
	//----------- Loop over Events ---------------------
	for(Int_t evt = 0; evt < nEntries; ++evt)
	{
	    // fmsArr->Clear(); //This is automatically called unless you turn off auto delete. 
	    // rpsArr->Clear(); //This is automatically called unless you turn off auto delete.
	    // Also Clear Event  buffer if it contains TClonesArray as member

	    tree->GetEntry(evt);

	    //----- Event Info ---------
	    hist13->Fill(event->GetTofMultiplicity());
	    hist14->Fill(event->GetBbcSumLarge(1)); //West Large BBC
	    hist15->Fill(event->GetBbcSumSmall(1));      //West small BBC

	    for(Int_t t = 0; t < event->GetNtrigs(); ++t)
		hist16->Fill(event->GetTrigger(t));
	    
	    //----- RP Tracks ----------
	    nRpsTracks = rpsArr->GetEntriesFast();	    
	    for(Int_t trk = 0; trk < nRpsTracks; ++trk)
	    {
		rpsTrack = (TStRpsTrackData*)rpsArr->At(trk);

		hist9->Fill(rpsTrack->GetP());
		hist10->Fill(rpsTrack->GetPt());
		
		if(rpsTrack->GetBranch() == 0 || rpsTrack->GetBranch() == 1) //East RP :: O East Up RP, 1: East Down RP
		{		
		    hist1East->Fill(rpsTrack->GetP());		
		    hist2East->Fill(rpsTrack->GetPt());
		    hist3East->Fill(rpsTrack->GetEta());
		    hist4East->Fill(rpsTrack->GetPhi());
		    hist5East->Fill(rpsTrack->GetXi());		
		}

		if(rpsTrack->GetBranch() == 2 || rpsTrack->GetBranch() == 3) //West RP :: 2: West Up RP, 3: West Down RP
		{
		    hist1West->Fill(rpsTrack->GetP());		
		    hist2West->Fill(rpsTrack->GetPt());
		    hist3West->Fill(rpsTrack->GetEta());
		    hist4West->Fill(rpsTrack->GetPhi());
		    hist5West->Fill(rpsTrack->GetXi());
		}
		
	    }
	    //---------- FMS Pion candidates ---------------
	    nPions = fmsArr->GetEntriesFast();
	    for(Int_t pi = 0; pi < nPions; ++pi)
	    {
		pion = (TStFmsPointPairData*)fmsArr->At(pi);

		if(pion->GetZgg() > 0.8)
		    continue;
	    
		if(pion->GetE() < 12 || pion->GetE() > 70)
		    continue;

		if(cutg1->IsInside(pion->GetX(), pion->GetY()) || cutg2->IsInside(pion->GetX(), pion->GetY()))
		    continue;
		    
		hist11->Fill(pion->GetM());
		hist12->Fill(pion->GetE());
		hist2d5->Fill(pion->GetX(), pion->GetY());

		if(pi == 0)
		{
		    hist6->Fill(pion->GetM());
		    hist2d6->Fill(pion->GetX(), pion->GetY());
		}
	    }
	}		
	//-----------
	
	++nRunsDone;
	file->Close();
	delete file;
    }

    outFile->Write();
}
