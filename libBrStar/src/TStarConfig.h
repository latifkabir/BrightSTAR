// Filename: TStarConfig.h
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Sun Sep  9 23:29:52 2018 (-0400)
// URL: jlab.org/~latif

#ifndef TSTARCONFIG_H
#define TSTARCONFIG_H

#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cassert>
#include "TObject.h"

using namespace std;

class TStarConfig : public TObject
{
    //-----Buffer for each configuration field to be read goes here------
    string fConfigFile;
    string fDataPath;
    string fResultsPath;
    string fVaultPath;
    string fDSTreadPath;
    string fDSTwritePath;
    string fStarHome;
    
    void CheckValidity();    
public:
    TStarConfig(string file );
    TStarConfig();
    ~TStarConfig();
    void LoadConfig();
    void Print();

    //----- Getter for each configuration field -------------
    const string & GetStarHome();
    const string & GetConfigPath();
    const string & GetDataPath();
    const string & GetResultsPath();
    const string & GetVaultPath();
    const string & GetDSTreadPath();
    const string & GetDSTwritePath();
    
    ClassDef(TStarConfig,0)
};

#endif
