// Filename: RunNanoDstMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Nov 18 01:15:35 2019 (-0500)
// URL: jlab.org/~latif

#include "TString.h"
#include <vector>


void RunNanoDstMaker(TString fileList = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/072/16072047/st_physics_16072047_raw_1000014.MuDst.root", TString outFile = "NanoDst.root" , Bool_t showMsg = false, vector <Int_t> *evtTrig = NULL,  vector <Int_t> *vetoTrig = NULL, Int_t maxEvents = -1);
