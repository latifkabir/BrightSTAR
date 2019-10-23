// Filename: SubmitJob.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Aug 10 17:19:34 2019 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include "json.h"
#include "RootInclude.h"
#include "TStar.h"

using namespace std;
using json = nlohmann::json;

//___________________________________________________________________________________________
void JobStatus(Int_t level)
{
    TString star_sh = TStar::Config->GetStarHome() + (TString)"/star";
    if(gSystem->AccessPathName(star_sh))
    {
	cout << "shell script NOT found at: "<<star_sh<<endl;
	return;
    }
    TString command;
    if(level > 0)
	command = (TString)".! " + star_sh + (TString)"\t" + "job-report";
    else
	command = (TString)".! " + star_sh + (TString)"\t" + "status"; 
    gROOT->ProcessLine(command);
}

//___________________________________________________________________________________________
void SubmitJob(TString functionName, Int_t firstRun,  Int_t lastRunOrNfiles, TString outName, TString jobName)    
{
    if(outName == "")
	 outName = functionName;  // Save locally and then copy back from job sh script

    TString starHome = TStar::Config->GetStarHome();
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
    string arguments;
    TString resultDir;
    unsigned firstDelimPos;
    unsigned endPosOfFirstDelim;
    unsigned lastDelimPos;
    const string startDelim = "_raw_";
    const string stopDelim = ".MuDst.root";
    
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
	    //--------- Extract file number from file-name------
	    firstDelimPos = fileName.find(startDelim);
	    endPosOfFirstDelim = firstDelimPos + startDelim.length();
	    lastDelimPos = fileName.find_first_of(stopDelim, endPosOfFirstDelim);
	    fileNumberStr = fileName.substr(endPosOfFirstDelim, lastDelimPos - endPosOfFirstDelim);
	    
	    //std::stringstream ss;
	    //ss << std::setw(5) << std::setfill('0') << fileCount;
	    //output_file = outName + "_" + to_string(runNumber) + (string)"_"+ ss.str() + (string)".root";
	    output_file = outName + "_" + to_string(runNumber) + (string)"_"+ fileNumberStr + (string)".root";
	    if(runNumber != prevRun)
	    {
		resultDir = TStar::Config->GetJobResultsPath() + jobName + (TString)"/" + to_string(runNumber);
		gROOT->ProcessLine((TString)".! mkdir -p " + resultDir);    
		condorConfig_out <<"Initialdir      = " << resultDir << endl; 		
	    }
	    prevRun = runNumber;
	    arguments = "Arguments       = " + (string)"\"" + filePath + "\t" + output_file + "\"";
	    condorConfig_out << arguments << endl; 
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

//_______________________________________________________________________________________________
void SubmitJob(TString functionName, TString inFileName, TString outName,  TString jobName)    
{
    if(outName == "")
	outName = functionName;  // Save locally and then copy back from job sh script
    
    //================================== Create Job Macro =============================
    ofstream macro_out(TStar::Config->GetStarHome() + (TString)("/jobMacro.C"));
    if(!macro_out)
    {
	cout << "Unable to create job macro" <<endl;
	return;
    }
    macro_out<<"void jobMacro(TString fileList, TString outName)"<<endl;
    macro_out<<"{"<<endl;
    macro_out<<"\t"<<"gROOT->Macro(\"rootlogon.C\");"<<endl;
    macro_out<<"\t"<<functionName<<"(fileList, outName);"<<endl;
    macro_out<<"}"<<endl;
    macro_out.close();
    
    cout << "====================== Reading Condor Job Configuration ... ... ================" <<endl;
    TString condor_config = TStar::Config->GetStarHome() + (TString)"/jobConfig/condor.config";
    if(gSystem->AccessPathName(condor_config))
    {
	cout << "Condor job config NOT found at: "<<condor_config<<endl;
	return;
    }
    
    ifstream condorConfig_in(condor_config);
    ofstream condorConfig_out(TStar::Config->GetStarHome() + (TString)("/condor.job"));
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
    condorConfig_in.close();

    //======================= Get File Path and wtite to condor file descriptor ===================================
    cout << "====================== Reading fileList and writing to Condor Job Description File ... ... ================" <<endl;
    string arguments;

    arguments = "Arguments       = " + (string)"\"" + inFileName + "\t" + outName + "\"";
    condorConfig_out << arguments << endl; 
    condorConfig_out << "Queue\n" << endl;
	    
    condorConfig_out.close();

    //======================== Submit the job using condor=======================
    TString subScript = TStar::Config->GetStarHome() + (TString)"/condor.job";
    if(gSystem->AccessPathName(subScript))
    {
    	cout << "Submission sh script NOT found at: "<<subScript<<endl;
    	return;
    }
    TString command = (TString)".! condor_submit" + (TString)"\t" + subScript;
    gROOT->ProcessLine(command);

    cout << "Submission attempt completed." <<endl;
}


//___________________________________________________________________________________________
void SubmitSumsJob(TString function, TString runList, TString outNamePrefix, TString jobName)    
{
    if(outNamePrefix == "")
	outNamePrefix = function;
    TString subScript = TStar::Config->GetStarHome() + (TString)"/submitSumsJob.sh";
    TString subConfig = TStar::Config->GetStarHome() + (TString)"/sumsConfig.sh";
    if(gSystem->AccessPathName(subScript) || gSystem->AccessPathName(subConfig))
    {
	cout << "Submission script or config NOT found"<<endl;
	return;
    }
    
    TString command = (TString)".! " + subScript + (TString)"\t" + function + (TString)"\t" + runList + (TString)"\t" + outNamePrefix;
    gROOT->ProcessLine(command);
}
