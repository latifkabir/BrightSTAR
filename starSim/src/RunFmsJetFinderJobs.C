// Filename: RunFmsJetFinderJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Jan 15 18:47:02 2020 (-0500)
// URL: jlab.org/~latif

// Make sure to change 3 places nummbered below

// #include <iostream>
// #include "RootInclude.h"
// #include "BrightStInclude.h"

// using namespace std;

void RunFmsJetFinderJobs(TString inFilePrefix, Int_t maxNfiles, Int_t firstNumber = 0)
{
    gSystem->Load("libBrStar.so");
    //------- 1.Change Here Function Name ------------
    TString funcName = "RunFmsJetFinderPro";
    TString jobName = "FmsJetSim";
    //--------------------------------------
    //TStScheduler::mCopyToExeHost = 0;   
    cout << "Total number of jobs to be processed: "<< maxNfiles <<endl;

    //------------ Loop for jobs --------------------------
    for (Int_t i = firstNumber; i < maxNfiles; ++i)
    {
	TString argList;
	TString currentJobName;
	TString muDstName = inFilePrefix + Form("%i_", i) + "evt5000.MuDst.root";
	TString outName = Form("FmsJet_Run15_%i_evt5000.root", i);
	if(gSystem->AccessPathName(muDstName))
	{
	    cout << "File not found: "<< muDstName <<" ...... SKIPPED"<<endl;
	    continue;
	}
	
	currentJobName = jobName;
	currentJobName += i;
	
	argList = "(";
	argList += "\"";
	argList += muDstName;
	argList += "\"";
	argList += ",";
	argList += "\"";
	argList += outName;
	argList += "\"";
	argList += ",-1";
	argList += ")";
	
	//---------------------------------------------------------------------
	
	cout << (funcName + argList)<<endl;

	TStScheduler::SubmitGenericJob(funcName + argList, currentJobName);
	//break;
    }
}
