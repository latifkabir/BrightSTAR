// Filename: RunPidTreeMaker.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Sep 26 15:40:09 2019 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "StRootInclude.h"
#include "BrPidMaker/TStPidTreeMaker.h"

void RunPidTreeMaker(TString infile, TString outFile, Int_t n_events)
{	
    StChain *chain = new StChain;
    StMuDstMaker *mudst_mk = new StMuDstMaker(0, 0, "", infile, "", 1000);

    St_db_Maker *db1 = new St_db_Maker("db","$HOME/StarDb","MySQL:StarDb","$STAR/StarDb");
    chain->SetDEBUG(0);
    
    TStPidTreeMaker *pidTreeMaker = new TStPidTreeMaker();
    pidTreeMaker->SetOutName(outFile);
    pidTreeMaker->FillHist(kTRUE);        
    chain->Init();

    StMuDst *muDst = mudst_mk->muDst();
    Int_t nEvents =  mudst_mk->chain()->GetEntries();
    if(n_events != -1 && n_events < nEvents)
	nEvents = n_events;
    cout << "Total events to be processed: " << nEvents << endl;
    for (Int_t i = 0;  i < nEvents; ++i)
    {
	chain->Make(i);

	if(i %1000 ==0)
	    cout << "Events processed: "<< i <<endl;	   
	chain->Clear();
    }
    chain->Finish();
    
    delete chain;
}

