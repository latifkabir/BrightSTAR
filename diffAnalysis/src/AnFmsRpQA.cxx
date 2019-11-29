// Filename: AnFmsRpQA.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Nov 21 13:00:15 2019 (-0500)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "TAnFmsRpTreeReader.h"
#include "TStRunList.h"
using namespace std;

void AnFmsRpQA(Int_t firstRun, Int_t lastRun)
{
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList(firstRun, lastRun);
    Int_t nRuns = runList->GetN();
    Int_t run;
    //TString basePath = "/star/u/kabir/GIT/BrightSTAR/dst/FmsRpTreeMaker_ucr/RunFmsRpTreeMaker_";
    TString basePath = "/star/u/kabir/GIT/BrightSTAR/dst/FmsRpTreeMaker_ucr/RunFmsRpTreeMaker_Merger_16067013_to_16080002.root";
    
    Int_t nEntries = 0;
    Int_t trk_i[2] = { -1, -1};
    Int_t nTrk = 0;
    Int_t fms_i = -1;
    Bool_t isValid = true;
    Int_t evtCount = 0;
    Int_t maxRpTracks = 1;

    TFile *fout = new TFile("FmsRpQA.root", "RECREATE");
    TH1D *hist1 = new TH1D("trkP", "trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2 = new TH1D("trkPt", "trk Pt; RP track P_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3 = new TH1D("trkEta", "trk Eta; RP Track #Eta", 200, -10, 10);
    TH1D *hist4 = new TH1D("trkPhi", "trk Phi; RP track #phi [mrad]", 200, -4, 4);
    TH1D *hist5 = new TH1D("trkXi", "trk Xi; RP track #xi", 200, 0, 0);
    TH1D *hist6 = new TH1D("trkP_east", "Trk P East; RP track P_{east} [GeV/c]", 200, 60, 150);
    TH1D *hist7 = new TH1D("trkP_west", "Trk P West; RP track P_{west} [GeV/c]", 200, 60, 150);
    TH1D *hist8 = new TH1D("sumE_east", "E_{p + #pi^{0}^{East}; E_{p + #pi^{0}^{East} [GeV]", 200, 60, 200);
    TH1D *hist9 = new TH1D("sumE_west", "Sum E West; E_{p + #pi^{0}^{West} [GeV]", 200, 60, 200);
    TH2D *hist2d1 = new TH2D("E_p_vs_E_pion", "E_{p} vs E_{#pi^{0}}; E_{p} [GeV]; E_{#pi^{0}} [GeV]", 100, 10, 100, 100, 60, 150);
    TH2D *hist2d2 = new TH2D("E_sum_vs_BBC_large", "E_{sum} vs BBC ADC Sum (Large); E_{p + #pi^{0}^{East} [GeV]; BBC ADC Sum (Large)", 100, 50, 200, 300, 0, 6000);
    TH2D *hist2d3 = new TH2D("E_sum_vs_BBC_small", "E_{sum} vs BBC ADC Sum (Small); E_{p + #pi^{0}^{East} [GeV]; BBC ADC Sum (Small)", 100, 50, 200, 300, 0, 4000);
    TH2D *hist2d4 = new TH2D("p_phi_vs_pion_phi", "#phi_{p} vs #phi_{#pi^{0}}; #phi_{#pi^{0}} [rad]; #phi_{p} [rad]", 100, -3.15, 3.15, 100, -3.15, 3.15);
    
    TH1D *hEvtCount_all = new TH1D("hEvtCount_all", "Event Count", 20, 0, 20);
    TH1D *hEvtCount_temp;
    
    for(Int_t r = 0; r < nRuns; ++r)
    {
	run = runList->GetEntry(r);

	//For now do have only one third of total runs
	if(lastRun != -1 && run > lastRun)
	    break;
	
	TString fileName;
	fileName = basePath;
	// fileName += run;
	// fileName += ".root";
	if(gSystem->AccessPathName(fileName))
	    continue;

	cout << "Processing run# "<< run << endl;

	// TFile *file = new TFile(fileName);
	// TTree *tree = (TTree*)file->Get("T");
	// nEntries = tree->GetEntries();
	// hEvtCount_temp = (TH1D*)file->Get("hEvtCount");
	// if(hEvtCount_temp && nEntries > 1000)  //Skip runs when RP was completely out or in for part of the run.
	//     hEvtCount_all->Add(hEvtCount_temp);

	//file->Close();
	//delete file;

	TChain *chain = new TChain("T");
	chain->Add(fileName);
	nEntries = chain->GetEntries();
	TAnFmsRpTreeReader *reader = new TAnFmsRpTreeReader(chain);
	//nEntries = tree->GetEntries();
	
	for(Int_t evt = 0; evt < nEntries; ++evt)
	{
	    reader->GetEntry(evt);
	    //reader->Show();

	    if(evt%2000 == 0)
		cout << "Events processed in this run:"<< (int)(100.0*evt / nEntries) <<" %"<<endl;
	    
	    //------------------- RP Cuts --------------------------------
	    trk_i[0] = -1;
	    trk_i[1] = -1;
	    nTrk = 0;
	    isValid = true;
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

		++nTrk;
		trk_i[nTrk - 1] = t;
		if(nTrk > maxRpTracks)
		{
		    trk_i[0] = -1;
		    trk_i[1] = -1;
		    isValid = false;
		    break;
		}
	    }

	    if(!(nTrk > 0 && nTrk <= maxRpTracks && isValid))
		continue;

	    for(Int_t trk = 0; trk < nTrk; ++trk)
	    {
		hist1->Fill(reader->rp_trackP[trk_i[trk]]);		
		hist2->Fill(reader->rp_trackPt[trk_i[trk]]);
		hist3->Fill(reader->rp_trackEta[trk_i[trk]]);
		hist4->Fill(reader->rp_trackPhi[trk_i[trk]]);
		hist5->Fill(reader->rp_trackXi[trk_i[trk]]);

		if(reader->rp_trackBranch[trk_i[trk]] == 0 || reader->rp_trackBranch[trk_i[trk]] == 1) //East RP
		    hist6->Fill(reader->rp_trackP[trk_i[trk]]);
		
		if(reader->rp_trackBranch[trk_i[trk]] == 2 || reader->rp_trackBranch[trk_i[trk]] == 3) //West RP
		    hist7->Fill(reader->rp_trackP[trk_i[trk]]);		
	    }

	    //-------------TESTING <??????????????????????????????????????????
	    // if(reader->fms_nPairs > 1)
	    // 	continue;
	    
	    //------------------------ FMS Cut -----------------------------------
	    fms_i = -1;
	    for(Int_t pair = 0; pair < reader->fms_nPairs; ++pair)
	    {
		if(reader->fms_pairZgg[pair] < 0.8)
		    continue;
		if(reader->fms_pairE[pair] < 13 || reader->fms_pairE[pair] > 70)
		    continue;

		fms_i = pair;
		break; //consider only highest energy pair of photons
	    }

	    if(fms_i < 0)
		continue;

	    //-------------- Veto on East RP track --------------------------
	    if(reader->rp_trackBranch[trk_i[0]] == 0 || reader->rp_trackBranch[trk_i[0]] == 1) //East
		continue;
	    
	    //------------- BBC and TOF Cut -----------------	    
	    if(!(reader->evt_tofMultiplicity > 0))
		continue;

	    if(!(reader->evt_bbcADCSum[0] > 0))  //bbc 0 is east and 1 is west ???? <--------Check
		continue;

	    hist2d2->Fill(reader->rp_trackP[trk_i[0]] + reader->fms_pairE[fms_i], reader->evt_bbcADCSumLarge[1]);
	    hist2d3->Fill(reader->rp_trackP[trk_i[0]] + reader->fms_pairE[fms_i], reader->evt_bbcADCSum[1]);
	    
	    if(reader->evt_bbcADCSum[1] > 60)  //bbc 0 is east and 1 is west ???? <--------Check
		continue;
	    
	    if(reader->evt_bbcADCSumLarge[1] > 110)  //bbc 0 is east and 1 is west ???? <--------Check
		continue;
	    
	    //-------------------------- FMS-RP Correlation -------------------------------
	    for(Int_t trk = 0; trk < nTrk; ++trk)
	    {
		if(reader->rp_trackBranch[trk_i[trk]] == 0 || reader->rp_trackBranch[trk_i[trk]] == 1) //East
		    hist8->Fill(reader->rp_trackP[trk_i[trk]] + reader->fms_pairE[fms_i]);
		
		if(reader->rp_trackBranch[trk_i[trk]] == 2 || reader->rp_trackBranch[trk_i[trk]] == 3) //West
		    hist9->Fill(reader->rp_trackP[trk_i[trk]] + reader->fms_pairE[fms_i]);

		hist2d1->Fill(reader->fms_pairE[trk_i[trk]], reader->rp_trackP[trk_i[trk]]);
	    }


	    if((reader->rp_trackP[trk_i[0]] + reader->fms_pairE[fms_i]) > 85 && (reader->rp_trackP[trk_i[0]] + reader->fms_pairE[fms_i]) < 105)
		hist2d4->Fill(reader->fms_pairPhi[fms_i], reader->rp_trackPhi[trk_i[0]]);
		
	    ++evtCount;
	}

        // delete reader; //This deletes the TFile as well in the generated code
	// delete chain;
    }
    cout<<"Total events passed the cut:" << evtCount <<endl; 

    //fout->cd();
    fout->Write();
    //fout->Close();
}
