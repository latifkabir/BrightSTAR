// Filename: TStNanoJetTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStNanoJetTreeMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StSpinPool/StJetEvent/StJetEvent.h"
#include "StSpinPool/StJetSkimEvent/StJetSkimEvent.h"
#include "StSpinPool/StJetEvent/StJetCandidate.h"
#include "StSpinPool/StJetEvent/StJetTower.h"
#include "StSpinPool/StJetEvent/StJetParticle.h"
#include "StSpinPool/StJetEvent/StJetVertex.h"
#include "StSpinPool/StJetEvent/StJetTrack.h"
#include "StJetFinder/StProtoJet.h"
#include "StJetMaker/StJetMaker2015.h"
#include "StJetMaker/StJetSkimEventMaker.h"

#include "TStJetEvent.h"
#include "TStJetSkimEvent.h"
#include "TStJetCandidate.h"

ClassImp(TStNanoJetTreeMaker)

//_____________________________________________________________________________ 
TStNanoJetTreeMaker::TStNanoJetTreeMaker(StJetMaker2015* jetMaker,  StJetSkimEventMaker* skimMaker, const char *name):StMaker(name)
{
    mOutJetEvent = new TStJetEvent();
    mOutSkimEvent = mOutJetEvent->GetEvent();

    mJetMaker = 0;
    mSkimEventMaker = 0;
    mInJetEvent = 0;
    mInSkimEvent = 0;

    mEtaMax = 6.0;
    mEtaMin = -2.0;
    mZdist = 735.0;

    mJetMaker = jetMaker;
    mSkimEventMaker = skimMaker;
}

//_____________________________________________________________________________ 
TStNanoJetTreeMaker::~TStNanoJetTreeMaker()
{
    //
    delete mOutJetEvent;
}


//_____________________________________________________________________________ 
Int_t TStNanoJetTreeMaker::Init()
{

    mOutFile = new TFile(mOutName, "recreate");
    mTree = new TTree("T", "EM Jet Analysis Tree");
    TStJetEvent::Class()->IgnoreTObjectStreamer();
    mTree->Branch("jetEvents", &mOutJetEvent, 256000, 99);

    //branch to hold FMS photons (used in FastJet) info from FMS maker

    cout << "Initialized NanoJetMaker!" <<endl;
    return kStOK;
}


//_____________________________________________________________________________
Int_t TStNanoJetTreeMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");

    // Check if mMuDst or mEvent is valid
    if(!mMuDst)
    {
	LOG_ERROR << "TSt<Template>Maker::Make - No MuDst found" <<endm;
	return kStFatal;
    }

    //mJetMaker = (StJetMaker2015*)GetMaker("StJetMaker2015"); //This did not work --> bug fixed, try again
    //mSkimEventMaker = (StJetSkimEventMaker*)GetMaker("StJetSkimEventMaker"); //Did not work --> bug fixed, try again
    if(!mJetMaker || !mSkimEventMaker)
    {
	LOG_ERROR << "TStNanoJetTreeMaker::Make - No JetMaker or SkimEventMaker found" <<endm;
	return kStFatal;	
    }

    mInJetEvent = mJetMaker->event("AntiKtR070NHits12"); //Set the branch name from a Set function
    mInSkimEvent = mSkimEventMaker->event();

    assert(mInJetEvent && mInSkimEvent);
    assert(mInJetEvent->runId() == mInSkimEvent->runId() && mInJetEvent->eventId() == mInSkimEvent->eventId());

    //intermediate variables
    Int_t spin4Bit;
    Int_t spinB;
    Int_t spinY;
    Int_t nPhotons;
    Double_t eta;
    Double_t phi;
    Double_t eng;
    Double_t pt;
    Double_t vtxZ;
    Double_t jetX;
    Double_t jetY;
    Double_t theta;
    Double_t rt;

    if ((mInSkimEvent->bx7() > 30 && mInSkimEvent->bx7() < 40) || (mInSkimEvent->bx7() > 110 && mInSkimEvent->bx7() < 120))
	    return kStOK;
    
    if(mInJetEvent->numberOfJets() == 0)
	return kStOK;
	
    mInVertex = mInJetEvent->vertex(); //same as mInJetEvent->vertex(0), i.e. highest ranked vertex only
    if (!mInVertex)
	return kStOK;
    
    mOutJetEvent->Reset();

    for(Int_t i = 0; i < mMaxTriggers; ++i)
	mJetTrig[i] = mInSkimEvent->trigger(mTrigIds[i]);
    
    for(Int_t i = 0; i < mMaxTriggers; ++i)
    {
	if(mJetTrig[i])
	{
	    if(mJetTrig[i]->didFire() && mJetTrig[i]->shouldFire())
		mOutSkimEvent->SetTrigFlag(i, 1);
	}
    }

    vtxZ = mInVertex->position().z();

    mOutSkimEvent->SetRunNumber(mInJetEvent->runId());
    mOutSkimEvent->SetEventId(mInJetEvent->eventId());
    mOutSkimEvent->SetVertexZ(vtxZ);
    mOutSkimEvent->SetUnixTime(mInSkimEvent->unixTime());
    mOutSkimEvent->SetBbcWestRate(mInSkimEvent->bbcWestRate());
    mOutSkimEvent->SetBbcEastRate(mInSkimEvent->bbcEastRate());
    mOutSkimEvent->SetFillNumber((Int_t)mInSkimEvent->fill());
    
    spin4Bit = mInSkimEvent->spin4usingBx48(); 
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

    mOutSkimEvent->SetSpinB(spinB);
    mOutSkimEvent->SetSpinY(spinY);
		
    for(Int_t i = 0; i < mInVertex->numberOfJets(); ++i) 
    {	    
	eta = mInVertex->jet(i)->eta();

	if(eta < mEtaMin  || eta > mEtaMax) 
	    continue;
	    
	phi = mInVertex->jet(i)->phi();
	eng = mInVertex->jet(i)->E();
	pt = mInVertex->jet(i)->pt();
	nPhotons = mInVertex->jet(i)->numberOfTowers();
	rt = mInVertex->jet(i)->rt();
	    
	theta =  2 * atan( exp(-eta) );
	jetX = (mZdist - vtxZ) * tan(theta) * cos(phi);
	jetY = (mZdist - vtxZ) * tan(theta) * sin(phi);

	mOutJet = mOutJetEvent->NewJet();	    
	mOutJet->SetNphotons(nPhotons);
	mOutJet->SetEta(eta);
	mOutJet->SetPhi(phi);
	mOutJet->SetE(eng);
	mOutJet->SetPt(pt);
	mOutJet->SetX(jetX);
	mOutJet->SetY(jetY);
	mOutJet->SetRt(rt);

	//Add Tower info
	for(Int_t j = 0; j < mInVertex->jet(i)->numberOfTowers(); ++j)
	{
	    mInTower = mInVertex->jet(i)->tower(j);
	    mOutTower = mOutJetEvent->NewTower();
	    mOutJetEvent->CopyTower(mInTower, mOutTower);		    
	    mOutJet->AddTower(mOutTower);
	}

	//Add particle info
	for(Int_t j = 0; j < mInVertex->jet(i)->numberOfParticles(); ++j)
	{
	    mInParticle = mInVertex->jet(i)->particle(j);
	    mOutParticle = mOutJetEvent->NewParticle();
	    mOutJetEvent->CopyParticle(mInParticle, mOutParticle);
	    mOutJet->AddParticle(mOutParticle);		
	}

	//Add track info (if needed) in a similar fashion as tower and particles	    
    }

    mTree->Fill();
        
    return kStOK;
}

//_____________________________________________________________________________
Int_t TStNanoJetTreeMaker::Finish()
{
    //Write histograms to root file etc.
    mOutFile->Write();
    return kStOK;
}

//_____________________________________________________________________________
void TStNanoJetTreeMaker:: SetTrigIds(Int_t *trigIds)
{
    for(Int_t i = 0; i < mMaxTriggers; ++i)
	mTrigIds[i] = trigIds[i];
}
