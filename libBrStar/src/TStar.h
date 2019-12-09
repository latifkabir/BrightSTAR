// Filename: TStar.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Oct 17 18:10:57 2017 (-0400)
// URL: jlab.org/~latif

#ifndef TSTAR_H
#define TSTAR_H

#include <iostream>
#include "TObject.h"
#include "TStConfig.h"

class TStar: public TObject
{

public:
    TStar();
    ~TStar();

    static TStConfig *gConfig;    
    static TStConfig *Config;           //For backward compatibility

    static Int_t gFirstRun;   //This will make library loading slow (to read large json file)
    static Int_t gLastRun;
    
    static Int_t GetCounter();
    static void SetCounter(Int_t cout);
    static void PrintCounter();
    static void Exit();
    static void ExitIfInvalid(TString filePath);
    static Bool_t IsValid(TString filePath);
    static void FileViewer(Int_t fileNumber);
    static void FileViewer(TString fileName);
    static void FilePrint(Int_t lowerRange, Int_t upperRange);
    static void FileListing(Int_t lowerRange, Int_t upperRange);
    static void FileListing();

    static void help();
    static void help(string file);
    static void history();
    ClassDef(TStar,1)
};

#endif
