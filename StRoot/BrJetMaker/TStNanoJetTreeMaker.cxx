// Filename: TStNanoJetTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStNanoJetTreeMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
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
#include "StEvent/StTriggerData.h"
#include "StEvent/StTriggerId.h"

#include "TStJetEvent.h"
#include "TStJetSkimEvent.h"
#include "TStJetCandidate.h"

ClassImp(TStNanoJetTreeMaker)

//_____________________________________________________________________________ 
TStNanoJetTreeMaker::TStNanoJetTreeMaker(const char *name):StMaker(name)
{
    mOutJetEvent = new TStJetEvent();
    mOutSkimEvent = mOutJetEvent->GetEvent();
    mTrigFlag = kFALSE;
    
    mJetMaker = 0;
    mSkimEventMaker = 0;
    mInJetEvent = 0;
    mInSkimEvent = 0;

    mEtaMax = 6.0;
    mEtaMin = -2.0;
    mZdist = 735.0;
}

//_____________________________________________________________________________ 
TStNanoJetTreeMaker::~TStNanoJetTreeMaker()
{
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
	LOG_ERROR << "TStNanoJetTreeMaker::Make - No MuDst found" <<endm;
	return kStFatal;
    }
    mMuEvent = mMuDst->event();
    
    mJetMaker = (StJetMaker2015*)GetMaker("StJetMaker2015"); 
    mSkimEventMaker = (StJetSkimEventMaker*)GetMaker("StJetSkimEventMaker"); 
    if(!mJetMaker || !mSkimEventMaker)
    {
	LOG_ERROR << "TStNanoJetTreeMaker::Make - No JetMaker or SkimEventMaker found" <<endm;
	return kStFatal;	
    }

    mInJetEvent = mJetMaker->event(mBranchName); //Set the branch name from a Set function
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

    Double_t towEta;
    Double_t towPhi;
    Double_t dR;
    Double_t dPhi;
    Double_t dEta;

    if ((mInSkimEvent->bx7() > 30 && mInSkimEvent->bx7() < 40) || (mInSkimEvent->bx7() > 110 && mInSkimEvent->bx7() < 120))
	    return kStOK;
    
    if(mInJetEvent->numberOfJets() == 0)
	return kStOK;

    //For EM jet we have mostly have one vertex from BBC/VPD/TPC. So we do not loop over vertices
    mInVertex = mInJetEvent->vertex(); //same as mInJetEvent->vertex(0), i.e. highest ranked vertex only
    if (!mInVertex)
	return kStOK;
    
    mOutJetEvent->Reset();

    mTrigFlag = kFALSE;
    for(Int_t k = 0; k < mMaxTriggers; ++k)
    {
	mTrigFlag = mMuEvent->triggerIdCollection().nominal().isTrigger(mTrigIds[k]);
	if(mTrigFlag)
	{
	    mOutSkimEvent->SetTrigFlag(k, 1);
	    mOutSkimEvent->SetTrigBit(k);
	}
    }	
    
    vtxZ = mInVertex->position().z();

    //tof and bbc multiplicity. Needed to minimize FMS ring of fire.
    mOutSkimEvent->SetTofMult(mMuEvent->triggerData()->tofMultiplicity()); 
    mOutSkimEvent->SetTofTrayMult(mMuEvent->btofTrayMultiplicity()); 
    //bbc multiplicity
    Int_t tac, adc;
    Int_t bbcMult = 0;
    Int_t east = 0;
    for(Int_t i = 1; i <= 16; i++)
    {
	tac = mMuEvent->triggerData()->bbcTDC((StBeamDirection)east,i);
	if(tac > 100 && tac < 2400)
	{
	    adc = mMuEvent->triggerData()->bbcADC((StBeamDirection)east, i);
	    if(adc > 50)
		bbcMult++;
	    //bbcEsum += trgd->bbcADC(east,i);
	}
    } 
    mOutSkimEvent->SetBbcMult(bbcMult);
    
    mOutSkimEvent->SetRunNumber(mInJetEvent->runId());
    mOutSkimEvent->SetEventId(mInJetEvent->eventId());
    mOutSkimEvent->SetVertexZ(vtxZ);
    //mOutSkimEvent->SetUnixTime(mInSkimEvent->unixTime()); //skimEvent does not have ET to GEM correction for SL20a
    mOutSkimEvent->SetUnixTime(mInJetEvent->unixTime());
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
	rt = mInVertex->jet(i)->rt();
	nPhotons = 0;
	    
	theta =  2 * atan( exp(-eta) );
	jetX = (mZdist - vtxZ) * tan(theta) * cos(phi);
	jetY = (mZdist - vtxZ) * tan(theta) * sin(phi);

	mOutJet = mOutJetEvent->NewJet();	    
	mOutJet->SetEta(eta);
	mOutJet->SetPhi(phi);
	mOutJet->SetE(eng);
	mOutJet->SetPt(pt);
	mOutJet->SetX(jetX);
	mOutJet->SetY(jetY);
	mOutJet->SetRt(rt);
	mOutJet->SetUedPt(mInVertex->jet(i)->ueDensity()["OffAxisConesR070"]*mInVertex->jet(i)->area());
	//cout << "Density: "<< mInVertex->jet(i)->ueDensity()["OffAxisConesR070"] <<" Area: "<< mInVertex->jet(i)->area() <<endl;
    
	//Add Tower info
	for(Int_t j = 0; j < mInVertex->jet(i)->numberOfTowers(); ++j)
	{
	    mInTower = mInVertex->jet(i)->tower(j);
	    mOutTower = mOutJetEvent->NewTower();
	    mOutJetEvent->CopyTower(mInTower, mOutTower);		    
	    mOutJet->AddTower(mOutTower);

	    towEta = mOutTower->GetEta();
	    towPhi = mOutTower->GetPhi();
	    dEta = (eta - towEta);
	    dPhi = (phi - towPhi);
	    
	    if(dPhi > TMath::Pi()) dPhi = -2*TMath::Pi() + dPhi;
	    if(dPhi < -TMath::Pi()) dPhi =  2*TMath::Pi() + dPhi;
	    
	    dR = sqrt(pow(dEta, 2) + pow(dPhi, 2));
	    if(dR < mR)
		++nPhotons;	    
	}
	mOutJet->SetNphotons(nPhotons);

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
