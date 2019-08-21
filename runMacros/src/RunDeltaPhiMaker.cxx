//anaDeltaPhiMaker

#include "StRootInclude.h"
#include "BrTemplateMaker/DeltaPhiMaker.h"

void RunDeltaPhiMaker(TString inFile, TString outFile, int nEvents)
{    
    StChain *chain = new StChain;
    //chain->SetDebug(0);

    StMuDebug::setLevel(0);
    StMuDbReader* dbReader = StMuDbReader::instance();
    dbReader->addDb("fileLists/ProductionMinBias.db");

    StMuDstMaker *muDstMaker = new StMuDstMaker(0,0,"",inFile,".",200);

    DeltaPhiMaker *anaMaker = new DeltaPhiMaker(muDstMaker);
    anaMaker->SetRefMult(14,1000);
    anaMaker->SetFileName(outFile);

    chain->Init();
    chain->EventLoop(1,nEvents);
    chain->Finish();

    delete chain;
}
