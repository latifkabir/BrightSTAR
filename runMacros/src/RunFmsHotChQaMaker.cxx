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
    //---- Comment out for new fills --------
    // Bool_t isHotCh[4][571] = {0};
    // TStFmsHotChDB *fmsHotChDb = new TStFmsHotChDB();
    // fmsHotChDb->GetHotChList(runNumber, isHotCh);
    // cout << "The following FMS cells are masked:" <<endl;
    // for(int i = 0; i < 4; ++i)
    // {
    // 	for(int j = 0; j < 571; ++j)
    // 	    if(isHotCh[i][j])
    // 		cout << "det "<< (i + 1)<< " ch "<< (j+1) << " hot/bad status:"<< isHotCh[i][j] <<endl;
    // }
    // fmsDb->maskChannels(isHotCh);
    //---------
    
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
