// Filename: AnFmsRpCorr.C
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


void AnFmsRpCorr(Int_t firstRun, Int_t lastRun, TString outName, TString inFilePrefix)
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

    TH1D *hist6 = new TH1D("sumE_west_before", "West Sum E Before BBC and TOF Cut; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH1D *hist7 = new TH1D("sumE_west_singPion", "West Sum E (Single pion in FMS); E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH1D *hist18 = new TH1D("sumE_west_pion_cut", "West Sum E (Single pion in FMS); E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    
    TH1D *hist8West = new TH1D("sumE_west", "E_{p + #pi^{0}}^{West}; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH1D *hist8East = new TH1D("sumE_east", "E_{p + #pi^{0}}^{East}; E_{p + #pi^{0}}^{East} [GeV]", 100, 60, 200);
    
    TH1D *hist9 = new TH1D("trkP", " RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist10 = new TH1D("trkPt", " RP trk Pt; RP track Pt [GeV/c]", 200, 0, 2);

    TH1D *hist11 = new TH1D("pi0M", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);
    TH1D *hist12 = new TH1D("pi0E", "Diphoton Energy; E_{#gamma#gamma} [GeV]", 200, 10, 100);
    
    TH1D* hist13 = new TH1D("tofMult", "TOF Multiplicity", 250, 0, 500);
    TH1D* hist14 = new TH1D("bbcSumL_west", "West Large BBC ADC Sum", 250, 0, 8000);
    TH1D* hist15 = new TH1D("bbcSumS_west", "West Small BBC ADC Sum", 250, 0, 45000);
    TH1D* hist16 = new TH1D("trigger", "Trigger Distribution", 1000, 480000, 481000);
    TH1D* hist17 = new TH1D("evt_trigger", "Event Trigger Distribution", 1000, 480000, 481000);

    TH2D *hist2d1 = new TH2D("E_p_vs_E_pion", "E_{p}^{west} vs E_{#pi^{0}}; E_{#pi^{0}} [GeV]; E_{p}^{west} [GeV]", 100, 10, 80, 100, 60, 150);
    TH2D *hist2d2 = new TH2D("E_sum_vs_BBC_large", "E_{sum} vs BBC ADC Sum (Large); E_{p + #pi^{0}}^{west} [GeV]; BBC ADC Sum (Large)", 100, 50, 200, 300, 0, 6000);
    TH2D *hist2d3 = new TH2D("E_sum_vs_BBC_small", "E_{sum} vs BBC ADC Sum (Small); E_{p + #pi^{0}}^{west} [GeV]; BBC ADC Sum (Small)", 100, 50, 200, 300, 0, 4000);
    TH2D *hist2d4 = new TH2D("p_phi_vs_pion_phi", "#phi_{p} vs #phi_{#pi^{0}}; #phi_{#pi^{0}} [rad]; #phi_{p} [rad]", 100, -3.15, 3.15, 100, -3.15, 3.15);
    TH2D *hist2d5 = new TH2D("pionXY", "#pi^{0} position; X [cm]; Y[cm]", 100, -100, 100, 100, -100, 100);
    TH2D *hist2d6 = new TH2D("pionXY_max", "#pi^{0} position with highest energy pair; X [cm]; Y[cm]", 100, -100, 100, 100, -100, 100);

    TH1D *hEvtCount_all = new TH1D("hEvtCount_all", "Event Count", 20, 0, 20);
      
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

    //-----
    Int_t eastTrk_i =  -1;
    Int_t westTrk_i =  -1;
    Int_t nTrkEast = 0;
    Int_t nTrkWest = 0;
    Int_t fms_i = -1;
    Double_t sumE_w = 0.0;
    Int_t eventCount[5] = {0};
    
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
	    ++eventCount[0]; //Event Counter
	    
	    for(Int_t t = 0; t < event->mNtrig; ++t)
		hist16->Fill(event->mTriggers[t]);
	    
	    //----- RP Tracks ----------
	    eastTrk_i = -1;
	    nTrkEast = 0;
	    westTrk_i = -1;
	    nTrkWest = 0;
	    
	    nRpsTracks = rpsArr->GetEntriesFast();	    
	    for(Int_t trk = 0; trk < nRpsTracks; ++trk)
	    {
		rpsTrack = (TStRpsTrackData*)rpsArr->At(trk);

		//-------- RP Cuts ----------
		if (rpsTrack->mType != 1)
		    continue;
		if(!(rpsTrack->mTheta_x > -1.5 && rpsTrack->mTheta_x < 5.0)) 
		    continue;
		if(!(fabs(rpsTrack->mTheta_y) > 1.0 &&  fabs(rpsTrack->mTheta_y) < 5.5))
		    continue;
		if(rpsTrack->mNplanes < 7)
		    continue;
		
		hist9->Fill(rpsTrack->mP);
		hist10->Fill(rpsTrack->mPt);
		
		if(rpsTrack->mBranch == 0 || rpsTrack->mBranch == 1) //East RP :: O East Up RP, 1: East Down RP
		{
		    ++nTrkEast;
		    eastTrk_i = trk;
		    
		    hist1East->Fill(rpsTrack->mP);		
		    hist2East->Fill(rpsTrack->mPt);
		    hist3East->Fill(rpsTrack->mEta);
		    hist4East->Fill(rpsTrack->mPhi);
		    hist5East->Fill(rpsTrack->mXi);		
		}

		if(rpsTrack->mBranch == 2 || rpsTrack->mBranch == 3) //West RP :: 2: West Up RP, 3: West Down RP
		{
		    ++nTrkWest;
		    westTrk_i = trk;
		    
		    hist1West->Fill(rpsTrack->mP);		
		    hist2West->Fill(rpsTrack->mPt);
		    hist3West->Fill(rpsTrack->mEta);
		    hist4West->Fill(rpsTrack->mPhi);
		    hist5West->Fill(rpsTrack->mXi);
		}		
	    }
	    //---------- FMS Pion candidates ---------------
	    nPions = fmsArr->GetEntriesFast();
	    fms_i = -1;
	    for(Int_t pi = 0; pi < nPions; ++pi)
	    {
		pion = (TStFmsPointPairData*)fmsArr->At(pi);

		//--------------- FMS Cut ----------------
		if(pion->mZgg > 0.8)
		    continue;
	    
		if(pion->mE < 12 || pion->mE > 70)
		    continue;

		if(cutg1->IsInside(pion->mX, pion->mY) || cutg2->IsInside(pion->mX, pion->mY))
		    continue;

		if(fms_i == -1) //consider only highest energy pair of photons, note: the Pions are already sorted based on energy
		{
		    fms_i = pi;
		    break;
		}		
	    }

	    if(nTrkEast == 1 && fms_i > -1 && event->mBbcADCSum[0] < 60 && event->mBbcADCSumLarge[0] < 110)  //Sanity Check
	    {
		pion = (TStFmsPointPairData*)fmsArr->At(fms_i);
		rpsTrack = (TStRpsTrackData*)rpsArr->At(eastTrk_i);
		hist8East->Fill(rpsTrack->mP + pion->mE);
	    }
	    
	    //if(!(nTrkWest == 1 && nTrkEast == 0)) //Full RP Cuts with One track in west and no track in the east
	    //if(!(nTrkWest == 1 && nTrkEast == 1)) //<------------ Allowing BOTH protons
	    if(!(nTrkWest == 1)) //<------------ Allowing west and no restriction on east
		continue;

	    ++eventCount[1]; //Post RP cut counter	
	
	    if(fms_i < 0)    //Full FMS Cuts
		continue;
	
	    ++eventCount[2]; //Post FMS cut counter

	    
	    hist11->Fill(pion->mM);
	    hist12->Fill(pion->mE);
	    hist2d5->Fill(pion->mX, pion->mY);

	    pion = (TStFmsPointPairData*)fmsArr->At(fms_i);
	    rpsTrack = (TStRpsTrackData*)rpsArr->At(westTrk_i);   
	    sumE_w = rpsTrack->mP + pion->mE;
	
	    hist13->Fill(event->mTofMultiplicity);
	    hist6->Fill(sumE_w);

	    /*
	    //------------- BBC and TOF Cut -----------------	    
	    if(!(event->mTofMultiplicity > 0))
		continue;

	    if(!(event->mBbcADCSum[0] > 0))	//bbc 0 is east and 1 is west 
		continue;

	    hist14->Fill(event->mBbcADCSumLarge[1]);
	    hist15->Fill(event->mBbcADCSum[1]);
	    hist2d2->Fill(sumE_w, event->mBbcADCSumLarge[1]);
	    hist2d3->Fill(sumE_w, event->mBbcADCSum[1]);
	    
	    if(event->mBbcADCSum[1] > 60)	//bbc 0 is east and 1 is west 
		continue;
	    
	    if(event->mBbcADCSumLarge[1] > 110) //bbc 0 is east and 1 is west 
		continue;

	    ++eventCount[3];			// Post BBC-TOF cut counter	    
	    */
	    //-------------------------- FMS-RP Correlation -------------------------------
	    if(nPions == 1)	    //Extreme Cut:Single pion in FMS
		hist7->Fill(sumE_w);
	
	    hist8West->Fill(sumE_w);
	    hist2d1->Fill(pion->mE,  rpsTrack->mP);

	    if(pion->mM > 0.0 && pion->mM < 0.20)
		hist18->Fill(sumE_w);
	    //----------------------------- Diffractive p + p -----> pi^0 + p + X event cut ------------------------
	    if((sumE_w > 80 && sumE_w < 107)    // <------- Energy Conservation cut (WIDER RANGE USED!!)
	       && (pion->mM > 0.0 && pion->mM < 0.25)) // <----- Pion mass range (WIDER RANGE USED!!)
	    {
		hist2d4->Fill(pion->mPhi, rpsTrack->mPhi);
		for(Int_t t = 0; t < event->mNtrig; ++t)
		    hist17->Fill(event->mTriggers[t]);
		
		++eventCount[4];	    
	    }	    
	}		
	//-----------
	
	++nRunsDone;
	file->Close();
	delete file;
    }

    hEvtCount_all->SetBinContent(4, eventCount[0]);
    hEvtCount_all->SetBinContent(6, eventCount[1]);
    hEvtCount_all->SetBinContent(8, eventCount[2]);
    hEvtCount_all->SetBinContent(10, eventCount[3]);
    hEvtCount_all->SetBinContent(12, eventCount[4]);
    
    outFile->Write();
}
