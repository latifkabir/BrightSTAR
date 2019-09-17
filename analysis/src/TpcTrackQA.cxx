// Filename: ExAnalysis3.C
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Sun May 12 16:19:03 2019 (-0400)
// URL: jlab.org/~latif

#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TObjArray.h>
#include "StEvent/StEvent.h"
#include "StEvent/StTrackNode.h"
#include "StChain/StChain.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuBTofPidTraits.h"
#include "TStar.h"
#include "TStRunList.h"
#include "TpcTrackQA.h"

void TpcTrackQA(Int_t runNumber, Int_t nFiles)
{
    TStRunList::MakeFileList(runNumber, nFiles);
    TString fileList = TStar::Config->GetFileList();  //"/star/u/kabir/pwg/data/st_physics_12114059_raw_3010001.MuDst.root"
    TpcTrackQA(fileList, "TpcQA.root");
}
void TpcTrackQA(TString fileList, TString outFile)
{

    StChain *chain = new StChain;
    StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);       

    // muDstMaker->SetStatus("*", 0);
    // muDstMaker->SetStatus("MuEvent", 1);
    // muDstMaker->SetStatus("*Track*", 1);
    // muDstMaker->SetStatus("*Vert*", 1);
    
    StMuDst *mDst = muDstMaker->muDst();  
    StMuEvent *ev = 0;
    StMuTrack *track = 0;
    TObjArray *tracks = 0;
    StEvent *stEvent;
    StMuPrimaryVertex *pVtx;
    StEventSummary evSummary;
    StMuBTofPidTraits btofPidTraits;
    Double_t beta = -999;
    Double_t mass2 = 0.0;
    
    TFile *file = new TFile(outFile, "RECREATE"); 
    TH1D *hNgTrk = new TH1D("hNgTrk", "Number of Global Tracks Per Event; No. of Global Tracks Per Event", 200, 0, 2000);
    TH1D *hNpTrk = new TH1D("hNpTrk", "Number of Primary Tracks Per Event; No. of Primary Tracks Per Event", 100, 0, 100);
    TH1D *hPvtx = new TH1D("hPvtx", "Number of Primary Vertices Per Event; No. of Primary Vertices Per Event", 20, 0, 10);
    TH1D *hTrkPt = new TH1D("hTrkPt", "Track Pt; Track Pt [GeV/c]", 200, 0, 10);
    // TH1D *hNtrkNodes = new TH1D("hNtrkNodes", "Number of Track Nodes Per Event; No. of Track Nodes Per Event", 200, 0, 0);
    TH1D *hVz = new TH1D("hVz", "Vertex z for triggered event; Vertex z [cm]", 200, -200, 200);
    TH1D *hVr = new TH1D("hVr", "Vertex dist r (Distance from x = 0, y = 0); V_{R} [cm]", 100, 0.3, 0.4);
    TH2D *hVxy = new TH2D("h2dVxy", "Vertex xy; Vertex x [cm]; Vertex y [cm]", 100, -5, 5, 100, -5, 5);
    TH1D *hVz_all = new TH1D("hVz_all", "Vertex z for all vertices; Vertex z [cm]", 200, -200, 200);
    TH1D *hMom = new TH1D("hMom", "Momentum p; p [GeV/c]", 200, 0, 10);
    TH1D *hPhi = new TH1D("hPhi", "Track phi; Track #phi [deg]", 200, -180, 180);
    TH1D *hEta = new TH1D("hEta", "Track Eta; #eta", 200, -2.0, 2.0);
    TH1D *hNhitsPoss = new TH1D("hNhitsPoss", "nHits Possible; nHits Possible", 100, 0, 100);
    TH1D *hNhitsFit = new TH1D("hNhitsFit", "nHits Fit; nHits Fit", 50, 0, 50);
    TH1D *hNhitsDedx = new TH1D("hNhitsDedx", "nHits Dedx; nHits Dedx", 50, 0, 50);
    TH1D *hDcaGlobal = new TH1D("hDcaGlobal", "DCA Global; DCA [cm]", 200, 0, 4.0);
    TH2D *hDedxVsQp = new TH2D("hDedxVsQp", "dE/dx vs qx|p|; qx|p| [GeV/c]; dE/dx [keV/cm]", 100, -3.0, 3.0, 100, 0, 10);
    TH2D *hm2VsQp = new TH2D("hm2VsQp", "m^{2} vs qx|p|; qx|p| [GeV/c]; m^{2} [(GeV/c)^{2}]", 100, -3.0, 3.0, 100, 0, 2.0);
    
    chain->Init();
    Int_t nEvents = muDstMaker->chain()->GetEntries();
    Int_t nTracks;
    
    cout << "Total events to be processed: "<< nEvents <<endl;
	
    for (int iev = 0; iev < nEvents; iev++)
    {
	chain->Clear();
	int iret = chain->Make(iev); 

	ev = mDst->event();
	tracks = mDst->primaryTracks();

	hNgTrk->Fill(mDst->numberOfGlobalTracks());
	hNpTrk->Fill(mDst->numberOfPrimaryTracks());
	hPvtx->Fill(mDst->numberOfPrimaryVertices());
	
	//---------- Access All Vertices including pile-ups-----------------
	for(Int_t i = 0; i < mDst->numberOfPrimaryVertices(); ++i)
	{
	    pVtx = mDst->primaryVertex(i);
	    if(!pVtx)
		continue;
	    hVz_all->Fill(pVtx->position().z());
	}
	//----- Access true vertex correspnding to triggerd event -----
	// By defaut, the vertex corresponding to index 0 is the true primary vertex, i.e. vertex that has most tracks associated and assumed to cause the trigger. If you come up with other true vertex selection algorithm that does a better job, then you can set the index of the principal vertex as
	// mDst->setVertexIndex(new_index);
	// Get the current true vertex index using:  mDst->currentVertexIndex()
	//Note: that mDst->primaryTracks() return collection of tracks corresponding to true primary vertex. So if true primary vertex index changes, the StMuDst::numberOfPromaryTracks and StMuDst::primaryTracks() will change as well.
	//Check FemtoDstMaker from Breit-Wheeler paper as an example
	// mDst->setVertexIndex(3);
	// Int_t vtxIndex = mDst->currentVertexIndex();
	// cout << "Index no: "<<vtxIndex <<endl;

	pVtx = mDst->primaryVertex();
	if(pVtx)
	{
	    hVz->Fill(pVtx->position().z());
	    hVxy->Fill(pVtx->position().x(), pVtx->position().y());
	    hVr->Fill(sqrt(pow(pVtx->position().x(), 2) + pow( pVtx->position().y(), 2)));
	}
	//------------ Access tracks ------------------
	for(Int_t i = 0; i < mDst->numberOfPrimaryTracks(); ++i)
	{
	    track = mDst->primaryTracks(i);
	    
	    hTrkPt->Fill(track->pt());
	    hMom->Fill(track->p().mag());
	    hPhi->Fill(track->phi()*TMath::RadToDeg());
	    hEta->Fill(track->eta());
	    hNhitsPoss->Fill(track->nHitsPoss());
	    hNhitsFit->Fill(track->nHitsFit());
	    hNhitsDedx->Fill(track->nHitsDedx());
	    hDcaGlobal->Fill(track->dcaGlobal().mag());

	    hDedxVsQp->Fill(track->p().mag()*track->charge(), track->dEdx()*1.0e6);  //1.0e6 to convert GeV to keV
	    //Should check first if there is BTOF respnse for the track in real analysis.
	    btofPidTraits = track->btofPidTraits();
	    beta = btofPidTraits.beta();
	    if(beta == -999)
		continue;
	    mass2 = track->p().mag2()*(1 - pow(beta, 2)) / pow(beta, 2);
	    hm2VsQp->Fill(track->p().mag()*track->charge(), mass2); 
	}

	//--------- Alternative (and may be proper of of accessing tracks when the size is bigger) ------------
	// if(fabs(ev->primaryVertexPosition().z())<25.)
	// {			
	//     TIter itr(tracks);
	//     while( track = (StMuTrack*) itr.Next() )
	//     {
	// 	// Put track cuts and track-wise analysis here
	// 	//cout << "track pt = " << track->pt() << endl;
	// 	hTrkPt->Fill(track->pt());
	//     } 	
	// }

	//----- You can create StTrack from StMuTrack as follows ----------
	//StTrack *st_track = mDst->track(track)
	
	// //---------- Event Summary is a nice collection of track/event info -------------------
	// evSummary = mDst->event()->eventSummary();
	// cout << "\n---------------------------- Event Summary -----------------------------------" <<endl;
	// cout << "Number of Tracks: "<<evSummary.numberOfTracks()<<endl;
	// cout << "Number of Good Tracks: "<<evSummary.numberOfGoodTracks()<<endl;
	// cout << "Number of Good Primary Tracks: "<<evSummary.numberOfGoodPrimaryTracks()<<endl;
	// cout << "Magnetic field: "<<evSummary.magneticField()<<endl;
	// cout << "Mean Pt: "<<evSummary.meanPt()<<endl;
	// cout << "Primary vertex position : " << evSummary.primaryVertexPosition()<<endl;
	// cout << "Number of vertices  : " << evSummary.numberOfVertices()<<endl;
	// cout << "Number of pileup vertices  : " << evSummary.numberOfPileupVertices()<<endl;


	// //--------- Four ways to get principal (vertex associated with highest number of tracks) primary vertex -----------------------
	// cout<<"Primary vertex from muDst (no index): "<< mDst->primaryVertex()->position() <<endl;  
	// cout<<"Primary vertex from muDst (0 index): "<< mDst->primaryVertex(0)->position() <<endl;  //Must use index 0 to get primary vertex associated with most tracks
	// cout<<"Primary vertex from muEvent: "<< mDst->event()->primaryVertexPosition() <<endl;
	//cout<<"Primary vertex from event summary: "<< mDst->event()->eventSummary().primaryVertexPosition() <<endl; // <---------- This is NOT giving the correct value
	

	//---------- In case you nned to create StEvent out of MuDst -----------------
	// For example, you only have access to TrackNodes from StEvent
	// stEvent = mDst->createStEvent();
	// StSPtrVecTrackNode& trackNodes = stEvent->trackNodes();
	// nTracks = trackNodes.size();
	// hNtrkNodes->Fill(nTracks);
	// delete stEvent;
	
	if(iev % 100 == 0)
	    cout << "Events processed: "<<iev <<endl;
    } 
    chain->Finish();

    file->Write();
    file->Close();
    delete chain;
}
