// Filename: RunFmsQAMaker.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Aug 20 14:14:39 2019 (-0400)
// URL: jlab.org/~latif

#include "RunFmsHotChQaMaker.h"
#include "StRootInclude.h"
#include "BrFmsMaker/TStFmsHotChQaMaker.h"
#include "BrightStInclude.h"
#include "RootInclude.h"

void RunFmsHotChQaMaker(TString inFile, TString outName, Int_t nEvents)
{
    gMessMgr->SetLimit("I", 0);
    gMessMgr->SetLimit("Q", 0);
    gMessMgr->SetLimit("W", 0);
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", inFile, "", 1000);
    St_db_Maker* dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
    StFmsDbMaker* fmsDb = new StFmsDbMaker("fmsDb");
    
    muDstMaker->SetStatus("*", 0);
    muDstMaker->SetStatus("Fms*", 1);
    muDstMaker->SetStatus("MuEvent*", 1);
    fmsDb->SetAttr("fmsGainCorr","fmsGainCorr-BNL-C");
    Int_t runNumber = TStRunList::GetRunFromFileName((string)inFile);
    StTriggerFilterMaker* filterMaker = new StTriggerFilterMaker;
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "BHT1*VPDMB-30"));
    filterMaker->addVetoTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-LED"));
    
    StEventMaker* eventMk = new StEventMaker();
    StFmsHitMaker*   fmsHitMk   = new StFmsHitMaker();
    //StFmsPointMaker* fmsPointMk = new StFmsPointMaker();

    fmsHitMk->SetReadMuDst(0);   // (0)
    //fmsPointMk->SetReadMuDst(0); // (0)
        
    TStFmsHotChQaMaker *fmsQA = new TStFmsHotChQaMaker("FmsHotChQaMaker");
    fmsQA->SetOutName(outName);

    Int_t totEntries = muDstMaker->chain()->GetEntries();
    Int_t events = (nEvents == -1)? totEntries : nEvents;
    
    chain->Init();    
    chain->EventLoop(events);  // Run specified number of events
    chain->Finish();
}
