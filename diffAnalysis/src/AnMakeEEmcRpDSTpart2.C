// Filename: AnMakeEEmcRpDSTPart1.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Dec  4 00:08:00 2019 (-0500)
// URL: jlab.org/~latif
#include <unistd.h>

//Workflow:
//Check filepath and file names as that can change.
//Submit jobs jusing AnMakeEEmcRpDSTpart2()
//Move job output using AnMakeEEmcRpDSTpart2_MoveFiles()
//Replace _0_ using `rename _0_.root .root PrefixPattern` or similar syntax.

//Run as: AnMakeEEmcRpDSTpart2.C("jobResults/R15EEmcRpTree/R15EEmcRpTreeRunList.txt")
void AnMakeEEmcRpDSTpart2(TString fileList)
{
    ifstream inFile(fileList);
    if(!inFile)
    {
	cout << "Filelist not found" <<endl;
	return;	
    }
    
    Int_t runNumber;
    TString  baseName = "/star/u/kabir/GIT/BrightSTAR/dst/R15EEmcRpTree/AnRunEEmcRpTreeMakerPart1_";
    
    while (!inFile.eof()) 
    {
    	TString jobName = "eemc_job";
    	TString input;
    	TString output = "EEmcRpTree_";
    	inFile >> runNumber;
    	if(inFile.eof())
    	    break;
    	input = baseName;
    	input += runNumber;
    	input += ".root";
    	output += runNumber;
    	output += ".root";
    	jobName += runNumber;
    	if(gSystem->AccessPathName(input))
    	    continue;
    	cout << "----> Submitting job:: input: "<<input <<" output: "<<output << " job name: "<<jobName <<endl;
	
    	TStScheduler::SubmitJob("RunEEmcTreeMakerPart2and3", input, output, jobName);
    	sleep(1);
    }

    inFile.close();    
}


//Run as : AnMakeEEmcRpDSTpart2_MoveFiles.C("jobResults/R15EEmcRpTree/R15EEmcRpTreeRunList.txt")
void AnMakeEEmcRpDSTpart2_MoveFiles(TString fileList)
{
    ifstream inFile(fileList);
    if(!inFile)
    {
	cout << "Filelist not found" <<endl;
	return;	
    }
    
    Int_t runNumber;
    TString  baseName = "/star/u/kabir/GIT/BrightSTAR/jobResults/";
    TString output = "/star/u/kabir/GIT/BrightSTAR/dst/R15EEmcRpTree/.";
    if(gSystem->AccessPathName(output))
    {
	cout << "Out Dir Does NOT exist" <<endl;
	return;
    }
    
    while (!inFile.eof()) 
    {
	TString jobName = "eemc_job";
	TString input;
	TString command;
	inFile >> runNumber;
	if(inFile.eof())
	    break;	
	jobName += runNumber;
	input = baseName + jobName;
	
	if(gSystem->AccessPathName(input))
	    continue;
	input += "/*";
	cout << "----> Moving Files:: input: "<<input <<" output: "<<output << " job name: "<<jobName <<endl;
	command = ".! mv " + input + " " + output;
	//cout << "Command:" << command <<endl;
	gROOT->ProcessLine(command);
	//sleep(1);
    }    
    inFile.close();    
}

