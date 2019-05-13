// Filename: ExAnalysis2.C
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Sun May 12 16:19:03 2019 (-0400)
// URL: jlab.org/~latif

#include <TH1F.h>
#include <TTree.h>
#include <TObjArray.h>
#include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StChain/StChain.h"
#include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuEvent.h"
#include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuDst.h"
#include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuTrack.h"

void ExAnalysis2()
{
    // Uncomment these if you are NOT using rootlogon.C to load libraries
    // gROOT->Macro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    // loadSharedLibraries();
    
    //---------------- Instantiate chain and MuDst reader ---------------//
    StChain *chain = new StChain;
    StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"","/star/u/kabir/pwg/data/st_physics_12114059_raw_3010001.MuDst.root","");
    StMuDst *mDst = muDstMaker->muDst();   // Get StMuDst
    StMuEvent *ev = 0;
    StMuTrack *track = 0;
    TObjArray *tracks = 0;
    TH1F *hist = new TH1F("h1", "h1", 200, 0, 0);
    
    //-------------------- The STAR chain Event loop -------------------//
    chain->Init();
    int nEvents = muDstMaker->tree()->GetEntries();
	
    for (int iev = 0; iev < nEvents; iev++)
    {
	chain->Clear();
	int iret = chain->Make(iev); 

	ev = mDst->event();
	tracks = mDst->primaryTracks();

	//---- Apply event cuts ----//
	if(fabs(ev->primaryVertexPosition().z())<25.)
	{			
	    TIter itr(tracks);
	    while( track = (StMuTrack*) itr.Next() )
	    {
		// Put track cuts and track-wise analysis here
		//cout << "track pt = " << track->pt() << endl;
		hist->Fill(track->pt());
	    } 	
	}
    } 
    chain->Finish();
    delete chain;
    hist->Draw();
}
