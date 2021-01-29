// Filename: AnIncPion.C
// Description: 
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

void AnIncPion(Int_t firstRun, Int_t lastRun, TString outName, TString inFilePrefix)
{    
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList(firstRun, lastRun);
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    TString fileName;
    TString filePrefix = inFilePrefix;

    TFile *outFile = new TFile(outName, "RECREATE");

    TH1D *histMass = new TH1D("pi0M", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);
    TH1D *histMassUp = new TH1D("pi0Mup", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);
    TH1D *histMassDn = new TH1D("pi0Mdn", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);
    TH1D *histEng = new TH1D("pi0E", "Diphoton Energy; E_{#gamma#gamma} [GeV]", 200, 10, 100);
    
    TH1D* histTof = new TH1D("tofMult", "TOF Multiplicity", 250, 0, 500);
    TH1D* histBbcL = new TH1D("bbcSumL_west", "West Large BBC ADC Sum", 250, 0, 8000);
    TH1D* histBbcS = new TH1D("bbcSumS_west", "West Small BBC ADC Sum", 250, 0, 45000);
    TH1D* histTrig = new TH1D("trigger", "Trigger Distribution", 1000, 480000, 481000);

    TH1D* histSpin = new TH1D("spin", "spin", 5, -2.0, 2.0);
    TH1D* histPhiB = new TH1D("phiB", "phiB", 100, -1.0*TMath::Pi(), 1.0*TMath::Pi());
    TH1D* histPhiY = new TH1D("phiY", "phiY", 100, -1.0*TMath::Pi(), 1.0*TMath::Pi());
    TH1D* histXf = new TH1D("xf", "xf", 120, -1.2, 1.2);    
    TH2D *histXY = new TH2D("pionXY", "#pi^{0} position; X [cm]; Y[cm]", 100, -100, 100, 100, -100, 100);

    //--- Histograms for A_N calculation ---
    const Int_t kPhiBins = 16;
    const Int_t kXfBins = 10;
    Double_t xfBins[] = {0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7};
    TH2D *bXfVsPhiUp = new TH2D("bXfVsPhiUp","bXfVsPhiUp", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
    TH2D *bXfVsPhiDn = new TH2D("bXfVsPhiDn","bXfVsPhiDn", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);

    TH2D *bXfVsPhiUp_sig = new TH2D("bXfVsPhiUp_sig","bXfVsPhiUp_sig", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
    TH2D *bXfVsPhiDn_sig = new TH2D("bXfVsPhiDn_sig","bXfVsPhiDn_sig", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);

    TH2D *bXfVsPhiUp_sb = new TH2D("bXfVsPhiUp_sb","bXfVsPhiUp_sb", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
    TH2D *bXfVsPhiDn_sb = new TH2D("bXfVsPhiDn_sb","bXfVsPhiDn_sb", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);


    TH2D *bEngVsMassUp = new TH2D("bEngVsMassUp","bEngVsMassUp", 100, 0.0, 0.35, 100, 0.0, 100);
    TH2D *bEngVsMassDn = new TH2D("bEngVsMassDn","bEngVsMassDn", 100, 0.0, 0.35, 100, 0.0, 100);
    
    TH3D *bXfVsPhiVsMassUp = new TH3D("bXfVsPhiVsMassUp","bXfVsPhiVsMassUp", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), 14, 0.0, 0.70 , 100, 0.0, 0.35);
    TH3D *bXfVsPhiVsMassDn = new TH3D("bXfVsPhiVsMassDn","bXfVsPhiVsMassDn", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), 14, 0.0, 0.70, 100, 0.0, 0.35);
    
    //-------

    
    //Input data     
    TTree *tree;
    Int_t nEntries = 0;
    TClonesArray *fmsArr = new TClonesArray("TStFmsPointPairData");
    TStEventData *event = new TStEventData();
    TStFmsPointPairData *pion;

    Int_t nPions = 0;
    Int_t spinB = 0;
    Int_t spinY = 0;
    Double_t mass;
    Double_t eng;
    Double_t phi;
    Double_t phiB;
    Double_t phiY;
    Double_t xf;
    Double_t sqrt_s = 200.0;
    TLorentzVector LV;
    
    cout << "Total number of files to be processed: "<< maxRuns <<endl;

    //-----
    vector <Int_t> trigList;
    
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

	//----------- Loop over Events ---------------------
	for(Int_t evt = 0; evt < nEntries; ++evt)
	{
	    // fmsArr->Clear(); //This is automatically called unless you turn off auto delete. 
	    // Also Clear Event  buffer if it contains TClonesArray as member

	    tree->GetEntry(evt);

	    spinB = event->GetBlueSpin();
	    spinY = event->GetYellowSpin();

	    histSpin->Fill(spinB);
	    
	    if(spinB == 0 || spinY == 0)
		continue;
	    
	    trigList.clear();
	    for(Int_t t = 0; t < event->GetNtrigs(); ++t)
	    {
		histTrig->Fill(event->GetTrigger(t));
		trigList.push_back(event->GetTrigger(t));
	    }
	    //-------Trigger Selection ------------
	    // if(std::find(trigList.begin(), trigList.end(), trig_SD) == trigList.end() && std::find(trigList.begin(), trigList.end(), trig_SDT) == trigList.end())
	    // 	continue;

	    //------------- BBC and TOF  -----------------
	    histTof->Fill(event->GetTofMultiplicity());
	    histBbcL->Fill(event->GetBbcSumLarge(0));
	    histBbcS->Fill(event->GetBbcSumSmall(0));
	    	    	    
	    //---------- FMS Pion candidates ---------------
	    nPions = fmsArr->GetEntriesFast();
	    for(Int_t pi = 0; pi < nPions; ++pi)
	    {
		pion = (TStFmsPointPairData*)fmsArr->At(pi);
		mass = pion->GetM();
		eng = pion->GetE();
		//--------------- FMS Cut ----------------
		if(pion->GetZgg() > 0.8)
		    continue;
	    
		if(eng < 12 || eng > 100)
		    continue;

		if(!(mass > 0.0 && mass < 0.35))
		    continue;
				
		histMass->Fill(mass);
		histEng->Fill(eng);
		histXY->Fill(pion->GetX(), pion->GetY());
		
		phi = pion->GetPhi();
		if(phi >= 0)
		{
		    phiB = phi;
		    phiY = TMath::Pi() - phi;
		}
		else
		{
		    phiB = phi;
		    phiY = -1.0*TMath::Pi() - phi;		
		}
		histPhiB->Fill(phiB);
		histPhiY->Fill(phiY);
	    
		LV.SetPtEtaPhiE(pion->GetPt(), pion->GetEta(), pion->GetPhi(), pion->GetE());
		xf = 2.0*(LV.Pz()) / sqrt_s;
		histXf->Fill(xf);

		//---- Fill Hist for A_N calculation -----
		if(spinB == 1)
		{
		    histMassUp->Fill(mass);
		    bXfVsPhiUp->Fill(phiB, xf);
		    bEngVsMassUp->Fill(mass, eng);
		    bXfVsPhiVsMassUp->Fill(phiB, xf, mass);
		    if(mass > 0 && mass <= 0.2)
			bXfVsPhiUp_sig->Fill(phiB, xf);
		    else if(mass > 0.2 && mass <= 0.3)
			bXfVsPhiUp_sb->Fill(phiB, xf);
		}
		else if(spinB == -1)
		{
		    histMassDn->Fill(mass);
		    bXfVsPhiDn->Fill(phiB, xf);
		    bEngVsMassDn->Fill(mass, eng);
		    bXfVsPhiVsMassDn->Fill(phiB, xf, mass);
		    if(mass > 0 && mass <= 0.2)
			bXfVsPhiDn_sig->Fill(phiB, xf);
		    else if(mass > 0.2 && mass <= 0.3)
			bXfVsPhiDn_sb->Fill(phiB, xf);
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
