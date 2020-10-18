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
//_________________________________________________________________________
TStFillNoDB::TStFillNoDB()
{

}
//_________________________________________________________________________
TStFillNoDB::~TStFillNoDB()
{

}

//_________________________________________________________________________
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
//_________________________________________________________________________
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
//_________________________________________________________________________
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
//_________________________________________________________________________
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
//_________________________________________________________________________
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
//_________________________________________________________________________
Int_t TStFillNoDB::GetFillNo(Int_t runNo)
{
    TStar::ExitIfInvalid((TString)TStar::Config->GetFillNoDB());
    std::ifstream i(TStar::Config->GetFillNoDB());
    json j;
    i >> j;

    Int_t diff = 99999;
    Int_t closestFill = -1;  //In case exact match for run number is not found
    
    for(int k = 0; k < j.size(); ++k)
    {
    	for(int r = 0; r < j[k]["run"].size(); ++r)
	{
    	    if(j[k]["run"][r] == runNo)
		return j[k]["fill"];

	    if(abs((int)j[k]["run"][r] - runNo) < diff)
	    {
		closestFill = j[k]["fill"];
		diff = abs((int)j[k]["run"][r] - runNo);
	    }
	}
    }

    cout << "Exact fill number not found. Returning closest fill number:"<< closestFill <<endl;
    return closestFill;
    i.close();    
}
//_________________________________________________________________________
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
//_________________________________________________________________________
void TStFillNoDB::GetFillPolarization(Int_t fill, Double_t &energy, Int_t &startTime, Int_t &stopTime, Double_t &p_b, Double_t &dp_b, Double_t &dpdt_b, Double_t &edpdt_b, Double_t &p_y, Double_t &dp_y, Double_t &dpdt_y, Double_t &edpdt_y)
{
    string polDb = TStar::gConfig->GetPolDB();
    ifstream polFile(polDb);
    if(!polFile)
    {
	cout << "Polarization data file NOT found" <<endl;
	return;
    }
    Int_t tFill; // prefix t to dindicate temporary buffer
    Double_t tEnergy;
    Int_t tStartTime;
    Int_t tStopTime;
    Double_t tP_b;
    Double_t tDp_b;
    Double_t tDpdt_b;
    Double_t tEdpdt_b;
    Double_t tP_y;
    Double_t tDp_y;
    Double_t tDpdt_y;
    Double_t tEdpdt_y;

    while (!polFile.eof()) 
    {
	polFile >> tFill >> tEnergy >> tStartTime >> tStopTime >> tP_b >> tDp_b >> tDpdt_b >> tEdpdt_b >> tP_y >> tDp_y >> tDpdt_y >> tEdpdt_y;

	if(tFill == fill)
	{
	    energy = tEnergy;
	    startTime = tStartTime;
	    stopTime = tStopTime;

	    p_b = tP_b;
	    dp_b = tDp_b;
	    dpdt_b = tDpdt_b;
	    edpdt_b = tEdpdt_b;

	    p_y = tP_y;
	    dp_y = tDp_y;
	    dpdt_y = tDpdt_y;
	    edpdt_y = tEdpdt_y;
	    
	    break;
	}
    }

    polFile.close();
    
}
