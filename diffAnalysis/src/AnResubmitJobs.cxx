// Filename: AnResubmitJobs.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Dec  7 00:55:53 2019 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sstream>
#include <iomanip>

#include "json.h"
#include "TStScheduler.h"
#include "TStConfig.h"
#include "TStar.h"
#include "RootInclude.h"
#include "TStRunList.h"

using namespace std;
using json = nlohmann::json;

//___________________________________________________________________________________________
void AnResubmitJobs(TString functionName, Int_t firstRun,  Int_t lastRunOrNfiles, TString outName, TString jobName)    
{
    if(outName == "")
	 outName = functionName;  // Save locally and then copy back from job sh script
    else
	outName.ReplaceAll(".root", "");
    TString starHome = TStar::Config->GetStarHome();
    TString outDir = starHome + "/jobOutput/";
    TString jobDir = starHome + (TString) "/jobs/" + jobName;
    TString createJobDir = (TString)".! mkdir -p " + jobDir;
    gROOT->ProcessLine(createJobDir);    
    //================================== Create Job Macro =============================
    ofstream macro_out(jobDir + (TString)"/jobMacro.C");
    if(!macro_out)
    {
	cout << "Unable to create job macro" <<endl;
	return;
    }
    macro_out<<"void jobMacro(TString fileList, TString outName)"<<endl;
    macro_out<<"{"<<endl;
    macro_out<<"\t"<<"gROOT->Macro(\""<<starHome<<"/rootlogon.C\");"<<endl;
    macro_out<<"\t"<<functionName<<"(fileList, outName);"<<endl;
    macro_out<<"}"<<endl;
    macro_out.close();
    //============================Create Condor execution shell script ========================
    ofstream shell_out(jobDir + (TString)"/condor.sh");
    if(!shell_out)
    {
	cout << "Unable to create shell script" <<endl;
	return;
    }
    shell_out<<"#!/bin/bash"<<endl;
    shell_out<<"stardev"<<endl;
    shell_out<<"source "<<starHome<<"/setup.sh"<<endl;
    shell_out<<"root4star -l -q -b \""<< jobDir <<"/jobMacro.C(\\\"$1\\\", \\\"$2\\\")\""<<endl;
    shell_out.close();
    
    cout << "====================== Reading Condor Job Configuration ... ... ================" <<endl;
    TString condor_config = starHome + (TString)"/condor/condor.config";
    if(gSystem->AccessPathName(condor_config))
    {
	cout << "Condor job config NOT found at: "<<condor_config<<endl;
	return;
    }
    
    ifstream condorConfig_in(condor_config);
    ofstream condorConfig_out(jobDir + (TString)("/condor.job"));
    if(!condorConfig_out)
    {
	cout<<"Unable to create condor job description file" << endl;
	return;
    }
    string str;
    while(getline(condorConfig_in, str))
    {
	condorConfig_out << str <<endl;
    }
    condorConfig_out << "Executable      = " << jobDir << "/condor.sh" <<endl;
    condorConfig_in.close();

    //======================= Get file path and wtite to condor file descriptor ===================================
    cout << "====================== Reading fileList and writing to Condor Job Description File ... ... ================" <<endl;
    Int_t lastRun = -1;
    Int_t limit = -1;
    Int_t fileCount = 0;
    if(lastRunOrNfiles == -1)
	lastRun = firstRun;
    else if(lastRunOrNfiles != -1 && lastRunOrNfiles < firstRun)
    {
	limit = lastRunOrNfiles;
	lastRun = firstRun;
    }
    else if(lastRunOrNfiles >= firstRun)
	lastRun = lastRunOrNfiles;
    
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    Int_t runNumber;
    Int_t prevRun = -1;
    const char *rNumber;
    string filePath;
    string fileName;
    string fileNumberStr;
    TString output_file;
    string namePrefix;
    string arguments;
    string outFile;
    string errorFile;
    string logFile;
    TString resultDir;
    TString resultFile;
    
    for(int i = 0; i < j.size(); ++i)
    {
	runNumber = (int)j[i]["run"];
	if(runNumber >= firstRun && runNumber <= lastRun)
	{
	    rNumber =  (std::to_string((int)j[i]["run"])).c_str();
	    fileName = (string)j[i]["data"]["file"];
	    filePath = "root://xrdstar.rcf.bnl.gov:1095/" + TStar::Config->GetProdPath() + rNumber[2] + rNumber[3] + rNumber[4] + "/" + to_string(runNumber) + "/" + fileName;
	    cout << filePath <<endl;
	    // if(!TStar::IsValid(filePath))
	    // {
	    // 	cout<<"The file path: " << filePath << " is no longer valid. Skipping it." << endl;
	    // 	continue;
	    // }
	    fileNumberStr = TStRunList::GetFileNoFromFileName(fileName);

	    //---------------- Skip job is file already exists
	    resultFile = "/star/u/kabir/GIT/BrightSTAR/jobResults/R15FmsRpTree/" + to_string(runNumber) + "/RunFmsRpTreeMaker_" + to_string(runNumber) + "_" + fileNumberStr + ".root";
	    if(!gSystem->AccessPathName(resultFile))
	    {
		cout << "\nThe file: "<< resultFile <<" already exists. Skipped ... ..." <<endl;
		continue;
	    }
	    else
		cout << "\n----> Re-Submitting job to make the missing file: "<<resultFile<<endl;

	    //----------------
	    
	    std::stringstream ss;
	    ss << std::setw(5) << std::setfill('0') << fileCount;
	    //output_file = outName + "_" + to_string(runNumber) + (string)"_"+ ss.str() + (string)".root";
	    if(fileNumberStr == "")
		fileNumberStr = ss.str();
	    namePrefix = outName + "_" + to_string(runNumber) + (string)"_"+ fileNumberStr;
	    output_file = namePrefix + (string)".root";
	    outFile = outDir + namePrefix + (string)".out";
	    errorFile = outDir + namePrefix + (string)".err";
	    logFile = outDir + namePrefix + (string)".log";
	    if(runNumber != prevRun)
	    {
		resultDir = TStar::Config->GetJobResultsPath() + jobName + (TString)"/" + to_string(runNumber);
		gROOT->ProcessLine((TString)".! mkdir -p " + resultDir);    
		condorConfig_out <<"Initialdir      = " << resultDir << endl; 		
	    }
	    prevRun = runNumber;
	    arguments = "Arguments       = " + (string)"\"" + filePath + "\t" + output_file + "\"";
	    condorConfig_out << arguments << endl;
	    condorConfig_out <<"Output          = "<<outFile<<endl;
	    condorConfig_out <<"Error           = "<<errorFile<<endl;
	    condorConfig_out <<"Log             = "<<logFile<<endl;
	    condorConfig_out << "Queue\n" << endl;
	    
	    ++fileCount;
	    if(limit == fileCount)
		break;
        }
    }
    i.close();
    condorConfig_out.close();

    if(fileCount < 1)
    {
	cout << "0 input file for submission" <<endl;
	return;
    }
    //======================== Submit the job using condor=======================
    TString subScript = jobDir + (TString)"/condor.job";
    if(gSystem->AccessPathName(subScript))
    {
    	cout << "Submission sh script NOT found at: "<<subScript<<endl;
    	return;
    }
    TString command = (TString)".! condor_submit" + (TString)"\t" + subScript;
    gROOT->ProcessLine(command);

    cout << "Submission attempt completed." <<endl;
}
