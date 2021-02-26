// Filename: TStScheduler.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed May 29 00:24:18 2019 (-0400)
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
#include "TString.h"
#include "RootInclude.h"
#include "TStRunList.h"
#include "TROOT.h"
#include "TEntryList.h"
#include "TSystem.h"


using namespace std;
using json = nlohmann::json;

ClassImp(TStScheduler)

//___________________________________________________________________________________________
TStScheduler::TStScheduler()
{

}

//___________________________________________________________________________________________
TStScheduler::~TStScheduler()
{
    
}

Int_t TStScheduler::mJobThreshold = 100;
Int_t TStScheduler::mSleepTime = 30; //In minutes
Int_t TStScheduler::mRunIncrement = 20;
Int_t TStScheduler::mMaxFilesPerJob = 5;
Int_t TStScheduler::mCopyToExeHost = 1;
Int_t TStScheduler::mJobCounter = 0;

//___________________________________________________________________________________________
void TStScheduler::JobStatus(Int_t level)
{
    TString star_sh = TStar::Config->GetStarHome() + (TString)"/star";
    if(gSystem->AccessPathName(star_sh))
    {
	cout << "shell script NOT found at: "<<star_sh<<endl;
	return;
    }
    TString command;
    if(level > 0)
	command = (TString)".! " + star_sh + (TString)"\t" + "jobs";
    else
	command = (TString)".! " + star_sh + (TString)"\t" + "status"; 
    gROOT->ProcessLine(command);
}

//___________________________________________________________________________________________
void TStScheduler::SubmitJob(vector <string> jobList, TString jobName)    
{
    TString namePrefix = jobName;   
    TString starHome = TStar::Config->GetStarHome();
    TString outFile = starHome + (TString)"/jobOutput/" + namePrefix + (TString)"$(Process).out";
    TString errorFile = starHome + (TString)"/jobOutput/" + namePrefix + (TString)"$(Process).err";
    TString logFile = starHome + (TString)"/jobOutput/" + namePrefix + (TString)"$(Process).log";
    TString outDir = starHome + "/jobOutput/";
    TString jobDir = starHome + (TString) "/jobs/" + jobName;
    TString createJobDir = (TString)".! mkdir -p " + jobDir;
    gROOT->ProcessLine(createJobDir);    
    
    cout << "====================== Reading Condor Job Configuration ... ... ================" <<endl;
    TString condor_config = TStar::gConfig->GetCondorConfig();
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
  
    condorConfig_out << "transfer_input_files =   " << starHome << "/.sl73_gcc485, " << starHome << "/lib, "<< starHome << "/rootlogon.C, "<< starHome << "/setup.sh, "<< starHome << "/setup.csh, "<< starHome << "/config, " << starHome << "/database" << endl;
    condorConfig_in.close();

    //======================= Get File Path and wtite to condor file descriptor ===================================
    cout << "====================== Reading fileList and writing to Condor Job Description File ... ... ================" <<endl;
    TString resultDir;
    TString rootCommand;
    resultDir = TStar::Config->GetJobResultsPath() + jobName + (TString)"/";
    gROOT->ProcessLine((TString)".! mkdir -p " + resultDir);
    condorConfig_out <<"Initialdir      = " << resultDir << endl; 		
    condorConfig_out << "Executable       =  /bin/bash" << endl;
    condorConfig_out <<"Output          = "<<outFile<<endl;
    condorConfig_out <<"Error           = "<<errorFile<<endl;
    condorConfig_out <<"Log             = "<<logFile<<endl;
    for(Int_t job = 0; job < jobList.size(); ++job)
    {
	rootCommand = jobList[job];
	rootCommand.ReplaceAll("\"", "\\\"\"");
	condorConfig_out << "Arguments   =  \"-c '" << "echo \"\""<< rootCommand << "\"\" | root4star -l -b'\"" <<endl;
	condorConfig_out << "Queue\n" << endl;
    }	    
    condorConfig_out.close();

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


//___________________________________________________________________________________________
void TStScheduler::SubmitJob(TString functionName, Int_t firstRun,  Int_t lastRunOrNfiles, TString outName, TString jobName)    
{
    if(outName == "")
	 outName = functionName;  // Save locally and then copy back from job sh script
    else
	outName.ReplaceAll(".root", "");

    if(jobName == "condor")
	jobName += to_string(++mJobCounter);
    
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
    if(!mCopyToExeHost)
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
    //shell_out<<"stardev"<<endl;
    if(mCopyToExeHost)
	shell_out<<"source setup.sh"<<endl;
    else
	shell_out<<"source "<<starHome<<"/setup.sh"<<endl;
    shell_out<<"root4star -l -q -b \""<< jobDir <<"/jobMacro.C(\\\"$1\\\", \\\"$2\\\")\""<<endl;
    shell_out.close();
    
    cout << "====================== Reading Condor Job Configuration ... ... ================" <<endl;
    TString condor_config = TStar::gConfig->GetCondorConfig();
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
    if(mCopyToExeHost)
	condorConfig_out << "transfer_input_files =   " << starHome << "/.sl73_gcc485, " << starHome << "/lib, "<< starHome << "/rootlogon.C, "<< starHome << "/setup.sh, "<< starHome << "/setup.csh, "<< starHome << "/config, " << starHome << "/database" << endl;
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
    string rNumber;
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
	    
	    std::stringstream ss;
	    ss << std::setw(5) << std::setfill('0') << fileCount;
	    //output_file = outName + "_" + to_string(runNumber) + (string)"_"+ ss.str() + (string)".root";
	    if(fileNumberStr == "")
		fileNumberStr = ss.str();
	    namePrefix = outName + "_" + to_string(runNumber) + (string)"_" + fileNumberStr;
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


//------------ Following is under dev. To be kept one implementation only ------------------
//__________________________________________________________________________________________________________________
void TStScheduler::SubmitJob(Int_t maxFilesPerJob, TString functionName, Int_t firstRun,  Int_t lastRunOrNfiles, TString outName, TString jobName)    
{
    if(outName == "")
	 outName = functionName;  // Save locally and then copy back from job sh script
    else
	outName.ReplaceAll(".root", "");

    if(jobName == "condor")
	jobName += to_string(++mJobCounter);
    
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
    if(!mCopyToExeHost)
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
    //shell_out<<"stardev"<<endl;
    if(mCopyToExeHost)
	shell_out<<"source setup.sh"<<endl;
    else
	shell_out<<"source "<<starHome<<"/setup.sh"<<endl;
    shell_out<<"root4star -l -q -b \""<< jobDir <<"/jobMacro.C(\\\"$1\\\", \\\"$2\\\")\""<<endl;
    //shell_out<<"rm jets_*.root skim_*.root ueoc_*.root"<<endl;  //<--- file you do not want
    shell_out.close();
    
    cout << "====================== Reading Condor Job Configuration ... ... ================" <<endl;
    TString condor_config = TStar::gConfig->GetCondorConfig();
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
    if(mCopyToExeHost)
	condorConfig_out << "transfer_input_files =   " << starHome << "/.sl73_gcc485, " << starHome << "/lib, "<< starHome << "/rootlogon.C, "<< starHome << "/setup.sh, "<< starHome << "/setup.csh, "<< starHome << "/config, " << starHome << "/database" << endl;
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
    string rNumber;
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

    TString jobFileListPath;
    Int_t jobFileCount = 0;
    string lastFileNoStr;
    Int_t k = 0;
    
    while(k < j.size())
    {
	runNumber = (int)j[k]["run"];
	if(runNumber >= firstRun && runNumber <= lastRun)
	{
	    rNumber =  std::to_string((int)j[k]["run"]);
	    fileName = (string)j[k]["data"]["file"];
	    filePath = "root://xrdstar.rcf.bnl.gov:1095/" + TStar::Config->GetProdPath() + rNumber[2] + rNumber[3] + rNumber[4] + "/" + to_string(runNumber) + "/" + fileName;
	    //cout << filePath <<endl;
	    // if(!TStar::IsValid(filePath))
	    // {
	    // 	cout<<"The file path: " << filePath << " is no longer valid. Skipping it." << endl;
	    // 	continue;
	    // }
	    fileNumberStr = TStRunList::GetFileNoFromFileName(fileName);

	    if(maxFilesPerJob > 1)
	    {
		jobFileListPath = jobDir + (TString)"/jobFileList_" + to_string(runNumber) + (TString)"_"+ fileNumberStr + (TString)".list";
		ofstream jobFileList(jobFileListPath);
		if(!jobFileList)
		{
		    cout<<"Unable to create job filelist" << endl;
		    return;
		}	

	    	jobFileCount = 0;
	    	while(jobFileCount < maxFilesPerJob)
	    	{
	    	    if(runNumber != (int)j[k]["run"])
	    		break;
		    if(k >= j.size())
			break;
		    
	    	    rNumber =  std::to_string((int)j[k]["run"]);
	    	    fileName = (string)j[k]["data"]["file"];
	    	    filePath = "root://xrdstar.rcf.bnl.gov:1095/" + TStar::Config->GetProdPath() + rNumber[2] + rNumber[3] + rNumber[4] + "/" + to_string(runNumber) + "/" + fileName;
	    	    jobFileList << filePath <<endl;
	    	    cout << filePath <<endl;

	    	    ++k;
	    	    ++jobFileCount;
	    	}
		cout<< endl;
	    	lastFileNoStr = TStRunList::GetFileNoFromFileName(fileName);
		filePath = jobFileListPath;
		jobFileList.close();
	    }
	    else
	    {
		cout << filePath <<endl;
		++k;
	    }
	    
	    std::stringstream ss;
	    ss << std::setw(5) << std::setfill('0') << fileCount;
	    //output_file = outName + "_" + to_string(runNumber) + (string)"_"+ ss.str() + (string)".root";
	    if(fileNumberStr == "")
		fileNumberStr = ss.str();
	    namePrefix = outName + "_" + to_string(runNumber) + (string)"_" + fileNumberStr;
	    if(maxFilesPerJob > 1)
		namePrefix += "_" + lastFileNoStr;

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
	else
	    ++k;
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


//____________________________________________________________________________________
void TStScheduler::SubmitJob(Int_t maxFilesPerJob, TString functionName, TEntryList *runList)
{
    TString jobName;
    Int_t runNumber;
    for(Int_t i = 0; i < runList->GetN(); ++i)
    {
	runNumber = runList->GetEntry(i);
	jobName = (TString)"condor" + to_string(i);
	SubmitJob(maxFilesPerJob, functionName, runNumber, runNumber, "", jobName);
    }
}


//_______________________________________________________________________________________________
void TStScheduler::SubmitJob(TString functionName, TString inFileName, TString outName,  TString jobName)    
{
    if(outName == "")
	outName = functionName;  
    else
	outName.ReplaceAll(".root", "");

    if(jobName == "condor")
	jobName += to_string(++mJobCounter);
    
    TString namePrefix = outName; //+ "_" + to_string(TStRunList::GetRunFromFileName((string)inFileName)) + (TString)"_"+ TStRunList::GetFileNoFromFileName((string)inFileName);
    outName = namePrefix + (TString)".root" ;    
    TString starHome = TStar::Config->GetStarHome();
    TString outFile = starHome + (TString)"/jobOutput/" + namePrefix + (TString)".out";
    TString errorFile = starHome + (TString)"/jobOutput/" + namePrefix + (TString)".err";
    TString logFile = starHome + (TString)"/jobOutput/" + namePrefix + (TString)".log";
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
    if(!mCopyToExeHost)
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
    //shell_out<<"stardev"<<endl;
    if(mCopyToExeHost)
	shell_out<<"source setup.sh"<<endl;
    else
	shell_out<<"source "<<starHome<<"/setup.sh"<<endl;
    shell_out<<"root4star -l -q -b \""<< jobDir <<"/jobMacro.C(\\\"$1\\\", \\\"$2\\\")\""<<endl;
    shell_out.close();
    
    cout << "====================== Reading Condor Job Configuration ... ... ================" <<endl;
    TString condor_config = TStar::gConfig->GetCondorConfig();
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
    if(mCopyToExeHost)
	condorConfig_out << "transfer_input_files =   " << starHome << "/.sl73_gcc485, " << starHome << "/lib, "<< starHome << "/rootlogon.C, "<< starHome << "/setup.sh, "<< starHome << "/setup.csh, "<< starHome << "/config, " << starHome << "/database" << endl;
    condorConfig_out << "Executable      = " << jobDir << "/condor.sh" <<endl;
    condorConfig_in.close();

    //======================= Get File Path and wtite to condor file descriptor ===================================
    cout << "====================== Reading fileList and writing to Condor Job Description File ... ... ================" <<endl;
    string arguments;
    TString resultDir;
    resultDir = TStar::Config->GetJobResultsPath() + jobName + (TString)"/";
    gROOT->ProcessLine((TString)".! mkdir -p " + resultDir);
    condorConfig_out <<"Initialdir      = " << resultDir << endl; 		
    arguments = "Arguments       = " + (string)"\"" + inFileName + "\t" + outName + "\"";
    condorConfig_out << arguments << endl;
    condorConfig_out <<"Output          = "<<outFile<<endl;
    condorConfig_out <<"Error           = "<<errorFile<<endl;
    condorConfig_out <<"Log             = "<<logFile<<endl;    
    condorConfig_out << "Queue\n" << endl;
	    
    condorConfig_out.close();

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

//___________________________________________________________________________________________
void TStScheduler::SubmitGenericJob(TString functionWithArg, TString jobName)
{
    TString namePrefix = jobName;   
    TString starHome = TStar::Config->GetStarHome();
    TString outFile = starHome + (TString)"/jobOutput/" + namePrefix + (TString)".out";
    TString errorFile = starHome + (TString)"/jobOutput/" + namePrefix + (TString)".err";
    TString logFile = starHome + (TString)"/jobOutput/" + namePrefix + (TString)".log";
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
    macro_out<<"void jobMacro()"<<endl;
    macro_out<<"{"<<endl;
    if(!mCopyToExeHost)
	macro_out<<"\t"<<"gROOT->Macro(\""<<starHome<<"/rootlogon.C\");"<<endl;
    macro_out<<"\t"<<functionWithArg<<";"<<endl;
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
    //shell_out<<"stardev"<<endl;
    if(mCopyToExeHost)
	shell_out<<"source setup.sh"<<endl;
    else
	shell_out<<"source "<<starHome<<"/setup.sh"<<endl;
    shell_out<<"root4star -l -q -b  "<< jobDir <<"/jobMacro.C"<<endl;
    shell_out.close();
    
    cout << "====================== Reading Condor Job Configuration ... ... ================" <<endl;
    TString condor_config = TStar::gConfig->GetCondorConfig();
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
    if(mCopyToExeHost)
	condorConfig_out << "transfer_input_files =   " << starHome << "/.sl73_gcc485, " << starHome << "/lib, "<< starHome << "/rootlogon.C, "<< starHome << "/setup.sh, "<< starHome << "/setup.csh, "<< starHome << "/config, " << starHome << "/database" << endl;
    condorConfig_out << "Executable      = " << jobDir << "/condor.sh" <<endl;
    condorConfig_in.close();

    //======================= Get File Path and wtite to condor file descriptor ===================================
    cout << "====================== Reading fileList and writing to Condor Job Description File ... ... ================" <<endl;
    string arguments;
    TString resultDir;
    resultDir = TStar::Config->GetJobResultsPath() + "condor" + (TString)"/";
    gROOT->ProcessLine((TString)".! mkdir -p " + resultDir);
    condorConfig_out <<"Initialdir      = " << resultDir << endl; 		
    arguments = "Arguments       =  \t";
    condorConfig_out << arguments << endl;
    condorConfig_out <<"Output          = "<<outFile<<endl;
    condorConfig_out <<"Error           = "<<errorFile<<endl;
    condorConfig_out <<"Log             = "<<logFile<<endl;    
    condorConfig_out << "Queue\n" << endl;
	    
    condorConfig_out.close();

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

//___________________________________________________________________________________________
void TStScheduler::SubmitSumsJob(TString function, TString runList, TString outNamePrefix, TString jobName)    
{
    if(outNamePrefix == "")
	outNamePrefix = function;
    TString subScript = TStar::gConfig->GetSumsJobSh();
    TString subConfig = TStar::gConfig->GetSumsConfig();
    if(gSystem->AccessPathName(subScript) || gSystem->AccessPathName(subConfig))
    {
	cout << "Submission script or config NOT found"<<endl;
	return;
    }
    
    TString command = (TString)".! " + subScript + (TString)"\t" + function + (TString)"\t" + runList + (TString)"\t" + outNamePrefix + "\t" + jobName;
    gROOT->ProcessLine(command);
}


void TStScheduler::CronJob(TString functionName,  Int_t first_run, Int_t last_run)
{
    string response;
    TStar::gConfig->Print();
    cout << "\n\nEnter y to confirm the above configuration (n to cancel):" <<endl;
    cin>>response;
    if(response != "y")
    {
	cout << "Aborting job submission" <<endl;
	return;
    }
    
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList(first_run, last_run);
    //runList->Print("all");
    Int_t totRuns = runList->GetN();
    
    Int_t firstRun = runList->GetEntry(0);
    Int_t lastRun = runList->GetEntry(totRuns - 1);
    Int_t index = 0;
    Int_t index_e = 0;
    Int_t startRun = firstRun;
    Int_t endRun;
    Int_t runIncrement = mRunIncrement;
    Int_t activeJobs = 9999;
    Int_t jobThreshold = mJobThreshold;
    Int_t sleepTime = mSleepTime*60;
    Int_t iteration = 0;

    TString command_sh = TStar::gConfig->GetJobCounterSh(); 
    TString fileName = TStar::gConfig->GetJobCounterTxt();
        
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

	if(activeJobs == 0 && iteration > 0)
	    activeJobs = 100000; //Arbitrary big number
	
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
	    TString emailMessage = (TString)"Submitted jobs:: functionName: " + functionName + (TString)" Start Rrun: " + to_string(startRun) + (TString)" End Run: " + to_string(endRun) + (TString)" Iteration: " + to_string(index_e / runIncrement) + (TString)" Active jobs: " + to_string(activeJobs);
	    TString emailCommand = (TString)".! echo \"" + emailMessage + (TString)"\" |  mail -s \"New Job Submission\" " + (TString)TStar::gConfig->GetUserEmail();
	    gROOT->ProcessLine(emailCommand);

	    if(mMaxFilesPerJob == 1)
	    	SubmitJob(functionName, startRun, endRun, "", jobName);
	    else
	    	SubmitJob(mMaxFilesPerJob, functionName, startRun, endRun, "", jobName);
	   
	    index += (runIncrement + 1);
	    if(index >= totRuns)
		index = (totRuns - 1);
	}
	else
	    cout << "Still substantial number of active jobs ..." <<endl;

	cout << "Now sleeping for a while .... ..." <<endl;
	sleep(sleepTime);	
    }
}

//_______________________________________________________________________________________________________
void TStScheduler::AnalyzeJobResults(TString dirPath, TString filePrefix, Int_t threshold, TString treeName)
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    Int_t nFiles = 0;
    string rNumber;
    string filePath;
    string fileNumberStr;
    string fileName;
    string outFileName;
    string xrootdPath;
    Int_t runNumber;
    Int_t nEntries_muDst = 0;
    Int_t nEntries_out = 0;
    Double_t frac;
    ofstream fileList(TStar::gConfig->GetStarHome() + "/resources/failedJobsFiles.list");
    
    for(int k = 0; k < j.size(); ++k)
    {
	rNumber =  std::to_string((int)j[k]["run"]);
	fileName = (string)j[k]["data"]["file"];
	runNumber =  (int)j[k]["run"];
	if(runNumber < 0) continue;
	fileNumberStr = TStRunList::GetFileNoFromFileName(fileName);
	outFileName = dirPath + "/" + to_string(runNumber) + "/" + filePrefix + to_string(runNumber) + (string)"_" + fileNumberStr + (string)".root";
	xrootdPath = "root://xrdstar.rcf.bnl.gov:1095/" + TStar::Config->GetProdPath() + rNumber[2] + rNumber[3] + rNumber[4] + "/" + to_string(runNumber) + "/" + fileName;
	    
	if(gSystem->AccessPathName(outFileName.c_str()))
	{
	    cout << "Out file is missing. To be reprocessed:"<< fileName <<endl;
	    fileList << xrootdPath << endl;
	    continue;
	}
	nEntries_muDst = j[k]["data"]["events"];

	TFile *tFile = new TFile(outFileName.c_str());
	TTree *t = (TTree*)tFile->Get(treeName);

	if(!t)
	{
	    cout << "Unable to retrieve tree named "<< treeName << " File: "<< fileName <<endl;
	    fileList << xrootdPath << endl;
	    tFile->Close();
	    delete tFile;
	    continue;
	}

	if(threshold == 0)
	{
	    tFile->Close();
	    delete tFile;
	    ++nFiles;
	    continue;
	}
	
	nEntries_out = t->GetEntries();
	if(nEntries_muDst != 0)
	    frac = (100.0*nEntries_out) / nEntries_muDst;
	else
	    frac = 0;
	cout << "File: "<< fileName<<", Fraction of Events Processed: "<< nEntries_out << " / " << nEntries_muDst <<" = "<< frac  <<endl;

	if(frac < threshold)
	    fileList << xrootdPath << endl;
	
	tFile->Close();
	delete tFile;
	++nFiles;
    }
    cout << "Number of files in the range:"<<nFiles<<endl;
    i.close();
    fileList.close();
}

//_______________________________________________________________________
TString TStScheduler::CopyInputFiles(TString inFileName)
{
    TString outName = inFileName;
    outName.ReplaceAll("root://xrdstar.rcf.bnl.gov:1095/","");
    outName.ReplaceAll(TStar::gConfig->GetProdPath(), "");
    outName.ReplaceAll("/", "_");
    outName = TStar::gConfig->GetTempPath() + outName;
    
    TString copyCmd = ".! xrdcp --retry 3 " + inFileName + " " + outName;
    cout << copyCmd << endl;
    if(gSystem->AccessPathName(outName))
    {
	gROOT->ProcessLine(".! mkdir -p " + (TString)TStar::gConfig->GetTempPath());
	gROOT->ProcessLine(copyCmd);
    }
    else
	cout << "File exists. Skipped xrootd copying ..." <<endl;

    if(gSystem->AccessPathName(outName))
	return "unavailable";
	
    cout << "copying xrootd file is successful" <<endl;
    return outName;    
}

//_______________________________________________________________________
void TStScheduler::DeleteTempFiles(TString inFileName)
{
    if(!gSystem->AccessPathName(inFileName))
    {
	cout << "Deleting input files from tmp ..." <<endl;
	gROOT->ProcessLine(".! rm " + inFileName);
    }
}
