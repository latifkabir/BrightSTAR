// Filename: PidTree.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Sep 28 22:59:13 2019 (-0400)
// URL: jlab.org/~latif

void PidTree()
{
    TFile *file = new TFile("~/GIT/BrightSTAR/dst/RunPidTreeMaker/RunPidTreeMaker_16072057.root");
    TTree *t = (TTree*)file->Get("T");
    t->MakeClass("PidTreeReader");    
}
