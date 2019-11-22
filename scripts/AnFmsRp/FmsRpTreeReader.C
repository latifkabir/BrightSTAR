// Filename: FmsRpTreeReader.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 20 15:10:15 2019 (-0500)
// URL: jlab.org/~latif

void FmsRpTreeReader()
{
    TFile *f = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/FmsRpTreeMaker/RunFmsRpTreeMaker_16079017.root");
    TTree *t = (TTree*)f->Get("T");
    t->Print();
    t->MakeClass("AnFmsRpTreeReader");    
}
