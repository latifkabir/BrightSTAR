// Filename: TStRun.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed May 29 00:24:18 2019 (-0400)
// URL: jlab.org/~latif

#include "TStRun.h"
#include "StMuDstMaker.h"

using namespace std;

ClassImp(TStRun)

TStRun::TStRun(TString fileList)
{
    muDstMaker = new StMuDstMaker(0, 0, "", fileList, "");
}

TStRun::~TStRun()
{
    
}

TTree* TStRun::GetTree()
{
    return muDstMaker->tree();
}

TChain* TStRun::GetChain()
{
    return muDstMaker->chain();
}
