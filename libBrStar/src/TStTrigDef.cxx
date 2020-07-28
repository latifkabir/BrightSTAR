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

#include "TSQLServer.h"
#include "TSQLRow.h"
#include "TSQLResult.h"
#include "TEntryList.h"

#include "TStRunList.h"


using namespace std;
using json = nlohmann::json;

ClassImp(TStTrigDef)
//____________________________________________________________________________
TStTrigDef::TStTrigDef()
{

}
//____________________________________________________________________________
TStTrigDef::~TStTrigDef()
{
    
}

//____________________________________________________________________________
Int_t TStTrigDef::GetTrigId(Int_t runNumber, TString trigger_name)
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetTrigDefFile());
    std::ifstream i(TStar::Config->GetTrigDefFile());
    json j;
    i >> j;

    Int_t trigId = -1;
    
    if(j.find(to_string(runNumber)) == j.end())
	trigId = GetNearestTrigId(runNumber, trigger_name);
    else if(j[to_string(runNumber)].find(trigger_name.Data()) == j[to_string(runNumber)].end())
	trigId = GetNearestTrigId(runNumber, trigger_name);
    else
	trigId = j[to_string(runNumber)][trigger_name.Data()];
    
    i.close();

    if(trigId > 0)
	return trigId;
    else
    {
	cout << "Invalid Trigger Request" <<endl;
	TStar::Exit();
    }    
}

//____________________________________________________________________________
Int_t TStTrigDef::GetNearestTrigId(Int_t runNumber, TString trigger_name)
{
    TStRunList *runList = new TStRunList();
    TEntryList *list = runList->GetRunList();
    Int_t index = runList->GetRunIndex(runNumber);

    if(index == -1)
    {
	cout<< "The run could not be found in the current run list" << endl;
	TStar::Exit();          
    }

    TStar::ExitIfInvalid((TString)TStar::Config->GetTrigDefFile());
    std::ifstream i(TStar::Config->GetTrigDefFile());
    json j;
    i >> j;

    Bool_t isFound = kFALSE;    
    Int_t trigId = -1;

    while(!isFound)
    {
	if(index < 0)
	    break;
	runNumber = list->GetEntry(index);
	
	if(j.find(to_string(runNumber)) == j.end())
	{
	    --index;
	    continue;
	}

	if(j[to_string(runNumber)].find(trigger_name.Data()) == j[to_string(runNumber)].end())
	{
	    --index;
	    continue;
	}
	trigId = j[to_string(runNumber)][trigger_name.Data()];
	isFound = kTRUE;
	break;
    }
    i.close();

    if(trigId > 0)
	return trigId;
    else
    {
	cout << "Invalid Trigger Request" <<endl;
	TStar::Exit();
    }            
}

//____________________________________________________________________________
void TStTrigDef::MakeTrigIdDB() 
{
    TSQLServer *db = TSQLServer::Connect("mysql://db04.star.bnl.gov:3414/Conditions_rts?timeout=60","", ""); //For run 15 only, change to read server from configuration file.
 
    TSQLRow *row;
    TSQLResult *res;

    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList();
    Int_t nEntries = runList->GetN();
    Int_t run;
    
    ofstream outFile("TriggerIdDB.json");
    if(!outFile)
    {
	cout << "Unable to create output file" <<endl;
	return;
    }
    
    outFile << "{" <<endl;
    for(Int_t index = 0; index < nEntries; ++index)
    {
	run = runList->GetEntry(index);
	TString sql = Form("SELECT idx_rn,offlineBit,name FROM triggers WHERE idx_rn=%d", run);
	res = db->Query(sql);
 
	int nrows = res->GetRowCount(); 
	int nfields = res->GetFieldCount();
       	  
	for (int i = 0; i < nrows; i++)
	{
	    row = res->Next();

	    if(i == 0)
		outFile<<"\t\""<<row->GetField(0)<<"\": {";
	    outFile<<"\""<<row->GetField(2)<<"\": "<<row->GetField(1);
	    if(i != (nrows -1))
		outFile << ", " ;
	    if(i == (nrows -1))
		outFile << "}";
	    if(i == (nrows -1) && index != (nEntries - 1))
		outFile << ", " <<endl;
	}
    }
    outFile << "\n}" <<endl;
    outFile.close();
    delete row;
    delete res; //Must delete manually
    delete db;  //Must delete manually       
}
