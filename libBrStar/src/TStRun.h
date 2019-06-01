// Filename: TStRun.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed May 29 00:26:00 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTRUN_H
#define TSTRUN_H

#include <TTree.h>
#include <TChain.h>
class StMuDstMaker;

class TStRun
{
    StMuDstMaker *muDstMaker;
    TString fFileList;
public:
    
    TStRun(TString runList);
    TStRun(Int_t firstRun, Int_t lastRunOrNfiles = -1);
    virtual ~TStRun();
    void Init();
    TTree *GetTree();
    TChain *GetChain();
    
    ClassDef(TStRun,1)
};
#endif
