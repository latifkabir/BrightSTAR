// Filename: JobPrototype.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Oct 21 22:46:55 2019 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "BrightStInclude.h"

void JobPrototype(TString inFile, TString outFile)
{
    TStar::Config->Print();
    TFile *f = new TFile(outFile, "RECREATE");
    gROOT->ProcessLine(".! ls -ltra");
    f->Close();
    
    inFile = TStScheduler::CopyInputFiles(inFile);
    gROOT->ProcessLine(".! ls -ltra " + (TString)TStar::gConfig->GetTempPath());
    if(inFile.Contains(".list"))
	gROOT->ProcessLine(".! cat " + inFile);

    Int_t run = TStRunList::GetRunFromFileName((string)inFile);
    cout << "File name: "<< inFile << " , Found run number: "<< run <<endl;
   
    TStScheduler::DeleteTempFiles(inFile);    
    gROOT->ProcessLine(".! cd " + (TString)TStar::gConfig->GetTempPath() + " && pwd -P" );
    // gROOT->ProcessLine(".! du " + (TString)TStar::gConfig->GetTempPath());
    // gROOT->ProcessLine(".! pwd -P");
    gROOT->ProcessLine(".! ls -ltra " + (TString)TStar::gConfig->GetTempPath());
}
