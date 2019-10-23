// Filename: RunFmsQAMaker.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Aug 20 14:14:39 2019 (-0400)
// URL: jlab.org/~latif

#include "RunFmsQAMaker.h"
#include "StRootInclude.h"
#include "BrFmsMaker/TStFmsQAMaker.h"
#include "BrightStInclude.h"
#include "RootInclude.h"

void RunFmsQAMaker(Int_t runNumber, TString outName, Int_t nFiles, Int_t nEvents)
{
    TStRunList::MakeFileList(runNumber, nFiles);
    TString fileList = TStar::Config->GetFileList();
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);
    St_db_Maker* dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
    StFmsDbMaker* fmsDb = new StFmsDbMaker("fmsDb");
    
    muDstMaker->SetStatus("*", 0);
    muDstMaker->SetStatus("Fms*", 1);
    muDstMaker->SetStatus("MuEvent*", 1);
    
    TStFmsQAMaker *fmsQA = new TStFmsQAMaker("FmsQAMaker");
    fmsQA->SetOutName(outName);

    Int_t totEntries = muDstMaker->chain()->GetEntries();
    Int_t events = (nEvents == -1)? totEntries : nEvents;
    
    chain->Init();    
    chain->EventLoop(events);  // Run specified number of events
    chain->Finish();
}
