// Filename: SubmitJob.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Aug 10 17:19:34 2019 (-0400)
// URL: jlab.org/~latif
#include "RootInclude.h"

void SubmitJob(TString function, TString runList, TString outName)    
{
    TString command = (TString)".! " + (TString)"SubmitJob.sh " + function + (TString)"\t" + runList + (TString)"\t" + outName;
    gROOT->ProcessLine(command);
}
