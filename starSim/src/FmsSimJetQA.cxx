
#include "StRootInclude.h"
#include "RootInclude.h"
#include "cppInclude.h"
#include <string>
// #include "TStTrigDef.h"

//Use file name with wildcard to chain many files
void FmsSimJetQA(
            Int_t nentries,
	    TString jetfile,
	    TString outfile
    )
{
    cout << "nentries = " << nentries << endl;
    cout << "jetfile  = " << jetfile  << endl;
    cout << "outfile = " << outfile << endl;

    // Load libraries
    // gSystem->Load("StJetEvent");
    // gSystem->Load("StJetSkimEvent");

    TFile *outFile = new TFile(outfile, "recreate");
    
    // Open jet & skim files
    TChain* jetChain = new TChain("jet");

    Int_t nFiles_jet;
    nFiles_jet = jetChain->Add(jetfile);


    Int_t nEvents = jetChain->GetEntries();
    if(nentries == -1 || nentries > nEvents)
	nentries = nEvents;

    cout << "Number of files added: "<<nFiles_jet <<endl;
    cout << "Total Entries to be processed: "<< nentries <<endl;
    //Check if added files are sorted properly which is crucial for skimChain vs jetChain synchronization
    jetChain->ls();

    // Set jet buffer
    StJetEvent* jetEvent = 0;
    jetChain->SetBranchAddress("AntiKtR070NHits12",&jetEvent);

    
    //-- All Triggers -------
    TH1D *h1TrigTypes = new TH1D ("h1TrigTypes", "Trigger Types; Trigger Types", 30, 0, 30);

    h1TrigTypes->GetXaxis()->SetBinLabel(3,"Small BS1");
    h1TrigTypes->GetXaxis()->SetBinLabel(5,"Small BS2");
    h1TrigTypes->GetXaxis()->SetBinLabel(7,"Small BS3");
    h1TrigTypes->GetXaxis()->SetBinLabel(9,"Large BS1");
    h1TrigTypes->GetXaxis()->SetBinLabel(11,"Large BS2");
    h1TrigTypes->GetXaxis()->SetBinLabel(13,"Large BS3");
    h1TrigTypes->GetXaxis()->SetBinLabel(15,"FMS-JP0");
    h1TrigTypes->GetXaxis()->SetBinLabel(17,"FMS-JP1");
    h1TrigTypes->GetXaxis()->SetBinLabel(19,"FMS-JP2");
    h1TrigTypes->GetXaxis()->SetBinLabel(21,"None");
    
    TH1D *h1nJets = new TH1D ("h1nJets", "Number of Jets; No. of Jets", 10, -1, 9);

    TH1D *h1nVtx = new TH1D("h1nVtx", "Number of Vertices", 5, 0, 5);
    TH1D* h1Vtxz = new TH1D("h1Vtxz", "Vtx z Distribution [All Sources]; Vertex z [cm]",100, -200, 200);
    
    TH1D *h1JetEta = new TH1D ("h1JetEta", "EM Jet Eta; Jet #eta", 100, -1.5, 5.0);
    TH1D *h1JetPhi = new TH1D ("h1JetPhi", "EM Jet Phi; Jet #phi [rad]", 100, -3.1, 3.1);
    TH1D *h1JetE = new TH1D ("h1JetE", "EM Jet E; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetE_s = new TH1D ("h1JetE_s", "EM Jet E [small cells]; Jet E [GeV]", 100, 0.0, 85.0);
    TH1D *h1JetE_l = new TH1D ("h1JetE_l", "EM Jet E [large cells]; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetPt = new TH1D ("h1JetPt", "Jet Pt; Jet Pt [GeV/c]", 100, 0.0, 50.0);
    TH1D *h1JetVtxZ = new TH1D ("h1JetVtxZ", "Jet Vtx z; Jet vtx z [cm]", 100, -200.0, 200.0);
    TH1D *h1nPhotons = new TH1D("h1nPhotons", "number of photons in EM jets", 20, 0, 20);
    
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
    Int_t trigBit;
    string trgBitStr;
    StJetParticle *part;
    Int_t vtxType = -1;
    Double_t vtxZ = -999;
    //----- Event loop ----------
    //Loads entries from Jet Tree and Skim Tree simultaneously
    for (int iEntry = 0; iEntry < nentries; ++iEntry)
    {
	if (jetChain->GetEvent(iEntry) <= 0) break;

	// Should not be null
	assert(jetEvent);

	if (iEntry % 1000 == 0) cout << iEntry << endl;
	
	// if(jetEvent->numberOfJets() == 0)
	//     continue;

	StJetVertex* vertex = jetEvent->vertex(0);
	if (!vertex) continue;
	
	// if(vertex->ranking() < 0) continue; //Not applicable for EM jet	
	// if(fabs(vertex->position().z()) > 80.0) continue;


	//Trigger Types
	trigBit = jetEvent->runId();

	trgBitStr = to_string(trigBit);
	for(Int_t ii = 1; ii < 10; ++ii)
	{
	    if(trgBitStr[ii] == '1')
		h1TrigTypes->Fill(2*ii);
	}	
	
	if(trigBit == 1000000000)
	{
	    h1TrigTypes->Fill(2*10);
	    continue;
	}
	
	vtxType = -1;
	vtxZ = vertex->position().z();
	h1nVtx->Fill(jetEvent->numberOfVertices());
	
	
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
		//h1nPhotons->Fill(jetEvent->jet(i)->numberOfParticles());

	    if(eta < 2.0  || eta > 4.5) //Consider FMS EM Jets only (for trigger specific plots)
		continue;
	}	
    } // End event loop

    outFile->Write();
}
