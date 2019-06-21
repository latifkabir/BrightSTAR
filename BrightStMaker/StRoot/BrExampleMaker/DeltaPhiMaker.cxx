#include "DeltaPhiMaker.h"

#include "TFile.h"
#include "TH1.h"
#include "TObjArray.h"
#include "TList.h"

#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"

ClassImp(DeltaPhiMaker)

DeltaPhiMaker::DeltaPhiMaker(StMuDstMaker *maker) : StMaker("DeltaPhiMaker") {
    mNEventsPassed = mNEventsFailed = 0;
    mFileName = "";
    mRefMult[0] = 0; 
    mRefMult[1] = 9999;

    mMuDstMaker  = maker;
}

//__________________________________________________________________//
Int_t DeltaPhiMaker::Init() {

    //---- Track-wise histograms ----//
    hDeltaPhi = new TH1F("hDeltaPhi","dPhi",64,-3.2,3.2);
    hPhi =  new TH1F("hPhi","Phi",64,-3.2,3.2);
    hPt = new TH1F("hPt","Pt",100,2.,10.);

    //---- Event-wise histograms ----//
    hRefMult = new TH1F("hRefMult","Ref Mult",100,0,1000);
    hVz = new TH1F("hVz","Primary Vz",100,-25.,25.);
	
    return StMaker::Init();
}

//__________________________________________________________________//
Int_t DeltaPhiMaker::Make() {

    StMuEvent *muEvent = mMuDstMaker->muDst()->event();

    if(!accept(muEvent)) {
	mNEventsFailed++;
	return kStOk;
    }

    //--------- Fill event-wise histograms here --------//
    hVz->Fill( muEvent->primaryVertexPosition().z() );
    hRefMult->Fill( muEvent->refMult() );

    TObjArray *tracks = mMuDstMaker->muDst()->primaryTracks();
    TIter next(tracks);

    TList trigTracks, assocTracks;

    StMuTrack *track=0;
    while ( (track = (StMuTrack*)next()) ) {

	if(!accept(track)) continue;

	//-------- Fill TList's with high-pT tracks --------//
	assocTracks.Add(track);
	if(track->pt()>=4.0) trigTracks.Add(track);
		
	//-------- Fill track-wise histograms --------//
	hPhi->Fill(track->phi());
	hPt->Fill(track->pt());
    }

    StMuTrack *trigPart = 0;
    StMuTrack *assocPart = 0;
	
    TIter nextTrig(&trigTracks);
    TIter nextAssoc(&assocTracks);

    //-------- Loop over  --------//
    while ( (trigPart = (StMuTrack*)nextTrig()) ) {

	nextAssoc.Reset();
	while ( (assocPart = (StMuTrack*)nextAssoc()) ) {

	    if( trigPart->pt() > assocPart->pt() ) { 
 
		float dPhi = assocPart->phi() - trigPart->phi();
		if (dPhi>TMath::Pi()) dPhi-=2*(TMath::Pi());
		if (dPhi<-TMath::Pi()) dPhi+=2*(TMath::Pi());
		hDeltaPhi->Fill( dPhi );
	    }
	}
    }

    mNEventsPassed++;
    return kStOK;
}


//__________________________________________________________________//
Int_t DeltaPhiMaker::Finish() {

    //  Output file.
    TFile *mFile =  new TFile(mFileName, "RECREATE");
    cout << "The output filename is " << mFileName.Data() << endl;

    cout << "\tEvents passed: " << mNEventsPassed << endl;
    cout << "\tEvents failed: " << mNEventsFailed << endl;

    hPhi->Write();
    hPt->Write(); 
    hDeltaPhi->Write();
    hRefMult->Write();
    hVz->Write();
    mFile->Close();

    return kStOK;
}

bool DeltaPhiMaker::accept(StMuEvent* muEvent) {

    float vertexZ = muEvent->primaryVertexPosition().z();
    int refMult = muEvent->refMult();  

    if( vertexZ==0. || fabs(vertexZ) > 25. || refMult<14 ) return false;
    if( refMult < mRefMult[0] || refMult > mRefMult[1] ) return false;

    return true;
}

bool DeltaPhiMaker::accept(StMuTrack* track) {
    return track && 
	track->flag() >= 0 &&
	track->nHits() >= 25 &&
	track->pt() >= 2.0 &&
	track->eta() <= 0.7 ;
}
