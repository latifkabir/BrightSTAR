// Filename: CepNanoDstMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:25:43 2019 (-0500)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"
#include "BrNanoDstMaker/TStRpFilterMaker.h"
#include "BrNanoDstMaker/TStNanoDstMaker.h"
#include "RunNanoDstMaker.h"

#include <vector>
#include <iostream>
using namespace std;

void CepNanoDstMaker(TString fileList, TString outFile)
{
    //------ Print Current Config ----------------
    cout << "Current Configuration:"<<endl;
    TStar::gConfig->Print();
    
    vector<Int_t> *evtTrigs = new vector<Int_t>();
    vector<Int_t> *vetoTrigs = new vector<Int_t>();
    string fileName;
    if(fileList.Contains(".list"))
    {
	ifstream inFile(fileList);
	if(!inFile)
	{
	    cout << "Unable to read run number from file list" <<endl;
	    return;
	}
	getline(inFile, fileName);
	inFile.close();
	cout << "\n------->Warning: Setting trigger ID based on first run number only: "<< fileName <<"<-----\n"<<endl;
    }
    else
	fileName = fileList;
    
    Int_t runNumber = TStRunList::GetRunFromFileName(fileName);
    if(runNumber < 1)
    {
	cout << "Unable to get run number" <<endl;
	return;
    }

    //------- CEP Analysis: RP stream ----------------
    evtTrigs->push_back(TStTrigDef::GetTrigId(runNumber, "RP_CPT2"));

    //Veto
    //vetoTrigs->push_back(TStTrigDef::GetTrigId(runNumber,"FMS-LED")); 
    
    //---------- EEmc-Rp Corr: Physics stream --------------
    // evtTrigs->push_back(TStTrigDef::GetTrigId(runNumber, "EHT0")); // Generally 480301;

    
    RunNanoDstMaker(fileList, outFile, false, evtTrigs, vetoTrigs);    

    // RunNanoDstMaker(fileList, outFile, true, evtTrigs, vetoTrigs, 500);  //test
    // RunNanoDstMaker(fileList, outFile, true, evtTrigs, NULL, 500); //test    
}
