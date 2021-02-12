// Filename: RunEEmcNanoDstMaker.cxx
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

void RunEEmcNanoDstMaker(TString inFile, TString outFile)
{
    //------ Print Current Config ----------------
    cout << "Current Configuration:"<<endl;
    TStar::gConfig->Print();

    if(gROOT->IsBatch() && !inFile.Contains(".list"))
	inFile = TStScheduler::CopyInputFiles(inFile);
    
    vector<Int_t> *evtTrigs = new vector<Int_t>();
    vector<Int_t> *vetoTrigs = new vector<Int_t>();
    
    // Int_t runNumber = TStRunList::GetRunFromFileName((string)inFile);
    // if(runNumber < 1)
    // {
    // 	cout << "Unable to get run number" <<endl;
    // 	return;
    // }

    //------- CEP Analysis: RP stream ----------------
    //evtTrigs->push_back(TStTrigDef::GetTrigId(runNumber, "RP_CPT2"));
    //---------- EEmc-Rp Corr: Physics stream --------------
    // evtTrigs->push_back(TStTrigDef::GetTrigId(runNumber, "EHT0")); // Generally 480301;
    
    //Veto
    //vetoTrigs->push_back(TStTrigDef::GetTrigId(runNumber,"FMS-LED")); 
    


    
    //RunNanoDstMaker(inFile, outFile, false, evtTrigs, vetoTrigs);    

    RunNanoDstMaker(inFile, outFile, false);
    
    //RunNanoDstMaker(inFile, outFile, true, NULL, NULL, 1000);  //test


    if(gROOT->IsBatch() && inFile.Contains("/tmp/"))
	TStScheduler::DeleteTempFiles(inFile);

    cout << "-----------> Deleting EEMC Trees parts 1-3 !!! <--------------------" <<endl;
    gROOT->ProcessLine(".! rm eempTreeP1.root eempTreeP2.root eempTreeP3.root");
    
    cout <<"Done!" <<endl;    
}
