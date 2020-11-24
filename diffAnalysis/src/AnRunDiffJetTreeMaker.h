// Filename: AnRunDiffJetTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Apr 10 22:08:43 2020 (-0400)
// URL: jlab.org/~latif

#include "TString.h"

void AnRunDiffJetTreeMaker(TString inFile = "./inFiles_fms.list", TString outFile = "jets.root", TString det = "fms", Bool_t isMC = kFALSE);
