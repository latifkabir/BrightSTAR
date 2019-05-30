// Filename: TStRunList.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed May 29 00:24:18 2019 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>

#include "json.h"
#include "TStRunList.h"
#include "TStConfig.h"
#include "TStar.h"
#include "TString.h"

using namespace std;
using json = nlohmann::json;

ClassImp(TStRunList)

TStRunList::TStRunList()
{

}

TStRunList::~TStRunList()
{
    
}

void TStRunList::PrintRunList()
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    std::cout <<"---------------------------------------------------------------------\n"<< std::endl;
    std::cout<<"\tRun Number\t\t\t\tFile Name\t\t\t\tnumber of events"<< std::endl;
    std::cout <<"---------------------------------------------------------------------\n"<< std::endl;
    for(int i = 0; i < j.size(); ++i)
    {
	std::cout <<""<<j[i]["run"]<<"\t"<<j[i]["data"]["file"]<<"\t"<<j[i]["data"]["events"]<< std::endl;
	std::cout <<"---------------------------------------------------------------------"<< std::endl;
    }
    i.close();
}

void TStRunList::PrintRunList(Int_t firstRun, Int_t lastRun)
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    std::cout <<"---------------------------------------------------------------------\n"<< std::endl;
    std::cout<<"\tRun Number\t\t\t\tFile Name\t\t\t\tnumber of events"<< std::endl;
    std::cout <<"---------------------------------------------------------------------\n"<< std::endl;
    for(int i = 0; i < j.size(); ++i)
    {
	if(j[i]["run"] >= firstRun && j[i]["run"] <= lastRun)
	{
	    std::cout <<""<<j[i]["run"]<<"\t"<<j[i]["data"]["file"]<<"\t"<<j[i]["data"]["events"]<< std::endl;
	    std::cout <<"---------------------------------------------------------------------"<< std::endl;
	}
    }
    i.close();
}

void TStRunList::PrintRunList(Int_t minEvents)
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    std::cout <<"---------------------------------------------------------------------\n"<< std::endl;
    std::cout<<"\tRun Number\t\tFile Name\t\tNumber of Events"<< std::endl;
    std::cout <<"---------------------------------------------------------------------\n"<< std::endl;
    for(int i = 0; i < j.size(); ++i)
    {
	if(j[i]["data"]["events"] >= minEntries)
	{
	    std::cout <<""<<j[i]["run"]<<"\t"<<j[i]["data"]["file"]<<"\t"<<j[i]["data"]["events"]<< std::endl;
	    std::cout <<"---------------------------------------------------------------------"<< std::endl;
	}
    }
    i.close();
}

void TStRunList::DumpRunList()
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    std::cout << j.dump(4) << std::endl;
    i.close();
}

Int_t TStRunList::GetFirstRun()
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    Int_t firstRun = j[0]["run"];    
    i.close();
    return firstRun;
}

Int_t TStRunList::GetLastRun()
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    Int_t lastRun = j[j.size() - 2]["run"];    
    i.close();
    return lastRun;
}