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
    string fBrightHome;
    string fDataPath;
    string fTempPath;
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
    string fFmsHotChDB;
    string fUserEmail;
    string fCondorConfig;
    string fCondorJob;
    string fCondorJobSh;
    string fSumsJobSh;
    string fSumsConfig;
    string fJobCounterSh;
    string fJobCounterTxt;
    string fXrootdPrefix;
    
    int fUseEvt;
    int fUseTpc;
    int fUseEmc;
    int fUseFms;
    int fUseRps;
    int fUseEEmc;
    
public:
    TStConfig(string file );
    TStConfig();
    ~TStConfig();
    void LoadConfig();
    void Print();
    void PrintAll();
    bool CheckValidity(int print = 0);    

    //----- Getter for each configuration field -------------
    const string & GetBrightHome(){return fBrightHome;}
    const string & GetStarHome(){return fStarHome;}
    const string & GetConfigPath(){return fConfigFile;}
    const string & GetDataPath(){return fDataPath;}
    const string & GetTempPath(){return fTempPath;}
    const string & GetResultsPath(){return fResultsPath;}
    const string & GetDSTpath(){return fDSTpath;}
    const string & GetJobResultsPath(){return fJobResultsPath;}
    const string & GetRunListDB(){return fRunListDB;}
    const string & GetFileList(){return fFileList;}
    const string & GetProdPath(){return fProdPath;}
    const string & GetTrigDefFile(){return fTrigDefFile;}
    const string & GetStreamPrefix(){return fStreamPrefix;}
    const string & GetFillNoDB(){return fFillNoDB;}
    const string & GetFmsHotChDB(){return fFmsHotChDB;}
    const string & GetUserEmail(){return fUserEmail;}
    const string & GetCondorConfig(){return fCondorConfig;}
    const string & GetCondorJob(){return fCondorJob;}
    const string & GetCondorJobSh(){return fCondorJobSh;}
    const string & GetSumsJobSh(){return fSumsJobSh;}
    const string & GetSumsConfig(){return fSumsConfig;}
    const string & GetJobCounterSh(){return fJobCounterSh;}
    const string & GetJobCounterTxt(){return fJobCounterTxt;}
    //const string & GetXrootdPrefix(){return fXrootdPrefix;}

    
    bool EnableEvent(){return fUseEvt == 1;}
    bool EnableTpc(){return fUseTpc == 1;}
    bool EnableEmc(){return fUseEmc == 1;}
    bool EnableFms(){return fUseFms == 1;}
    bool EnableRps(){return fUseRps == 1;}
    bool EnableEEmc(){return fUseEEmc == 1;}
    
    ClassDef(TStConfig,1)
};

#endif
