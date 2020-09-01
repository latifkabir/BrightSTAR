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

    if(fillNo == -1)
    {
	cout << "-----------> Unable to get valid fill number <------------" <<endl;
	cout << "Hot Channel List Will be Empty" <<endl;
	return;
    }
    
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

    //----------- Mask beam background / ring of fire channels -----------------
    
    //(11, 284) and its surrrounding
    isHotCh[11 - 8][271 - 1] = 1;
    isHotCh[11 - 8][272 - 1] = 1;
    isHotCh[11 - 8][273 - 1] = 1;
    isHotCh[11 - 8][283 - 1] = 1;
    isHotCh[11 - 8][284 - 1] = 1;
    isHotCh[11 - 8][285 - 1] = 1;
    isHotCh[9 - 8][429 - 1] = 1;
    isHotCh[9 - 8][430 - 1] = 1;
    isHotCh[9 - 8][431 - 1] = 1;

    //(11, 286) and its surrounding
    isHotCh[11 - 8][273 - 1] = 1;
    isHotCh[11 - 8][274 - 1] = 1;
    isHotCh[11 - 8][275 - 1] = 1;
    isHotCh[11 - 8][285 - 1] = 1;
    isHotCh[11 - 8][286 - 1] = 1;
    isHotCh[11 - 8][287 - 1] = 1;
    isHotCh[9 - 8][431 - 1] = 1;
    isHotCh[9 - 8][432 - 1] = 1;
    isHotCh[9 - 8][433 - 1] = 1;	    

   
    isHotCh[11 - 8][218 - 1] = 1;
    isHotCh[11 - 8][219 - 1] = 1;
    
    isHotCh[10 - 8][282 - 1] = 1;
    isHotCh[10 - 8][283 - 1] = 1;
    isHotCh[10 - 8][284 - 1] = 1;
    isHotCh[10 - 8][285 - 1] = 1;
    isHotCh[10 - 8][286 - 1] = 1;
    isHotCh[10 - 8][274 - 1] = 1;
    isHotCh[10 - 8][275 - 1] = 1;
    isHotCh[10 - 8][262 - 1] = 1;
    isHotCh[10 - 8][263 - 1] = 1;
    
    isHotCh[10 - 8][19 - 1] = 1;
    isHotCh[10 - 8][20 - 1] = 1;
    isHotCh[10 - 8][31 - 1] = 1;
    isHotCh[10 - 8][32 - 1] = 1;
    isHotCh[10 - 8][69 - 1] = 1;
    
    isHotCh[11 - 8][92 - 1] = 1;
    isHotCh[11 - 8][93 - 1] = 1;
    isHotCh[11 - 8][82 - 1] = 1;
    isHotCh[11 - 8][81 - 1] = 1;
    isHotCh[11 - 8][6 - 1] = 1;
    isHotCh[11 - 8][7 - 1] = 1;
    isHotCh[11 - 8][8 - 1] = 1;
    isHotCh[11 - 8][34 - 1] = 1;
    isHotCh[11 - 8][35 - 1] = 1;

    isHotCh[10 - 8][276 - 1] = 1;
    isHotCh[10 - 8][275 - 1] = 1;


    


    isHotCh[11][34] = 1;
    
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


void TStFmsHotChDB::HotChCounter()
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetFmsHotChDB());
    std::ifstream i(TStar::Config->GetFmsHotChDB());
    json j;
    i >> j;
    
    for(int k = 0; k < j.size(); ++k)
    {
    	std::cout <<"Fill No: "<<j[k]["fill"] << ", Number of hot + bad channels: " << j[k]["hot"].size() << " and " << j[k]["bad"].size() << ", Total:" << (j[k]["hot"].size() + j[k]["bad"].size()) << std::endl;
    }
    
    i.close();
}

