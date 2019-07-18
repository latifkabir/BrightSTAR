// Filename: TStRun.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed May 29 00:24:18 2019 (-0400)
// URL: jlab.org/~latif

#include "TStRun.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "TStar.h"
#include "TStRunList.h"

using namespace std;

ClassImp(TStRun)

TStRun::TStRun(TString fileList)
{
    fFileList = fileList;
    Init();
}

TStRun::TStRun(Int_t firstRun, Int_t lastRunOrNfiles)
{
    TStRunList::MakeFileList(firstRun, lastRunOrNfiles);
    fFileList = TStar::Config->GetFileList();
    Init();
}
void TStRun::Init()
{
    TStar::ExitIfInvalid(fFileList);
    muDstMaker = new StMuDstMaker(0, 0, "", fFileList, "", 1000);
}
TStRun::~TStRun()
{
    delete muDstMaker;    
}

TTree* TStRun::GetTree()
{
    return muDstMaker->tree();
}

TChain* TStRun::GetChain()
{
    return muDstMaker->chain();
}
