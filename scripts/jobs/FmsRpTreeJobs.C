// Filename: FmsRpTreeJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep  8 16:58:40 2019 (-0400)
// URL: jlab.org/~latif

#include<unistd.h> 
void FmsRpTreeJobs(TString functionName)
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
    
    
    cout << "First run: "<< firstRun <<endl;
    cout << "Last run: "<< lastRun <<endl;

    while(endRun < lastRun || index_e < (totRuns -1))
    {	
	//Read number of active jobs
	TString sh_path = "/star/u/kabir/GIT/BrightSTAR/bin/activeJobs.sh";
	TString command_sh = ".! /star/u/kabir/GIT/BrightSTAR/bin/activeJobs.sh";
	if(gSystem->AccessPathName(sh_path))
	{
	    cout << "shell script NOT found at: "<<sh_path<<endl;
	    return;
	}
	gROOT->ProcessLine(command_sh);
	ifstream fileName("/star/u/kabir/GIT/BrightSTAR//resources/temp/ActiveJobs.txt");
	if(!fileName)
	{
	    cout << "Unable to open active job number record"<<endl;
	    return;
	}
	fileName >> activeJobs;
	fileName.close();
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
	    cout << "Submitting jobs for run range: "<< startRun << " to "<< endRun <<endl;
	    //echo "This is a Test" | mail -s "New Job Submission" kabir@rcf.rhic.bnl.gov
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
