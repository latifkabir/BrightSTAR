// Filename: FmsSimTreeJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Sep 11 07:20:13 2020 (-0400)
// URL: jlab.org/~latif

void FmsSimTreeJobs()
{
    TString inFile;
    TString outFile;

    for(Int_t i = 1500; i < 2500; ++i)
    {
	inFile = "/star/u/kabir/GIT/BrightSTAR/scratch/condor/FmsSim_Run15_";
	inFile += i;
	inFile += "_evt5000.MuDst.root";

	outFile = "FmsSimTreeTriggered_";
	outFile += i;
	outFile += ".root";
	
	if(gSystem->AccessPathName(inFile))
	{
	    cout << "infile NOT found. Skipped ...:"<< inFile <<endl;
	    continue;
	}
	TStScheduler::SubmitJob("FmsSimTreeMaker", inFile, outFile);
	//break;	
    }
    
}
