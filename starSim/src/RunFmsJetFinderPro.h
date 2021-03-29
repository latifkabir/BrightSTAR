// Filename: RunFmsJetFinderPro.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Aug 14 23:30:44 2020 (-0400)
// URL: jlab.org/~latif

#include "TString.h"
void RunFmsJetFinderPro(TString inMuDstFile, TString outJetName, Int_t nEvents = -1);
void RunFmsJetFinderPro(Int_t cycle, Int_t nEntries);

