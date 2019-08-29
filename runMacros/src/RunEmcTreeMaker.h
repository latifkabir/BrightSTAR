// Filename: RunEmcTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 26 18:09:29 2019 (-0400)
// URL: jlab.org/~latif

void RunEmcTreeMaker(
    TString infile = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/072/16072047/st_physics_16072047_raw_1000014.MuDst.root",
    TString outFile = "EmcTree.root",
    const Int_t n_events = -1);
