// Filename: TStScheduler.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed May 29 00:26:00 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTSCHEDULER_H
#define TSTSCHEDULER_H

#include <TObject.h>
#include <string>
#include "TString.h"

class TStScheduler: public TObject
{

public:
    static Int_t mJobThreshold;
    static Int_t mSleepTime;
    static Int_t mRunIncrement;
    
    TStScheduler();
    virtual ~TStScheduler();

    static void JobStatus(Int_t level = 0); // Check Job Status
    static void SubmitJob(TString functionName, Int_t firstRun,  Int_t lastRunOrNfiles = -1, TString outName = "", TString jobName = "condor");    // Submit job using condor
    static void SubmitJob(TString functionName, TString inFileName, TString outName = "", TString jobName = "condor");                             // Submit Job using condor
    static void SubmitGenericJob(TString functionWithArg, TString jobName);    // Sumit Job for any function and an argument list
    static void SubmitSumsJob(TString function, TString runList, TString outNamePrefix = "", TString jobName = "sums");                            //Submit job using SUMS
    static void CronJob(TString functionName, Int_t first_run = -1, Int_t last_run = -1);                                                          // Cron job for job-submission for full dataset
    static void ResubmitJobs(TString functionName, Int_t firstRun,  Int_t lastRunOrNfiles, TString existingOutDir, TString existingOutPrefix);
    
    void SetJobThreshold(Int_t threshold){mJobThreshold = threshold;}
    void SetInterval(Int_t interval){mSleepTime = interval;}
    void SetRunIncrement(Int_t increment){mRunIncrement = increment;}
    
    ClassDef(TStScheduler,1)
};
#endif
