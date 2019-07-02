// Filename: RunRpDistMaker.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Jun 22 01:35:03 2019 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "BrRpMaker/TStRpDistMaker.h"
#include "TChain.h"
#include "RunRpDistMaker.h"

void RunRpDistMaker(TString inFile, TString outFile)
{
    if(inFile == "")
	inFile = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/071/16071046/st_physics_16071046_raw_1500013.MuDst.root";
	
    StChain *chain = new StChain;
    //chain->SetDebug(0);
    StMuDstMaker *muDstMaker = new StMuDstMaker(0,0,"", inFile, ".", 200);
    TChain *ch = muDstMaker->chain();
    Int_t nEvents = ch->GetEntries();
    
    TStRpDistMaker *RpMaker = new TStRpDistMaker(muDstMaker);
    RpMaker->SetFileName(outFile);

    chain->Init();
    chain->EventLoop(1,nEvents);
    chain->Finish();

    delete chain;
}
