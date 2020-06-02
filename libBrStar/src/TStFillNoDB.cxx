// Filename: TStFillNoDB.cxx
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

using namespace std;
using json = nlohmann::json;

ClassImp(TStFillNoDB)

TStFillNoDB::TStFillNoDB()
{

}

TStFillNoDB::~TStFillNoDB()
{

}


//Use the shell script to generate txt DB and use this script to generate JSON DB from text DB
void TStFillNoDB::GenerateFillDB(TString inFile)
{
    ifstream inTxtFile(inFile);
    if(!inTxtFile)
    {
	cout << "Inut file not fund" <<endl;
	return;
    }
    Int_t runNo;
    Int_t FillNo;
    Int_t timeDiff;
    Int_t currentFillNo = 0;
    Int_t previousFillNo = 0;
    Int_t runCount = 0;
    
    ofstream outFile("FillNumberDB.json");
    if(!outFile)
    {
	cout << "Unable to create outfile" <<endl;
	return;
    }

    outFile << "[" << endl;
    while(!inTxtFile.eof())
    {
	inTxtFile >> runNo >> FillNo >> timeDiff;

	if(inTxtFile.eof())
	{
	    outFile << "]}" << endl;
	    outFile << "]" << endl;
	    break;
	}
	currentFillNo = FillNo;

	if(currentFillNo == previousFillNo)
	{
	    outFile << ", "<< runNo;
	}
	else
	{
	    if(runCount > 0)
		outFile << "]}," << endl;
	    outFile << "{\"fill\":" << currentFillNo << ", \"run\": [" << runNo; 
	}
	previousFillNo = currentFillNo;
	++runCount;	
    }

    inTxtFile.close();
    outFile.close();
}

void TStFillNoDB::PrintFillNoDB()
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetFillNoDB());
    std::ifstream i(TStar::Config->GetFillNoDB());
    json j;
    i >> j;
    
    for(int k = 0; k < j.size(); ++k)
    {
    	std::cout <<"Fill No: "<<j[k]["fill"]<< std::endl;
    	std::cout <<"Runs: "<< std::endl;
    	for(int r = 0; r < j[k]["run"].size(); ++r)
    	    std::cout <<j[k]["run"][r] << "    ";
    	std::cout <<"\n---------------------------------------------------------------------"<< std::endl;
    }
    
    i.close();
}

map <int, vector<int> > TStFillNoDB::GetFillNoDB()
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetFillNoDB());
    std::ifstream i(TStar::Config->GetFillNoDB());
    json j;
    i >> j;

    map <int, vector<int> > fillNoDB;
    vector <Int_t> runs;
    
    for(int k = 0; k < j.size(); ++k)
    {
	runs.clear();
    	for(int r = 0; r < j[k]["run"].size(); ++r)
    	    runs.push_back(j[k]["run"][r]);
	
	fillNoDB[j[k]["fill"]] = runs;	
    }
    
    i.close();

    return fillNoDB;
}

void TStFillNoDB::ReadFillNoDB()
{
    map <int, vector <int> > db = GetFillNoDB();
    map <int, vector <int> >::iterator it;

    for(it = db.begin(); it!= db.end(); ++it)
    {
	cout << "Fill: "<< it->first <<endl;
	cout << "Runs:" <<endl;
	for(int i = 0; i < it->second.size(); ++i)
	    cout << it->second[i]<<"    " <<endl;
    }
    //cout << db[18856].size() <<endl;
}

vector <int> TStFillNoDB::GetRunsWithFill(Int_t fillNo)
{
    map <int, vector <int> > db = GetFillNoDB();
    map <int, vector <int> >::iterator it;
    vector <int> vec;

    it = db.find(fillNo);
    if(it == db.end())
	return vec;
    else
	vec = it->second; //same as db[fillNo]
    return vec;
}

Int_t TStFillNoDB::GetFillNo(Int_t runNo)
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetFillNoDB());
    std::ifstream i(TStar::Config->GetFillNoDB());
    json j;
    i >> j;
    
    for(int k = 0; k < j.size(); ++k)
    {
    	for(int r = 0; r < j[k]["run"].size(); ++r)
	{
    	    if(j[k]["run"][r] == runNo)
		return j[k]["fill"];
	}
    }

    return -1;
    i.close();    
}
//--------------------------------------------
vector<int> TStFillNoDB::GetAllFillNos()
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetFillNoDB());
    std::ifstream i(TStar::Config->GetFillNoDB());
    json j;
    i >> j;
    vector <int> vec;
    for(int k = 0; k < j.size(); ++k)
    {
	vec.push_back(j[k]["fill"]);
    }
    
    i.close();

    return vec;
}
