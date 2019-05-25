// Filename: TStarConfig.cc
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Sun Sep  9 23:29:47 2018 (-0400)
// URL: jlab.org/~latif

#include "TStarConfig.h"
#include "TStar.h"
#include "TApplication.h"
using namespace std;

ClassImp(TStarConfig)

TStarConfig::TStarConfig()
{
    if(getenv("STARHOME") == nullptr)
    {
    	cout << "\nThe environment STARHOME must be set" <<endl;
    	gApplication->Terminate();
    }
    else
	fStarHome = getenv("STARHOME");	
    fConfigFile = fStarHome + (string)"/config/config.cfg"; 
    fDataPath = "./";
    fResultsPath = "./";
    fVaultPath = "./";
    fDSTreadPath = "./";
    fDSTwritePath = "./";
    LoadConfig();
}

TStarConfig::TStarConfig(string file)
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

TStarConfig::~TStarConfig()
{
    
}

void TStarConfig::LoadConfig()
{
    ifstream configFile;
    configFile.open( fConfigFile, ifstream::in);
    if(!configFile)
        cout<<"TStarConfig: Config file NOT found."<<endl;
    
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
	    else
	    {
		tokens = strtok(NULL, " :,");
	    }
	}
    }
    configFile.close();
}

const string& TStarConfig::GetDataPath()
{
    return fDataPath;
}

const string& TStarConfig::GetResultsPath()
{
    return fResultsPath;
}

const string& TStarConfig::GetVaultPath()
{
    return fVaultPath;
}

const string& TStarConfig::GetDSTreadPath()
{
    return fDSTreadPath;
}

const string& TStarConfig::GetDSTwritePath()
{
    return fDSTwritePath;
}

const string& TStarConfig::GetStarHome()
{
    return fStarHome;
}

const string& TStarConfig::GetConfigPath()
{
    return fConfigFile;
}


void TStarConfig::Print()
{
    cout << "==============Star Configuration===================" <<endl;
    cout << "Star Home: "<< GetStarHome() <<endl;
    cout << "Config file: "<< GetConfigPath()<<endl;
    cout << "Data-file path: "<< GetDataPath() <<endl;
    cout << "Results path: "<< GetResultsPath() <<endl;
    cout << "Vault path: "<< GetVaultPath() <<endl;
    cout << "DST read path: "<< GetDSTreadPath() <<endl;
    cout << "DST write path: "<< GetDSTwritePath() <<endl;
    cout << "==================================================" <<endl;    
}

void TStarConfig::CheckValidity()
{

}
