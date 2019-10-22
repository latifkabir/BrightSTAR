// Filename: JobPrototype.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Oct 21 22:46:55 2019 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "BrightStInclude.h"

void JobPrototype(TString inFile, TString outFile)
{
    TStar::Config->Print();
    TFile *f = new TFile(outFile, "RECREATE");

    f->Close();
}
