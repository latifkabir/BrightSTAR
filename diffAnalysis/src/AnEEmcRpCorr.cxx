// Filename: AnEEmcRpCorr.C
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

void AnEEmcRpCorr(Int_t firstRun, Int_t lastRun)
{
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList(firstRun, lastRun);
    Int_t nRuns = runList->GetN();
    Int_t run;
    TString basePath = "/star/u/kabir/GIT/BrightSTAR/dst/R15EEmcRpTree";
    
    Int_t nEntries_eemc1 = 0;
    Int_t nEntries_eemc3 = 0;
    Int_t nEntries_rp = 0;
    Int_t trk_i =  -1;
    Int_t nTrk = 0;
    Int_t eemc_i = -1;
    Bool_t isValid = true;
    Int_t evtCount = 0;

    TFile *fout = new TFile("EEmcRpCorr.root", "RECREATE");
    TH1D *hist0 = new TH1D("pi0M", "Pion M",  200, 0.0, 1.0);
    TH1D *hist0_ = new TH1D("pi0E", "Pion E",  200, 0.0, 50.0);
    TH1D *hist1 = new TH1D("trkP", "trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2 = new TH1D("trkPt", "trk Pt; RP track P_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3 = new TH1D("trkEta", "trk Eta; RP Track #Eta", 200, -10, 10);
    TH1D *hist4 = new TH1D("trkPhi", "trk Phi; RP track #phi [mrad]", 200, -4, 4);
    TH1D *hist5 = new TH1D("trkXi", "trk Xi; RP track #xi", 200, 0, 0);
    TH1D *hist6 = new TH1D("trkP_east", "Trk P East; RP track P_{east} [GeV/c]", 200, 60, 150);
    TH1D *hist7 = new TH1D("trkP_west", "Trk P West; RP track P_{west} [GeV/c]", 200, 60, 150);
    TH1D *hist8 = new TH1D("sumE_east", "E_{p + #pi^{0}}^{East}; E_{p + #pi^{0}}^{East} [GeV]", 200, 60, 200);
    TH1D *hist9 = new TH1D("sumE_west", "Sum E West; E_{p + #pi^{0}}^{West} [GeV]", 200, 60, 200);
    TH2D *hist2d1 = new TH2D("E_p_vs_E_pion", "E_{p} vs E_{#pi^{0}}; E_{#pi^{0}} [GeV]; E_{p} [GeV]", 100, 10, 80, 100, 60, 150);
    TH2D *hist2d2 = new TH2D("E_sum_vs_BBC_large", "E_{sum} vs BBC ADC Sum (Large); E_{p + #pi^{0}}^{west} [GeV]; BBC ADC Sum (Large)", 100, 50, 200, 300, 0, 6000);
    TH2D *hist2d3 = new TH2D("E_sum_vs_BBC_small", "E_{sum} vs BBC ADC Sum (Small); E_{p + #pi^{0}}^{west} [GeV]; BBC ADC Sum (Small)", 100, 50, 200, 300, 0, 4000);
    TH2D *hist2d4 = new TH2D("p_phi_vs_pion_phi", "#phi_{p} vs #phi_{#pi^{0}}; #phi_{#pi^{0}} [rad]; #phi_{p} [rad]", 100, -3.15, 3.15, 100, -3.15, 3.15);
    
    TH1D *hEvtCount_all = new TH1D("hEvtCount_all", "Event Count", 20, 0, 20);
    TH1D *hEvtCount_temp;

    TClonesArray *photon_arr = new TClonesArray("EEmcParticleCandidate_t");
    TClonesArray *pion_arr  = new TClonesArray("EEmc2ParticleCandidate_t");
    EEmc2ParticleCandidate_t *pion;
    Double_t pionE = 0;
    Double_t maxEng = 0;
    Int_t nPions = 0;
    
    for(Int_t r = 0; r < nRuns; ++r)
    {
	run = runList->GetEntry(r);

	// A dirty hack to resolve file closing issue for some DST files that results in freezing or crashing
	// Alternative solution is to merge all files in a single big tree and you can use these runs as well
	// if(run == 16072057 || run == 16072060 || run == 16073018 || run == 16079015 || run == 16080046 || run == 16082056 || run == 16083007 || run == 16083008 || run == 16085065 || run == 16087043 || run == 16089051 || run == 16090028 || run == 16090045)
	//     continue;
	
	TString fileName_eemc1;
	TString fileName_eemc3;
	TString fileName_rp;
	
	fileName_eemc1 = basePath + "/eemcTreeP1_EEmcRpTree_";
	fileName_eemc1 += run;
	fileName_eemc1 += ".root";

	fileName_eemc3 = basePath + "/eemcTreeP3_EEmcRpTree_";
	fileName_eemc3 += run;
	fileName_eemc3 += ".root";

	fileName_rp = basePath + "/RpTree_";
	fileName_rp += run;
	fileName_rp += ".root";
	
	if(gSystem->AccessPathName(fileName_eemc1) || gSystem->AccessPathName(fileName_eemc3) || gSystem->AccessPathName(fileName_rp))
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
	    continue;
	}
	
	TAnEEmcRpTreeReader *reader = new TAnEEmcRpTreeReader(ch_eemc1, ch_eemc3, ch_rp);
	//nEntries = tree->GetEntries();
	
	for(Int_t evt = 0; evt < nEntries_rp; ++evt)
	{
	    reader->GetEntry(evt);
	    //reader->Show();
	    
	    if(evt%2000 == 0)
		cout << "Events processed in this run:"<< (int)(100.0*evt / nEntries_rp) <<" %"<<endl;

	    if((reader->evt_run != reader->mRunNumber) || (reader->evt_id != reader->mEventNumber))
	    {
		cout << "The event numbers or run numbers do NOT match. Skipped ...." <<endl;
		cout << "Run number - RP T: "<< reader->evt_run<<" EEMC T: "<< reader->mRunNumber <<endl;
		cout << "Event number - RP T: "<< reader->evt_id<<" EEMC T: "<< reader->mEventNumber <<endl;
		continue;
	    }
	    
	    //------------------- RP Cuts --------------------------------
	    trk_i = -1;
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
		trk_i = t;
		if(nTrk > 1) //Allow only one good quality track in the event allowed
		{
		    trk_i = -1;
		    isValid = false;
		    break;
		}
	    }

	    if(!(nTrk == 1 && isValid))
		continue;

	    hist1->Fill(reader->rp_trackP[trk_i]);		
	    hist2->Fill(reader->rp_trackPt[trk_i]);
	    hist3->Fill(reader->rp_trackEta[trk_i]);
	    hist4->Fill(reader->rp_trackPhi[trk_i]);
	    hist5->Fill(reader->rp_trackXi[trk_i]);

	    if(reader->rp_trackBranch[trk_i] == 0 || reader->rp_trackBranch[trk_i] == 1) //East RP
		hist6->Fill(reader->rp_trackP[trk_i]);
		
	    if(reader->rp_trackBranch[trk_i] == 2 || reader->rp_trackBranch[trk_i] == 3) //West RP
		hist7->Fill(reader->rp_trackP[trk_i]);		
	    	    
	    //------------------------ EEMC Cut -----------------------------------
	    pion_arr = reader->pion_array;
	    photon_arr = reader->photon_array;
	    
	    if(!pion_arr || !photon_arr)
	    {
		cout << "Failed to get pion or photon array" <<endl;
		return;
	    }

	    nPions = pion_arr->GetEntriesFast();
	    //-------------TESTING <?????????????????????????????????????????? --------------------
	    if(nPions != 1)
	    	continue;
	   
	    eemc_i = -1;
	    maxEng = 0;
	    pionE = 0;
	    for(Int_t pair = 0; pair < nPions; ++pair)
	    {
		pion = (EEmc2ParticleCandidate_t*) pion_arr->At(pair);
		hist0->Fill(pion->M);

		if(pion->Z > 0.8)
		    continue;
		if(pion->E < 13 || pion->E > 70)
		    continue;

		if(pion->E > maxEng)
		{
		    eemc_i = pair;
		    maxEng = pion->E;
		    pionE = maxEng;		    
		}
	    }
	    
	    if(eemc_i < 0)
	    	continue;
	    hist0_->Fill(pionE);
	    //------------- BBC and TOF Cut -----------------	    
	    if(!(reader->evt_tofMultiplicity > 0))
	    	continue;

	    if(!(reader->evt_bbcADCSum[0] > 0))  //bbc 0 is east and 1 is west ???? <--------Check
	    	continue;

	    hist2d2->Fill(reader->rp_trackP[trk_i] + pionE, reader->evt_bbcADCSumLarge[1]);
	    hist2d3->Fill(reader->rp_trackP[trk_i] + pionE, reader->evt_bbcADCSum[1]);
	    
	    if(reader->evt_bbcADCSum[1] > 60)  //bbc 0 is east and 1 is west ???? <--------Check
	    	continue;
	    
	    if(reader->evt_bbcADCSumLarge[1] > 110)  //bbc 0 is east and 1 is west ???? <--------Check
	    	continue;
	    
	    //-------------------------- FMS-RP Correlation -------------------------------

	    //-------------- Veto on East RP track --------------------------
	    if(reader->rp_trackBranch[trk_i] == 0 || reader->rp_trackBranch[trk_i] == 1) //East
	    {
		hist8->Fill(reader->rp_trackP[trk_i] + pionE);
		continue;
	    }

	    //--- Ensure track comes from West RP and exclude undecided branch case --------
	    if(!(reader->rp_trackBranch[trk_i] == 2 || reader->rp_trackBranch[trk_i] == 3)) //West
		continue;
	    
	    hist9->Fill(reader->rp_trackP[trk_i] + pionE);
	    hist2d1->Fill(pionE, reader->rp_trackP[trk_i]);

	    if((reader->rp_trackP[trk_i] + pionE) > 85 && (reader->rp_trackP[trk_i] + pionE) < 105) // <------- Add cut on pion mass
		//hist2d4->Fill(reader->fms_pairPhi[eemc_i], reader->rp_trackPhi[trk_i]);	    
		++evtCount;	   
	}
	
        delete reader; 
	delete ch_eemc1;
	delete ch_eemc3;
	delete ch_rp;
    }
    cout<<"Total events passed the cut:" << evtCount <<endl; 

    //fout->cd();
    fout->Write();
    //fout->Close();
}
