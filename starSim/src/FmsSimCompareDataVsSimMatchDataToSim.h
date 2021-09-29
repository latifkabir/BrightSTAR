// Filename: FmsSimCompareDataVsSimMatchDataToSim.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Sep 14 14:01:27 2021 (-0400)
// URL: jlab.org/~latif

#include "TString.h"
void FmsSimCompareDataVsSimMatchDataToSim(TString simFile = "dst/fmsSimData/fmsJet/pass7/FmsJet*.root", TString dataFile = "dst/emJet/run15/pass5/fms/nanoDst/NanoJetTree_EjRunEmJetTreeMaker_1608002*.root", TString trigger = "jp0");
//Trigger can be either jp0, jp1, sm-bs1, lg-bs1
