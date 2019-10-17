// Filename: SubmitJob.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Aug 10 17:19:34 2019 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>
#include <string>
#include "json.h"
#include "RootInclude.h"
#include "TStar.h"

using namespace std;
using json = nlohmann::json;

//___________________________________________________________________________________________
void JobStatus()
{
    TString star_sh = TStar::Config->GetStarHome() + (TString)"/star";
    if(gSystem->AccessPathName(star_sh))
    {
	cout << "shell script NOT found at: "<<star_sh<<endl;
	return;
    }
    TString command = (TString)".! " + star_sh + (TString)"\t" + "status"; 
    gROOT->ProcessLine(command);
}

//___________________________________________________________________________________________
void SubmitJob(TString functionName, Int_t firstRun,  Int_t lastRunOrNfiles, Int_t nFilesPerProcess, TString outName)    
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

    const char *rNumber;
    string fileName;
    TString output_file;
    string arguments;
    for(int i = 0; i < j.size(); ++i)
    {
	if(j[i]["run"] >= firstRun && j[i]["run"] <= lastRun)
	{
	    rNumber =  (std::to_string((int)j[i]["run"])).c_str();
	    fileName = "root://xrdstar.rcf.bnl.gov:1095/" + TStar::Config->GetProdPath() + rNumber[2] + rNumber[3] + rNumber[4] + "/" + to_string((int)j[i]["run"]) + "/" + (string)j[i]["data"]["file"];
	    cout << fileName <<endl;
	    // if(!TStar::IsValid(fileName))
	    // {
	    // 	cout<<"The file path: " << fileName << " is no longer valid. Skipping it." << endl;
	    // 	continue;
	    // }
	    output_file = outName + "_" + to_string((int)j[i]["run"]) + (string)"_"+ to_string(fileCount) + (string)".root";
	    arguments = "Arguments       = " + (string)"\"" + fileName + "\t" + output_file + "\"";
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

//_______________________________________________________________________________________________
void SubmitJob(TString functionName, TString inFileName, TString outName)    
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
void SubmitSumsJob(TString function, TString runList, TString outNamePrefix)    
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
