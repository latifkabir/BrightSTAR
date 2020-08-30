// Filename: FmsSimRunStarsimAndBfc.cxx
// Description: RUn Pythia + Geant3 + Big Full Chain
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Jul 10 01:24:05 2020 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "iostream"
#include "TStConfig.h"
#include "TStar.h"

using namespace std;

void FmsSimRunStarsimAndBfc(Int_t cycle, Int_t nEvents, Int_t filterThres = 15)
{
    TString kumacFile = TStar::gConfig->GetBrightHome() + (TString)"starSim/pythia.kumac";

    if(gSystem->AccessPathName(kumacFile))
    {
	cout << "Pythia kumac file Not found:"<< kumacFile<<endl;
	return;
    }

    TString fileName = "FmsSim_Run15_" + to_string(cycle) + Form("_evt%i", nEvents) + ".fzd";   
    TString pythiaCmd = ".! starsim -w 0 -b ";
    pythiaCmd += kumacFile; 
    pythiaCmd += Form(" NEVENTS=%i ", nEvents);
    pythiaCmd += Form(" CYCLE=%i ", cycle);
    pythiaCmd += " FILE=";
    pythiaCmd += fileName;
    
    cout << "starsim command:"<< pythiaCmd <<endl;

    gROOT->ProcessLine(".! ls -lha");
    gROOT->ProcessLine(pythiaCmd);
    // if(gSystem->AccessPathName(fileName.Data()))
    // {
    // 	cout << "No fzd file. SKipping ..." <<endl;
    // 	return;
    // }

    TString bfcMacro =  TStar::gConfig->GetBrightHome() + "/starSim/src/FmsSimRunBfc.C(-1,\"" + fileName + "\"" + Form(",%i", filterThres) + ")";
    cout << bfcMacro <<endl;
    gROOT->Macro(bfcMacro);
    
    cout << "------------------> Warning: Deleting the fzd file <-------------------" <<endl;
    gROOT->ProcessLine(".! rm *.fzd");
}

