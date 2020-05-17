// Filename: TStEmJetAnaMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStEmJetAnaMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"

ClassImp(TStEmJetAnaMaker)

//_____________________________________________________________________________ 
TStEmJetAnaMaker::TStEmJetAnaMaker(const char *name):StMaker(name)
{
    //
}

//_____________________________________________________________________________ 
TStEmJetAnaMaker::~TStEmJetAnaMaker()
{
    //
}


//_____________________________________________________________________________ 
Int_t TStEmJetAnaMaker::Init()
{
    mOutFile = new TFile(mOutName, "recreate");
    mTree = new TTree("T", "EM Jet Analysis Tree");


    tree->Branch("run", &runNumber, "run/I");
    tree->Branch("event", &eventId, "event/I");
    tree->Branch("spinB", &spinB, "spinB/I");
    tree->Branch("spinY", &spinY, "spinY/I");
    tree->Branch("phiBin", &phiBin, "phiBin/I");
    tree->Branch("nPhotons", &nPhotons, "nPhotons/I");
    tree->Branch("phi", &phi, "phi/D");
    tree->Branch("eta", &eta, "eta/D");
    tree->Branch("eng", &eng, "eng/D");
    tree->Branch("pt", &pt, "pt/D");
    tree->Branch("vtxZ", &vtxZ, "vtxZ/D");
    tree->Branch("jetX", &jetX, "jetX/D");
    tree->Branch("jetY", &jetY, "jetY/D");

    //branch to hold FMS photons (used in FastJet) info from FMS maker
    
    return kStOK;
}


//_____________________________________________________________________________
Int_t TStEmJetAnaMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");
    // Get St<det>MuCollection
    // Or Get St<det>Colelction
    // Check if mMuDst or mEvent is valid
    if(!mMuDst)
    {
	LOG_ERROR << "TSt<Template>Maker::Make - No MuDst found" <<endm;
	return kStFatal;
    }
      
    return kStOK;
}

//_____________________________________________________________________________
Int_t TStEmJetAnaMaker::Finish()
{
    //Write histograms to root file etc.

    return kStOK;
}
