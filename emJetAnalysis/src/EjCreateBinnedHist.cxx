// Filename: EjCreateBinnedHist.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri May  8 15:08:25 2020 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include "EjAna.h"
#include "EjCreateBinnedHistExtended.h"
#include "RootInclude.h"
#include "cppInclude.h"
#include "BrightStInclude.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"
#include "BrJetMaker/TStJetSkimEvent.h"
using namespace std;

void EjCreateBinnedHist(Int_t fillNo, TString fileNamePrefix, TString det, Int_t firstRun, Int_t lastRun)
{
    /*
      We need to bin in: energy (5), number of photons (6), phi (16), spin (2), pt(6).
      Let's create TH2D histograms of array size [2(spin)][4(energy)][#photon(5)]. The 2D histogram to be filled with phi bins along x and pt bins along y.
      We need another similar array for yellow beam as well.
    */
    TString outName = (TString)"BinnedHist_" + to_string(fillNo) + (TString)".root";
    TFile *file = new TFile(outName, "recreate");
    const Int_t kSpinBins = 2;
    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;
    const Int_t kPhiBins = 16;
    TH2D *bHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    TH2D *yHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    Double_t ptBins[] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0};
    Double_t engBins[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0}; //For info only
    const Int_t nPtBins = sizeof(ptBins) / sizeof(Double_t) - 1;
    
    for(Int_t i = 0; i < kSpinBins; ++i)
    {
	for(Int_t j = 0; j < kEnergyBins; ++j)
	{
	    for(Int_t k = 0; k < kPhotonBins; ++k)
	    {
		TString bTitle = Form("bHist_%i_%i_%i", i, j, k);
		TString yTitle = Form("yHist_%i_%i_%i", i, j, k);
		bHist[i][j][k] = new TH2D(bTitle, bTitle, kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), nPtBins, ptBins);
		yHist[i][j][k] = new TH2D(yTitle, yTitle, kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), nPtBins, ptBins);
	    }
	}
    }

    Int_t bSpin_i;
    Int_t ySpin_i;
    Int_t nPhotons_i;
    Int_t phi_i;
    Int_t eng_i;

    TH1D *h1bSpinI = new TH1D("h1bSpinI", "Blue Spin index", 10, -1, 4);
    TH1D *h1ySpinI = new TH1D("h1ySpinI", "Yellow Spin index", 10, -1, 4);
    TH1D *h1nPhotonsI = new TH1D("h1nPhotonsI", "Number of photons index", 11, -1, 10);    
    TH1D *h1EngI = new TH1D("h1EngI", "Energy bin index", 7, -1, 6);
    TH1D *h1PhiB = new TH1D("h1PhiB", "Phi [Blue beam]", kPhiBins, -1.0*TMath::Pi(), TMath::Pi());
    TH1D *h1PhiY = new TH1D("h1PhiY", "Phi [Yellow beam]", kPhiBins, -1.0*TMath::Pi(), TMath::Pi());
    TH1D *hPolB = new TH1D("hPolB", "Polarization [Blue Beam]", 400, 40, 80);
    TH1D *hPolY = new TH1D("hPolY", "Polarization [Yellow Beam]", 400, 40, 80);
    TH1D *hPolBerr = new TH1D("hPolBerr", "Polarization Error[Blue Beam]", 500, 0, 10);
    TH1D *hPolYerr = new TH1D("hPolYerr", "Polarization Error[Yellow Beam]", 500, 0, 10);
    
    Double_t etaMin;
    Double_t etaMax;
    if(det == "fms")
    {
	//limit jet axis within 2.8 - 3.8 following Carl's suggestion in spin pwg  mailing list 2019-11-22 email.
	etaMin = 2.8;  //2.4;
	etaMax = 3.8;  //4.5;
    }
    else if(det == "eemc")
    {
	etaMin = 1.0; //0.8;	
	etaMax = 2.0; //2.5;
    }
    else
    {
	cout << "Invalid detector" <<endl;
	return;
    }

    //---------- QA Histograms for events that enter in A_N Calculation i.e after all event selection cuts ------------
    TH1D *h1nJets  = new TH1D("h1nJets ", "Number of Jets [FMS or EEMC] after cuts", 10, 0, 10);
    TH1D *h1spinB = new TH1D("h1spinB", "Blue beam spin after all cuts; jet spin", 5, -2, 2);
    TH1D *h1spinY = new TH1D("h1spinY", "Yellow beam spin after all cuts; jet spin", 5, -2, 2);
    TH1D *h1Eta = new TH1D ("h1Eta", "EM Jet Eta after all cuts; Jet #eta", 100, 1.0, 5.0);
    TH1D *h1Phi = new TH1D ("h1Phi", "EM Jet Phi after all cuts; Jet #phi [rad]", 100, -3.3, 3.3);
    TH1D *h1E = new TH1D ("h1E", "EM Jet E after all cuts; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1Pt = new TH1D ("h1Pt", "Jet Pt after all cuts; Jet Pt [GeV/c]", 100, 0.0, 25.0);
    TH1D *h1nPhotons = new TH1D("h1nPhotons", "number of photons in EM jets after all cuts; Number of Photons", 20, 0, 20);
    TH1D *h1vtxZ = new TH1D("h1vtxZ", "Jet vetrex z; Jet vertex z [cm]", 100, -200, 200);
    TH1D *h1Trig = new TH1D("h1Trig", "Trigger Distribution (Without any cut)", 10, 0, 10);

    TH1D *hAvgXf[kEnergyBins][nPtBins];
    for(Int_t i = 0; i < kEnergyBins; ++i)
    {
	for(Int_t j = 0; j < nPtBins; ++j)
	    hAvgXf[i][j] = new TH1D(Form("hAvgXf%i_%i", i, j), Form("hAvgXf%i_%i", i, j), 100, 0, 1.0);
    }
    
    if(det == "fms")
    {
	h1Trig->GetXaxis()->SetBinLabel(1,"FMS JP0");
	h1Trig->GetXaxis()->SetBinLabel(2,"FMS JP1");
	h1Trig->GetXaxis()->SetBinLabel(3,"FMS JP2");
	h1Trig->GetXaxis()->SetBinLabel(4,"Small BS1");
	h1Trig->GetXaxis()->SetBinLabel(5,"Small BS2");
	h1Trig->GetXaxis()->SetBinLabel(6,"Small BS3");
	h1Trig->GetXaxis()->SetBinLabel(7,"Large BS1");
	h1Trig->GetXaxis()->SetBinLabel(8,"Large BS2");
	h1Trig->GetXaxis()->SetBinLabel(9,"Large BS3");
    }
    else if(det == "eemc")
    {
	h1Trig->GetXaxis()->SetBinLabel(1,"EHT0");
	h1Trig->GetXaxis()->SetBinLabel(2,"JP1");
	h1Trig->GetXaxis()->SetBinLabel(3,"JP2");
	h1Trig->GetXaxis()->SetBinLabel(4,"EHT0*EJP1*L2Egamma");
	h1Trig->GetXaxis()->SetBinLabel(5,"JP2*L2JetHigh");
	h1Trig->GetXaxis()->SetBinLabel(6,"BHT1*VPDMB-30");
	h1Trig->GetXaxis()->SetBinLabel(7,"BHT0*BBCMB");
	h1Trig->GetXaxis()->SetBinLabel(8,"BHT1*BBCMB");
	h1Trig->GetXaxis()->SetBinLabel(9,"BHT2*BBCMB");
    }
    
    //Event buffer
    TStJetEvent *jetEvent = 0;
    TStJetSkimEvent *skimEvent;
    TStJetCandidate *jet;
    TStJetTower *tower;
    TStJetParticle *particle;
    
    Int_t nPhotons;
    Int_t spinB;
    Int_t spinY;
    Int_t phiBin;
    Double_t eng;
    Double_t pt;
    Double_t eta;
    Double_t phi;
    Double_t theta;
    Double_t jetX;
    Double_t jetY;
    Double_t vtxZ;
    Double_t rt;
    Double_t xf;
    TLorentzVector LV; 
    Double_t sqrt_s;
    
    Double_t phi_y;
    Double_t phi_b;

    TStFillNoDB fillDb;
    TStRunList runListDb;
    //vector <int> runList = fillDb.GetRunsWithFill(fillNo);
    TEntryList *runList = runListDb.GetRunList(firstRun, lastRun);
    
    Int_t runNumber;
    TString fileName;
    Bool_t didPassPtCut;
    
    //For Beam Polarization
    Int_t fillNoFmData;
    Int_t lastFillNoFmData = -1;
    Int_t evtTime;
    Double_t dT;
    Double_t pol_b;
    Double_t pol_y;
    Double_t ePol_b;
    Double_t ePol_y;
    Int_t nPoints = 0;
    Int_t nRuns = 0;
    Bool_t eventAccepted = kFALSE;
        
    Int_t fillNoFmDb;
    Double_t energy;
    Int_t startTime;
    Int_t stopTime;
    Double_t p_b;
    Double_t dp_b;
    Double_t dpdt_b;
    Double_t edpdt_b;
    Double_t p_y;
    Double_t dp_y;
    Double_t dpdt_y;
    Double_t edpdt_y;
    
    //for(Int_t r = 0; r < runList.size(); ++r)
    for(Int_t r = 0; r < runList->GetN(); ++r)
    {
	//runNumber = runList[r];
	runNumber = runList->GetEntry(r);
	fileName = fileNamePrefix + to_string(runNumber) + ".root";
	if(gSystem->AccessPathName(fileName))
	{
	    cout << "File Not found:" << fileName <<endl;
	    cout << "SKIPPED" <<endl;
	    continue;
	}

	//Skip wrongly filled spin pattern runs: See Run 15 Polarization file. Fill: 18764
	fillNoFmData = fillDb.GetFillNo(runNumber); //Get it from the DST in the future
	if(fillNoFmData == 18764)
	    continue;
	TFile *tFile = new TFile(fileName);
	TTree *tree = (TTree*)tFile->Get("T");
	if(!tree)
	{
	    cout << "No Tree found in"<< fileName <<" ... SKIPPED"<<endl;
	    tFile->Close();
	    delete tFile;	    
	    continue;
	}	
	tree->SetBranchAddress("jetEvents", &jetEvent);

	//See: Carl's e-mail to Cold QCD pwg mailing list on 2019-11-22. 15% higher than nominal trigger threshold.
	Double_t sm_bs1 = 1.15; 
	Double_t sm_bs3 = 2.18; 
	Double_t lg_bs1 = 1.15; 
	if(runNumber < 16069040)
	{
	    sm_bs1 = 1.26;
	    sm_bs3 = 2.53;
	    lg_bs1 = 1.26;
	}
	Double_t fmsTrigPtTh[9] = {1.84, 2.76, 3.68, sm_bs1, 1.84, sm_bs3, lg_bs1, 1.84, 2.76}; //"FMS-JP0", "FMS-JP1", "FMS-JP2", "FMS-sm-bs1", "FMS-sm-bs2", "FMS-sm-bs3", "FMS-lg-bs1", "FMS-lg-bs2", "FMS-lg-bs3"

	Double_t eemcTrigPtTh[9] = {4.25, 5.405, 7.285, 4.25, 7.285, 0, 0, 0, 0}; //"EHT0", "JP1", "JP2", "EHT0*EJP1*L2Egamma", "JP2*L2JetHigh", "BHT1*VPDMB-30", "BHT0*BBCMB", "BHT1*BBCMB", "BHT2*BBCMB";
	
	nPoints = 0;
	    
	Int_t nEntries = tree->GetEntries();
	cout << "Processing run number: "<< runNumber <<endl;
	cout << "Total events to be processed: "<< nEntries <<endl;

	for(Int_t evt = 0; evt < nEntries; ++evt)
	{
	    jetEvent->Reset();
	    
	    if(evt % 5000 == 0)
		cout << "Events processed: "<< evt <<endl;
	
	    tree->GetEntry(evt);

	    skimEvent = jetEvent->GetEvent();
	    vtxZ = skimEvent->GetVertexZ();
	    spinB = skimEvent->GetSpinB();	    
	    spinY = skimEvent->GetSpinY();
	    evtTime = skimEvent->GetUnixTime();
	    eventAccepted = kFALSE;

	    if(runNumber < 18000000)
		sqrt_s = 200;        //Run 15
	    else
		sqrt_s = 510;        //Run 17
	    
	    //Trigger distribution without cuts
	    if(skimEvent->GetTrigFlag(0)) //Alternatively use: if(skimEvent->IsTrigBitSet(0)) etc
		h1Trig->Fill(0);
	    if(skimEvent->GetTrigFlag(1))
		h1Trig->Fill(1);
	    if(skimEvent->GetTrigFlag(2))
		h1Trig->Fill(2);
	    if(skimEvent->GetTrigFlag(3))
		h1Trig->Fill(3);
	    if(skimEvent->GetTrigFlag(4))
		h1Trig->Fill(4);
	    if(skimEvent->GetTrigFlag(5))
		h1Trig->Fill(5);
	    if(skimEvent->GetTrigFlag(6))
		h1Trig->Fill(6);
	    if(skimEvent->GetTrigFlag(7))
		h1Trig->Fill(7);
	    if(skimEvent->GetTrigFlag(8))
		h1Trig->Fill(8);
	    
	    //Exclude FMS small-bs3 trigger that gives ring of fire issue.
	    //The ring of fire is a real problem. It gives lots of unphysical jets i.e. jets with E > s /2 or X_F > 1.0. It must be removed for any reliable analysis.
	    
	    // if(det == "fms")
	    // {
	    // 	if(skimEvent->GetTrigFlag(5)) 
	    // 	    continue;
	    // }
	    
	    if(fabs(vtxZ) > 80)
		continue;
	
	    if(spinB == -1)
		bSpin_i = 0;
	    else if(spinB == 1)
		bSpin_i = 1;
	    else
		continue;
	
	    if(spinY == -1)
		ySpin_i = 0;
	    else if(spinY == 1)
		ySpin_i = 1;
	    else
		continue;

	    h1nJets->Fill(jetEvent->GetNumberOfJets());
	    h1vtxZ->Fill(vtxZ);
	    for(Int_t j = 0; j < jetEvent->GetNumberOfJets(); ++j)
	    {
		didPassPtCut = kFALSE;
		jet = jetEvent->GetJet(j);
		
		eta = jet->GetEta();
		phi = jet->GetPhi();
		theta =  2 * atan( exp(-eta) );
		eng = jet->GetE();
		pt = jet->GetPt();
		nPhotons = jet->GetNumberOfTowers();

		if(det == "fms")
		{
		    pt = jet->GetPt() - jet->GetUedPt();

		    pt = EjJetPtCorr(pt, eng);
		    eng = EjJetEngCorr(eng);
		}
		
		if(eta < etaMin || eta > etaMax) //Conside only EEMC and FMS coverage
		    continue;

		if(pt < EjAna::kPtMin)
		    continue;
		
		//Trigger dependent Pt cuts: See: Carl's e-mail to Cold QCD pwg mailing list on 2019-11-22.
		for(Int_t t = 0; t < 9; ++t)
		{
		    if(det == "fms")
		    {
			if(skimEvent->GetTrigFlag(t) && pt > fmsTrigPtTh[t])
			{
			    didPassPtCut = kTRUE;
			    break;
			}
		    }
		    else if(det == "eemc")
		    {
			if(skimEvent->GetTrigFlag(t) && pt > eemcTrigPtTh[t]) // <------------- !!To be confirmed!!
			{
			    didPassPtCut = kTRUE;
			    break;
			}			
		    }
		}

		if(runNumber < 18000000) //Exclude Run 17 for now
		{
		    if(!didPassPtCut)
			continue;
		}
		
		if(nPhotons > 0 && nPhotons < kPhotonBins)
		    nPhotons_i = nPhotons - 1;
		else if(nPhotons >= kPhotonBins)
		    nPhotons_i = kPhotonBins - 1;
		else
		    continue;

		eng_i = (Int_t)(eng / 20.0); 

		if(eng_i >= kEnergyBins)
		    continue;

		if(phi >= 0)
		{
		    phi_b = phi;
		    phi_y = TMath::Pi() - phi;
		}
		else
		{
		    phi_b = phi;
		    phi_y = -1.0*TMath::Pi() - phi;		
		}

		LV.SetPtEtaPhiE(pt, eta, phi, eng);
		xf = 2.0*(LV.Pz()) / sqrt_s;
		xf = fabs(xf);
		
		bHist[bSpin_i][eng_i][nPhotons_i]->Fill(phi_b, pt);
		yHist[ySpin_i][eng_i][nPhotons_i]->Fill(phi_y, pt);

		h1bSpinI->Fill(bSpin_i);
		h1ySpinI->Fill(ySpin_i);
		h1nPhotonsI->Fill(nPhotons_i);
		h1EngI->Fill(eng_i);
		h1PhiB->Fill(phi_b);
		h1PhiY->Fill(phi_y);

		eventAccepted = kTRUE;

		//--- QA plots after all cuts ----------
		h1spinB->Fill(spinB);  //note: per jet
		h1spinY->Fill(spinY);  //note: per jet
		h1Eta->Fill(eta);
		h1Phi->Fill(phi);
		h1E->Fill(eng);
		h1Pt->Fill(pt);
		h1nPhotons->Fill(nPhotons);

		for(Int_t t = 0; t < nPtBins; ++t)
		{
		    if(pt >= ptBins[t] && pt < ptBins[t + 1])
		    {
			hAvgXf[eng_i][t]->Fill(xf);
			break;
		    }
		}
	    }

	    //------------ Calculate Average Polarization -----------------------------------
	    if(!eventAccepted)
		continue;

	    if(fillNoFmData != lastFillNoFmData)
	    {
		lastFillNoFmData = fillNoFmData;
		energy = startTime = stopTime = p_b = dp_b = dpdt_b = edpdt_b = p_y = dp_y = dpdt_y = edpdt_y = -1.0;
		fillDb.GetFillPolarization(fillNoFmData, energy, startTime, stopTime, p_b, dp_b, dpdt_b, edpdt_b, p_y, dp_y, dpdt_y, edpdt_y);
		
		cout << fillNoFmData << "\t"<< energy << "\t"<< startTime << "\t"<< stopTime << "\t"<< p_b << "\t"<< dp_b << "\t"<< dpdt_b << "\t"<< edpdt_b << "\t"<< p_y << "\t"<< dp_y << "\t"<< dpdt_y << "\t"<< edpdt_y <<endl;
		cout << "Fill No.: "<< fillNoFmData <<" Start time: "<< startTime << " Current Evernt Time: "<< evtTime << " Time Diff in hours: "<< (evtTime - startTime) / 3600.0 <<endl;
	    }

	    if(p_b == -1 || p_y == -1)
		continue;

	    dT = (evtTime - startTime) / 3600.0;
	    
	    pol_b = p_b + dpdt_b*dT;
	    pol_y = p_y + dpdt_y*dT;

	    ePol_b = sqrt( pow(dp_b, 2) + pow(dT*edpdt_b, 2) );
	    ePol_y = sqrt( pow(dp_y, 2) + pow(dT*edpdt_y, 2) );

	    hPolB->Fill(pol_b);
	    hPolY->Fill(pol_y);

	    hPolBerr->Fill(ePol_b);
	    hPolYerr->Fill(ePol_y);
	    
	    ++nPoints;
	}

	++nRuns;
	
	tFile->Close();
	delete tFile;
    }

    file->Write();    
}

