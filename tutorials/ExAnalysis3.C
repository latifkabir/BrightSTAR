// Filename: ExAnalysis1.C
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Sun May 12 16:19:03 2019 (-0400)
// URL: jlab.org/~latif


// #include <TH1F.h>
// #include <TTree.h>
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuDstMaker.h"
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuEvent.h"
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuDst.h"

void ExAnalysis3()
{
    // Uncomment these if you are NOT using rootlogon.C to load libraries
    // gROOT->Macro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    // loadSharedLibraries();

    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", "/star/u/kabir/pwg/data/st_physics_12114059_raw_3010001.MuDst.root", "");
    TTree * stTree = muDstMaker->tree();   // Get the TTree, Use mDstMaker->chain() if a file list is used.
    //stTree->Print();
    StMuDst *mDst = muDstMaker->muDst();   // Get StMuDst
    //mDst->Print();                      // You need to call this makter StChain::Make() call, otherwise it will crash.
    StMuEvent *mEvent;                     // Pointer to loaded StMuEvent
    TH1F *hist = new TH1F("h1", "h1", 200, 0, 0);
    chain->Init();
    for (Int_t evt = 0; evt < stTree->GetEntries(); ++evt)
    {
	chain->Clear();
	chain->Make(evt);
	mEvent = mDst->event();

	if(mEvent == NULL)
	    continue;
	cout<<"Event no. : "<<mEvent->eventNumber()<<endl;	
	hist->Fill(mEvent->primaryVertexPosition().z());  // <-------- This will crash unless you use StChain.	
    }
    chain->Finish();
    delete chain;
    hist->Draw();    
}

// A Good run17 run: 18138007
