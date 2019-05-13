// Filename: ExAnalysis1.C
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Sun May 12 16:19:03 2019 (-0400)
// URL: jlab.org/~latif

/*
This example is kept as a reminder.
A plain tree type access was NOT successful for all leaves. 
You need to use StChain to be consistant.
Other option is to use root MakeClass option to create your own class out of the MuDSt root file.
See ExAnalysis2.cpp for successfull example.
 */

// #include <TH1F.h>
// #include <TTree.h>
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuDstMaker.h"
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuEvent.h"
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuDst.h"

void ExAnalysis1()
{
    // Uncomment these if you are NOT using rootlogon.C to load libraries
    // gROOT->Macro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    // loadSharedLibraries();
    
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", "/star/u/kabir/pwg/data/st_physics_12114059_raw_3010001.MuDst.root", "");
    TTree * stTree = muDstMaker->tree();   // Get the TTree, Use mDstMaker->chain() if a file list is used.
    //stTree->Print();
    StMuDst *mDst = muDstMaker->muDst();   // Get StMuDst
    // mDst->Print();                      // You need to call this makter StChain::Make() call, otherwise it will crash.
    StMuEvent *mEvent;                     // Pointer to loaded StMuEvent
    TH1F *hist = new TH1F("h1", "h1", 200, 0, 0);

    for (Int_t evt = 0; evt < stTree->GetEntries(); ++evt)
    {
	stTree->GetEntry(evt);
	mEvent = mDst->event();

	if(mEvent == NULL)
	    continue;
	cout<<"Event no. : "<<mEvent->eventNumber()<<endl;	
	hist->Fill(mEvent->primaryVertexPosition().z());  // <-------- This will crash unless you use StChain and call Clear() for every event.	
    }
    hist->Draw();    
}

// A Good run17 run: 18138007
