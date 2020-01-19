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

void AnRemoveBadFiles()
{
    string basePath = "/star/u/kabir/GIT/BrightSTAR/jobResults/R15RpStream/";
    string filePrefix = "AnRunAnTreeMaker";
    ifstream inFile("/star/u/kabir/GIT/BrightSTAR/resources/temp/temp.list");
    ofstream zFile("Run15RpStreamZombieFileList.txt");    
    if(!inFile || !zFile)
    {
	cout << "Unable to read input file or write outout file" <<endl;
	return;
    }

    string fileName;
    string secFileName;
    string dataFileName;
    Int_t run;
    string runStr;
    Int_t fileCount = 0;
    Int_t numBadFiles = 0;
    string command;
    while(!inFile.eof())
    {
	inFile >> dataFileName;
	// cout << "Data File Name: "<< dataFileName <<endl;
	run = TStRunList::GetRunFromFileName(dataFileName);
	runStr = Form("%d", run);
	// cout << runStr <<endl;
	fileName = basePath + runStr + "/" + filePrefix + "_" + runStr  + "_" + TStRunList::GetFileNoFromFileName(dataFileName) + ".root";
	//secFileName = basePath + runStr + "/RpTree_" + filePrefix + "_" + runStr  + "_" + TStRunList::GetFileNoFromFileName(dataFileName) + ".root";
	
	// cout << "Output data file name:"<< fileName <<endl;
	
	if(!gSystem->AccessPathName(fileName.c_str()))
	{
	    //cout << "File found ..." <<endl;
	    TFile *f = new TFile(fileName.c_str());
	    if(f->IsZombie())
	    {
		cout << "Outfile: "<< fileName <<endl;
		cout << "----------------> Zombie file:"<< dataFileName <<endl;
		zFile << dataFileName <<endl;
		command = ".! rm " + fileName;
		// cout << command <<endl;
		cout << "!!!Removing the file !!! " <<endl;
		gROOT->ProcessLine(command.c_str());
		// command = ".! rm " + secFileName;
		// gROOT->ProcessLine(command.c_str());
		// cout << command <<endl;
		++numBadFiles;
	    }
	    f->Close();
	    delete f;
	}
	++fileCount;
	if(fileCount%100==0)
	    cout << "Files processed:" << fileCount <<endl;
	
	// if(fileCount > 5) //Test on small number of files first
	//     break;
    }
    
    cout << "Total files:" << fileCount <<endl;
    cout << "Unablailable/Bad/Zombie files:"<< numBadFiles <<endl;
}
