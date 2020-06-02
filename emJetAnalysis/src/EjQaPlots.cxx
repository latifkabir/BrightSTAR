
#include "StRootInclude.h"
#include "RootInclude.h"
#include "cppInclude.h"

#include "TStTrigDef.h"

//Use file name with wildcard to chain many files
void EjQaPlots(
            Int_t nentries,
	    TString jetfile,
	    TString skimfile,
	    TString outfile
    )
{
    cout << "nentries = " << nentries << endl;
    cout << "jetfile  = " << jetfile  << endl;
    cout << "skimfile = " << skimfile << endl;
    cout << "outfile = " << outfile << endl;

    // Load libraries
    // gSystem->Load("StJetEvent");
    // gSystem->Load("StJetSkimEvent");

    TFile *outFile = new TFile(outfile, "recreate");
    
    // Open jet & skim files
    TChain* jetChain = new TChain("jet");
    TChain* skimChain = new TChain("jetSkimTree");

    Int_t nFiles_jet, nFiles_skim;
    nFiles_jet = jetChain->Add(jetfile);
    nFiles_skim = skimChain->Add(skimfile);

    Int_t nEvents = jetChain->GetEntries();
    if(nentries == -1 || nentries > nEvents)
	nentries = nEvents;

    cout << "Number of files added: "<<nFiles_jet <<endl;
    cout << "Total Entries to be processed: "<< nentries <<endl;
    //Check if added files are sorted properly which is crucial for skimChain vs jetChain synchronization
    jetChain->ls();
    skimChain->ls();
    // Set jet buffer
    StJetEvent* jetEvent = 0;
    jetChain->SetBranchAddress("AntiKtR070NHits12",&jetEvent);

    // Set skim buffer
    StJetSkimEvent* skimEvent = 0;
    skimChain->SetBranchAddress("skimEventBranch",&skimEvent);

    TH1D *h1Triggers = new TH1D("h1Triggers", "Trigger counts with nJets > 0", 10, 0, 10);
    h1Triggers->GetXaxis()->SetBinLabel(1,"JP0");
    h1Triggers->GetXaxis()->SetBinLabel(2,"JP1");
    h1Triggers->GetXaxis()->SetBinLabel(3,"JP2");
    h1Triggers->GetXaxis()->SetBinLabel(4,"Small BS1");
    h1Triggers->GetXaxis()->SetBinLabel(5,"Small BS2");
    h1Triggers->GetXaxis()->SetBinLabel(6,"Small BS3");
    h1Triggers->GetXaxis()->SetBinLabel(7,"Large BS1");
    h1Triggers->GetXaxis()->SetBinLabel(8,"Large BS2");
    h1Triggers->GetXaxis()->SetBinLabel(9,"Large BS3");
    
    //-- All Triggers -------
    TH1D *h1nJets = new TH1D ("h1nJets", "Number of Jets; No. of Jets", 10, -1, 9);
    TH1D *h1VtxType = new TH1D("h1VtxType", "Vertex Type", 5, 0, 5);
    h1VtxType->GetXaxis()->SetBinLabel(1,"TPC");
    h1VtxType->GetXaxis()->SetBinLabel(2,"VPD");
    h1VtxType->GetXaxis()->SetBinLabel(3,"BBC");
    h1VtxType->GetXaxis()->SetBinLabel(4,"None");
    TH1D *h1nVtx = new TH1D("h1nVtx", "Number of Vertices", 5, 0, 5);
    TH1D* h1Vtxz = new TH1D("h1Vtxz", "Vtx z Distribution [All Sources]; Vertex z [cm]",100, -200, 200);
    TH1D* h1Vtxz1 = new TH1D("h1Vtxz1", "Vtx z Distribution [TPC]; Vertex z [cm]",100, -200, 200);
    TH1D* h1Vtxz2 = new TH1D("h1Vtxz2", "Vtx z Distribution [VPD]; Vertex z [cm]",100, -200, 200);
    TH1D* h1Vtxz3 = new TH1D("h1Vtxz3", "Vtx z Distribution [BBC]; Vertex z [cm]",100, -200, 200);
    TH1D* h1Vtxz4 = new TH1D("h1Vtxz4", "Vtx z Distribution [None]; Vertex z [cm]",100, -200, 200);
    
    TH1D *h1JetEta = new TH1D ("h1JetEta", "EM Jet Eta; Jet #eta", 100, -1.5, 5.0);
    TH1D *h1JetPhi = new TH1D ("h1JetPhi", "EM Jet Phi; Jet #phi [rad]", 100, -3.1, 3.1);
    TH1D *h1JetE = new TH1D ("h1JetE", "EM Jet E; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetE_s = new TH1D ("h1JetE_s", "EM Jet E [small cells]; Jet E [GeV]", 100, 0.0, 85.0);
    TH1D *h1JetE_l = new TH1D ("h1JetE_l", "EM Jet E [large cells]; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetPt = new TH1D ("h1JetPt", "Jet Pt; Jet Pt [GeV/c]", 100, 0.0, 50.0);
    TH1D *h1JetVtxZ = new TH1D ("h1JetVtxZ", "Jet Vtx z; Jet vtx z [cm]", 100, -200.0, 200.0);
    TH1D *h1nPhotons = new TH1D("h1nPhotons", "number of photons in EM jets", 20, 0, 20);

    //---- JP -----
    TH1D *h1nJets_jp = new TH1D ("h1nJets_jp", "Number of Jets; No. of Jets", 10, -1, 9);
    TH1D *h1JetEta_jp = new TH1D ("h1JetEta_jp", "Jet Eta; Jet #eta", 100, 2.0, 4.5);
    TH1D *h1JetPhi_jp = new TH1D ("h1JetPhi_jp", "Jet Phi; Jet #phi [rad]", 100, -3.1, 3.1);
    TH1D *h1JetE_jp = new TH1D ("h1JetE_jp", "FMS EM Jet E; FMS Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetPt_jp = new TH1D ("h1JetPt_jp", "Jet Pt; Jet Pt [GeV/c]", 100, 0.0, 50.0);
    TH1D *h1JetVtxZ_jp = new TH1D ("h1JetVtxZ_jp", "Jet Vtx z; Jet vtx z [cm]", 100, -200.0, 200.0);
    TH1D *h1nPhotons_jp = new TH1D("h1nPhotons_jp", "number of photons in FMS EM jets", 20, 0, 20);

    //---- Small BS -----
    TH1D *h1nJets_sb = new TH1D ("h1nJets_sb", "Number of Jets; No. of Jets", 10, -1, 9);
    TH1D *h1JetEta_sb = new TH1D ("h1JetEta_sb", "Jet Eta; Jet #eta", 100, 2.0, 4.5);
    TH1D *h1JetPhi_sb = new TH1D ("h1JetPhi_sb", "Jet Phi; Jet #phi [rad]", 100, -3.1, 3.1);
    TH1D *h1JetE_sb = new TH1D ("h1JetE_sb", "FMS EM Jet E; FMS Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetPt_sb = new TH1D ("h1JetPt_sb", "Jet Pt; Jet Pt [GeV/c]", 100, 0.0, 50.0);
    TH1D *h1JetVtxZ_sb = new TH1D ("h1JetVtxZ_sb", "Jet Vtx z; Jet vtx z [cm]", 100, -200.0, 200.0);
    TH1D *h1nPhotons_sb = new TH1D("h1nPhotons_sb", "number of photons in FMS EM jets", 20, 0, 20);

    //---- Large BS -----
    TH1D *h1nJets_lb = new TH1D ("h1nJets_lb", "Number of EM Jets; No. of EM Jets", 10, -1, 9);
    TH1D *h1JetEta_lb = new TH1D ("h1JetEta_lb", "Jet Eta; Jet #eta", 100, 2.0, 4.5);
    TH1D *h1JetPhi_lb = new TH1D ("h1JetPhi_lb", "Jet Phi; Jet #phi [rad]", 100, -3.1, 3.1);
    TH1D *h1JetE_lb = new TH1D ("h1JetE_lb", "FMS Jet E; FMS EM Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetPt_lb = new TH1D ("h1JetPt_lb", "Jet Pt; Jet Pt [GeV/c]", 100, 0.0, 50.0);
    TH1D *h1JetVtxZ_lb = new TH1D ("h1JetVtxZ_lb", "Jet Vtx z; Jet vtx z [cm]", 100, -200.0, 200.0);
    TH1D *h1nPhotons_lb = new TH1D("h1nPhotons_lb", "number of photons in jets [FMS]", 20, 0, 20);
    
    TH2D *h2Jetxy = new TH2D ("h2Jetxy", "Jet Position [FMS]; Jet X [cm]; Jet Y [cm]", 100, -100, 100, 100, -100, 100);
    TH2D *h2JetEP = new TH2D ("h2Jetep_fms", "Jet Eta Phi [FMS]; Jet #eta; Jet #phi", 100, 2.5, 4.5, 100, -3.2, 3.2);
    
    TH2D *h2Jetxy_eemc = new TH2D ("h2Jetxy_eemc", "Jet position [EEMC]; Jet X [cm]; Jet Y [cm]", 100, -200.0, 200.0, 100, -200, 200);
    
    TLorentzVector lv;
    TVector3 v3;
    Double_t eta;
    Double_t phi;
    Double_t E;
    Double_t pt;
    Double_t z_vtx;
    Double_t jetX;
    Double_t jetY;
    Double_t theta;

    skimChain->GetEvent(0);
    Int_t trigIdJP0 = TStTrigDef::GetTrigId(skimEvent->runId(), "FMS-JP0");
    Int_t trigIdJP1 = TStTrigDef::GetTrigId(skimEvent->runId(), "FMS-JP1");
    Int_t trigIdJP2 = TStTrigDef::GetTrigId(skimEvent->runId(), "FMS-JP2");
    Int_t trigIdSB1 = TStTrigDef::GetTrigId(skimEvent->runId(), "FMS-sm-bs1");
    Int_t trigIdSB2 = TStTrigDef::GetTrigId(skimEvent->runId(), "FMS-sm-bs2");
    Int_t trigIdSB3 = TStTrigDef::GetTrigId(skimEvent->runId(), "FMS-sm-bs3");
    Int_t trigIdLB1 = TStTrigDef::GetTrigId(skimEvent->runId(), "FMS-lg-bs1");
    Int_t trigIdLB2 = TStTrigDef::GetTrigId(skimEvent->runId(), "FMS-lg-bs2");
    Int_t trigIdLB3 = TStTrigDef::GetTrigId(skimEvent->runId(), "FMS-lg-bs3");
    
    StJetSkimTrig* trigJP0; 
    StJetSkimTrig* trigJP1; 
    StJetSkimTrig* trigJP2; 
    StJetSkimTrig* trigSB1; 
    StJetSkimTrig* trigSB2; 
    StJetSkimTrig* trigSB3; 
    StJetSkimTrig* trigLB1; 
    StJetSkimTrig* trigLB2; 
    StJetSkimTrig* trigLB3;

    Bool_t didFireJP;
    Bool_t didFireJP0;
    Bool_t didFireJP1;
    Bool_t didFireJP2;
    Bool_t didFireSB;
    Bool_t didFireLB;
    Bool_t didFireSB1, didFireSB2, didFireSB3; 
    Bool_t didFireLB1, didFireLB2, didFireLB3; 
    
    StJetParticle *part;
    Int_t vtxType = -1;
    Double_t vtxZ = -999;
    //----- Event loop ----------
    //Loads entries from Jet Tree and Skim Tree simultaneously
    for (int iEntry = 0; iEntry < nentries; ++iEntry)
    {
	if (jetChain->GetEvent(iEntry) <= 0 || skimChain->GetEvent(iEntry) <= 0) break;

	// Should not be null
	assert(jetEvent && skimEvent);

	// Enforce event synchronization
	assert(jetEvent->runId() == skimEvent->runId() && jetEvent->eventId() == skimEvent->eventId());

	if (iEntry % 1000 == 0) cout << iEntry << endl;

	trigJP0 = skimEvent->trigger(trigIdJP0);
	trigJP1 = skimEvent->trigger(trigIdJP1);
	trigJP2 = skimEvent->trigger(trigIdJP2);
	trigSB1 = skimEvent->trigger(trigIdSB1);
	trigSB2 = skimEvent->trigger(trigIdSB2);
	trigSB3 = skimEvent->trigger(trigIdSB3);
	trigLB1 = skimEvent->trigger(trigIdLB1);
	trigLB2 = skimEvent->trigger(trigIdLB2);
	trigLB3 = skimEvent->trigger(trigIdLB3);
	
	// if(jetEvent->numberOfJets() == 0)
	//     continue;

	StJetVertex* vertex = jetEvent->vertex(0);
	if (!vertex) continue;
	
	// if(vertex->ranking() < 0) continue; //Not applicable for EM jet	
	// if(fabs(vertex->position().z()) > 80.0) continue;

	vtxType = -1;
	vtxZ = vertex->position().z();
	h1nVtx->Fill(jetEvent->numberOfVertices());
	
	if(jetEvent->numberOfVertices() > 1 || (vertex->position().x() != 0. && vertex->position().y() != 0.))
	{
	    vtxType = 1;
	    h1Vtxz1->Fill(vtxZ);	    
	}
	else if (skimEvent->vpdZvertex() != -999. && vtxType == -1)
	{
	    vtxType = 2;
	    h1Vtxz2->Fill(vtxZ);	    
	}
	else if (vertex->position().z() != 0. && vtxType == -1)
	{
	    vtxType = 3;
	    h1Vtxz3->Fill(vtxZ);	    
	}
	else
	{
	    vtxType = 4;
	    h1Vtxz4->Fill(vtxZ);	    
	}
	h1Vtxz->Fill(vtxZ);
	h1VtxType->Fill(vtxType);
	
	didFireJP0 = didFireJP1 = didFireJP2 = kFALSE;
	didFireSB1 = didFireSB2 = didFireSB3 = kFALSE;
	didFireLB1 = didFireLB2 = didFireLB3 = kFALSE;
	didFireJP = didFireSB = didFireLB = kFALSE;
	
	if(trigJP0)
	{
	    if(trigJP0->didFire() && trigJP0->shouldFire())
	    {
		h1Triggers->Fill(0);		
		didFireJP0 = kTRUE;
	    }
	}
	
	if(trigJP1)
	{
	    if(trigJP1->didFire() && trigJP1->shouldFire())  //<-------- Check what did/should fire actually is
	    {
		h1Triggers->Fill(1);		
		didFireJP1 = kTRUE;
	    }
	}

	if(trigJP2)
	{
	    if(trigJP2->didFire() && trigJP2->shouldFire())
	    {
		h1Triggers->Fill(2);		
		didFireJP2 = kTRUE;
	    }
	}

	if(trigSB1)
	{
	    if(trigSB1->didFire() && trigSB1->shouldFire())
	    {
		h1Triggers->Fill(3);		
		didFireSB1 = kTRUE;
	    }
	}

	if(trigSB2)
	{
	    if(trigSB2->didFire() && trigSB2->shouldFire())
	    {
		h1Triggers->Fill(4);		
		didFireSB2 = kTRUE;
	    }
	}

	if(trigSB3)
	{
	    if(trigSB3->didFire() && trigSB3->shouldFire())
	    {
		h1Triggers->Fill(5);		
		didFireSB3 = kTRUE;
	    }
	}

	if(trigLB1)
	{
	    if(trigLB1->didFire() && trigLB1->shouldFire())
	    {
		h1Triggers->Fill(6);		
		didFireLB1 = kTRUE;
	    }
	}

	if(trigLB2)
	{
	    if(trigLB2->didFire() && trigLB2->shouldFire())
	    {
		h1Triggers->Fill(7);		
		didFireLB2 = kTRUE;
	    }
	}

	if(trigLB3)
	{
	    if(trigLB3->didFire() && trigLB3->shouldFire())
	    {
		h1Triggers->Fill(8);		
		didFireLB3 = kTRUE;
	    }
	}

	if(didFireJP0 || didFireJP1 || didFireJP2)
	    didFireJP = kTRUE;
	
	if(didFireSB1 || didFireSB2 || didFireSB3)
	    didFireSB = kTRUE;

	if(didFireLB1 || didFireLB2 || didFireLB3)
	    didFireLB = kTRUE;
	
	h1nJets->Fill(jetEvent->numberOfJets());

	//It seems most people loop over vertices. Find the justification.
	//I am looping over number of jets
	for(Int_t i = 0; i < jetEvent->numberOfJets(); ++i) 
	{
	    eta = jetEvent->jet(i)->eta();
	    phi = jetEvent->jet(i)->phi();
	    E = jetEvent->jet(i)->E();
	    pt = jetEvent->jet(i)->pt();
	    z_vtx = jetEvent->jet(i)->vertex()->position().z();
	    theta =  2 * atan( exp(-eta) );
	    jetX = (735. - z_vtx) * tan(theta) * cos(phi);
	    jetY = (735. - z_vtx) * tan(theta) * sin(phi);
	    
	    h1JetEta->Fill(eta);
	    h1JetPhi->Fill(phi);
	    h1JetPt->Fill(pt);
	    h1JetVtxZ->Fill(z_vtx); // no vtx found case i.e. vtx = 0 will dominate if all types filled together

	    h1JetE->Fill(E);
	    if(eta > 2.0 && eta < 3.15) //large cells
		h1JetE_l->Fill(E); 
	    if(eta > 3.15 && eta < 4.0)  //small cells
		h1JetE_s->Fill(E);
	    
	    if(eta > 2.5 && eta < 4.5)  //actual range 2.65 - 3.9
	    {
		h2Jetxy->Fill(jetX, jetY);
		h2JetEP->Fill(eta, phi);
	    }
	    if(eta > 1.0 && eta < 2.0)   // actual range 1.086 - 2.0
	    {
		jetX = (kEEmcZSMD - z_vtx) * tan(theta) * cos(phi);
		jetY = (kEEmcZSMD - z_vtx) * tan(theta) * sin(phi);
		h2Jetxy_eemc->Fill(jetX, jetY);
	    }

	    if(eta > 2.65 && eta < 3.9)
		h1nPhotons->Fill(jetEvent->jet(i)->numberOfTowers());

	    if(eta < 2.0  || eta > 4.5) //Consider FMS EM Jets only (for trigger specific plots)
		continue;

	    if(didFireJP)
	    {
		h1nJets_jp->Fill(jetEvent->numberOfJets());
		h1JetEta_jp->Fill(eta);
		h1JetPhi_jp->Fill(phi);
		h1JetPt_jp->Fill(pt);
		h1JetE_jp->Fill(E);
		h1JetVtxZ_jp->Fill(z_vtx);
		h1nPhotons_jp->Fill(jetEvent->jet(i)->numberOfTowers());
	    }

	    if(didFireSB)
	    {
		h1nJets_sb->Fill(jetEvent->numberOfJets());
		h1JetEta_sb->Fill(eta);
		h1JetPhi_sb->Fill(phi);
		h1JetPt_sb->Fill(pt);
		h1JetE_sb->Fill(E);
		h1JetVtxZ_sb->Fill(z_vtx);
		h1nPhotons_sb->Fill(jetEvent->jet(i)->numberOfTowers());
	    }

	    if(didFireLB)
	    {
		h1nJets_lb->Fill(jetEvent->numberOfJets());
		h1JetEta_lb->Fill(eta);
		h1JetPhi_lb->Fill(phi);
		h1JetPt_lb->Fill(pt);
		h1JetE_lb->Fill(E);
		h1JetVtxZ_lb->Fill(z_vtx);
		h1nPhotons_lb->Fill(jetEvent->jet(i)->numberOfTowers());
	    }	    
	}	
    } // End event loop

    outFile->Write();
}
