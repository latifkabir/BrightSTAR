// Filename: FmsSimJetMakerJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Jan 15 18:47:02 2020 (-0500)
// URL: jlab.org/~latif

// Make sure to change 3 places numbered below

#include <iostream>
#include "RootInclude.h"
#include "BrightStInclude.h"

using namespace std;

void FmsSimJetMakerJobs(TString inFilePrefix, Int_t maxNfiles)
{    
    //------- 1.Change Here Function Name ------------
    TString funcName = "FmsSimRunJetMaker";
    TString jobName = "FmsJetSim";
    //--------------------------------------
        
    cout << "Total number of jobs to be processed: "<< maxNfiles <<endl;

    //------------ Loop for jobs --------------------------
    for (Int_t i = 0; i < maxNfiles; ++i)
    {
	TString argList;
	TString currentJobName;
	TString muDstName = inFilePrefix + Form("%i_", i) + "evt5000.MuDst.root";
	TString pyName = inFilePrefix + Form("%i_", i) + "evt5000.pythia.root";
	TString outName = "FmsJetSim_evt5000_pass0";
	outName += Form("_%i.root",i);
	if(gSystem->AccessPathName(muDstName) || gSystem->AccessPathName(pyName))
	{
	    cout << "File not found:"<< muDstName <<" ...... SKIPPED"<<endl;
	    continue;
	}
	
	currentJobName = jobName;
	currentJobName += i;
	
	argList = "(";
	argList += "-1,";
	argList += "\"";
	argList += muDstName;
	argList += "\"";
	argList += ",";
	argList += "\"";
	argList += pyName;
	argList += "\"";
	argList += ",";
	argList += "\"";
	argList += outName;
	argList += "\"";
	argList += ")";
	
	//---------------------------------------------------------------------
	
	cout << (funcName + argList)<<endl;

	TStScheduler::SubmitGenericJob(funcName + argList, currentJobName);
	//break;
    }
}
