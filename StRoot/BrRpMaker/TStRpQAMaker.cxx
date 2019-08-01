// Filename: TStRpQAMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Jun 22 00:16:21 2019 (-0400)
// URL: jlab.org/~latif


#include "TStRpQAMaker.h"

#include "TFile.h"
#include "TH1.h"
#include "TObjArray.h"
#include "TList.h"

#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"

ClassImp(TStRpQAMaker)

TStRpQAMaker::TStRpQAMaker(StMuDstMaker *maker) : StMaker("TStRpQAMaker")
{
    mNEventsPassed = mNEventsFailed = 0;
    mFileName = "";
    mMuDstMaker  = maker;
}

//__________________________________________________________________//
Int_t TStRpQAMaker::Init()
{
    mMuDstMaker->SetStatus("*",0);
    mMuDstMaker->SetStatus("MuEvent",1);
    mMuDstMaker->SetStatus("Event",1);
    mMuDstMaker->SetStatus("pp2pp*",1);
    
    mFile = new TFile(mFileName,"recreate");    
    mHist1[0] = new TH1F("h1nPlanes", "number of planes; number of planes", 100, 0.0, 10.0);
    mHist1[1] = new TH1F("h1nRP", "number of RP; number of RP", 100, 0.0, 10.0);
    mHist1[2] = new TH1F("h1BrID", "branch ID; branch ID", 20, 0.0, 5.0);
    mHist1[3] = new TH1F("h1nTracks", "Number of Tracks per event [ with nTracks > 0 ]; Number of Tracks", 500, 0.0, 500.0);
    mHist1[4] = new TH1F("h1Theta", "#theta; #theta [mRad]", 200, 0.0, 30.0);
    mHist1[5] = new TH1F("h1Theta_x", "#theta_{x}; #theta_{x} [mRad]", 200, -30.0, 10.0);
    mHist1[6] = new TH1F("h1Theta_y", "#theta_{y}; #theta_{y} [mRad]", 200, -30.0, 30.0);
    mHist1[7] = new TH1F("h1Eta", "#eta; #eta", 100, -10.0, 10.0);
    mHist1[8] = new TH1F("h1Pt", "Pt [GeV/c]; Pt [GeV/c]", 100, 0.0, 5.0);
    mHist1[9] = new TH1F("h1p", "p [GeV/c]; p [GeV/c]", 100, 0.0, 200.0);
    mHist1[10] = new TH1F("h1xi", "#xi; #xi", 100, -5.0, 5.0);
    mHist1[11] = new TH1F("h1tNeg", "-t [(GeV/c)^{2}]; -t [(GeV/c)^{2}]", 100, 0.0, 5.0);
    mHist1[12] = new TH1F("h1Phi", "#phi [mRad]; #phi [mRad]", 100, -4.0, 4.0);

    mMuDst = mMuDstMaker->muDst();
    return StMaker::Init();
}

//__________________________________________________________________//
Int_t TStRpQAMaker::Make()
{
    mMuEvent = mMuDst->event();

    if(!Accept(mMuEvent))
    {
	mNEventsFailed++;
	return kStOk;
    }

    mRpsMuColl = mMuDst->RpsCollection();

    if(!mRpsMuColl)
    {
	cout<<"No RP data for this event"<<endl;
	return -1;
    }
    mHist1[0]->Fill(mRpsMuColl->numberOfPlanes());
    mHist1[1]->Fill(mRpsMuColl->numberOfRomanPots());
    if(mRpsMuColl->numberOfTracks() > 0)
    {
	mHist1[3]->Fill(mRpsMuColl->numberOfTracks());
	for(Int_t i = 0; i < mRpsMuColl->numberOfTracks(); ++i)
	{
	    mHist1[2]->Fill(mRpsMuColl->track(i)->branch());	
	    mHist1[4]->Fill(1000.0*mRpsMuColl->track(i)->theta());	
	    mHist1[5]->Fill(1000.0*mRpsMuColl->track(i)->theta(0));	
	    mHist1[6]->Fill(1000.0*mRpsMuColl->track(i)->theta(1));	
	    mHist1[7]->Fill(mRpsMuColl->track(i)->eta());
	    mHist1[8]->Fill(mRpsMuColl->track(i)->pt());		
	    mHist1[9]->Fill(mRpsMuColl->track(i)->p());		
	    mHist1[10]->Fill(mRpsMuColl->track(i)->xi(100.0)); // Beam momentum is approximate		
	    mHist1[11]->Fill(-1.0*mRpsMuColl->track(i)->t(100.0));		
	    mHist1[12]->Fill(mRpsMuColl->track(i)->phi());		
	}
    }
    ++mNEventsPassed;
    return kStOK;
}

//__________________________________________________________________//
Int_t TStRpQAMaker::Finish()
{
    //TFile *mFile =  new TFile(mFileName, "RECREATE");
    cout << "The output filename is " << mFileName.Data() << endl;

    cout << "\tEvents passed: " << mNEventsPassed << endl;
    cout << "\tEvents failed: " << mNEventsFailed << endl;

    mFile->cd();
    for(Int_t i = 0; i < mNplots; ++i)
	mHist1[i]->Write();
    
    mFile->Close();

    return kStOK;
}

Bool_t TStRpQAMaker::Accept(StMuEvent* muEvent)
{
    // Float_t vertexZ = muEvent->primaryVertexPosition().z();
    // Int_t refMult = muEvent->refMult();  

    // if( vertexZ==0. || fabs(vertexZ) > 25. || refMult<14 ) return false;
    // if( refMult < mRefMult[0] || refMult > mRefMult[1] ) return false;

    return kTRUE;
}

Bool_t TStRpQAMaker::Accept(StMuTrack* track)
{
    // return track && 
    // 	track->flag() >= 0 &&
    // 	track->nHits() >= 25 &&
    // 	track->pt() >= 2.0 &&
    // 	track->eta() <= 0.7 ;

    return kTRUE;
}
