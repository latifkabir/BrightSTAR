// Filename: AnFmsRpCorrMergedFull.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Nov 21 13:00:15 2019 (-0500)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "TAnFmsRpTreeReader.h"
#include "TStRunList.h"
using namespace std;

void AnFmsRpCorrMergedFull(TString filePath, Int_t maxEvents)
{        
    Int_t nEntries = 0;
    Int_t eastTrk_i =  -1;
    Int_t westTrk_i =  -1;
    Int_t nTrkEast = 0;
    Int_t nTrkWest = 0;
    Int_t fms_i = -1;
    Double_t sumE_w = 0.0;

    TFile *fout = new TFile("FmsRpCorr.root", "RECREATE");

    TH1D *hist1West = new TH1D("trkPwest", "West RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2West = new TH1D("trkPtWest", "West RP trk Pt; RP track P_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3West = new TH1D("trkEtaWest", "West RP trk Eta; RP Track #eta", 200, -10, 10);
    TH1D *hist4West = new TH1D("trkPhiWest", "West RP trk Phi; RP track #phi [mrad]", 200, -3, 3);
    TH1D *hist5West = new TH1D("trkXiWest", "West RP trk Xi; RP track #xi", 200, 0, 0);

    TH1D *hist1East = new TH1D("trkPeast", "East RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2East = new TH1D("trkPtEast", "East RP trk Pt; RP track P_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3East = new TH1D("trkEtaEast", "East RP trk Eta; RP Track #eta", 200, -10, 10);
    TH1D *hist4East = new TH1D("trkPhiEast", "East RP trk Phi; RP track #phi [mrad]", 200, -3, 3);
    TH1D *hist5East = new TH1D("trkXiEast", "East RP trk Xi; RP track #xi", 200, 0, 0);

    TH1D *hist6 = new TH1D("sumE_west_before", "West Sum E Before BBC and TOF Cut; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH1D *hist7 = new TH1D("sumE_west_singPion", "West Sum E (Single pion in FMS); E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    
    TH1D *hist8West = new TH1D("sumE_west", "E_{p + #pi^{0}}^{West}; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH1D *hist8East = new TH1D("sumE_east", "E_{p + #pi^{0}}^{East}; E_{p + #pi^{0}}^{East} [GeV]", 100, 60, 200);

    TH1D *hist9 = new TH1D("trkP", " RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist10 = new TH1D("trkPt", " RP trk Pt; RP track Pt [GeV/c]", 200, 0, 2);
    TH1D *hist11 = new TH1D("pi0M", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);
    TH1D *hist12 = new TH1D("pi0E", "Diphoton Energy; E_{#gamma#gamma} [GeV]", 200, 10, 100);

    TH1D* hist13 = new TH1D("tofMult", "TOF Multiplicity", 250, 0, 500);
    TH1D* hist14 = new TH1D("bbcSumL_west", "West Large BBC ADC Sum", 250, 0, 8000);
    TH1D* hist15 = new TH1D("bbcSumS_west", "West Small BBC ADC Sum", 250, 0, 45000);
    
    TH2D *hist2d1 = new TH2D("E_p_vs_E_pion", "E_{p}^{west} vs E_{#pi^{0}}; E_{#pi^{0}} [GeV]; E_{p}^{west} [GeV]", 100, 10, 80, 100, 60, 150);
    TH2D *hist2d2 = new TH2D("E_sum_vs_BBC_large", "E_{sum} vs BBC ADC Sum (Large); E_{p + #pi^{0}}^{west} [GeV]; BBC ADC Sum (Large)", 100, 50, 200, 300, 0, 6000);
    TH2D *hist2d3 = new TH2D("E_sum_vs_BBC_small", "E_{sum} vs BBC ADC Sum (Small); E_{p + #pi^{0}}^{west} [GeV]; BBC ADC Sum (Small)", 100, 50, 200, 300, 0, 4000);
    TH2D *hist2d4 = new TH2D("p_phi_vs_pion_phi", "#phi_{p} vs #phi_{#pi^{0}}; #phi_{#pi^{0}} [rad]; #phi_{p} [rad]", 100, -3.15, 3.15, 100, -3.15, 3.15);
    TH2D *hist2d5 = new TH2D("pionXY", "#pi^{0} position; X [cm]; Y[cm]", 100, -100, 100, 100, -100, 100);

    
    TH1D *hEvtCount_all = new TH1D("hEvtCount_all", "Event Count", 20, 0, 20);
    TH1D *hEvtCount;
    	
    if(gSystem->AccessPathName(filePath))
    {
	cout << "Input file NOT found" <<endl;
	return;
    }

    //------- Event count after each cut ------------
    TFile *file = new TFile(filePath);
    hEvtCount = (TH1D*) file->Get("hEvtCount");
    Int_t eventCount[5] = {0};
    
    TChain *chain = new TChain("T");
    chain->Add(filePath);
    nEntries = chain->GetEntries();
    TAnFmsRpTreeReader *reader = new TAnFmsRpTreeReader(chain);

    cout << "Total number of events: "<< nEntries <<endl;

    for(Int_t evt = 0; evt < nEntries; ++evt)
    {
	reader->GetEntry(evt);
	++eventCount[0]; //Trigger Counter (Events with at least one RP track)
	
	if(evt%2000 == 0)
	    cout << "Events processed: "<< (int)(100.0*evt / nEntries) <<" %"<<endl;
	if(maxEvents != -1)
	{
	    if(evt > maxEvents)
		break;
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

	    hist9->Fill(reader->rp_trackP[t]);
	    hist10->Fill(reader->rp_trackPt[t]);
	    
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
	  	    
	//------------------------ FMS Cut -----------------------------------
	fms_i = -1;
	for(Int_t pair = 0; pair < reader->fms_nPairs; ++pair)
	{	    
	    if(reader->fms_pairZgg[pair] > 0.8)
		continue;
	    
	    if(reader->fms_pairE[pair] < 12 || reader->fms_pairE[pair] > 70)
		continue;
	    	    
	    if(fms_i == -1)                        //consider only highest energy pair of photons, note: the Pions are already sorted based on energy
	    {
		fms_i = pair;
		break;
	    }
	}

	if(nTrkEast == 1 && fms_i > -1 && reader->evt_bbcADCSum[0] < 60 && reader->evt_bbcADCSumLarge[0] < 110)  //Sanity Check
	    hist8East->Fill(reader->rp_trackP[eastTrk_i] + reader->fms_pairE[fms_i]);
	    
	if(!(nTrkWest == 1 && nTrkEast == 0)) //Full RP Cuts with One track in west and no track in the east
	    continue;

	++eventCount[1]; //Post RP cut counter	
	
	if(fms_i < 0)  //Full FMS Cuts
	    continue;
	
	++eventCount[2]; //Post FMS cut counter

	hist11->Fill(reader->fms_pairM[fms_i]);
	hist12->Fill(reader->fms_pairE[fms_i]);
	hist2d5->Fill(reader->fms_pairX[fms_i], reader->fms_pairY[fms_i]);
	
	sumE_w = reader->rp_trackP[westTrk_i] + reader->fms_pairE[fms_i];
	
	hist13->Fill(reader->evt_tofMultiplicity);
	hist6->Fill(sumE_w);
	//------------- BBC and TOF Cut -----------------	    
	if(!(reader->evt_tofMultiplicity > 0))
	    continue;

	if(!(reader->evt_bbcADCSum[0] > 0))  //bbc 0 is east and 1 is west 
	    continue;

	hist14->Fill(reader->evt_bbcADCSumLarge[1]);
	hist15->Fill(reader->evt_bbcADCSum[1]);
	hist2d2->Fill(sumE_w, reader->evt_bbcADCSumLarge[1]);
	hist2d3->Fill(sumE_w, reader->evt_bbcADCSum[1]);
	    
	if(reader->evt_bbcADCSum[1] > 60)  //bbc 0 is east and 1 is west 
	    continue;
	    
	if(reader->evt_bbcADCSumLarge[1] > 110)  //bbc 0 is east and 1 is west 
	    continue;

	++eventCount[3];// Post BBC-TOF cut counter
	
	//-------------------------- FMS-RP Correlation -------------------------------
	if(reader->fms_nPairs == 1)	    //Extreme Cut:Single pion in FMS
	    hist7->Fill(sumE_w);
	
	hist8West->Fill(sumE_w);
	hist2d1->Fill(reader->fms_pairE[fms_i], reader->rp_trackP[westTrk_i]);

	//----------------------------- Diffractive p + p -----> pi^0 + p + X event cut ------------------------
	if((sumE_w > 80 && sumE_w < 107)    // <------- Energy Conservation cut (WIDER RANGE USED!!)
	   && (reader->fms_pairM[fms_i] > 0.0 && reader->fms_pairM[fms_i] < 0.25)) // <----- Pion mass range (WIDER RANGE USED!!)
	{
	    hist2d4->Fill(reader->fms_pairPhi[fms_i], reader->rp_trackPhi[westTrk_i]);
	    ++eventCount[4];	    
	}
    }

    hEvtCount_all->SetBinContent(2, hEvtCount->GetBinContent(4));
    hEvtCount_all->SetBinContent(4, eventCount[0]);
    hEvtCount_all->SetBinContent(6, eventCount[1]);
    hEvtCount_all->SetBinContent(8, eventCount[2]);
    hEvtCount_all->SetBinContent(10, eventCount[3]);
    hEvtCount_all->SetBinContent(12, eventCount[4]);
    
    // delete reader; 
    // delete chain;
    
    cout<<"Total events passed all cuts:" << eventCount[4] <<endl; 

    fout->cd();
    hEvtCount->Write();
    fout->Write();
    //fout->Close();
}
