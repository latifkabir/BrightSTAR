// Filename: TStConfig.h
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Sun Sep  9 23:29:52 2018 (-0400)
// URL: jlab.org/~latif

#ifndef TSTCONFIG_H
#define TSTCONFIG_H

#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cassert>
#include "TObject.h"

using namespace std;

class TStConfig : public TObject
{
    //-----Buffer for each configuration field to be read goes here------
    string fConfigFile;
    string fDataPath;
    string fResultsPath;
    string fDSTpath;
    string fJobResultsPath;
    string fStarHome;
    string fRunListDB;
    string fFileList;
    string fProdPath;
    string fTrigDefFile;
    string fStreamPrefix;
    string fFillNoDB;

    int fUseEvt;
    int fUseTpc;
    int fUseEmc;
    int fUseFms;
    int fUseRps;
    int fUseEEmc;
    
    void CheckValidity();    
public:
    TStConfig(string file );
    TStConfig();
    ~TStConfig();
    void LoadConfig();
    void Print();

    //----- Getter for each configuration field -------------
    const string & GetStarHome(){return fStarHome;}
    const string & GetConfigPath(){return fConfigFile;}
    const string & GetDataPath(){return fDataPath;}
    const string & GetResultsPath(){return fResultsPath;}
    const string & GetFilePath(){return fResultsPath;}
    const string & GetDSTpath(){return fDSTpath;}
    const string & GetJobResultsPath(){return fJobResultsPath;}
    const string & GetRunListDB(){return fRunListDB;}
    const string & GetFileList(){return fFileList;}
    const string & GetProdPath(){return fProdPath;}
    const string & GetTrigDefFile(){return fTrigDefFile;}
    const string & GetStreamPrefix(){return fStreamPrefix;}
    const string & GetFillNoDB(){return fFillNoDB;}

    bool EnableEvent(){return fUseEvt == 1;}
    bool EnableTpc(){return fUseTpc == 1;}
    bool EnableEmc(){return fUseEmc == 1;}
    bool EnableFms(){return fUseFms == 1;}
    bool EnableRps(){return fUseRps == 1;}
    bool EnableEEmc(){return fUseEEmc == 1;}
    
    ClassDef(TStConfig,1)
};

#endif
