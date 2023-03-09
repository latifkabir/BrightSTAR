// Filename: EjCreateBinnedHistExtended.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri May  8 15:08:25 2020 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include "EjAna.h"
#include "RootInclude.h"
#include "cppInclude.h"
#include "BrightStInclude.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"
#include "BrJetMaker/TStJetSkimEvent.h"
#include "EjCreateBinnedHistExtended.h"
using namespace std;


void EjCreateBinnedHistExtended(Int_t fillNo, TString fileNamePrefix, TString det, Int_t firstRun, Int_t lastRun, Int_t minNphotons)
{   
    // We need to bin in: energy (5), number of photons (6), phi (16), spin (2), pt(6).
    
    TString outName = (TString)"BinnedHist_ext_" + to_string(fillNo) + (TString)".root";
    TFile *file = new TFile(outName, "recreate");
    const Int_t kSpinBins = 2;
    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;
    const Int_t kPhiBins = 16;
    const Int_t kXfBins = 10;
    
    Double_t ptBins[] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0};
    Double_t engBins[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0}; //For info only
    Double_t xfBins[] = {0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7};
    Double_t sqrt_s;
    
    Int_t nPtBins = sizeof(ptBins) / sizeof(Double_t) - 1;
    
    TH2D *bHistNp[kSpinBins]; //At least N (=4) number of photons
    TH2D *bHist2p[kSpinBins]; //Just 1 or 2 photons
    TH2D *bHist3p[kSpinBins]; //Just 3 photons
    TH2D *bHistPtVsXfNp;
    TH2D *bHistPtVsXf2p;
    TH2D *bHistPtVsXf3p;
    for(Int_t i = 0; i < kSpinBins; ++i)
    {
	TString title = Form("bHistNp_%i", i);
	bHistNp[i] = new TH2D(title, title, kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
	title = Form("bHist2p_%i", i);
	bHist2p[i] = new TH2D(title, title, kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
	title = Form("bHist3p_%i", i);
	bHist3p[i] = new TH2D(title, title, kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
    }
    bHistPtVsXfNp = new TH2D("PtVsXfNp", "PtVsXfNp; x_{F}; p_{T}", kXfBins, xfBins, nPtBins, ptBins);
    bHistPtVsXf2p = new TH2D("PtVsXf2p", "PtVsXf2p; x_{F}; p_{T}", kXfBins, xfBins, nPtBins, ptBins);
    bHistPtVsXf3p = new TH2D("PtVsXf3p", "PtVsXf3p; x_{F}; p_{T}", kXfBins, xfBins, nPtBins, ptBins);
    
    Int_t bSpin_i;
    Int_t ySpin_i;

    TH1D *h1bSpinI = new TH1D("h1bSpinI", "Blue Spin index", 10, -1, 4);
    TH1D *h1ySpinI = new TH1D("h1ySpinI", "Yellow Spin index", 10, -1, 4);
    TH1D *h1nPhotons = new TH1D("h1nPhotons", "Number of photons", 20, 0, 20);    
    TH2D *h2nPhotonsVsPt = new TH2D("h2nPhotonsVsPt", "Number of photons vs Pt", 10, 2, 7, 11, -1, 10);    
    TH2D *h2nPhotonsVsXf = new TH2D("h2nPhotonsVsXf", "Number of photons vs Xf", 10, 0, 1.0, 11, -1, 10);    
    TH1D *h1Eng = new TH1D("h1Eng", "Jet Energy", 100, 0, 200);
    TH1D *h1Xf = new TH1D("h1Xf", "X_{F}", 40, -1.0, 1.0);
    TH1D *h1PhiB = new TH1D("h1PhiB", "Phi [Blue beam]", kPhiBins, -1.0*TMath::Pi(), TMath::Pi());
    TH1D *h1PhiY = new TH1D("h1PhiY", "Phi [Yellow beam]", kPhiBins, -1.0*TMath::Pi(), TMath::Pi());

    TH1D *h1E = new TH1D ("h1E", "EM Jet E after all cuts; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1Pt = new TH1D ("h1Pt", "Jet Pt after all cuts; Jet Pt [GeV/c]", 100, 0.0, 25.0);
    TH1D *h1Enew = new TH1D ("h1Enew", "EM Jet E after all cuts; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1PtNew = new TH1D ("h1PtNew", "Jet Pt after all cuts; Jet Pt [GeV/c]", 100, 0.0, 25.0);
    
    TH1D *hPolB = new TH1D("hPolB", "Polarization [Blue Beam]", 400, 40, 80);
    TH1D *hPolY = new TH1D("hPolY", "Polarization [Yellow Beam]", 400, 40, 80);
    
    Double_t etaMin;
    Double_t etaMax;
    Double_t engMin;
    Double_t engMax;
    if(det == "fms")
    {
	//limit jet axis within 2.8 - 3.8 following Carl's suggestion in spin pwg  mailing list 2019-11-22 email.
	etaMin = 2.8;  //2.4;
	etaMax = 3.8;  //4.5;

	engMin = 20;  //Overwritten later
	engMax = 100; //Overwritten later
    }
    else if(det == "eemc")
    {
	etaMin = 1.0; //0.8;
	etaMax = 2.0; //2.5;

	engMin = 0;   //Overwritten later
	engMax = 20;  //Overwritten later	
    }
    else
    {
	cout << "Invalid detector" <<endl;
	return;
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
    Double_t pt;     // UE and detector smearing corrected pt
    Double_t ptRaw;  // pt without any correction
    Double_t eta;
    Double_t phi;
    Double_t theta;
    Double_t jetX;
    Double_t jetY;
    Double_t vtxZ;
    Double_t rt;
    Double_t xf;
    TLorentzVector LV; 
    
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
    
    Double_t pol_ave_b;
    Double_t pol_ave_y;
    Double_t ePol_ave_b;
    Double_t ePol_ave_y;
    
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
    
    for(Int_t r = 0; r < runList->GetN(); ++r)
    {
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
	Double_t eemcTrigPtTh[9] = {4.25, 5.405, 7.285, 4.25, 7.285, 0, 0, 0, 0};               //"EHT0", "JP1", "JP2", "EHT0*EJP1*L2Egamma", "JP2*L2JetHigh", "BHT1*VPDMB-30", "BHT0*BBCMB", "BHT1*BBCMB", "BHT2*BBCMB";
	//Confirm thresholds for convoluted triggers.
	
	nPoints = 0;
	if(runNumber < 18000000)
	{
	    sqrt_s = 200;        //Run 15
	    engMin = 20;
	    engMax = 100;
	}
	else
	{
	    sqrt_s = 510;        //Run 17
	    engMin = 20;
	    engMax = 255;
	}
	    		
	Int_t nEntries = tree->GetEntries();
	cout << "Processing run number: "<< runNumber <<endl;
	cout << "Total events to be processed: "<< nEntries <<endl;

	for(Int_t evt = 0; evt < nEntries; ++evt)
	{
	    jetEvent->Reset();
	    
	    if(evt%5000 == 0)
		cout << "Events processed: "<< evt <<endl;
	
	    tree->GetEntry(evt);

	    skimEvent = jetEvent->GetEvent();
	    vtxZ = skimEvent->GetVertexZ();
	    spinB = skimEvent->GetSpinB();	    
	    spinY = skimEvent->GetSpinY();
	    evtTime = skimEvent->GetUnixTime();
	    eventAccepted = kFALSE;

	    //Exclude FMS small-bs3 trigger that gives ring of fire issue.
	    //The ring of fire is a real problem. It gives lots of unphysical jets i.e. jets with E > s /2 or X_F > 1.0. It must be removed for any reliable analysis.
	    // if(det == "fms") 
	    // {
	    	// if(skimEvent->GetTrigFlag(5)) 
	    	//     continue;
		
	    	// if(!(skimEvent->GetBbcMult() > 0.5 && skimEvent->GetTofTrayMult() > 2.5)) //Ring of fire cut
	    	//     continue;
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

	    for(Int_t j = 0; j <  jetEvent->GetNumberOfJets(); ++j)
	    {
		didPassPtCut = kFALSE;
		jet = jetEvent->GetJet(j);
		
		eta = jet->GetEta();
		phi = jet->GetPhi();
		theta =  2 * atan( exp(-eta) );
		eng = jet->GetE();
		ptRaw = jet->GetPt();
		if(det == "eemc")
		    pt = jet->GetPt();            // !!!!!!!!!!! Currently no UE correction for EEMC
		else
		    pt = jet->GetPt() - jet->GetUedPt();
		nPhotons = jet->GetNumberOfTowers();

		if(eta < etaMin || eta > etaMax) //Conside only EEMC and FMS coverage
		    continue;
		
		//--- Jet energy and pt correction --- 
		h1E->Fill(eng);
		h1Pt->Fill(pt);

		//--- Enabled since tag v2.2 for my results---
		pt = EjJetPtCorr(pt, eng);
		eng = EjJetEngCorr(eng); 

		//------- end of jet eng/pt correction ----
		
		// if(pt < EjAna::kPtMin)
		//     continue;
		if(pt < 1 || pt > 2) //FIXIT: TEMPORARY for ssdmb-stream
		    continue;
		
		LV.SetPtEtaPhiE(pt, eta, phi, eng);
		xf = 2.0*(LV.Pz()) / sqrt_s;
		
		//h1Xf->Fill(xf); //Moved to the event loop
		h1Eng->Fill(eng);
		/* //FIXIT: TEMPORARY for ssdmb-stream		
		//Trigger dependent Pt cuts: See: Carl's e-mail to Cold QCD pwg mailing list on 2019-11-22.
		for(Int_t t = 0; t < 9; ++t) 
		{
		    if(det == "fms")
		    {
			if(skimEvent->GetTrigFlag(t) && ptRaw > fmsTrigPtTh[t])
			{
			    didPassPtCut = kTRUE;
			    break;
			}
		    }
		    else if(det == "eemc")
		    {
			if(skimEvent->GetTrigFlag(t) && ptRaw >  eemcTrigPtTh[t])
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
		//-- End of Trigger Selection --
		*/		
		if(fabs(xf) < 0 || fabs(xf) > 1)
		{
		    //cout << "Unphysical x_F value. Investigate ...."<< "X_F: "<<xf <<"\t Eng: "<< eng <<endl;
		    continue;
		}
		
		if(nPhotons <= 0)
		    continue;

		if(eng > (sqrt_s / 2.0) || eng < engMin || eng > engMax)
		    continue;

		phi_b = phi;
			
		if(nPhotons >= minNphotons)
		{
		    bHistNp[bSpin_i]->Fill(phi_b, xf);
		    bHistPtVsXfNp->Fill(xf, pt);
		}
		
		if(nPhotons == 2 || nPhotons == 1)  
		{
		    bHist2p[bSpin_i]->Fill(phi_b, xf);
		    bHistPtVsXf2p->Fill(xf, pt);
		}

		if(nPhotons == 3)  
		{
		    bHist3p[bSpin_i]->Fill(phi_b, xf);
		    bHistPtVsXf3p->Fill(xf, pt);
		}
		
		h1bSpinI->Fill(bSpin_i);
		h1ySpinI->Fill(ySpin_i);
		h1PhiB->Fill(phi_b);
		h1PhiY->Fill(phi_y);
		h1nPhotons->Fill(nPhotons);
		h2nPhotonsVsPt->Fill(pt, nPhotons);
		h2nPhotonsVsXf->Fill(xf, nPhotons);
		h1Enew->Fill(eng);
		h1PtNew->Fill(pt);
		h1Xf->Fill(xf); //Moved here to calculate average Xf for EEMC plots for pwg group
		//if(xf < 0.2) cout <<xf<<"\t"<<eng<<"\t"<<eta<<"\t"<<pt<<"\t"<<phi<<"\t"<<LV.Pz() <<"\t"<< jet->GetUedPt() <<endl;		
		eventAccepted = kTRUE;
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

	    hPolB->Fill(pol_b);
	    hPolY->Fill(pol_y);
	    
	    ++nPoints;
	}

	++nRuns;
	
	tFile->Close();
	delete tFile;
    }
    
    file->Write();    
}


Double_t EjJetEngCorr(Double_t E)
{    
    Double_t p0 = -8.482;
    Double_t p1 = 4.069;
    Double_t p2 = -0.3063;
    Double_t p3 = 0.01401;
    Double_t p4 = -0.0003243;
    Double_t p5 = 3.729*pow(10, -6);
    Double_t p6 = -1.697*pow(10, -8);
    
    Double_t eNew;

    if(E <= 60) //The fit function works only for fit range. Range can be extended with more simulated samples
	eNew = p0 + p1*E + p2*pow(E, 2) + p3*pow(E, 3) + p4*pow(E, 4) + p5*pow(E, 5) + p6*pow(E, 6);
    else
	eNew = 8.248 + E*0.9365; //Linear fit at higher energy
    
    return eNew;
}


Double_t EjJetPtCorr(Double_t pt, Double_t E)
{    
    Double_t p0 = 1.486;
    Double_t p1 = 0.07337;
    Double_t p2 = 0.215;
    Double_t p3 = -0.01525;

    Double_t ptNew;

    if(pt <= 5.8)  //Fit function does not work outside the fit range. You can extend once you have simulated sample for higher pt values      
	ptNew = p0 + p1*pt + p2*pow(pt, 2) + p3*pow(pt, 3);
    else
	ptNew = (0.4478 + 0.9685*pt); //Linear fit at higher pt
    
    return ptNew;
}
