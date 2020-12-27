// Filename: AnDiffJetCreateBinnedHist.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri May  8 15:08:25 2020 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include "RootInclude.h"
#include "cppInclude.h"
#include "BrightStInclude.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"
#include "BrJetMaker/TStJetSkimEvent.h"
#include "BrContainers/TStRpsTrackData.h"

using namespace std;

/*
THIS SCRIPT IS MISSING FOLLOWING IMPLEMENTATIONS:
//-----> Implement BBC,TOF and VPD cuts here if required to match diff pion analysis <------
This will require keeping relevant variables in the jet tree
*/

void AnDiffJetCreateBinnedHist(Int_t runNo, TString fileNamePrefix, TString det, Int_t firstRun, Int_t lastRun)
{
    /*
      We need to bin in: energy (5), number of photons (6), phi (16), spin (2), pt(6).
      Let's create TH2D histograms of array size [2(spin)][4(energy)][#photon(5)]. The 2D histogram to be filled with phi bins along x and pt bins along y.
      We need another similar array for yellow beam as well.
    */
    TString outName = (TString)"BinnedHist_" + to_string(runNo) + (TString)".root";
    TFile *file = new TFile(outName, "recreate");
    const Int_t kSpinBins = 2;
    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;
    const Int_t kPhiBins = 16;
    TH2D *bHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    TH2D *yHist[kSpinBins][kEnergyBins][kPhotonBins]; // [spin][energy bin][#photons]
    Double_t ptBins[] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0};
    Double_t engBins[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0}; //For info only
    Int_t nPtBins = sizeof(ptBins) / sizeof(Double_t) - 1;
    
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

    //--- Histograms for A_N calculation ---
    const Int_t kXfBins = 10;
    Double_t xfBins[] = {0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7};
    TH2D *bXfVsPhiUp = new TH2D("bXfVsPhiUp","bXfVsPhiUp", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
    TH2D *bXfVsPhiDn = new TH2D("bXfVsPhiDn","bXfVsPhiDn", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
    TH2D *yXfVsPhiUp = new TH2D("yXfVsPhiUp","yXfVsPhiUp", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
    TH2D *yXfVsPhiDn = new TH2D("yXfVsPhiDn","yXfVsPhiDn", kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
    Double_t xf;
    Double_t sqrt_s = 200.0;
    TLorentzVector LV;
    //-------

    
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
    TH1D* h1Xf = new TH1D("xf", "xf", 120, -1.2, 1.2);
    //Histos for RP
    TH1D *histTrkP = new TH1D("trkP", " RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *histTrkPt = new TH1D("trkPt", " RP trk Pt; RP track Pt [GeV/c]", 200, 0, 2);
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
    TH1D *hist8West = new TH1D("sumE_west", "E_{p + #pi^{0}}^{West}; E_{p + #pi^{0}}^{West} [GeV]", 100, 60, 200);
    TH2D *hist2d1 = new TH2D("E_p_vs_E_jet", "E_{p}^{west} vs E_{jet}; E_{jet} [GeV]; E_{p}^{west} [GeV]", 100, 10, 80, 100, 60, 150);
    
    TGraphErrors *grPol_b = new TGraphErrors();
    grPol_b->SetName("grPol_blue");
    grPol_b->SetTitle("Polarization [blue beam]");
    TGraphErrors *grPol_y = new TGraphErrors();
    grPol_y->SetName("grPol_yellow");
    grPol_y->SetTitle("Polarization [yellow beam]");
    TGraphErrors *grPolRunEx_b;
    TGraphErrors *grPolRunEx_y;
    TH1D *hPolB = new TH1D("hPolB", "Polarization [Blue Beam]", 400, 40, 80);
    TH1D *hPolY = new TH1D("hPolY", "Polarization [Yellow Beam]", 400, 40, 80);
    
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
    
    //Event buffer
    TStJetEvent *jetEvent = 0;
    TStJetSkimEvent *skimEvent;
    TStJetCandidate *jet;
    TStJetTower *tower;
    TStJetParticle *particle;

    //RP
    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");
    TStRpsTrackData *rpsTrack;
    Int_t nRpsTracks = 0;
    Int_t eastTrk_i =  -1;
    Int_t westTrk_i =  -1;
    Int_t nTrkEast = 0;
    Int_t nTrkWest = 0;
    Double_t sumE_w = 0.0;
    
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
    Int_t gmt2etCorr;
    
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
	tree->SetBranchAddress("rpTrack", &rpsArr);

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
	
	//For polarization
	if(runNumber < 16067006)
	    gmt2etCorr = 5*3600; //GMT to EST
	else
	    gmt2etCorr = 4*3600; //GMT to EDT

	nPoints = 0;
	pol_ave_b = 0;
	pol_ave_y = 0;
	ePol_ave_b = 0;
	ePol_ave_y = 0;
	    
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
	    
	    //Exclude FMS small-bs3 trigger that gives ring of fire issue.
	    //The ring of fire is a real problem. It gives lots of unphysical jets i.e. jets with E > s /2 or X_F > 1.0. It must be removed for any reliable analysis.
	    if(det == "fms")
	    {
	    	if(skimEvent->GetTrigFlag(5))
	    	    continue;
	    }
	    
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

	    //------------- RP Event Selection Cuts ---------------------------
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
		
		histTrkP->Fill(rpsTrack->GetP());
		histTrkPt->Fill(rpsTrack->GetPt());
		
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
	    if(!(nTrkWest == 1 && nTrkEast == 0))   //Full RP Cuts with One track in west and no track in the east
	    	continue;
	    //-----> Implement BBC,TOF and VPD cuts here if required to match diff pion analysis <------
	    
	    
	    //---------- EM-Jet ----------------------------------------------
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

		LV.SetPtEtaPhiE(pt, eta, phi, eng);
		xf = 2.0*(LV.Pz()) / sqrt_s;
		h1Xf->Fill(xf);
		
		if(eta < etaMin || eta > etaMax) //Conside only EEMC and FMS coverage
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

		if(!didPassPtCut)
		    continue;

		sumE_w = rpsTrack->GetP() + eng;
		hist8West->Fill(sumE_w);
		hist2d1->Fill(eng, rpsTrack->GetP());
		if(!(sumE_w > 80 && sumE_w < 107)) //<-------- Energy conservation cut // TO BE FIXED !!!!!!!!!!!!!!!!!
		    continue;
		
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
	    
		bHist[bSpin_i][eng_i][nPhotons_i]->Fill(phi_b, pt);
		yHist[ySpin_i][eng_i][nPhotons_i]->Fill(phi_y, pt);

		if(spinB == 1)
		    bXfVsPhiUp->Fill(phi_b, xf);
		else if(spinB == -1)
		    bXfVsPhiDn->Fill(phi_b, xf);

		if(spinY == 1)
		    yXfVsPhiUp->Fill(phi_y, xf);
		else if(spinY == -1)
		    yXfVsPhiDn->Fill(phi_y, xf);	    
		
		h1bSpinI->Fill(bSpin_i);
		h1ySpinI->Fill(ySpin_i);
		h1nPhotonsI->Fill(nPhotons_i);
		h1EngI->Fill(eng_i);
		h1PhiB->Fill(phi_b);
		h1PhiY->Fill(phi_y);

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
		cout << "Fill No.: "<< fillNoFmData <<" Start time: "<< startTime << " Current Evernt Time: "<< evtTime << " Time Diff in hours: "<< (evtTime - 4.0*3600 - startTime) / 3600.0 <<endl;
	    }

	    if(p_b == -1 || p_y == -1)
		continue;
	    
	    dT = (evtTime - gmt2etCorr - startTime) / 3600.0; // gmt2etCorr is subtracted to correct event time in GMT. This is not required for startTime from spin group table
	    pol_b = p_b + dpdt_b*dT;
	    pol_y = p_y + dpdt_y*dT;

	    ePol_b = sqrt( pow(dp_b, 2) + pow(dT*edpdt_b, 2) );
	    ePol_y = sqrt( pow(dp_y, 2) + pow(dT*edpdt_y, 2) );
	    
	    pol_ave_b += pol_b;
	    pol_ave_y += pol_b;
	    ePol_ave_b += pow(ePol_b, 2);
	    ePol_ave_y += pow(ePol_y, 2);

	    hPolB->Fill(pol_b);
	    hPolY->Fill(pol_y);
	    
	    ++nPoints;
	}

	pol_ave_b /= nPoints;
	pol_ave_y /= nPoints;

	++nRuns;
	
	tFile->Close();
	delete tFile;
    }

    file->Write();    
}

