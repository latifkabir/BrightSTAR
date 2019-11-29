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
    
    void CheckValidity();    
public:
    TStConfig(string file );
    TStConfig();
    ~TStConfig();
    void LoadConfig();
    void Print();

    //----- Getter for each configuration field -------------
    const string & GetStarHome();
    const string & GetConfigPath();
    const string & GetDataPath();
    const string & GetResultsPath();
    const string & GetFilePath();
    const string & GetDSTpath();
    const string & GetJobResultsPath();
    const string & GetRunListDB();
    const string & GetFileList();
    const string & GetProdPath();
    const string & GetTrigDefFile();
    const string & GetStreamPrefix();
    TString GetRootFileName();
    
    ClassDef(TStConfig,1)
};

#endif
