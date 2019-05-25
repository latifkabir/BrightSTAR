// Filename: TStar.h
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Tue Oct 17 18:10:57 2017 (-0400)
// URL: latifkabir.github.io

#ifndef TSTAR_H
#define TSTAR_H

#include <iostream>
#include "TObject.h"
#include "TStarConfig.h"

class TStar: public TObject
{

public:
    TStar();
    ~TStar();
    static TStarConfig Config;
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
    ClassDef(TStar,0)
};

#endif
