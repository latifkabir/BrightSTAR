
#include "StRootInclude.h"
#include "RootInclude.h"
#include "cppInclude.h"
#include <string>
// #include "TStTrigDef.h"

//Use file name with wildcard to chain many files
void FmsSimMakeResponseMatrix(
            Int_t nentries,
	    TString jetfile,
	    TString outfile
    )
{
    cout << "nentries = " << nentries << endl;
    cout << "jetfile  = " << jetfile  << endl;
    cout << "outfile = " << outfile << endl;

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
    StJetEvent* jetEventDet = 0;
    StJetEvent* jetEventPart = 0;
    jetChain->SetBranchAddress("AntiKtR070NHits12",&jetEventDet);
    jetChain->SetBranchAddress("AntiKtR070Particle",&jetEventPart);

    TString title;
    Int_t thresCut = 1.0;
    if(thresCut != -1)
	title = " [" + to_string(thresCut) + " GeV Et Threshold]";
    else
	title = " ";
    //-- All Triggers -------
    TH1D *h1TrigTypes = new TH1D("h1TrigTypes", "Trigger Types" + title + "; Trigger Types", 30, 0, 30);

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

    //Detector level jets
    TH1D *h1nJets = new TH1D("h1nJets", "Number of Jets" + title + "; No. of Jets", 10, -1, 9);

    TH1D *h1nVtx = new TH1D("h1nVtx", "Number of Vertices" + title + "; No of vertices", 5, 0, 5);
    TH1D* h1Vtxz = new TH1D("h1Vtxz", "Vtx z Distribution [All Sources]" + title + "; Vertex z [cm]",100, -200, 200);
    
    TH1D *h1JetEta = new TH1D("h1JetEta", "EM Jet Eta" + title + "; Jet #eta", 100, -1.5, 5.0);
    TH1D *h1JetPhi = new TH1D("h1JetPhi", "EM Jet Phi" + title + "; Jet #phi [rad]", 100, -3.1, 3.1);
    TH1D *h1JetE = new TH1D("h1JetE", "EM Jet E" + title + "; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetE_s = new TH1D("h1JetE_s", "EM Jet E [small cells]" + title + "; Jet E [GeV]", 100, 0.0, 85.0);
    TH1D *h1JetE_l = new TH1D("h1JetE_l", "EM Jet E [large cells]" + title + "; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetPt = new TH1D("h1JetPt", "Jet Pt" + title + "; Jet Pt [GeV/c]", 100, 0.0, 50.0);
    TH1D *h1JetVtxZ = new TH1D("h1JetVtxZ", "Jet Vtx z" + title + "; Jet vtx z [cm]", 100, -200.0, 200.0);
    TH1D *h1nPhotonsDet = new TH1D("h1nPhotonsDet", "number of photons in EM jets" + title + "; Number of photons", 20, 0, 20);
    
    TH2D *h2Jetxy = new TH2D("h2Jetxy", "Jet Position [FMS]" + title + "; Jet X [cm]; Jet Y [cm]", 100, -100, 100, 100, -100, 100);
    TH2D *h2JetEP = new TH2D("h2Jetep_fms", "Jet Eta Phi [FMS]" + title + "; Jet #eta; Jet #phi", 100, 2.5, 4.5, 100, -3.2, 3.2);
    
    TH2D *h2Jetxy_eemc = new TH2D("h2Jetxy_eemc", "Jet position [EEMC]" + title + "; Jet X [cm]; Jet Y [cm]", 100, -200.0, 200.0, 100, -200, 200);
    TH2D *h2EvsPt = new TH2D("h2EvsPt", "Eng vs Pt" + title + "; Pt [GeV/C]; E [GeV]", 100, 0, 20, 100, 0, 100);
    TH2D *h2PtvsE = new TH2D("h2PtvsE", "Pt vs E" + title + "; E [GeV]; Pt [GeV/c]", 100,  0, 100, 100, 0, 20);

    //Particle level jets
    TH1D *h1JetEpart = new TH1D("h1JetEpart", "EM Jet E [Particle Level Jets]; Jet E [GeV]", 100, 0.0, 70.0);
    TH1D *h1JetPtPart = new TH1D("h1JetPtPart", "Jet Pt [Particle Level Jets]; Jet Pt [GeV/c]", 100, 0.0, 50.0);
    TH1D *h1nPhotonsPart = new TH1D("h1nPhotonsPart", "Number of photons in EM jets [Particle Level Jets]; Number of photons", 20, 0, 20);
    
    //Response Matrix
    TH2D *h2EngResMat = new TH2D ("h2EngResMat", "Particle level vs Detector level jet E; E^{Det}_{jet} [GeV]; E^{Part}_{jet} [GeV]", 50, 0.0, 70.0, 100, 0.0, 70.0);
    TH2D *h2PtResMat = new TH2D ("h2PtResMat", "Particle level vs Detector level jet Pt; p_{T, jet}^{Det} [GeV]; p_{T, jet}^{Part} [GeV]", 50, 0.0, 10.0, 100, 0.0, 10.0);
    TH2D *h2nPhResMat = new TH2D ("h2nPhResMat", "Particle level vs Detector level jet photon multiplicity; nPhotons_{jet}^{Det}; nPhotons_{jet}^{Part}", 20, 0.0, 20.0, 20, 0.0, 20.0);

    TProfile *h2EngResMatProf = new TProfile("h2EngResMatProf", "Particle level vs Detector level jet E; E^{Det}_{jet} [GeV]; E^{Part}_{jet} [GeV]", 50, 0.0, 70.0, 0.0, 70.0);
    TProfile *h2PtResMatProf = new TProfile("h2PtResMatProf", "Particle level vs Detector level jet Pt; p_{T, jet}^{Det} [GeV]; p_{T, jet}^{Part} [GeV]", 50, 0.0, 10.0, 0.0, 10.0);
    TProfile *h2nPhResMatProf = new TProfile("h2nPhResMatProf", "Particle level vs Detector level jet photon multiplicity; nPhotons_{jet}^{Det}; nPhotons_{jet}^{Part}", 20, 0.0, 20.0, 0.0, 20.0);
    
    TLorentzVector lv;
    TVector3 v3;
    Double_t eta;
    Double_t phi;
    Double_t eng;
    Double_t pt;
    Double_t z_vtx;
    Double_t jetX;
    Double_t jetY;
    Double_t theta;
    Int_t trigBit;
    string trgBitStr;
    Int_t vtxType = -1;
    Double_t vtxZ = -999;
    Int_t nPhotonsDet;
    StJetParticle *part;
    
    Double_t etaPart;
    Double_t phiPart;
    Double_t engPart;
    Double_t ptPart;
    Int_t nPhotonsPart;
    Int_t jc = -1; //Closest particle jet index
    Double_t dR;
    Double_t dRc = 999;
    
    //----- Event loop ----------
    //Loads entries from Jet Tree and Skim Tree simultaneously
    for (int iEntry = 0; iEntry < nentries; ++iEntry)
    {
	if (jetChain->GetEvent(iEntry) <= 0) break;

	// Should not be null
	assert(jetEventDet);

	if (iEntry % 1000 == 0) cout << iEntry << endl;
	
	// if(jetEventDet->numberOfJets() == 0)
	//     continue;

	StJetVertex* vertex = jetEventDet->vertex(0);
	if (!vertex) continue;
	
	// if(vertex->ranking() < 0) continue; //Not applicable for EM jet	
	// if(fabs(vertex->position().z()) > 80.0) continue;

	//Trigger Types
	trigBit = jetEventDet->runId();

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
	h1nVtx->Fill(jetEventDet->numberOfVertices());
	
	
	h1nJets->Fill(jetEventDet->numberOfJets());

	//It seems most people loop over vertices. Find the justification.
	//I am looping over number of jets
	for(Int_t i = 0; i < jetEventDet->numberOfJets(); ++i) 
	{
	    nPhotonsDet = jetEventDet->jet(i)->numberOfTowers();
	    eta = jetEventDet->jet(i)->eta();
	    phi = jetEventDet->jet(i)->phi();
	    eng = jetEventDet->jet(i)->E();
	    pt = jetEventDet->jet(i)->pt();
	    z_vtx = jetEventDet->jet(i)->vertex()->position().z();
	    theta =  2 * atan( exp(-eta) );
	    jetX = (735. - z_vtx) * tan(theta) * cos(phi);
	    jetY = (735. - z_vtx) * tan(theta) * sin(phi);

	    if(eta < 2.0  || eta > 4.5) //Consider FMS EM Jets only (for trigger specific plots)
		continue;
	    
	    h1JetEta->Fill(eta);
	    h1JetPhi->Fill(phi);
	    h1JetPt->Fill(pt);
	    h1JetVtxZ->Fill(z_vtx); // no vtx found case i.e. vtx = 0 will dominate if all types filled together
	    h1JetE->Fill(eng);

	    if(eta > 2.0 && eta < 3.15) //large cells
		h1JetE_l->Fill(eng); 
	    if(eta > 3.15 && eta < 4.0)  //small cells
		h1JetE_s->Fill(eng);
	    
	    h2Jetxy->Fill(jetX, jetY);
	    h2JetEP->Fill(eta, phi);
	    h1nPhotonsDet->Fill(nPhotonsDet);
	    //h1nPhotonsDet->Fill(jetEventDet->jet(i)->numberOfParticles());

	    h2EvsPt->Fill(pt, eng);
	    h2PtvsE->Fill(eng, pt);

	    //---- Find the closest particle jet and take that as the corresponding particle jet for the given detector jet---
	    dRc = 999;
	    jc = -1;
	    for(Int_t j = 0; j < jetEventPart->numberOfJets(); ++j) 
	    {
		etaPart = jetEventPart->jet(j)->eta();
		phiPart = jetEventPart->jet(j)->phi();

		dR = sqrt(pow((eta - etaPart), 2) + pow((phi - phiPart), 2));

		if(dR < dRc)
		{
		    dRc = dR;
		    jc = j;
		}
	    }
	    
	    if(dRc > 0.2 || jc == -1)
		continue;
	    
	    engPart = jetEventPart->jet(jc)->E();
	    ptPart = jetEventPart->jet(jc)->pt();
	    nPhotonsPart = jetEventPart->jet(jc)->numberOfParticles();

	    h1JetEpart->Fill(engPart);
	    h1JetPtPart->Fill(ptPart);
	    h1nPhotonsPart->Fill(nPhotonsPart);
	    
	    h2EngResMat->Fill(eng, engPart);
	    h2PtResMat->Fill(pt, ptPart);
	    h2nPhResMat->Fill(nPhotonsDet, nPhotonsPart);

	    h2EngResMatProf->Fill(eng, engPart);
	    h2PtResMatProf->Fill(pt, ptPart);
	    h2nPhResMatProf->Fill(nPhotonsDet, nPhotonsPart);	    
	}	
    } // End event loop

    outFile->Write();
}
