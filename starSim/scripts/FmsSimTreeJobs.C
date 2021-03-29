// Filename: FmsSimTreeJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Sep 11 07:20:13 2020 (-0400)
// URL: jlab.org/~latif

void FmsSimTreeJobs(TString filePrefix, Int_t first, Int_t last)
{
    TString inFile;
    TString outFile;
    vector <string> jobs;

    for(Int_t i = first; i <= last; ++i)
    {
	inFile = filePrefix;
	inFile += i;
	//inFile += "_evt5000.MuDst.root";
	inFile += "_evt1000.MuDst.root";

	outFile = "FmsSimTreeTriggered_";
	outFile += i;
	outFile += ".root";
	
	if(gSystem->AccessPathName(inFile))
	{
	    cout << "infile NOT found. Skipped ...:"<< inFile <<endl;
	    continue;
	}
	string job;
	job = "FmsSimTreeMaker(\"" + inFile + "\",\"" + outFile + "\")";
	cout << job <<endl;
	jobs.push_back(job);
    }
    
    TStScheduler::SubmitJob(jobs, "FmsSimTree");
}
