// Filename: FmsRpTreeJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep  8 16:58:40 2019 (-0400)
// URL: jlab.org/~latif

#include <unistd.h>
#include <fstream>
#include <iostream>
#include "TROOT.h"
#include "TEntryList.h"
#include "TStar.h"
#include "TSystem.h"

#include "TStRunList.h"
#include "TStConfig.h"
#include "CronJob.h"
#include "SubmitJob.h"

using namespace std;

void CronJob(TString functionName)
{
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList();
    //runList->Print("all");
    Int_t totRuns = runList->GetN();
    
    Int_t firstRun = runList->GetEntry(0);
    Int_t lastRun = runList->GetEntry(totRuns - 1);
    Int_t index = 0;
    Int_t index_e = 0;
    Int_t startRun = firstRun;
    Int_t endRun;
    Int_t runIncrement = 20;
    Int_t activeJobs = 9999;
    Int_t jobThreshold = 100;
    Int_t iteration = 0;

    TString command_sh = TStar::Config->GetStarHome() + (TString)"/bin/activeJobs.sh"; 
    TString fileName = TStar::Config->GetStarHome() + (TString)"/resources/temp/ActiveJobs.txt";
        
    cout << "First run: "<< firstRun <<endl;
    cout << "Last run: "<< lastRun <<endl;

    while(endRun < lastRun || index_e < (totRuns -1))
    {	
	//Read number of active jobs
	if(gSystem->AccessPathName(command_sh))
	{
	    cout << "shell script NOT found at: "<<command_sh<<endl;
	    return;
	}
	gROOT->ProcessLine((TString)".! " + command_sh);
	ifstream inFile(fileName);
	if(!inFile)
	{
	    cout << "Unable to open active job number record"<<endl;
	    return;
	}
	inFile >> activeJobs;
	inFile.close();
	cout << "Currently number of active jobs: "<< activeJobs <<endl;

	if(activeJobs < jobThreshold)
	{
	    startRun = runList->GetEntry(index);
	    index_e = (index + runIncrement >= totRuns) ? (totRuns - 1) :  (index + runIncrement);
	    endRun = runList->GetEntry(index_e);
	    if(endRun == -1)
		endRun = lastRun;      
	    if(startRun == -1)
		break;
	    iteration = (index_e / runIncrement);
	    TString jobName = functionName + to_string(iteration);
	    cout << "Submitting jobs for run range: "<< startRun << " to "<< endRun <<endl;
	    TString emailMessage = (TString)"Submitted jobs:: functionName: " + functionName + (TString)" Start Rrun: " + to_string(startRun) + (TString)" End Run: " + to_string(endRun) + (TString)" Iteration: " + to_string(index_e / runIncrement);
	    TString emailCommand = (TString)".! echo \"" + emailMessage + (TString)"\" |  mail -s \"New Job Submission\" kabir@rcf.rhic.bnl.gov";
	    gROOT->ProcessLine(emailCommand);
	                           	    
	    //SubmitJob(functionName, startRun, endRun, "", jobName);
	    SubmitJob(functionName, startRun, endRun);
	    index += (runIncrement + 1);
	    if(index >= totRuns)
		index = (totRuns - 1);
	}
	else
	    cout << "Still substantial number of active jobs ..." <<endl;

	cout << "Now sleeping for a while .... ..." <<endl;
	sleep(1800);	
    }
}
