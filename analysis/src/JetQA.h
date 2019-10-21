// Filename: JetQA.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Oct 11 16:30:23 2019 (-0400)
// URL: jlab.org/~latif



void JetQA(
            Int_t nentries = -1,
	    TString jetfile  = "/star/u/tinglin/gpsf01/Run15/2015pp200JetTree/20190703/16077041/st_physics_*.jets.root",
	    TString skimfile = "/star/u/tinglin/gpsf01/Run15/2015pp200JetTree/20190703/16077041/st_physics_*.skim.root",
	    TString outfile = "test_jetQADatajet500Gev10103041_AntiKtR060.root"
    );
