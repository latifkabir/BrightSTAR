// Filename: RemoveBadFiles.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 20 17:35:27 2019 (-0500)
// URL: jlab.org/~latif

#include <iostream>
#include <string>
#include <fstream>
#include "TFile.h"

using namespace std;


void RemoveBadFiles()
{
    string basePath = "/star/u/kabir/GIT/BrightSTAR/jobResults/condor/";
    ifstream inFile("/star/u/kabir/GIT/BrightSTAR/resources/temp/temp.list");
    ofstream zFile("Run15ZombieFileList.txt");    
    if(!inFile || !zFile)
    {
	cout << "Unable to read input file or write outout file" <<endl;
	return;
    }

    string fileName;
    string dataFileName;
    Int_t run;
    string runStr;
    Int_t fileCount = 0;
    while(!inFile.eof())
    {
	inFile >> dataFileName;
	//cout << "Data File Name: "<< dataFileName <<endl;
	run = 16081017;//TStRunList::GetRunFromFileName(dataFileName);
	runStr = Form("%d", run);
	//cout << runStr <<endl;
	fileName = basePath + "" + runStr+ "/RunFmsRpTreeMaker_" + runStr  + "_" + TStRunList::GetFileNoFromFileName(dataFileName) + ".root";
	//cout << "Output data file name:"<< fileName <<endl;

	if(!gSystem->AccessPathName(fileName.c_str()))
	{
	    //cout << "File found ..." <<endl;
	    TFile *f = new TFile(fileName.c_str());
	    if(f->IsZombie())
	    {
		cout << "----------------> Zombie file:"<< dataFileName <<endl;
		zFile << dataFileName <<endl;
	    }
	    f->Close();
	    delete f;
	}
	++fileCount;
	if(fileCount > 20)
	    break;
    }
}
