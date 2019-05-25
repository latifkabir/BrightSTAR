#include "TFmsMaker.h"

#include "TFile.h"
#include "TH1.h"
#include "TObjArray.h"
#include "TList.h"

#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"

ClassImp(TFmsMaker)

TFmsMaker::TFmsMaker(StMuDstMaker *maker) : StMaker("TFmsMaker")
{
    mNEventsPassed = mNEventsFailed = 0;
    mFileName = "";
    mRefMult[0] = 0; 
    mRefMult[1] = 9999;

    mMuDstMaker  = maker;
}

//__________________________________________________________________//
Int_t TFmsMaker::Init()
{
    //---- Track-wise histograms ----//
    hPhi =  new TH1F("hPhi","Phi",64,-3.2,3.2);
    hPt = new TH1F("hPt","Pt",100,2.,10.);

    //---- Event-wise histograms ----//
    hRefMult = new TH1F("hRefMult","Ref Mult",100,0,1000);
    hVz = new TH1F("hVz","Primary Vz",100,-25.,25.);
	
    return StMaker::Init();
}

//__________________________________________________________________//
Int_t TFmsMaker::Make()
{
    StMuEvent *muEvent = mMuDstMaker->muDst()->event();

    if(!accept(muEvent))
    {
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
    while ( (track = (StMuTrack*)next()) )
    {
	if(!accept(track)) continue;

	//-------- Fill TList's with high-pT tracks --------//
	assocTracks.Add(track);
	if(track->pt()>=4.0) trigTracks.Add(track);
		
	//-------- Fill track-wise histograms --------//
	hPhi->Fill(track->phi());
	hPt->Fill(track->pt());
    }

    mNEventsPassed++;
    return kStOK;
}


//__________________________________________________________________//
Int_t TFmsMaker::Finish()
{
    //  Output file.
    TFile *mFile =  new TFile(mFileName, "RECREATE");
    cout << "The output filename is " << mFileName.Data() << endl;

    cout << "\tEvents passed: " << mNEventsPassed << endl;
    cout << "\tEvents failed: " << mNEventsFailed << endl;

    hPhi->Write();
    hPt->Write(); 
    hRefMult->Write();
    hVz->Write();
    mFile->Close();

    return kStOK;
}

bool TFmsMaker::accept(StMuEvent* muEvent)
{
    // float vertexZ = muEvent->primaryVertexPosition().z();
    // int refMult = muEvent->refMult();  

    // if( vertexZ==0. || fabs(vertexZ) > 25. || refMult<14 ) return false;
    // if( refMult < mRefMult[0] || refMult > mRefMult[1] ) return false;

    return true;
}

bool TFmsMaker::accept(StMuTrack* track)
{
    // return track && 
    // 	track->flag() >= 0 &&
    // 	track->nHits() >= 25 &&
    // 	track->pt() >= 2.0 &&
    // 	track->eta() <= 0.7 ;

    return true;
}
