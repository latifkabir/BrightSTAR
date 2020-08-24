// Filename: FmsSimReadSimData.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Jul 31 05:58:43 2020 (-0400)
// URL: jlab.org/~latif

#include "TString.h"

void ReadPythiaEvent(TString fileList = "/star/u/kabir/GIT/BrightSTAR/scratch/condor/FmsSim_Run15_2669_evt500.MuDst.root"); // Use wildcat to combine files
void ReadMuDstEvent(TString fileList = "/star/u/kabir/GIT/BrightSTAR/scratch/condor/FmsSim_Run15_2669_evt500.MuDst.root"); //Use wildcat to combine files
void FmsSimReadSimData(TString fileList =  "/star/u/kabir/GIT/BrightSTAR/scratch/condor/FmsSim_Run15_266*_evt500.MuDst.root");
