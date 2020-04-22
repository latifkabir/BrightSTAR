// Filename: TStFmsHotChDB.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Apr 10 15:24:42 2020 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>
#include <string>

#include "json.h"
#include "TStConfig.h"
#include "TStar.h"
#include "TString.h"

#include "TStRunList.h"
#include "TStFillNoDB.h"
#include "TStFmsHotChDB.h"

using namespace std;
using json = nlohmann::json;

ClassImp(TStFmsHotChDB)

TStFmsHotChDB::TStFmsHotChDB()
{

}

TStFmsHotChDB::~TStFmsHotChDB()
{

}

void TStFmsHotChDB::GetHotChList(Int_t runNo, Bool_t isHotCh[][571])
{
    TStFillNoDB db;
    Int_t fillNo = db.GetFillNo(runNo);

    TStar::ExitIfInvalid((TString)TStar::Config->GetFmsHotChDB());
    std::ifstream i(TStar::Config->GetFmsHotChDB());
    json j;
    i >> j;

    Int_t det;
    Int_t ch;
    for(Int_t k = 0; k < j.size(); ++k)
    {
	if(j[k]["fill"] == fillNo)
	{
	    
	    for(int r = 0; r < j[k]["hot"].size(); ++r)
	    {
		det = j[k]["hot"][r]["det"];
		ch = j[k]["hot"][r]["ch"];
		if((det - 8) < 4 && (ch - 1) < 571)
		    isHotCh[det - 8][ch -1] = 1;
	    }

	    for(Int_t r = 0; r < j[k]["bad"].size(); ++r)
	    {
		det = j[k]["bad"][r]["det"];
		ch = j[k]["bad"][r]["ch"];
		if((det - 8) < 4 && (ch - 1) < 571)
		    isHotCh[det - 8][ch -1] = 1;
	    }
	    break;
	}
    }
    
    i.close();    
}


void TStFmsHotChDB::PrintHotChDB()
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetFmsHotChDB());
    std::ifstream i(TStar::Config->GetFmsHotChDB());
    json j;
    i >> j;
    
    for(int k = 0; k < j.size(); ++k)
    {
    	std::cout <<"Fill No: "<<j[k]["fill"]<< std::endl;
    	std::cout <<"Hot Channels: "<< std::endl;
    	for(int r = 0; r < j[k]["hot"].size(); ++r)
    	    std::cout <<"det: "<<j[k]["hot"][r]["det"] <<" ch:"<< j[k]["hot"][r]["ch"] <<endl;
    	std::cout <<"\n---------------------------------------------------------------------"<< std::endl;

	std::cout <<"Bad Channels: "<< std::endl;
    	for(int r = 0; r < j[k]["bad"].size(); ++r)
    	    std::cout <<"det: "<<j[k]["bad"][r]["det"] <<" ch:"<< j[k]["bad"][r]["ch"] <<endl;
    	std::cout <<"\n---------------------------------------------------------------------"<< std::endl;
    }
    
    i.close();
}

