// Filename: AnFmsRpPionSource.C
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

void AnFmsRpPionSource(Int_t firstRun, Int_t lastRun, TString outName, TString inFilePrefix)
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
    TH1D *hist3West = new TH1D("trkEtaWest", "West RP trk Eta; RP Track #eta", 200, -10, 10);
    TH1D *hist4West = new TH1D("trkPhiWest", "West RP trk Phi; RP track #phi [rad]", 200, -3, 3);
    TH1D *hist5West = new TH1D("trkXiWest", "West RP trk Xi; RP track #xi", 200, -100, 20);

    TH1D *hist1East = new TH1D("trkPeast", "East RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2East = new TH1D("trkPtEast", "East RP trk Pt; RP track P_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3East = new TH1D("trkEtaEast", "East RP trk Eta; RP Track #eta", 200, -10, 10);
    TH1D *hist4East = new TH1D("trkPhiEast", "East RP trk Phi; RP track #phi [rad]", 200, -3, 3);
    TH1D *hist5East = new TH1D("trkXiEast", "East RP trk Xi; RP track #xi", 200, -100, 20);
        
    TH1D *hist9 = new TH1D("trkP", " RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist10 = new TH1D("trkPt", " RP trk Pt; RP track Pt [GeV/c]", 200, 0, 2);

    TH1D *hist11 = new TH1D("pi0M_all", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist12 = new TH1D("pi0E", "Diphoton Energy; E_{#gamma#gamma} [GeV]", 200, 10, 100);
    TH2D *hist2d5 = new TH2D("pionXY", "#pi^{0} position; X [cm]; Y[cm]", 100, -100, 100, 100, -100, 100);

    TH1D *hist11w = new TH1D("pi0M_w", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist11e = new TH1D("pi0M_e", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist11ew = new TH1D("pi0M_ew", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist110 = new TH1D("pi0M_0", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    

    TH1D *hist11_sd = new TH1D("pi0M_sd", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist11_cp = new TH1D("pi0M_cp", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist11_et = new TH1D("pi0M_et", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    

    TH1D *hist11e_sd = new TH1D("pi0Me_sd", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist11w_sd = new TH1D("pi0Mw_sd", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist11ew_sd = new TH1D("pi0Mew_sd", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist110_sd = new TH1D("pi0M0_sd", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
   
    TH1D *hist11_r = new TH1D("pi0M_r", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist11e_r = new TH1D("pi0Me_r", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist11w_r = new TH1D("pi0Mw_r", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    TH1D *hist11ew_r = new TH1D("pi0Mew_r", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);    
    
    TH1D* hist16 = new TH1D("trigger", "Trigger Distribution", 1000, 480000, 481000);
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
    Int_t trig_SD = 480701;
    Int_t trig_SDT = 480703;
    Int_t trig_CPT2 = 480713;
    Int_t trig_CP = 480707;
    Int_t trig_ET = 480706;
    vector <Int_t> trigList;
    Int_t trigEvt = 0;
    Int_t spEvt = 0;
    Int_t goodTrkEvt = 0;
    Int_t hasTrkEvt = 0;
    
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

	    trigList.clear();
	    for(Int_t t = 0; t < event->GetNtrigs(); ++t)
	    {
		hist16->Fill(event->GetTrigger(t));
		trigList.push_back(event->GetTrigger(t));
	    }

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
		if (rpsTrack->GetType() != 1)
		    continue;
		if(!(rpsTrack->GetThetaX() > -1.5 && rpsTrack->GetThetaX() < 5.0)) 
		    continue;
		if(!(fabs(rpsTrack->GetThetaY()) > 1.0 &&  fabs(rpsTrack->GetThetaY()) < 5.5))
		    continue;
		if(rpsTrack->GetNplanes() < 7)
		    continue;
		
		hist9->Fill(rpsTrack->GetP());
		hist10->Fill(rpsTrack->GetPt());
		
		if(rpsTrack->GetBranch() == 0 || rpsTrack->GetBranch() == 1) //East RP :: O East Up RP, 1: East Down RP
		{
		    ++nTrkEast;
		    eastTrk_i = trk;
		    
		    hist1East->Fill(rpsTrack->GetP());		
		    hist2East->Fill(rpsTrack->GetPt());
		    hist3East->Fill(rpsTrack->GetEta());
		    hist4East->Fill(rpsTrack->GetPhi());
		    hist5East->Fill(rpsTrack->GetXi());		
		}

		if(rpsTrack->GetBranch() == 2 || rpsTrack->GetBranch() == 3) //West RP :: 2: West Up RP, 3: West Down RP
		{
		    ++nTrkWest;
		    westTrk_i = trk;
		    
		    hist1West->Fill(rpsTrack->GetP());		
		    hist2West->Fill(rpsTrack->GetPt());
		    hist3West->Fill(rpsTrack->GetEta());
		    hist4West->Fill(rpsTrack->GetPhi());
		    hist5West->Fill(rpsTrack->GetXi());
		}		
	    }
	    if(nRpsTracks > 0)
		++hasTrkEvt;
	    if(nTrkEast > 0 || nTrkWest > 0)
		++goodTrkEvt;

	    if(nRpsTracks == 0 && nPions > 0)
		++spEvt;
	    //---------- FMS Pion candidates ---------------
	    nPions = fmsArr->GetEntriesFast();
	    fms_i = -1;
	    for(Int_t pi = 0; pi < nPions; ++pi)
	    {
		pion = (TStFmsPointPairData*)fmsArr->At(pi);

		//--------------- FMS Cut ----------------
		if(pion->GetZgg() > 0.8)
		    continue;
	    
		if(pion->GetE() < 12 || pion->GetE() > 70)
		    continue;

		if(cutg1->IsInside(pion->GetX(), pion->GetY()) || cutg2->IsInside(pion->GetX(), pion->GetY()))
		    continue;

		if(fms_i == -1) //consider only highest energy pair of photons, note: the Pions are already sorted based on energy
		{
		    fms_i = pi;
		    break;
		}		
	    }
	    	    
	    if(fms_i < 0)    //Full FMS Cuts
		continue;

	    if(nTrkWest >= 2 || nTrkEast >= 2)
		continue;
	    
	    pion = (TStFmsPointPairData*)fmsArr->At(fms_i);

	    hist11->Fill(pion->GetM());
	    hist12->Fill(pion->GetE());
	    hist2d5->Fill(pion->GetX(), pion->GetY());
	
	    //-------- based on RP track topology ----------
	    if(nTrkWest == 1 && nTrkEast == 0)
		hist11w->Fill(pion->GetM());		
	    if(nTrkWest == 0 && nTrkEast == 1)
		hist11e->Fill(pion->GetM());
	    if(nTrkWest == 1 && nTrkEast == 1)
		hist11ew->Fill(pion->GetM());
	    if(nTrkWest == 0 && nTrkEast == 0)
		hist110->Fill(pion->GetM());
    
	    //------ based on trigger topology -----------
	    if(std::find(trigList.begin(), trigList.end(), trig_SD) != trigList.end() || std::find(trigList.begin(), trigList.end(), trig_SDT) != trigList.end())	   
		hist11_sd->Fill(pion->GetM());
	    if(std::find(trigList.begin(), trigList.end(), trig_CPT2) != trigList.end() || std::find(trigList.begin(), trigList.end(), trig_CP) != trigList.end())
		hist11_cp->Fill(pion->GetM());
	    if(std::find(trigList.begin(), trigList.end(), trig_ET) != trigList.end())
		hist11_et->Fill(pion->GetM());
	    
	    //------- based on trigger and RP track topology ------------
	    if(std::find(trigList.begin(), trigList.end(), trig_SD) != trigList.end() || std::find(trigList.begin(), trigList.end(), trig_SDT) != trigList.end())
	    {		
		if(nTrkWest == 1 && nTrkEast == 0)
		    hist11w_sd->Fill(pion->GetM());		
		if(nTrkWest == 0 && nTrkEast == 1)
		    hist11e_sd->Fill(pion->GetM());
		if(nTrkWest == 1 && nTrkEast == 1)
		    hist11ew_sd->Fill(pion->GetM());
		if(nTrkWest == 0 && nTrkEast == 0)
		    hist110_sd->Fill(pion->GetM());
	    }
	    else
	    {
		hist11_r->Fill(pion->GetM());
		
		if(nTrkWest == 1 && nTrkEast == 0)
		    hist11w_r->Fill(pion->GetM());		
		if(nTrkWest == 0 && nTrkEast == 1)
		    hist11e_r->Fill(pion->GetM());
		if(nTrkWest == 1 && nTrkEast == 1)
		    hist11ew_r->Fill(pion->GetM());
	    }
	}		
	//-----------
	
	++nRunsDone;
	file->Close();
	delete file;
    }

    hEvtCount_all->SetBinContent(2, eventCount[0]);
    hEvtCount_all->SetBinContent(4, hasTrkEvt);
    hEvtCount_all->SetBinContent(6, goodTrkEvt);
    hEvtCount_all->SetBinContent(8, spEvt);
    
    outFile->Write();
}
