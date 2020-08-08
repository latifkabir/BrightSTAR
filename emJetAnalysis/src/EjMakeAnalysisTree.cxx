
#include "StRootInclude.h"
#include "RootInclude.h"
#include "cppInclude.h"
#include "TStTrigDef.h"
#include "BrJetMaker/TStJetSkimEvent.h"
#include "BrJetMaker/TStJetCandidate.h"
#include "BrJetMaker/TStJetEvent.h"

/*
 It is possible to integrate it as part of maker and create the tree from JetMaker buffer. See Zhanwen's FMS jet analysis maker as the example. This way you do not need to keep large jet trees.
*/

//Use file name with wildcard to chain many files
void EjMakeAnalysisTree(
    TString det, Int_t nentries,
	    TString jetfile,
	    TString skimfile,
	    TString outfile
    )
{
    cout << "nentries = " << nentries << endl;
    cout << "jetfile  = " << jetfile  << endl;
    cout << "skimfile = " << skimfile << endl;
    cout << "outfile = " << outfile << endl;

    if(!(det == "fms" || det == "eemc"))
    {
	cout << "Invalid detector name" <<endl;
	return;
    }
    
    // Load libraries
    // gSystem->Load("StJetEvent");
    // gSystem->Load("StJetSkimEvent");

    TFile *outFile = new TFile(outfile, "recreate");
    TTree *tree = new TTree("T", "Analysis Jet Tree");
    
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
    StJetEvent* inJetEvent = 0;
    jetChain->SetBranchAddress("AntiKtR070NHits12",&inJetEvent);

    // Set skim buffer
    StJetSkimEvent* inSkimEvent = 0;
    skimChain->SetBranchAddress("skimEventBranch",&inSkimEvent);

    Double_t mEEmcZSMD = kEEmcZSMD;
    Double_t mFmsZ = 735.0;
    Double_t fmsEtaMin = 2.0;
    Double_t fmsEtaMax = 4.5;
    Double_t eemcEtaMin = -2.0;//0.8; //<----------- TEMPORARY, to match unintentional other trees
    Double_t eemcEtaMax = 6.0; //2.5;
    Double_t etaMin;
    Double_t etaMax;
    Double_t zDist;

    if(det == "fms")
    {
	zDist = mFmsZ;
	etaMin = fmsEtaMin;
	etaMax = fmsEtaMax;
    }
    else if(det == "eemc")
    {
	zDist = mEEmcZSMD;
	etaMin = eemcEtaMin;
	etaMax = eemcEtaMax;
    }
    
    TStJetEvent::Class()->IgnoreTObjectStreamer();
    TStJetEvent *outJetEvent = new TStJetEvent();
    
    TStJetSkimEvent *outSkimEvent = outJetEvent->GetEvent();    
    TStJetCandidate *outJet;
    TStJetParticle *outParticle;
    TStJetTower *outTower;
    
    StJetParticle *inParticle;
    StJetTower *inTower;
    StJetVertex* inVertex;

    tree->Branch("jetEvents", &outJetEvent, 256000, 99);
    
    Int_t runNumber;
    Int_t eventId;
    Int_t spin4Bit;
    Int_t spinB;
    Int_t spinY;
    Int_t trigFlag[9]; // index 0:JP0 1:JP1 2:JP2 3:sm-bs-1 4:sm-bs-2 5:sm-bs-3 6:lg-bs-1 7:lg-bs-2 8:lg-bs-3 
    Int_t nPhotons;
    
    Double_t eta;
    Double_t phi;
    Double_t eng;
    Double_t pt;
    Double_t vtxZ;
    Double_t jetX;
    Double_t jetY;
    Double_t rt;
    Double_t phEng;
    Double_t theta;
                
    const Int_t kMaxTriggers = 9;
    Int_t trigIds[kMaxTriggers];
    TString triggerNames_fms[kMaxTriggers] = {"FMS-JP0", "FMS-JP1", "FMS-JP2", "FMS-sm-bs1", "FMS-sm-bs2", "FMS-sm-bs3", "FMS-lg-bs1", "FMS-lg-bs2", "FMS-lg-bs3"};
    TString triggerNames_eemc[kMaxTriggers] = {"EHT0", "JP1", "JP2", "EHT0*EJP1*L2Egamma", "JP2*L2JetHigh", "BHT1*VPDMB-30", "BHT0*BBCMB", "BHT1*BBCMB", "BHT2*BBCMB"};

    skimChain->GetEvent(0);
    if(det == "fms")
    {
	for(Int_t i = 0; i < kMaxTriggers; ++i)
	    trigIds[i] = TStTrigDef::GetTrigId(inSkimEvent->runId(), triggerNames_fms[i]);
    }
    else if(det == "eemc")
    {
	for(Int_t i = 0; i < kMaxTriggers; ++i)
	    trigIds[i] = TStTrigDef::GetTrigId(inSkimEvent->runId(), triggerNames_eemc[i]);
    }
        
    StJetSkimTrig* jetTrig[kMaxTriggers]; 
    
    vtxZ = -999;
    //----- Event loop ----------
    //Loads entries from Jet Tree and Skim Tree simultaneously
    for(unsigned long iEntry = 0; iEntry < nentries; ++iEntry)
    {
	if (jetChain->GetEvent(iEntry) <= 0 || skimChain->GetEvent(iEntry) <= 0)
	    break;

	// Should not be null
	assert(inJetEvent && inSkimEvent);

	// Enforce event synchronization
	assert(inJetEvent->runId() == inSkimEvent->runId() && inJetEvent->eventId() == inSkimEvent->eventId());

	if (iEntry % 1000 == 0) cout << iEntry << endl;
		
	if(inJetEvent->numberOfJets() == 0)
	    continue;
	
	inVertex = inJetEvent->vertex(); //same as inJetEvent->vertex(0), i.e. highest ranked vertex only
	if (!inVertex)
	    continue;
	
	// if(inVertex->ranking() < 0) continue; //Not applicable for EM jet

	//--------- Event Selection --------
	// if(fabs(inVertex->position().z()) > 80.0) //Do it from next level of processing
	//     continue;

	if ((inSkimEvent->bx7() > 30 && inSkimEvent->bx7() < 40) || (inSkimEvent->bx7() > 110 && inSkimEvent->bx7() < 120))
	    continue;

	//------ Reset values --------
	outJetEvent->Reset();

	for(Int_t i = 0; i < kMaxTriggers; ++i)
	    jetTrig[i] = inSkimEvent->trigger(trigIds[i]);

	//------- Trigger Info -------
	for(Int_t i = 0; i < kMaxTriggers; ++i)
	{
	    if(jetTrig[i])
	    {
		if(jetTrig[i]->didFire() && jetTrig[i]->shouldFire()) //<-------- Check what did/should fire actually is
		    outSkimEvent->SetTrigFlag(i, 1);
	    }
	}
	
	vtxZ = inVertex->position().z();

	outSkimEvent->SetRunNumber(inJetEvent->runId());
	outSkimEvent->SetEventId(inJetEvent->eventId());
	outSkimEvent->SetVertexZ(vtxZ);
	outSkimEvent->SetUnixTime(inSkimEvent->unixTime());
	
	spin4Bit = inSkimEvent->spin4usingBx48(); 
	/*
	  Spin info to be interpreted based on: https://drupal.star.bnl.gov/STAR/blog/oleg/spin-patterns-and-polarization-direction
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

	outSkimEvent->SetSpinB(spinB);
	outSkimEvent->SetSpinY(spinY);
		
	for(Int_t i = 0; i < inVertex->numberOfJets(); ++i) 
	{	    
	    eta = inVertex->jet(i)->eta();

	    if(eta < etaMin  || eta > etaMax) //Consider FMS or EEMC EM Jets only 
		continue;
	    
	    phi = inVertex->jet(i)->phi();
	    eng = inVertex->jet(i)->E();
	    pt = inVertex->jet(i)->pt();
	    nPhotons = inVertex->jet(i)->numberOfTowers();
	    rt = inVertex->jet(i)->rt();
	    
	    theta =  2 * atan( exp(-eta) );
	    jetX = (zDist - vtxZ) * tan(theta) * cos(phi);
	    jetY = (zDist - vtxZ) * tan(theta) * sin(phi);

	    outJet = outJetEvent->NewJet();	    
	    outJet->SetNphotons(nPhotons);
	    outJet->SetEta(eta);
	    outJet->SetPhi(phi);
	    outJet->SetE(eng);
	    outJet->SetPt(pt);
	    outJet->SetX(jetX);
	    outJet->SetY(jetY);
	    outJet->SetRt(rt);

	    //Add Tower info
	    for(Int_t j = 0; j < inVertex->jet(i)->numberOfTowers(); ++j)
	    {
		inTower = inVertex->jet(i)->tower(j);
		outTower = outJetEvent->NewTower();
		outJetEvent->CopyTower(inTower, outTower);		    
		outJet->AddTower(outTower);
	    }

	    //Add particle info
	    for(Int_t j = 0; j < inVertex->jet(i)->numberOfParticles(); ++j)
	    {
		inParticle = inVertex->jet(i)->particle(j);
		outParticle = outJetEvent->NewParticle();
		outJetEvent->CopyParticle(inParticle, outParticle);
		outJet->AddParticle(outParticle);		
	    }

	    //Add track info (if needed) in a similar fashion as tower and particles	    
	}
	tree->Fill();
			
    } // End event loop

    outFile->Write();
}
