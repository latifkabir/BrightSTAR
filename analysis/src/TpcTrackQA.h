// Filename: TpcTrackQA.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep 15 12:48:39 2019 (-0400)
// URL: jlab.org/~latif

#include "TString.h"

void TpcTrackQA(Int_t runNumber, Int_t nFiles = 1);
void TpcTrackQA(TString fileList, TString outFile = "TpcQA.root");
