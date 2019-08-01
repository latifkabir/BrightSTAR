// Filename: RunEEmcEvtDispMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jun 27 17:20:16 2019 (-0400)
// URL: jlab.org/~latif

#include "TString.h"
void RunEEmcEvtDispMaker(TString InputFileList="", TString outputfile= "EEmcDistMaker.root", Int_t eventNo = -1, Int_t maxEvents = -1);

// Use eventNo = -1 for displaying all events, otherwise set desired event number
// use maxEvents = -1 for displaying all events in the Tree, otherwise set max event number to desired value.
