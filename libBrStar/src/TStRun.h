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
public:
    
    TStRun(TString runList);
    virtual ~TStRun();
    void Init();
    TTree *GetTree();
    TChain *GetChain();
    
    ClassDef(TStRun,0)
};
#endif
