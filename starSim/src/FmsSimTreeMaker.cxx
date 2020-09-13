#include <iostream>
#include "TString.h"
#include "StRootIncludeFmsJet.h"

#include "StIOMaker/StIOMaker.h"
#include "StMcEventMaker/StMcEventMaker.h"
#include "StJetMaker/mcparticles/StjMCParticleCutEta.h"
#include "StSpinPool/StFmsTriggerMaker/StFmsTriggerMaker.h"
#include "StJetMaker/StJetMaker2015.h"
#include "StJetMaker/towers/StjTowerEnergyCutFMS.h"

#include "BrFmsSimTreeMaker/TStFmsSimTreeMaker.h"


using namespace std;

void FmsSimTreeMaker(TString inMuDstFile, TString outName, Int_t nEvents)
{
    TString orgName = inMuDstFile;
    TString pythiaFile = orgName.ReplaceAll("MuDst", "pythia");

    cout<<"Input MuDst file is: "<< inMuDstFile <<endl;
    cout<<"Input pythia file is: "<< pythiaFile <<endl;    
    cout<<" Out Tree file is: "<< outName <<endl;
     
    StChain *chain = new StChain;
    StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"", inMuDstFile,"",1000);

    StMuDbReader* muDstDb = StMuDbReader::instance();

    St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");
    starDb->SetDateTime(20150321,121503);
    StFmsDbMaker* fmsdb = new StFmsDbMaker("fmsDb");
    fmsdb->SetAttr("fmsGainCorr","fmsGainCorr-BNL-C");
    StEEmcDbMaker* eemcDb = new StEEmcDbMaker;
    StSpinDbMaker* spinDb = new StSpinDbMaker;
     
    StFmsHitMaker* fmshitMk = new StFmsHitMaker();
    // fmshitMk->SetTimeDepCorr(0);
    // fmshitMk->SetTowerRej(1);
    //   fmshitMk->SetReadMuDst(1); ///mmm working
    fmshitMk->SetReadMuDst(1); ///mmm

    StFmsPointMaker *fmspt =  new StFmsPointMaker("StFmsPointMaker");
    //fmspt->setMergeSmallToLarge(1); //                                                                                  
    //   fmspt->SetReadMuDst(0);  // mmm working
    fmspt->SetReadMuDst(1);  // mmm working,


    //-------------------------- FMS Trigger Simulator ----------------------------
    StTriggerSimuMaker* simuTrig = new StTriggerSimuMaker;
    simuTrig->setMC(1); // Must be before individual detectors, to be passed    
    //   simuTrig->useOnlineDB();
    //simuTrig->InitRun(11098017);
    simuTrig->InitRun(16080020);
    simuTrig->useOfflineDB();

    // FMS trigger simulator     
    StFmsTriggerMaker* fmstrig = new StFmsTriggerMaker;
    fmstrig->useMuDst();
    fmstrig->forceRunNumber(16080020);
    //  fmstrig->InitRun(11098017);    
    fmstrig->useQTSim();  // was there
    //fmstrig->useDsmData(); //Does not work

    //for pythia event 
    // St_pythia_Maker* pythia = new St_pythia_Maker;
    // pythia->SetFile(pythiaFile);

    TStFmsSimTreeMaker *fmsSimTreeMaker = new TStFmsSimTreeMaker();
    fmsSimTreeMaker->SetOutFileName(outName);
    fmsSimTreeMaker->SetPythiaFile(pythiaFile);
    
    chain->Init();
    if(nEvents == -1)
	chain->EventLoop();
    else
	chain->EventLoop(nEvents);   
    chain->Finish();
  
    cout<<"SUCCESS!!"<<endl;
    delete chain;  
}
