// Filename: AnEEmcRpCorrUpb.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Nov 21 13:00:15 2019 (-0500)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "TAnEEmcRpTreeReader.h"
#include "TStRunList.h"
#include "StEEmcPool/EEmcTreeContainers/EEmc2ParticleCandidate.h"
#include "StEEmcPool/EEmcTreeContainers/EEmcParticleCandidate.h"
#include "BrContainers/TStFmsPointPairData.h"
#include "BrContainers/TStEventData.h"
#include "BrContainers/TStRpsTrackData.h"
#include "BrContainers/TStEmcPointData.h"
#include "StChain/StEvtHddr.h"

using namespace std;

void AnEEmcRpCorrUpb(Int_t firstRun, Int_t lastRun, TString outName)
{
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList(firstRun, lastRun);
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    
    TString fileName_eemc1;
    TString fileName_eemc3;
    TString fileName_rp;
    TString prefix_eemc1 = "/star/u/kabir/GIT/BrightSTAR/dst/R15PhyTrigNanoDst/EEmc_Part1_AnRunNanoDstMaker_";
    TString prefix_eemc3 = "/star/u/kabir/GIT/BrightSTAR/dst/R15PhyTrigNanoDst/eemcTreeP3_EEmc_Tree_AnRunNanoDstMaker_";
    TString prefix_rp = "/star/u/kabir/GIT/BrightSTAR/dst/R15PhyTrigNanoDst/AnRunNanoDstMaker_";
    
    //Histograms
    TFile *fout = new TFile(outName, "RECREATE");
    
    TH1D *hist0 = new TH1D("pi0M", "Pion M",  200, 0.0, 1.0);
    TH1D *hist0_ = new TH1D("pi0E", "Pion E",  200, 0.0, 50.0);
    
    TH1D *hist1West = new TH1D("trkPwest", "West RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2West = new TH1D("trkPtWest", "West RP trk Pt; RP track p_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3West = new TH1D("trkEtaWest", "West RP trk Eta; RP Track #eta", 200, -10, 10);
    TH1D *hist4West = new TH1D("trkPhiWest", "West RP trk Phi; RP track #phi [rad]", 200, -3, 3);
    TH1D *hist5West = new TH1D("trkXiWest", "West RP trk Xi; RP track #xi", 200, 0, 0);

    TH1D *hist1East = new TH1D("trkPeast", "East RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2East = new TH1D("trkPtEast", "East RP trk Pt; RP track p_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3East = new TH1D("trkEtaEast", "East RP trk Eta; RP Track #eta", 200, -10, 10);
    TH1D *hist4East = new TH1D("trkPhiEast", "East RP trk Phi; RP track #phi [rad]", 200, -3, 3);
    TH1D *hist5East = new TH1D("trkXiEast", "East RP trk Xi; RP track #xi", 200, 0, 0);
    
    TH1D *hist6 = new TH1D("trkP", "Trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist7 = new TH1D("trkPt", "Trk Pt; RP track Pt [GeV/c]", 200, 0, 2.0);
    
    TH1D *hist8 = new TH1D("sumE_east", "E_{p + #pi^{0}}^{East}; E_{p + #pi^{0}}^{East} [GeV]", 100, 45, 180);
    TH1D *hist9 = new TH1D("sumE_west", "E_{p + #pi^{0}}^{West}; E_{p + #pi^{0}}^{West} [GeV]", 100, 45, 180);
    TH1D *hist10 = new TH1D("sumE_west_singPion", "West Sum E (Single pion in FMS); E_{p + #pi^{0}}^{West} [GeV]", 100, 45, 180);
    
    TH2D *hist2d1 = new TH2D("E_p_vs_E_pion", "E_{p} vs E_{#pi^{0}}; E_{#pi^{0}} [GeV]; E_{p} [GeV]", 100, 0, 80, 100, 60, 150);
    TH2D *hist2d2 = new TH2D("E_sum_vs_BBC_large", "E_{sum} vs BBC ADC Sum (Large); E_{p + #pi^{0}}^{West} [GeV]; BBC ADC Sum (Large)", 100, 50, 200, 300, 0, 6000);
    TH2D *hist2d3 = new TH2D("E_sum_vs_BBC_small", "E_{sum} vs BBC ADC Sum (Small); E_{p + #pi^{0}}^{West} [GeV]; BBC ADC Sum (Small)", 100, 50, 200, 300, 0, 4000);
    TH2D *hist2d4 = new TH2D("p_phi_vs_pion_phi", "#phi_{p} vs #phi_{#pi^{0}}; #phi_{#pi^{0}} [rad]; #phi_{p} [rad]", 100, -3.15, 3.15, 100, -3.15, 3.15);
    
    TH1D *hEvtCount_all = new TH1D("hEvtCount_all", "Event Count", 20, 0, 20);
    TH1D *hEvtCount;

    TClonesArray *photonArr = new TClonesArray("EEmcParticleCandidate_t");
    TClonesArray *pionArr  = new TClonesArray("EEmc2ParticleCandidate_t");
    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");
    TClonesArray *fmsArr = new TClonesArray("TStFmsPointPairData");
    TClonesArray *emcArr = new TClonesArray("TStEmcPointData");
    TStEventData *event = new TStEventData();
    StEvtHddr *eemcEvent = new StEvtHddr();
    
    EEmc2ParticleCandidate_t *pion;
    TStRpsTrackData *rpsTrack;
    TStEmcPointData *emcPhoton;
    Int_t eemcRunNumber;
    Int_t eemcEventNumber;
    
    Int_t nEntries_eemc1 = 0;
    Int_t nEntries_eemc3 = 0;
    Int_t nEntries_rp = 0;
    Int_t eastTrk_i =  -1;
    Int_t westTrk_i =  -1;
    Int_t nTrkEast = 0;
    Int_t nTrkWest = 0;
    Int_t eemc_i = -1;
    Int_t evtCount = 0;
    Double_t sumE_w = 0.0;
    
    Double_t pionE = 0;
    Double_t maxEng = 0;
    Double_t pionM = 0;
    Int_t nPions = 0;
    Int_t eventCount[5] = {0}; //Event count after each cut 
    
    //------------ Loop over runs --------------------------
    for (Int_t run_i = 0; run_i < maxRuns; ++run_i)
    {
	run = runList->GetEntry(run_i);

	fileName_eemc1 = prefix_eemc1 + to_string(run) + (TString)".root";
	fileName_eemc3 = prefix_eemc3 + to_string(run) + (TString)".root";
	fileName_rp = prefix_rp + to_string(run) + (TString)".root";
	cout << fileName_rp <<endl;
	
	if(gSystem->AccessPathName(fileName_eemc1) || gSystem->AccessPathName(fileName_eemc3) || gSystem->AccessPathName(fileName_rp))
	{
	    cout << "Input file NOT found" <<endl;
	    return;
	}
        
	TFile *file_eemc1 = new TFile(fileName_eemc1);
	TTree *tr_eemc1 = (TTree*)file_eemc1->Get("tree");


	TFile *file_eemc3 = new TFile(fileName_eemc3);
	TTree *tr_eemc3 = (TTree*)file_eemc3->Get("tree");

	TFile *file_dst = new TFile(fileName_rp);
	TTree *tr_dst = (TTree*)file_dst->Get("T");

	
	nEntries_eemc1 = tr_eemc1->GetEntries();
	nEntries_eemc3 = tr_eemc3->GetEntries();
	nEntries_rp = tr_dst->GetEntries();

	cout << "EEMC P1 nEntries: "<< nEntries_eemc1 <<endl;
	cout << "EEMC P3 nEntries: "<< nEntries_eemc3 <<endl;
	cout << "RP nEntries: "<< nEntries_rp <<endl;

	if(nEntries_eemc1 != nEntries_rp || nEntries_eemc3 != nEntries_rp)
	{
	    cout << "Trees do NOT have equal number of entries. Skipped ...." <<endl;
	    return;
	}
       
	tr_dst->SetBranchAddress("event", &event);
	tr_dst->SetBranchAddress("fmsPointPair", &fmsArr);
	tr_dst->SetBranchAddress("rpTrack", &rpsArr);
	tr_dst->SetBranchAddress("emcPoint", &emcArr);
	
	tr_eemc3->SetBranchAddress("photon", &photonArr);
	tr_eemc3->SetBranchAddress("pi0", &pionArr);

	tr_eemc1->SetBranchAddress("evtHddr", &eemcEvent);
		
	for(Int_t evt = 0; evt < nEntries_rp; ++evt)
	{
	    tr_dst->GetEntry(evt);
	    tr_eemc1->GetEntry(evt);
	    tr_eemc3->GetEntry(evt);
	    
	    ++eventCount[0]; //Trigger Counter (Events with at least one RP track)
	    
	    if(evt%2000 == 0)
		cout << "Events processed in this run:"<< (int)(100.0*evt / nEntries_rp) <<" %"<<endl;

	    // cout << "RP Tree:: Run number: "<< reader->evt_run << " Event no.: " << reader->evt_id <<endl;
	    // cout << "EEMC Tree:: Run number: "<< reader->eemcRunNumber << " Event no.: " << reader->eemcEventNumber <<"\n"<<endl;

	    if((event->GetRunNumber() != eemcEvent->GetRunNumber()) || (event->GetEventId() != eemcEvent->GetEventNumber()))
	    {
		cout << "The event numbers or run numbers do NOT match. Skipped ...." <<endl;
		cout << "Run number - RP T: "<< event->GetRunNumber()<<" EEMC T: "<< eemcEvent->GetRunNumber() <<endl;
		cout << "Event number - RP T: "<< event->GetEventId() <<" EEMC T: "<< eemcEvent->GetEventNumber() <<endl;
		return;
	    }
	    
	    //------------------- RP Cuts --------------------------------
	    eastTrk_i = -1;
	    nTrkEast = 0;
	    westTrk_i = -1;
	    nTrkWest = 0;
	    
	    for(Int_t t = 0; t < rpsArr->GetEntriesFast(); ++t)
	    {
		rpsTrack = (TStRpsTrackData*)rpsArr->At(t);
		if (rpsTrack->GetType() != 1)
		    continue;
		if(!(rpsTrack->GetThetaX() > -1.5 && rpsTrack->GetThetaX() < 5.0) ) 
		    continue;
		if( !(fabs(rpsTrack->GetThetaY()) > 1.0 &&  fabs(rpsTrack->GetThetaY()) < 5.5) )
		    continue;
		if(rpsTrack->GetNplanes() < 7)
		    continue;

		hist6->Fill(rpsTrack->GetP());
		hist7->Fill(rpsTrack->GetPt());
	    
		if(rpsTrack->GetBranch() == 0 || rpsTrack->GetBranch() == 1) //East RP :: O East Up RP, 1: East Down RP
		{
		    ++nTrkEast;
		    eastTrk_i = t;
		
		    hist1East->Fill(rpsTrack->GetP());		
		    hist2East->Fill(rpsTrack->GetPt());
		    hist3East->Fill(rpsTrack->GetEta());
		    hist4East->Fill(rpsTrack->GetPhi());
		    hist5East->Fill(rpsTrack->GetXi());				
		}

		if(rpsTrack->GetBranch() == 2 || rpsTrack->GetBranch() == 3) //West RP :: 2: West Up RP, 3: West Down RP
		{
		    ++nTrkWest;
		    westTrk_i = t;

		    hist1West->Fill(rpsTrack->GetP());		
		    hist2West->Fill(rpsTrack->GetPt());
		    hist3West->Fill(rpsTrack->GetEta());
		    hist4West->Fill(rpsTrack->GetPhi());
		    hist5West->Fill(rpsTrack->GetXi());
		}	    
	    }
	    	    
	    //------------------------ EEMC Cut -----------------------------------		    
	    nPions = pionArr->GetEntriesFast();
	   
	    eemc_i = -1;
	    maxEng = 0;
	    pionE = 0;
	    for(Int_t pair = 0; pair < nPions; ++pair)
	    {
		pion = (EEmc2ParticleCandidate_t*) pionArr->At(pair);
	    
		if(pion->Z > 0.8)
		    continue;
		if(pion->E < 5 || pion->E > 70) //<---------- Used Different lower threshold than FmsRP !!!!!
		    continue;
	    
		if(pion->E > maxEng) //Considering highest energy pion
		{
		    eemc_i = pair;
		    maxEng = pion->E;
		    pionE = maxEng;
		    pionM = pion->M;
		}
	    }

	    if(nTrkEast == 1 && eemc_i > -1 && event->GetBbcSumSmall(0) < 60 && event->GetBbcSumLarge(0) < 110)  //Sanity Check
	    {
		pion = (EEmc2ParticleCandidate_t*) pionArr->At(eemc_i);
		rpsTrack = (TStRpsTrackData*)rpsArr->At(eastTrk_i);
		hist8->Fill(rpsTrack->GetP() + pion->E);
	    }
	    if(!(nTrkWest == 1 && nTrkEast == 0)) //Full RP Cuts with One track in west and no track in the east
		continue;

	    ++eventCount[1]; //Post RP cut counter	
	
	    if(eemc_i < 0) //Full EEMC Cuts
		continue;
	    ++eventCount[2]; //Post EEMC cut counter

	    hist0->Fill(pion->M);
	    hist0_->Fill(pion->E);

	    pion = (EEmc2ParticleCandidate_t*) pionArr->At(eemc_i);
	    rpsTrack = (TStRpsTrackData*)rpsArr->At(westTrk_i);
	    
	    sumE_w = rpsTrack->GetP() + pion->E;
	    //------------- BBC and TOF Cut -----------------	    
	    if(!(event->GetTofMultiplicity() > 0))
		continue;

	    if(!(event->GetBbcSumSmall(0) > 0))  //bbc 0 is east and 1 is west 
		continue;

	    hist2d2->Fill(sumE_w, event->GetBbcSumLarge(1));
	    hist2d3->Fill(sumE_w, event->GetBbcSumSmall(1));
	    
	    if(event->GetBbcSumSmall(1) > 60)  //bbc 0 is east and 1 is west 
		continue;
	    
	    if(event->GetBbcSumLarge(1) > 110)  //bbc 0 is east and 1 is west
		continue;

	    ++eventCount[3];    // Post BBC-TOF cut counter
	    //-------------------------- FMS-RP Correlation -------------------------------
	    hist9->Fill(sumE_w);
	    hist2d1->Fill(pion->E, rpsTrack->GetP());

	    if(nPions == 1)          //Extreme Cut:Single pion in EEMC
		hist10->Fill(sumE_w);
	    
	    if((sumE_w > 70 && sumE_w < 105)    // <------- Add cut on pion mass (WIDER RANGE USED!!)
	       && (pion->M > 0.0 && pion->M < 0.3)) // <----- Pion mass range (WIDER RANGE USED!!)
		++eventCount[4];
	}

	file_eemc1->Close();
	delete file_eemc1;
	file_eemc3->Close();
	delete file_eemc3;
	file_dst->Close();
	delete file_dst;
    }
    
    // TFile *file = new TFile(fileName_rp);
    // hEvtCount = (TH1D*) file->Get("EvtCountHist");
    // hEvtCount_all->SetBinContent(2, hEvtCount->GetBinContent(4));
    hEvtCount_all->SetBinContent(4, eventCount[0]);
    hEvtCount_all->SetBinContent(6, eventCount[1]);
    hEvtCount_all->SetBinContent(8, eventCount[2]);
    hEvtCount_all->SetBinContent(10, eventCount[3]);
    hEvtCount_all->SetBinContent(12, eventCount[4]);
  
   
    cout<<"Total events passed the cut:" << eventCount[4] <<endl; 

    // fout->cd();
    // hEvtCount->Write();
    fout->Write();
    //fout->Close();
}
