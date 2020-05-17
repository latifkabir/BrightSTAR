
#include "StRootInclude.h"
#include "RootInclude.h"
#include "cppInclude.h"
#include "TStTrigDef.h"

/*
 It is possible to integrate it as part of maker and create the tree from JetMaker buffer. See Zhanwen's FMS jet analysis maker as the example. This way you do not need to keep large jet trees.
*/

//Use file name with wildcard to chain many files
void EjBinnedJetTree(
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
    TTree *tree = new TTree("T", "Binned Jet Tree");
    
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

    Int_t runNumber;
    Int_t eventId;
    Int_t spin4Bit;
    Int_t spinB;
    Int_t spinY;
    Int_t trigFlag[9]; // index 0:JP0 1:JP1 2:JP2 3:sm-bs-1 4:sm-bs-2 5:sm-bs-3 6:lg-bs-1 7:lg-bs-2 8:lg-bs-3 
    Int_t phiBin;      // 16 segments, Bins 1-8 for [0, pi], 9-16 for [-pi, 0]
    // Int_t engBin;      // Bins 1: 0 - 20 GeV, 2: 20 - 40 Gev, 3: 40 - 60 GeV, 4: 60 - 80 GeV, 5: 80 - 100 GeV
    // Int_t ptBin;       // Bin 1: 0.0 - 0.5, 2: 0.5 - 1.0 and so on i.e. [0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 10]
    Int_t nPhotons;
    
    Double_t eta;
    Double_t phi;
    Double_t eng;
    Double_t pt;
    Double_t vtxZ;
    Double_t jetX;
    Double_t jetY;

    Int_t nPhiBins = 8; //Per semi circle
    Double_t widthPhi = TMath::Pi() / nPhiBins;
    TLorentzVector lv;
    TVector3 v3;
    Double_t theta;

    tree->Branch("run", &runNumber, "run/I");
    tree->Branch("event", &eventId, "event/I");
    tree->Branch("spinB", &spinB, "spinB/I");
    tree->Branch("spinY", &spinY, "spinY/I");
    tree->Branch("trigFlag", trigFlag, "trigFlag[9]/I");  //index 0:JP0 1:JP1 2:JP2 3:sm-bs-1 4:sm-bs-2 5:sm-bs-3 6:lg-bs-1 7:lg-bs-2 8:lg-bs-3 
    tree->Branch("phiBin", &phiBin, "phiBin/I");
    // tree->Branch("engBin", &engBin, "engBin/I");
    // tree->Branch("ptBin", &ptBin, "ptBin/I");
    tree->Branch("nPhotons", &nPhotons, "nPhotons/I");

    tree->Branch("phi", &phi, "phi/D");
    tree->Branch("eta", &eta, "eta/D");
    tree->Branch("eng", &eng, "eng/D");
    tree->Branch("pt", &pt, "pt/D");
    tree->Branch("vtxZ", &vtxZ, "vtxZ/D");
    tree->Branch("jetX", &jetX, "jetX/D");
    tree->Branch("jetY", &jetY, "jetY/D");
    
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
    
    StJetParticle *part;
    Int_t vtxType = -1;
    vtxZ = -999;
    //----- Event loop ----------
    //Loads entries from Jet Tree and Skim Tree simultaneously
    for (int iEntry = 0; iEntry < nentries; ++iEntry)
    {
	if (jetChain->GetEvent(iEntry) <= 0 || skimChain->GetEvent(iEntry) <= 0)
	    break;

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
	
	if(jetEvent->numberOfJets() == 0)
	    continue;
	
	StJetVertex* vertex = jetEvent->vertex(0);
	if (!vertex)
	    continue;
	
	// if(vertex->ranking() < 0) continue; //Not applicable for EM jet

	//--------- Event Selection --------
	// if(fabs(vertex->position().z()) > 80.0)
	//     continue;

	//------ Reset values --------
	eta = -999.0;
	phi = -999.0;
	spinB = -9;
	spinY = -9;
	phiBin = -1;
	pt = -999;
	eng = -999;
	nPhotons = -1;
	jetX = -999;
	jetY = -999;
	vtxZ = -999;
	std::fill_n(trigFlag, 9, 0);
	
	//vtxZ = vertex->position().z();

	runNumber = jetEvent->runId();
	eventId = jetEvent->eventId();
	spin4Bit = skimEvent->spin4usingBx48(); 
	/*Spin info to be interpreted based on: https://drupal.star.bnl.gov/STAR/blog/oleg/spin-patterns-and-polarization-direction
	 spin = 10:  STAR pol B+ Y+
	      =  9:  STAR pol B+ Y-
	      =  6:  STAR pol B- Y+
              =  5:  STAR pol B- Y-
         (Note: Bunches can also be unpolarized, i.e. other possible values for the 4-spin bits are 1, 2, 4, and 8.)
	*/

	switch(spin4Bit) 
	{
	case 5:
	{  
	    spinB = -1;
	    spinY = -1;
	    break;
	}
	case 6:
	{
	    spinB = -1;
	    spinY = +1;
	    break;
	}
	case 9:
	{
	    spinB = +1;
	    spinY = -1;
	    break;
	}
	case 10:
	{
	    spinB = +1;
	    spinY = +1;
	    break;
	}
	default: //either one is unpolarized or undefined
	    spinB = 0;
	    spinY = 0;
	}

	
	if(trigJP0)
	{
	    if(trigJP0->didFire() && trigJP0->shouldFire())
		trigFlag[0] = 1;
	}
	
	if(trigJP1)
	{
	    if(trigJP1->didFire() && trigJP1->shouldFire())  //<-------- Check what did/should fire actually is
		trigFlag[1] = 1;
	}

	if(trigJP2)
	{
	    if(trigJP2->didFire() && trigJP2->shouldFire())
		trigFlag[2] = 1;
	}

	if(trigSB1)
	{
	    if(trigSB1->didFire() && trigSB1->shouldFire())
		trigFlag[3] = 1;
	}

	if(trigSB2)
	{
	    if(trigSB2->didFire() && trigSB2->shouldFire())
		trigFlag[4] = 1;
	}

	if(trigSB3)
	{
	    if(trigSB3->didFire() && trigSB3->shouldFire())
		trigFlag[5] = 1;
	}

	if(trigLB1)
	{
	    if(trigLB1->didFire() && trigLB1->shouldFire())
		trigFlag[6] = 1;
	}

	if(trigLB2)
	{
	    if(trigLB2->didFire() && trigLB2->shouldFire())
		trigFlag[7] = 1;
	}

	if(trigLB3)
	{
	    if(trigLB3->didFire() && trigLB3->shouldFire())
		trigFlag[8] = 1;
	}
       

	//It seems most jet people loop over vertices. Find the justification.
	//I am looping over number of jets
	for(Int_t i = 0; i < jetEvent->numberOfJets(); ++i) 
	{
	    eta = jetEvent->jet(i)->eta();

	    if(eta < 2.0  || eta > 4.5) //Consider FMS EM Jets only 
		continue;
	    
	    phi = jetEvent->jet(i)->phi();
	    eng = jetEvent->jet(i)->E();
	    pt = jetEvent->jet(i)->pt();
	    vtxZ = jetEvent->jet(i)->vertex()->position().z();
	    nPhotons = jetEvent->jet(i)->numberOfTowers();

	    theta =  2 * atan( exp(-eta) );
	    jetX = (735. - vtxZ) * tan(theta) * cos(phi);
	    jetY = (735. - vtxZ) * tan(theta) * sin(phi);

	    for(Int_t j = 0; j < nPhiBins; ++j)
	    {
		if(phi >= j*widthPhi && phi < (j+1)*widthPhi)
		{
		    phiBin = j + 1;
		    break;
		}
		else if(-1.0*phi >= j*widthPhi && -1.0*phi < (j+1)*widthPhi)
		{
		    phiBin = nPhiBins + j + 1;
		    break;
		}
	    }
	    tree->Fill();
	}	
    } // End event loop

    outFile->Write();
}
