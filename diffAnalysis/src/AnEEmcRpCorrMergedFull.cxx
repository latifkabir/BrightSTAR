// Filename: AnEEmcRpCorrMergedFull.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Nov 21 13:00:15 2019 (-0500)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "TAnEEmcRpTreeReader.h"
#include "TStRunList.h"
#include "StEEmcPool/EEmcTreeContainers/EEmc2ParticleCandidate.h"
#include "StEEmcPool/EEmcTreeContainers/EEmcParticleCandidate.h"
using namespace std;

void AnEEmcRpCorrMergedFull(TString dirPath, Int_t maxEvents)
{
    TString fileName_eemc1;
    TString fileName_eemc3;
    TString fileName_rp;
	
    fileName_eemc1 = dirPath + "/R15EEmcRpTree_Part1_Merged.root";
    fileName_eemc3 = dirPath + "/R15EEmcRpTree_Part3_Merged.root";
    fileName_rp = dirPath + "/R15RpTree_Merged.root";
    
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
    
    TFile *fout = new TFile("EEmcRpCorr.root", "RECREATE");
    
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

    TClonesArray *photon_arr = new TClonesArray("EEmcParticleCandidate_t");
    TClonesArray *pion_arr  = new TClonesArray("EEmc2ParticleCandidate_t");
    EEmc2ParticleCandidate_t *pion;
    Double_t pionE = 0;
    Double_t maxEng = 0;
    Double_t pionM = 0;
    Int_t nPions = 0;
    		
    if(gSystem->AccessPathName(fileName_eemc1) || gSystem->AccessPathName(fileName_eemc3) || gSystem->AccessPathName(fileName_rp))
    {
	cout << "Input file NOT found" <<endl;
	return;
    }
    
    //------- Event count after each cut ------------
    Int_t eventCount[5] = {0};
    
    TChain *ch_eemc1 = new TChain("tree");
    ch_eemc1->Add(fileName_eemc1);

    TChain *ch_eemc3 = new TChain("tree");
    ch_eemc3->Add(fileName_eemc3);

    TChain *ch_rp = new TChain("T");
    ch_rp->Add(fileName_rp);
	
    nEntries_eemc1 = ch_eemc1->GetEntries();
    nEntries_eemc3 = ch_eemc3->GetEntries();
    nEntries_rp = ch_rp->GetEntries();

    cout << "EEMC P1 nEntries: "<< nEntries_eemc1 <<endl;
    cout << "EEMC P3 nEntries: "<< nEntries_eemc3 <<endl;
    cout << "RP nEntries: "<< nEntries_rp <<endl;

    if(nEntries_eemc1 != nEntries_rp || nEntries_eemc3 != nEntries_rp)
    {
	cout << "Trees do NOT have equal number of entries. Skipped ...." <<endl;
	return;
    }
       
    TAnEEmcRpTreeReader *reader = new TAnEEmcRpTreeReader(ch_eemc1, ch_eemc3, ch_rp);
	
    for(Int_t evt = 0; evt < nEntries_rp; ++evt)
    {
	reader->GetEntry(evt);
	++eventCount[0]; //Trigger Counter (Events with at least one RP track)
	    
	if(evt%2000 == 0)
	    cout << "Events processed in this run:"<< (int)(100.0*evt / nEntries_rp) <<" %"<<endl;
	if(maxEvents != -1)
	{
	    if(evt > maxEvents)
		break;
	}
	// cout << "RP Tree:: Run number: "<< reader->evt_run << " Event no.: " << reader->evt_id <<endl;
	// cout << "EEMC Tree:: Run number: "<< reader->mRunNumber << " Event no.: " << reader->mEventNumber <<"\n"<<endl;

	if((reader->evt_run != reader->mRunNumber) || (reader->evt_id != reader->mEventNumber))
	{
	    cout << "The event numbers or run numbers do NOT match. Skipped ...." <<endl;
	    cout << "Run number - RP T: "<< reader->evt_run<<" EEMC T: "<< reader->mRunNumber <<endl;
	    cout << "Event number - RP T: "<< reader->evt_id<<" EEMC T: "<< reader->mEventNumber <<endl;
	    return;
	}
	
	//------------------- RP Cuts --------------------------------
	eastTrk_i = -1;
	nTrkEast = 0;
	westTrk_i = -1;
	nTrkWest = 0;
	for(Int_t t = 0; t < reader->rp_nTracks; ++t)
	{
	    if (reader->rp_trackType[t] != 1)
		continue;
	    if(!(reader->rp_trackTheta_x[t] > -1.5 && reader->rp_trackTheta_x[t] < 5.0) ) 
		continue;
	    if( !(fabs(reader->rp_trackTheta_y[t]) > 1.0 &&  fabs(reader->rp_trackTheta_y[t]) < 5.5) )
		continue;
	    if(reader->rp_trackNplanes[t] < 7)
		continue;

	    hist6->Fill(reader->rp_trackP[t]);
	    hist7->Fill(reader->rp_trackPt[t]);
	    
	    if(reader->rp_trackBranch[t] == 0 || reader->rp_trackBranch[t] == 1) //East RP :: O East Up RP, 1: East Down RP
	    {
		++nTrkEast;
		eastTrk_i = t;
		
		hist1East->Fill(reader->rp_trackP[eastTrk_i]);		
		hist2East->Fill(reader->rp_trackPt[eastTrk_i]);
		hist3East->Fill(reader->rp_trackEta[eastTrk_i]);
		hist4East->Fill(reader->rp_trackPhi[eastTrk_i]);
		hist5East->Fill(reader->rp_trackXi[eastTrk_i]);				
	    }

	    if(reader->rp_trackBranch[t] == 2 || reader->rp_trackBranch[t] == 3) //West RP :: 2: West Up RP, 3: West Down RP
	    {
		++nTrkWest;
		westTrk_i = t;

		hist1West->Fill(reader->rp_trackP[westTrk_i]);		
		hist2West->Fill(reader->rp_trackPt[westTrk_i]);
		hist3West->Fill(reader->rp_trackEta[westTrk_i]);
		hist4West->Fill(reader->rp_trackPhi[westTrk_i]);
		hist5West->Fill(reader->rp_trackXi[westTrk_i]);
	    }	    
	}
	    	    
	//------------------------ EEMC Cut -----------------------------------
	pion_arr = reader->pion_array;
	photon_arr = reader->photon_array;
	    
	if(!pion_arr || !photon_arr)
	{
	    cout << "Failed to get pion or photon array" <<endl;
	    return;
	}

	nPions = pion_arr->GetEntriesFast();
	   
	eemc_i = -1;
	maxEng = 0;
	pionE = 0;
	for(Int_t pair = 0; pair < nPions; ++pair)
	{
	    pion = (EEmc2ParticleCandidate_t*) pion_arr->At(pair);
	    
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

	if(nTrkEast == 1 && eemc_i > -1 && reader->evt_bbcADCSum[0] < 60 && reader->evt_bbcADCSumLarge[0] < 110)  //Sanity Check
	    hist8->Fill(reader->rp_trackP[eastTrk_i] + pionE);
	    
	if(!(nTrkWest == 1 && nTrkEast == 0)) //Full RP Cuts with One track in west and no track in the east
	    continue;

	++eventCount[1]; //Post RP cut counter	
	
	if(eemc_i < 0) //Full EEMC Cuts
		continue;
	++eventCount[2]; //Post EEMC cut counter

	hist0->Fill(pion->M);
	hist0_->Fill(pion->E);

	sumE_w = reader->rp_trackP[westTrk_i] + pionE;
	//------------- BBC and TOF Cut -----------------	    
	if(!(reader->evt_tofMultiplicity > 0))
	    continue;

	if(!(reader->evt_bbcADCSum[0] > 0))  //bbc 0 is east and 1 is west ???? <--------Check
	    continue;

	hist2d2->Fill(sumE_w, reader->evt_bbcADCSumLarge[1]);
	hist2d3->Fill(sumE_w, reader->evt_bbcADCSum[1]);
	    
	if(reader->evt_bbcADCSum[1] > 60)  //bbc 0 is east and 1 is west ???? <--------Check
	    continue;
	    
	if(reader->evt_bbcADCSumLarge[1] > 110)  //bbc 0 is east and 1 is west ???? <--------Check
	    continue;

	++eventCount[3];// Post BBC-TOF cut counter
	//-------------------------- FMS-RP Correlation -------------------------------
	hist9->Fill(sumE_w);
	hist2d1->Fill(pionE, reader->rp_trackP[westTrk_i]);

	if(nPions == 1)          //Extreme Cut:Single pion in EEMC
	    hist10->Fill(sumE_w);
	    
	if((sumE_w > 70 && sumE_w < 105)    // <------- Add cut on pion mass (WIDER RANGE USED!!)
	   && (pionM > 0.0 && pionM < 0.3)) // <----- Pion mass range (WIDER RANGE USED!!)
	    ++eventCount[4];
    }

    TFile *file = new TFile(fileName_rp);
    hEvtCount = (TH1D*) file->Get("EvtCountHist");
    hEvtCount_all->SetBinContent(2, hEvtCount->GetBinContent(4));
    hEvtCount_all->SetBinContent(4, eventCount[0]);
    hEvtCount_all->SetBinContent(6, eventCount[1]);
    hEvtCount_all->SetBinContent(8, eventCount[2]);
    hEvtCount_all->SetBinContent(10, eventCount[3]);
    hEvtCount_all->SetBinContent(12, eventCount[4]);
  
    // delete reader; 
    // delete ch_eemc1;
    // delete ch_eemc3;
    // delete ch_rp;
    
    cout<<"Total events passed the cut:" << eventCount[4] <<endl; 

    fout->cd();
    hEvtCount->Write();
    fout->Write();
    //fout->Close();
}
