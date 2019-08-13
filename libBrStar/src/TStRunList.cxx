// Filename: TStRunList.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed May 29 00:24:18 2019 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>
#include <string>

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
    std::cout<<"\tRun Number\t\tFile Name\t\tnumber of events"<< std::endl;
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

    Int_t nFiles = 0;
    std::cout <<"---------------------------------------------------------------------\n"<< std::endl;
    std::cout<<"\tRun Number\t\tFile Name\t\tnumber of events"<< std::endl;
    std::cout <<"---------------------------------------------------------------------\n"<< std::endl;
    for(int i = 0; i < j.size(); ++i)
    {
	if(j[i]["run"] >= firstRun && j[i]["run"] <= lastRun)
	{
	    std::cout <<""<<j[i]["run"]<<"\t"<<j[i]["data"]["file"]<<"\t"<<j[i]["data"]["events"]<< std::endl;
	    std::cout <<"---------------------------------------------------------------------"<< std::endl;
	    ++nFiles;
	}
    }
    cout << "Number of files in the range:"<<nFiles<<endl;
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
	if(j[i]["data"]["events"] >= minEvents)
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

Int_t TStRunList::MakeFileList(Int_t firstRun, Int_t lastRunOrNfiles)
{
    Int_t lastRun = -1;
    Int_t limit = -1;
    Int_t fileCount = 0;
    if(lastRunOrNfiles == -1)
	lastRun = firstRun;
    else if(lastRunOrNfiles != -1 && lastRunOrNfiles < firstRun)
    {
	limit = lastRunOrNfiles;
	lastRun = firstRun;
    }
    else if(lastRunOrNfiles >= firstRun)
	lastRun = lastRunOrNfiles;
    
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    std::ofstream fileList(TStar::Config->GetFileList());
    const char *rNumber;
    for(int i = 0; i < j.size(); ++i)
    {
	if(j[i]["run"] >= firstRun && j[i]["run"] <= lastRun)
	{
	    rNumber =  (std::to_string((int)j[i]["run"])).c_str();
	    std::cout<<"root://xrdstar.rcf.bnl.gov:1095/"<<TStar::Config->GetProdPath()<<rNumber[2]<<rNumber[3]<<rNumber[4]<<"/"<<(int)j[i]["run"]<<"/"<<(string)j[i]["data"]["file"]<< std::endl;
	    fileList<<"root://xrdstar.rcf.bnl.gov:1095/"<<TStar::Config->GetProdPath()<<rNumber[2]<<rNumber[3]<<rNumber[4]<<"/"<<(int)j[i]["run"]<<"/"<<(string)j[i]["data"]["file"]<< std::endl;
	    ++fileCount;
	    if(limit == fileCount)
		break;
        }
    }
    fileList<<std::endl;
    cout << "File-list has been generated!" <<endl;
    i.close();
    fileList.close();
    return fileCount;
}

Int_t TStRunList::PrintFileList(Int_t firstRun, Int_t lastRunOrNfiles)
{
    Int_t lastRun = -1;
    Int_t limit = -1;
    Int_t fileCount = 0;
    if(lastRunOrNfiles == -1)
	lastRun = firstRun;
    else if(lastRunOrNfiles != -1 && lastRunOrNfiles < firstRun)
    {
	limit = lastRunOrNfiles;
	lastRun = firstRun;
    }
    else if(lastRunOrNfiles >= firstRun)
	lastRun = lastRunOrNfiles;
    
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    const char *rNumber;
    for(int i = 0; i < j.size(); ++i)
    {
	if(j[i]["run"] >= firstRun && j[i]["run"] <= lastRun)
	{
	    rNumber =  (std::to_string((int)j[i]["run"])).c_str();
	    std::cout<<"root://xrdstar.rcf.bnl.gov:1095/"<<TStar::Config->GetProdPath()<<rNumber[2]<<rNumber[3]<<rNumber[4]<<"/"<<(int)j[i]["run"]<<"/"<<(string)j[i]["data"]["file"]<< std::endl;
	    ++fileCount;
	    if(limit == fileCount)
		break;
        }
    }
    i.close();
    return fileCount;
}

void TStRunList::PrintFileList()
{
    std::ifstream fileList(TStar::Config->GetFileList());
    string str;
    if(!fileList)
    {
	std::cout << "Source file NOT found" <<std::endl;
	return;
    }

    while(std::getline(fileList,str))
    {
	std::cout << str <<std::endl;
    }
}

Int_t TStRunList::MakeFileListWithEvents(Int_t minEvents)
{        
    TStar::ExitIfInvalid((TString)TStar::Config->GetRunListDB());
    std::ifstream i(TStar::Config->GetRunListDB());
    json j;
    i >> j;

    std::ofstream fileList(TStar::Config->GetFileList());
    const char *rNumber;
    Int_t fileCount = 0;
    for(int i = 0; i < j.size(); ++i)
    {
	if(j[i]["data"]["events"] >= minEvents)
	{
	    rNumber =  (std::to_string((int)j[i]["run"])).c_str();
	    std::cout<<"root://xrdstar.rcf.bnl.gov:1095/"<<TStar::Config->GetProdPath()<<rNumber[2]<<rNumber[3]<<rNumber[4]<<"/"<<(int)j[i]["run"]<<"/"<<(string)j[i]["data"]["file"]<< std::endl;
	    fileList<<"root://xrdstar.rcf.bnl.gov:1095/"<<TStar::Config->GetProdPath()<<rNumber[2]<<rNumber[3]<<rNumber[4]<<"/"<<(int)j[i]["run"]<<"/"<<(string)j[i]["data"]["file"]<< std::endl;
	    ++fileCount;
        }
    }
    fileList<<std::endl;
    cout << "File-list has been generated!" <<endl;
    i.close();
    fileList.close();
    return fileCount;
}

