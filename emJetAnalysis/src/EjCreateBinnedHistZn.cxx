// Filename: EjCreateBinnedHistZn.C
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
using namespace std;

void EjCreateBinnedHistZn(TString fileNamePrefix, TString det, Int_t minNphotons)
{
    /*
      We need to bin in: energy (5), number of photons (6), phi (16), spin (2), pt(6).
      Let's create TH2D histograms of array size [2(spin)][4(energy)][#photon(5)]. The 2D histogram to be filled with phi bins along x and pt bins along y.
      We need another similar array for yellow beam as well.
    */
    TString outName = (TString)"BinnedHist_ext_zn.root";
    TFile *file = new TFile(outName, "recreate");
    const Int_t kSpinBins = 2;
    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;
    const Int_t kPhiBins = 16;
    const Int_t kXfBins = 10;
    
    Double_t ptBins[] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0};
    Double_t engBins[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0}; //For info only
    Double_t xfBins[] = {0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7};
    Double_t sqrt_s = 200.0;
    
    Int_t nPtBins = sizeof(ptBins) / sizeof(Double_t) - 1;
    
    TH2D *bHistNp[kSpinBins]; //At least N number of photons
    TH2D *bHist2p[kSpinBins]; //Just 2 photons
    TH2D *bHistPtVsXfNp;
    TH2D *bHistPtVsXf2p;
    for(Int_t i = 0; i < kSpinBins; ++i)
    {
	TString title = Form("bHistNp_%i", i);
	bHistNp[i] = new TH2D(title, title, kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
	title = Form("bHist2p_%i", i);
	bHist2p[i] = new TH2D(title, title, kPhiBins, -1.0*TMath::Pi(), TMath::Pi(), kXfBins, xfBins);
    }
    bHistPtVsXfNp = new TH2D("PtVsXfNp", "PtVsXfNp; x_{F}; p_{T}", kXfBins, xfBins, nPtBins, ptBins);
    bHistPtVsXf2p = new TH2D("PtVsXf2p", "PtVsXf2p; x_{F}; p_{T}", kXfBins, xfBins, nPtBins, ptBins);
    
    Int_t bSpin_i;
    Int_t ySpin_i;
    Int_t nPhotons_i;
    Int_t phi_i;
    Int_t eng_i;
    Int_t xf_i;

    TH1D *h1bSpinI = new TH1D("h1bSpinI", "Blue Spin index", 10, -1, 4);
    TH1D *h1ySpinI = new TH1D("h1ySpinI", "Yellow Spin index", 10, -1, 4);
    TH1D *h1nPhotonsI = new TH1D("h1nPhotonsI", "Number of photons index", 11, -1, 10);
    TH1D *h1nPhotons = new TH1D("h1nPhotons", "Number of photons", 11, -1, 10);    
    TH2D *h2nPhotonsVsPt = new TH2D("h2nPhotonsVsPt", "Number of photons vs Pt", 10, 2, 7, 11, -1, 10);    
    TH2D *h2nPhotonsVsXf = new TH2D("h2nPhotonsVsXf", "Number of photons vs Xf", 10, 0, 1.0, 11, -1, 10);        
    TH1D *h1EngI = new TH1D("h1EngI", "Energy bin index", 7, -1, 6);
    TH1D *h1Eng = new TH1D("h1Eng", "Jet Energy", 100, 0, 200);
    TH1D *h1XfI = new TH1D("h1XfI", "X_{F} index", 10, 0, 10);
    TH1D *h1Xf = new TH1D("h1Xf", "X_{F}", 50, 0, 2.0);
    TH1D *h1PhiB = new TH1D("h1PhiB", "Phi [Blue beam]", kPhiBins, -1.0*TMath::Pi(), TMath::Pi());
    TH1D *h1PhiY = new TH1D("h1PhiY", "Phi [Yellow beam]", kPhiBins, -1.0*TMath::Pi(), TMath::Pi());

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
    
    //----------- Tree Branch Setup: Event buffer------------------------
    //Declaration of leaves types
    Int_t           fnjet;
    Int_t           fjetntow;
    Float_t         fjE;
    Float_t         fjpT;
    Float_t         fjeta;
    Float_t         fjphi;
    Int_t           fjnph;
    Int_t           TrigBits;
    Int_t           run;
    Int_t           ievt;
    Int_t           spinbit;
    Float_t         vertexz;
    Float_t         vertexzyuxi;
    Int_t           CollinsTag;
    Int_t           LeadEjet;
    Float_t         uedensity;
    Float_t         ueMul;
    //------------------ End of Event Buffer --------------------
    
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
    
    Double_t phi_y;
    Double_t phi_b;

    TStFillNoDB fillDb;
    TStRunList runListDb;
    vector <int> fillList = fillDb.GetAllFillNos();
    
    Int_t fillNumber;
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
    Int_t nFills = 0;
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
    
    for(Int_t r = 0; r < fillList.size(); ++r)
    {
	fillNumber = fillList[r];
	fileName = fileNamePrefix + to_string(fillNumber) + ".root";
	if(gSystem->AccessPathName(fileName))
	{
	    cout << "File Not found:" << fileName <<endl;
	    cout << "SKIPPED" <<endl;
	    continue;
	}

	//Skip wrongly filled spin pattern runs: See Run 15 Polarization file. Fill: 18764
	fillNoFmData = fillDb.GetFillNo(fillNumber); //Get it from the DST in the future
	if(fillNoFmData == 18764)
	    continue;
	TFile *tFile = new TFile(fileName);
	TTree *tree = (TTree*)tFile->Get("tFmsjet");
	if(!tree)
	{
	    cout << "No Tree found in"<< fileName <<" ... SKIPPED"<<endl;
	    tFile->Close();
	    delete tFile;	    
	    continue;
	}	

	//------------- Set Branch Addresses -------------------
	tree->SetBranchAddress("fnjet",&fnjet);
	tree->SetBranchAddress("fjetntow",&fjetntow);
	tree->SetBranchAddress("fjE",&fjE);
	tree->SetBranchAddress("fjpT",&fjpT);
	tree->SetBranchAddress("fjeta",&fjeta);
	tree->SetBranchAddress("fjphi",&fjphi);
	tree->SetBranchAddress("fjnph",&fjnph);
	tree->SetBranchAddress("TrigBits",&TrigBits);
	tree->SetBranchAddress("run",&run);
	tree->SetBranchAddress("ievt",&ievt);
	tree->SetBranchAddress("spinbit",&spinbit);
	tree->SetBranchAddress("vertexz",&vertexz);
	tree->SetBranchAddress("vertexzyuxi",&vertexzyuxi);
	tree->SetBranchAddress("CollinsTag",&CollinsTag);
	tree->SetBranchAddress("LeadEjet",&LeadEjet);
	tree->SetBranchAddress("uedensity",&uedensity);
	tree->SetBranchAddress("ueMul",&ueMul);
	//---------------- End of Set Branch Address -------------------

	//See: Carl's e-mail to Cold QCD pwg mailing list on 2019-11-22. 15% higher than nominal trigger threshold.
	Double_t sm_bs1 = 1.15; 
	Double_t sm_bs3 = 2.18; 
	Double_t lg_bs1 = 1.15; 
	// if(runNumber < 16069040)
	// {
	//     sm_bs1 = 1.26;
	//     sm_bs3 = 2.53;
	//     lg_bs1 = 1.26;
	// }
	Double_t fmsTrigPtTh[9] = {1.84, 2.76, 3.68, sm_bs1, 1.84, sm_bs3, lg_bs1, 1.84, 2.76}; //"FMS-JP0", "FMS-JP1", "FMS-JP2", "FMS-sm-bs1", "FMS-sm-bs2", "FMS-sm-bs3", "FMS-lg-bs1", "FMS-lg-bs2", "FMS-lg-bs3"
	Double_t eemcTrigPtTh[9] = {4.25, 5.405, 7.285, 4.25, 7.285, 0, 0, 0, 0}; //"EHT0", "JP1", "JP2", "EHT0*EJP1*L2Egamma", "JP2*L2JetHigh", "BHT1*VPDMB-30", "BHT0*BBCMB", "BHT1*BBCMB", "BHT2*BBCMB";
	//Confirm thresholds for convoluted triggers.
	
	nPoints = 0;
	
	Int_t nEntries = tree->GetEntries();
	cout << "Processing fill number: "<< fillNumber <<endl;
	cout << "Total events to be processed: "<< nEntries <<endl;

	for(Int_t evt = 0; evt < nEntries; ++evt)
	{	    
	    if(evt%5000 == 0)
		cout << "Events processed: "<< evt <<endl;
	
	    tree->GetEntry(evt);

	    vtxZ = vertexz;
	    evtTime = 0;
	    eventAccepted = kFALSE;

	    if(spinbit == 5 || spinbit == 6)
		spinB = -1;
	    else if(spinbit == 9 || spinbit == 10)
		spinB = 1;
	    else
		spinB = 0;

	    if(spinbit == 5 || spinbit == 9)
		spinY = -1;
	    else if(spinbit == 10 || spinbit == 6)
		spinY = 1;
	    else
		spinY = 0;
	    
	    //Exclude FMS small-bs3 trigger that gives ring of fire issue.
	    //The ring of fire is a real problem. It gives lots of unphysical jets i.e. jets with E > s /2 or X_F > 1.0. It must be removed for any reliable analysis.
	    if(det == "fms")
	    {
	    	if(TrigBits == ((0x01)<<4))
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

	    didPassPtCut = kFALSE;
	       	
	    eta = fjeta;
	    phi = fjphi;
	    theta =  2 * atan( exp(-eta) );
	    eng = fjE;
	    pt = fjpT;
	    nPhotons = fjetntow;//fjnph;
	    LV.SetPtEtaPhiE(pt, eta, phi, eng);
	    xf = 2.0*(LV.Pz()) / sqrt_s;
				
	    if(eta < etaMin || eta > etaMax) //Conside only EEMC and FMS coverage
		continue;

	    if(pt < 2)
		continue;
	    
	    h1Xf->Fill(xf);
	    h1Eng->Fill(eng);
		
	    //!!!!!!----> For comparing with Zhanwen's em-jet result only. Consider FMS JP0, JP1 and JP2 Triggers only <---------------------------
	    Int_t JP = ((0x07)<<8);
	    if(!(JP & TrigBits))
		continue;
	    //----------- > Ensure FMS-JP trigger dependent threshold here <----------------------
		

	    xf_i = int(fabs(xf)*10.0);
		
	    if(xf_i < 0 || xf_i > 9)
	    {
		cout << "Unphysical x_F value. Investigate ...."<< "X_F: "<<xf <<"\t Eng: "<< eng <<endl;
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
		
	    if(nPhotons >= minNphotons)
	    {
		bHistNp[bSpin_i]->Fill(phi_b, xf);
		bHistPtVsXfNp->Fill(xf, pt);
	    }
		
	    if(nPhotons == 2)
	    {
		bHist2p[bSpin_i]->Fill(phi_b, xf);
		bHistPtVsXf2p->Fill(xf, pt);
	    }
		
	    h1bSpinI->Fill(bSpin_i);
	    h1ySpinI->Fill(ySpin_i);
	    h1nPhotonsI->Fill(nPhotons_i);
	    h1EngI->Fill(eng_i);
	    h1XfI->Fill(xf_i);
	    h1PhiB->Fill(phi_b);
	    h1PhiY->Fill(phi_y);
	    h1nPhotons->Fill(nPhotons);
	    h2nPhotonsVsPt->Fill(pt, nPhotons);
	    h2nPhotonsVsXf->Fill(xf, nPhotons);
	}
	++nFills;
	
	tFile->Close();
	delete tFile;
    }
    
    file->Write();    
}

