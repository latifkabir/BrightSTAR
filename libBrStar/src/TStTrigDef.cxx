// Filename: TStTrigDef.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed May 29 00:24:18 2019 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>
#include <string>

#include "json.h"
#include "TStTrigDef.h"
#include "TStConfig.h"
#include "TStar.h"
#include "TString.h"

using namespace std;
using json = nlohmann::json;

ClassImp(TStTrigDef)

TStTrigDef::TStTrigDef()
{

}

TStTrigDef::~TStTrigDef()
{
    
}

Int_t TStTrigDef::GetTrigId(Int_t runNumber, TString trigger_name)
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetTrigDefFile());
    std::ifstream i(TStar::Config->GetTrigDefFile());
    json j;
    i >> j;
    
    Int_t trigId = -1;
    trigId = j[to_string(runNumber)][trigger_name.Data()];
    i.close();

    if( trigId > 0)
	return trigId;
    else
    {
	cout << "Invalid Trigger Request" <<endl;
	TStar::Exit();
    }    
}
