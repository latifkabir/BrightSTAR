// Filename: SubmitJob.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Aug 10 17:23:44 2019 (-0400)
// URL: jlab.org/~latif

#include "TString.h"

void JobStatus();
void SubmitJob(TString functionName, Int_t firstRun,  Int_t lastRunOrNfiles = -1, Int_t nFilesPerProcess = 1, TString outName = "", TString jobName = "condor");    
void SubmitJob(TString functionName, TString inFileName, TString outName = "", TString jobName = "condor");    
void SubmitSumsJob(TString function, TString runList, TString outNamePrefix = "", TString jobName = "sums");

