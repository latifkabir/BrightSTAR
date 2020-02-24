// Filename: AnRunAnTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:25:43 2019 (-0500)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"
#include "BrNanoDstMaker/TStRpFilterMaker.h"
#include "BrNanoDstMaker/TStNanoDstMaker.h"

#include <vector>
#include <iostream>
using namespace std;

void AnRunAnTreeMaker(TString fileList, TString outFile)
{
    // TFile *f = new TFile(outFile, "RECREATE");
    TH1D* hEvtCount = new TH1D("hEvtCount", "Event counts", 20, 0, 20);
    // TTree *tree = new TTree("T", "An TREE");
    
    vector<Int_t> trigs(9);
 
    // gMessMgr->SetLimit("I", 0);   //Disable StInfo messages including Skipped event message
    // gMessMgr->SetLimit("Q", 0);   //Disable StQAInfo messages (includes event processing status)
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);
    St_db_Maker* dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
    StSpinDbMaker* spindb = new StSpinDbMaker("spinDb");
    StFmsDbMaker* fmsDb = new StFmsDbMaker("fmsDb");

    //Enable new FMS calibration
    fmsDb->SetAttr("fmsGainCorr","fmsGainCorr-BNL-C");
    
    muDstMaker->SetStatus("*", 0);
    muDstMaker->SetStatus("Fms*", 1);
    muDstMaker->SetStatus("pp2pp*", 1);
    muDstMaker->SetStatus("MuEvent*", 1);
    muDstMaker->SetStatus("*Track*", 1);

    //-------------- Filter/Skip Events if no RP or FMS BS/JP Trigger----------
    // TStRpFilterMaker* filterMaker = new TStRpFilterMaker("TStRpFilterMaker");
    // for(Int_t i = 0; i < trigs.size(); ++i)
    // 	filterMaker->addTrigger(trigs[i]);
    // filterMaker->SetHist1d(hEvtCount);
    //----------------
    
    StEventMaker* eventMk = new StEventMaker();
    StFmsHitMaker*   fmsHitMk   = new StFmsHitMaker();
    StFmsPointMaker* fmsPointMk = new StFmsPointMaker();
    // FPS
    StFmsFpsMaker * fpsMk = new StFmsFpsMaker();
    
    TStNanoDstMaker *AnTreeMk = new TStNanoDstMaker("TStNanoDstMaker");
    //AnTreeMk->SetTree(tree);
    AnTreeMk->SetOutFileName(outFile);

    // mudst reading
    // if 0, get info from StTriggerData from StTriggerDataMaker/StEvent/MuDst
    // and apply new DB; cluster finding/fitting is redone
    // if 1, cluster finding/fitting is NOT redone (i.e., mudst result is read
    // out); however, some recalculations (like point positions) from updated DB values
    // are done
    fmsPointMk->SetReadMuDst(0); // (0)
    fmsHitMk->SetReadMuDst(0);   // (0)
    fpsMk->setReadMuDST(1);      // (1) // (reverse 0<->1 convention as FmsHitMaker and FmsPointMaker)

    // shower shape
    // 0='use lednev params', 1='use zhanwen params', 2='use yuxi params'
    // fmsPointMk->setShowerShapeWithAngle(1); // (1)
  
    // cluster merging over large/small boundary
    // if 1, merges large and small cell clusters (experimental!; default 1 in StFmsPointMaker)
    fmsPointMk->setMergeSmallToLarge(1); // (1)

    // vertex correction
    // if 0, no vertex correction; if 1, use MuDst BBC vertex based on run11 calibration (needs update?)
    fmsPointMk->setVertexZ(1); // (1)

    Int_t nEvents = muDstMaker->chain()->GetEntries();
    cout << "----------->Total Events to be processed: "<< nEvents <<" <----------------"<<endl;
    
    chain->Init();    
    chain->EventLoop(nEvents);  // Run specified number of events
    //chain->EventLoop(1000);  // Run specified number of events
    chain->Finish();

    // f->Write();
    // f->Close();
    cout << "Status:SUCCESS!!" <<endl;

    delete chain;
}
