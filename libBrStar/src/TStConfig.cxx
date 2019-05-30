// Filename: TStConfig.cc
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep  9 23:29:47 2018 (-0400)
// URL: jlab.org/~latif

#include "TStConfig.h"
#include "TStar.h"
#include "TApplication.h"
using namespace std;

ClassImp(TStConfig)

TStConfig::TStConfig()
{
    if(getenv("STARHOME") == nullptr)
    {
    	cout << "\nThe environment STARHOME must be set" <<endl;
    	gApplication->Terminate();
    }
    else
	fStarHome = getenv("STARHOME");	
    fConfigFile = fStarHome + (string)"/config/config.cfg"; 
    fDataPath = "./data";
    fResultsPath = "./results";
    fVaultPath = "./results";
    fDSTreadPath = "./results";
    fDSTwritePath = "./results";
    fRunListDB = "./resources/RunList.json";
    LoadConfig();
}

TStConfig::TStConfig(string file)
{
    if(getenv("STARHOME") == nullptr)
    {
    	cout << "\nThe environment STARHOME must be set" <<endl;
    	gApplication->Terminate();
    }
    else
	fStarHome = getenv("STARHOME");	

    fConfigFile = file;
    LoadConfig();
}

TStConfig::~TStConfig()
{
    
}

void TStConfig::LoadConfig()
{
    ifstream configFile;
    configFile.open( fConfigFile, ifstream::in);
    if(!configFile)
        cout<<"TStConfig: Config file NOT found."<<endl;
    
    string line;
    while( getline(configFile, line) )
    {
	line.erase( remove_if(line.begin(), line.end(), ::isspace), line.end() ); // strip spaces
	if(line.find("#") == 0) continue; //skip comments

	char* tokens = strtok( (char*)line.data(), " :,");
	while( tokens != NULL)
	{
	    string s = tokens;
	    if(s == "DATA_PATH")
	    {
		tokens = strtok(NULL, " :,");
		fDataPath = tokens;
	    }
	    else if(s == "RESULTS_PATH")
	    {
		tokens = strtok(NULL, " :,");
		fResultsPath = tokens;
	    }
	    else if(s == "VAULT_PATH")
	    {
		tokens = strtok(NULL, " :,");
		fVaultPath = tokens;
	    }
	    else if(s == "DST_READ_PATH")
	    {
		tokens = strtok(NULL, " :,");
		fDSTreadPath = tokens;
	    }
	    else if(s == "DST_WRITE_PATH")
	    {
		tokens = strtok(NULL, " :,");
		fDSTwritePath = tokens;
	    }
	    else if(s == "RUN_LIST_DB")
	    {
		tokens = strtok(NULL, " :,");
		fRunListDB = tokens;
	    }
	    else
	    {
		tokens = strtok(NULL, " :,");
	    }
	}
    }
    configFile.close();
}

const string& TStConfig::GetDataPath()
{
    return fDataPath;
}

const string& TStConfig::GetResultsPath()
{
    return fResultsPath;
}

const string& TStConfig::GetFilePath()
{
    return fVaultPath;
}

const string& TStConfig::GetDSTreadPath()
{
    return fDSTreadPath;
}

const string& TStConfig::GetDSTwritePath()
{
    return fDSTwritePath;
}

const string& TStConfig::GetStarHome()
{
    return fStarHome;
}

const string& TStConfig::GetConfigPath()
{
    return fConfigFile;
}

const string& TStConfig::GetRunListDB()
{
    return fRunListDB;
}


void TStConfig::Print()
{
    cout << "==============Star Configuration===================" <<endl;
    cout << "Star Home: "<< GetStarHome() <<endl;
    cout << "Config file: "<< GetConfigPath()<<endl;
    cout << "Data-file path: "<< GetDataPath() <<endl;
    cout << "Results path: "<< GetResultsPath() <<endl;
    cout << "Vault path: "<< GetFilePath() <<endl;
    cout << "DST read path: "<< GetDSTreadPath() <<endl;
    cout << "DST write path: "<< GetDSTwritePath() <<endl;
    cout << "Run-list DB file: "<< GetRunListDB() <<endl;
    cout << "==================================================" <<endl;    
}

void TStConfig::CheckValidity()
{

}
