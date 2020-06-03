// Filename: AnFmsRpCorrUpb.C
// Description: Fms-Rp correlations for unpolarized proton break up
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Nov 21 13:00:15 2019 (-0500)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "TStRunList.h"
#include "BrContainers/TStFmsPointPairData.h"
#include "BrContainers/TStEventData.h"
#include "BrContainers/TStRpsTrackData.h"
#include "BrContainers/TStEmcPointData.h"

using namespace std;

void AnFmsRpCorrUpb(Int_t firstRun, Int_t lastRun, TString outName, TString inFilePrefix)
{    
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList(firstRun, lastRun);
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    TString fileName;
    TString filePrefix = inFilePrefix;
    //----- Mask Hot Channels (naive approach) --------
    TFile *fHotCh = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/R15FmsTrigNanoDst/FmsPointXYmerged.root");
    TH2D *histHot = (TH2D*)fHotCh->Get("FmsPointXY_Merged");
    for(Int_t i = 1; i <= histHot->GetNbinsX(); ++i)
    {
    	for (Int_t j = 1; j <= histHot->GetNbinsY(); ++j)
    	{
    	    if(histHot->GetBinContent(i, j) > 2.5e6)
    		histHot->SetBinContent(i, j, 0);
    	}
    }
    
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
    TH1D* hist19 = new TH1D("emc_nPoints", "BEMC point multiplicity", 100, 0, 10);
    TH1D* hist20 = new TH1D("emc_pointE", "BEMC point energy; BEMC point energy [GeV]", 100, 0, 5);
    TH1D* hist21 = new TH1D("emc_eta", "BEMC point eta", 40, -1.0, 1.0);

    TH1D *hist_emc0 = new TH1D("sumE_west_emc0", "E_{p + #pi^{0}}^{West} [No activity in all bins]; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH1D *hist_emc1 = new TH1D("sumE_west_emc1", "E_{p + #pi^{0}}^{West} [No activity in bin 1]; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH1D *hist_emc2 = new TH1D("sumE_west_emc2", "E_{p + #pi^{0}}^{West} [No activity in bin 2 & 3]; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH1D *hist_emc3 = new TH1D("sumE_west_emc3", "E_{p + #pi^{0}}^{West} [No activity in bin 4]; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH1D *hist_emc4 = new TH1D("sumE_west_emc4", "E_{p + #pi^{0}}^{West} [No activity in bin 1 & 4]; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH1D *hist_emc5 = new TH1D("sumE_west_emc5", "E_{p + #pi^{0}}^{West} [No activity in bin 3 & 4]; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);

    
    TH2D *hist2d1 = new TH2D("E_p_vs_E_pion", "E_{p}^{west} vs E_{#pi^{0}}; E_{#pi^{0}} [GeV]; E_{p}^{west} [GeV]", 100, 10, 80, 100, 60, 150);
    TH2D *hist2d2 = new TH2D("E_sum_vs_BBC_large", "E_{sum} vs BBC ADC Sum (Large); E_{p + #pi^{0}}^{west} [GeV]; BBC ADC Sum (Large)", 100, 50, 200, 300, 0, 6000);
    TH2D *hist2d3 = new TH2D("E_sum_vs_BBC_small", "E_{sum} vs BBC ADC Sum (Small); E_{p + #pi^{0}}^{west} [GeV]; BBC ADC Sum (Small)", 100, 50, 200, 300, 0, 4000);
    TH2D *hist2d4 = new TH2D("p_phi_vs_pion_phi", "#phi_{p} vs #phi_{#pi^{0}}; #phi_{#pi^{0}} [rad]; #phi_{p} [rad]", 100, -3.15, 3.15, 100, -3.15, 3.15);
    TH2D *hist2d5 = new TH2D("pionXY", "#pi^{0} position; X [cm]; Y[cm]", 100, -100, 100, 100, -100, 100);

    TH2D *hist2d6 = new TH2D("E_sum_vs_VPD_west", "E_{sum} vs VPD ADC Sum (west); E_{p + #pi^{0}}^{west} [GeV]; BBC ADC Sum (Large)", 100, 50, 200, 300, 0, 6000);
    TH2D *hist2d7 = new TH2D("E_sum_vs_ZDC_west", "E_{sum} vs ZDC ADC Sum (west); E_{p + #pi^{0}}^{west} [GeV]; BBC ADC Sum (Small)", 100, 50, 200, 300, 0, 4000);

    
    TH1D *histEtaBin = new TH1D("histEtaBin", "BEMC Eta Bins", 4, -1.0, 1.0);
    
    TH1D *hEvtCount_all = new TH1D("hEvtCount_all", "Event Count", 20, 0, 20);
      
    //Input data     
    TTree *tree;
    Int_t nEntries = 0;
    TClonesArray *fmsArr = new TClonesArray("TStFmsPointPairData");
    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");
    TClonesArray *emcArr = new TClonesArray("TStEmcPointData");
    TStEventData *event = new TStEventData();
    TStFmsPointPairData *pion;
    TStRpsTrackData *rpsTrack;
    TStEmcPointData *emcPhoton;
    Int_t nPions = 0;
    Int_t nRpsTracks = 0;
    Int_t nEmcPhotons = 0;
    Double_t emcPhotonEng = 0.0;
    TVector3 emcVec;
    
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
    vector <Int_t> trigList;
    Int_t trigEvt = 0;
    
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
	tree->SetBranchAddress("emcPoint", &emcArr);

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
	    //-------Trigger Selection ------------
	    // if(std::find(trigList.begin(), trigList.end(), trig_SD) == trigList.end() && std::find(trigList.begin(), trigList.end(), trig_SDT) == trigList.end())
	    // 	continue;

	    ++trigEvt;
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

		//--------- Mask Hot Channel (Naive approach)------------
		if(histHot->GetBinContent(histHot->FindBin(pion->GetX1(), pion->GetY1())) == 0)
		    continue;
		if(histHot->GetBinContent(histHot->FindBin(pion->GetX2(), pion->GetY2())) == 0)
		    continue;

		// ------ Exclude Small Cells --------
		// if(fabs(pion->GetX1()) < 48 || fabs(pion->GetY1()) < 48 )
		//    continue;

		//    if(fabs(pion->GetX2()) < 48 || fabs(pion->GetY2()) < 48)
		//    continue;

		//-------- FPS Pid Cut ------------
		if(!(pion->GetFpsPid1() > 9 && pion->GetFpsPid1() < 17))
		    continue;

		if(!(pion->GetFpsPid2() > 9 && pion->GetFpsPid2() < 17))
		    continue;
		
		if(fms_i == -1) //consider only highest energy pair of photons, note: the Pions are already sorted based on energy
		{
		    fms_i = pi;
		    break;
		}		
	    }

	    if(nTrkEast == 1 && fms_i > -1 && event->GetBbcSumSmall(0) < 60 && event->GetBbcSumLarge(0) < 110)  //Sanity Check
	    {
		pion = (TStFmsPointPairData*)fmsArr->At(fms_i);
		rpsTrack = (TStRpsTrackData*)rpsArr->At(eastTrk_i);
		hist8East->Fill(rpsTrack->GetP() + pion->GetE());
	    }
	    
	    if(!(nTrkWest == 1 && nTrkEast == 0))   //Full RP Cuts with One track in west and no track in the east
	    	continue;

	    ++eventCount[1]; //Post RP cut counter	
	
	    if(fms_i < 0)    //Initial FMS Cuts
		continue;
	
	    pion = (TStFmsPointPairData*)fmsArr->At(fms_i);
	    rpsTrack = (TStRpsTrackData*)rpsArr->At(westTrk_i);	
	    	    
	    hist11->Fill(pion->GetM());
	    hist12->Fill(pion->GetE());
	    hist2d5->Fill(pion->GetX(), pion->GetY());
	    sumE_w = rpsTrack->GetP() + pion->GetE();
	
	    hist13->Fill(event->GetTofMultiplicity());
	    hist6->Fill(sumE_w);

	    if(!(pion->GetM() > 0.07 && pion->GetM() < 0.20)) //Pion mass cut. Full Pion cut
		continue;
	    
	    ++eventCount[2]; //Post FMS cut counter
	    //------------- BBC and TOF Cut -----------------	    
	    if(!(event->GetTofMultiplicity() > 0)) 
	    	continue;

	    if(!(event->GetBbcSumSmall(0) > 0))	//bbc 0 is east and 1 is west 
		continue;

	    hist14->Fill(event->GetBbcSumLarge(1));
	    hist15->Fill(event->GetBbcSumSmall(1));
	    hist2d2->Fill(sumE_w, event->GetBbcSumLarge(1));
	    hist2d3->Fill(sumE_w, event->GetBbcSumSmall(1));

	    hist2d6->Fill(sumE_w, event->GetVpdSum(1));
	    hist2d7->Fill(sumE_w, event->GetZdcSum(1));
	    
	    if(event->GetBbcSumSmall(1) > 60)	//bbc 0 is east and 1 is west 
		continue;
	    
	    if(event->GetBbcSumLarge(1) > 110) //bbc 0 is east and 1 is west 
		continue;

	    ++eventCount[3];			// Post BBC-TOF cut counter	    

	    // ---- VPD and ZDC Cut -----------
	    if(!(event->GetVpdSum(1) < 150 && event->GetZdcSum(1) < 15))
		continue;
	    
	    //----------- Mid Rapidity Cut for Rapidity Gap --------------------
	    emcPhotonEng = 0.0;
	    histEtaBin->Reset();
	    nEmcPhotons = emcArr->GetEntriesFast();
	    hist19->Fill(nEmcPhotons);
	    for(Int_t e = 0; e < nEmcPhotons; ++e)
	    {
		emcPhoton = (TStEmcPointData*)emcArr->At(e);
		hist20->Fill(emcPhoton->GetE());
		emcVec.SetXYZ(emcPhoton->GetX(), emcPhoton->GetY(), emcPhoton->GetZ());
		hist21->Fill(emcVec.Eta());
		histEtaBin->Fill(emcVec.Eta());
		if(emcPhoton->GetE() > emcPhotonEng)
		    emcPhotonEng = emcPhoton->GetE();      // Find maximum emc photon energy
	    }

	    if(histEtaBin->GetEntries() == 0)
		hist_emc0->Fill(sumE_w);
	    if(histEtaBin->GetBinContent(1) == 0)
		hist_emc1->Fill(sumE_w);
	    if(histEtaBin->GetBinContent(2) == 0 && histEtaBin->GetBinContent(3) == 0)
		hist_emc2->Fill(sumE_w);
	    if(histEtaBin->GetBinContent(4) == 0)
		hist_emc3->Fill(sumE_w);
	    if(histEtaBin->GetBinContent(1) == 0 && histEtaBin->GetBinContent(4) == 0)
		hist_emc4->Fill(sumE_w);
	    if(histEtaBin->GetBinContent(3) == 0 && histEtaBin->GetBinContent(4) == 0)
		hist_emc5->Fill(sumE_w);
	    
	    //-------------------------- FMS-RP Correlation -------------------------------
	    if(nPions == 1)	                //Extreme Cut:Single pion in FMS
		hist7->Fill(sumE_w);
	
	    hist8West->Fill(sumE_w);
	    hist2d1->Fill(pion->GetE(),  rpsTrack->GetP());

	    if(pion->GetM() > 0.0 && pion->GetM() < 0.20)
		hist18->Fill(sumE_w);
	    //----------------------------- Diffractive p + p -----> pi^0 + p + X event cut ------------------------
	    if((sumE_w > 80 && sumE_w < 107)            // <------- Energy Conservation cut (WIDER RANGE USED!!)
	       && (pion->GetM() > 0.0 && pion->GetM() < 0.25))  // <----- Pion mass range (WIDER RANGE USED!!)
	    {
		hist2d4->Fill(pion->GetPhi(), rpsTrack->GetPhi());
		for(Int_t t = 0; t < event->GetNtrigs(); ++t)
		    hist17->Fill(event->GetTrigger(t));
		
		++eventCount[4];	    
	    }	    
	}		
	//-----------
	
	++nRunsDone;
	file->Close();
	delete file;
    }

    hEvtCount_all->SetBinContent(2, eventCount[0]);
    hEvtCount_all->SetBinContent(4, trigEvt);
    hEvtCount_all->SetBinContent(6, eventCount[1]);
    hEvtCount_all->SetBinContent(8, eventCount[2]);
    hEvtCount_all->SetBinContent(10, eventCount[3]);
    hEvtCount_all->SetBinContent(12, eventCount[4]);
    
    outFile->Write();
}
