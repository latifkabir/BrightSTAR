// Filename: ExAnalysis3.C
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Sun May 12 16:19:03 2019 (-0400)
// URL: jlab.org/~latif

// #include <TH1D.h>
// #include <TTree.h>
// #include <TObjArray.h>
// #include "StChain/StChain.h"
// #include "StMuDSTMaker/COMMON/StMuDstMaker.h"
// #include "StMuDSTMaker/COMMON/StMuEvent.h"
// #include "StMuDSTMaker/COMMON/StMuDst.h"
// #include "StMuDSTMaker/COMMON/StMuTrack.h"

void TpcTrackQA(Int_t runNumber)
{
    TStRunList::MakeFileList(runNumber, 1);
    TString fileList = TStar::Config->GetFileList(); 
    StChain *chain = new StChain;
    StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);       

    muDstMaker->SetStatus("*", 0);
    muDstMaker->SetStatus("MuEvent", 1);
    muDstMaker->SetStatus("*Track*", 1);
    muDstMaker->SetStatus("*Vert*", 1);
    
    StMuDst *mDst = muDstMaker->muDst();  
    StMuEvent *ev = 0;
    StMuTrack *track = 0;
    TObjArray *tracks = 0;
    StEvent *stEvent;
    StMuPrimaryVertex pVx;
    StEventSummary evSummary;
    TH1D *hist1 = new TH1D("h1", "Number of Global Tracks Per Event", 200, 0, 0);
    TH1D *hist2 = new TH1D("h2", "Number of Primary Tracks Per Event", 200, 0, 0);
    TH1D *hist3 = new TH1D("h3", "Number of Primary Vertices Per Event", 200, 0, 0);
    TH1D *hist4 = new TH1D("h4", "Track Pt", 200, 0, 0);
    TH1D *hist5 = new TH1D("h5", "Number of Track Nodes Per Event", 200, 0, 0);
    TH1D *hist6 = new TH1D("h6", "Vertex z", 200, 0, 0);
    
    chain->Init();
    int nEvents = 1000;//muDstMaker->chain()->GetEntries();
    Int_t nTracks;
    
    cout << "Total events to be processed: "<< nEvents <<endl;
	
    for (int iev = 0; iev < nEvents; iev++)
    {
	chain->Clear();
	int iret = chain->Make(iev); 

	ev = mDst->event();
	tracks = mDst->primaryTracks();

	hist1->Fill(mDst->numberOfGlobalTracks());
	hist2->Fill(mDst->numberOfPrimaryTracks());
	hist3->Fill(mDst->numberOfPrimaryVertices());
	
	//---------- Access Vertices -----------------
	for(Int_t i = 0; i < mDst->numberOfPrimaryVertices(); ++i)
	{
	    pVx = mDst->primaryVertex(i);
	    //cout << "\nEvent: "<< iev <<" vertex no. "<<i<<" x:"<< pVx.position().x() <<" y: "<<pVx.position().y()<<" z: "<<pVx.position().z() <<endl;
	    hist6->Fill(pVx.position().z());
	}

	//------------ Access tracks ------------------
	if(fabs(ev->primaryVertexPosition().z())<25.)
	{			
	    TIter itr(tracks);
	    while( track = (StMuTrack*) itr.Next() )
	    {
		// Put track cuts and track-wise analysis here
		//cout << "track pt = " << track->pt() << endl;
		hist4->Fill(track->pt());
	    } 	
	}
	
	
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


	// //--------- Three ways to get principal (vertex associated with highest number of tracks) primary vertex -----------------------
	// cout<<"Primary vertex from muDst: "<< mDst->primaryVertex(0)->position() <<endl;  //Must use index 0 to get primary vertex associated with most tracks
	// cout<<"Primary vertex from muEvent: "<< mDst->event()->primaryVertexPosition() <<endl;
	// cout<<"Primary vertex from event summary: "<< mDst->event()->eventSummary().primaryVertexPosition() <<endl;
	

	//---------- In case you nned to create StEvent out of MuDst -----------------
	// For example, you only have access to TrackNodes from StEvent
	// stEvent = mDst->createStEvent();
	// StSPtrVecTrackNode& trackNodes = stEvent->trackNodes();
	// nTracks = trackNodes.size();
	// hist5->Fill(nTracks);
	// delete stEvent;
	

	if(iev % 100 == 0)
	    cout << "Events processed: "<<iev <<endl;
    } 
    chain->Finish();

    TCanvas *c1 = new TCanvas();
    hist1->Draw();
    TCanvas *c2 = new TCanvas();
    hist2->Draw();
    TCanvas *c3 = new TCanvas();
    hist3->Draw();
    TCanvas *c4 = new TCanvas();
    hist4->Draw();
    TCanvas *c5 = new TCanvas();
    hist5->Draw();
    TCanvas *c6 = new TCanvas();
    hist6->Draw();
    
    delete chain;
}
