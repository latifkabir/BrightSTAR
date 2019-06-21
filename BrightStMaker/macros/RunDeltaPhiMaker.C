//anaDeltaPhiMaker

void RunDeltaPhiMaker(TString inFile = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/AuAu19_production/ReversedFullField/P11id/2011/112/12112034/st_physics_12112034_raw_0010001.MuDst.root", TString outFile = "DeltaPhi.root", int nEvents=50000)
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
