// Filename: RunFmsQAMaker.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Aug 20 14:14:39 2019 (-0400)
// URL: jlab.org/~latif

void RunFmsQAMaker()
{
    TStRunList::MakeFileList(16072057, 1);
    TString fileList = TStar::Config->GetFileList();
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);
    St_db_Maker* dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
    StFmsDbMaker* fmsDb = new StFmsDbMaker("fmsDb");
    
    muDstMaker->SetStatus("*", 0);
    muDstMaker->SetStatus("Fms*", 1);
    muDstMaker->SetStatus("MuEvent*", 1);
    
    TStFmsQAMaker *fmsQA = new TStFmsQAMaker("FmsQAMaker");
    
    chain->Init();    
    chain->EventLoop(1000);  // Run specified number of events
    chain->Finish();
}
