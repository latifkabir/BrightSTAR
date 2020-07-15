// Filename: TStConfig.cc
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep  9 23:29:47 2018 (-0400)
// URL: jlab.org/~latif

#include "TStConfig.h"
#include "TApplication.h"
#include "TSystem.h"
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
    //For some terminal, it does not get the environment correctly. For example from SUMS job
    //Let's set it to current directory for those cases.
    if(fStarHome == "" || fStarHome == "/")
    {
	fStarHome = gSystem->pwd();
	cout << "\nThe environment STARHOME is set to: " << fStarHome<<endl;
    }
    fConfigFile = fStarHome + (string)"/config/config.cfg";
    //Following are initial values and are overwritten later from the configuratuion file
    fDataPath = "data/";
    fResultsPath = "results/";
    fDSTpath = "results/";
    fJobResultsPath = "jobResults/";
    fRunListDB = "database/RunList.json";
    fFileList = "resources/temp/temp.list";
    fProdPath = "./";
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
	    else if(s == "DST_PATH")
	    {
		tokens = strtok(NULL, " :,");
		fDSTpath = tokens;
	    }
	    else if(s == "JOB_RESULTS_PATH")
	    {
		tokens = strtok(NULL, " :,");
		fJobResultsPath = tokens;
	    }
	    else if(s == "RUN_LIST_DB")
	    {
		tokens = strtok(NULL, " :,");
		fRunListDB = fStarHome + "/" + tokens;
	    }
	    else if(s == "FILE_LIST")
	    {
		tokens = strtok(NULL, " :,");
		fFileList = fStarHome + "/" + tokens;
	    }
	    else if(s == "PROD_PATH")
	    {
		tokens = strtok(NULL, " :,");
		fProdPath = tokens;
	    }
	    else if(s == "TRIG_DEF_FILE")
	    {
		tokens = strtok(NULL, " :,");
		fTrigDefFile = fStarHome + "/" + tokens;
	    }
	    else if(s == "STREAM_PREFIX")
	    {
		tokens = strtok(NULL, " :,");
		fStreamPrefix = tokens;
	    }
	    else if(s == "FILL_NO_DB")
	    {
		tokens = strtok(NULL, " :,");
		fFillNoDB = fStarHome + "/" + tokens;
	    }
	    else if(s == "FMS_HOT_CH_DB")
	    {
		tokens = strtok(NULL, " :,");
		fFmsHotChDB = fStarHome + "/" + tokens;
	    }
	    else if(s == "ENABLE_EVT")
	    {
		tokens = strtok(NULL, " :,");
		fUseEvt = atoi(tokens);
	    }
	    else if(s == "ENABLE_TPC")
	    {
		tokens = strtok(NULL, " :,");
		fUseTpc = atoi(tokens);
	    }
	    else if(s == "ENABLE_EMC")
	    {
		tokens = strtok(NULL, " :,");
		fUseEmc = atoi(tokens);
	    }
	    else if(s == "ENABLE_FMS")
	    {
		tokens = strtok(NULL, " :,");
		fUseFms = atoi(tokens);
	    }
	    else if(s == "ENABLE_RPS")
	    {
		tokens = strtok(NULL, " :,");
		fUseRps = atoi(tokens);
	    }
	    else if(s == "ENABLE_EEMC")
	    {
		tokens = strtok(NULL, " :,");
		fUseEEmc = atoi(tokens);
	    }
	    else
	    {
		tokens = strtok(NULL, " :,");
	    }
	}
    }
    configFile.close();
}

void TStConfig::Print()
{
    cout << "==============Bright-Star Configuration===================" <<endl;
    cout << "Star Home: "<< GetStarHome()<<"\n"<<endl;
    cout << "Config file: "<< GetConfigPath()<<"\n"<<endl;
    cout << "Data-file path: "<< GetDataPath()<<"\n"<<endl;
    cout << "Results path: "<< GetResultsPath()<<"\n"<<endl;
    cout << "DST path: "<< GetDSTpath()<<"\n"<<endl;
    cout << "Job results path: "<< GetJobResultsPath()<<"\n"<<endl;
    cout << "File-list: "<< GetFileList()<<"\n"<<endl;
    cout << "\033[1;31mRun-list DB file: \033[0m"<< GetRunListDB()<<"\n"<<endl;
    cout << "\033[1;31mFill Number DB file: \033[0m"<< GetFillNoDB()<<"\n"<<endl;
    cout << "\033[1;31mFms Hot Channel DB file: \033[0m"<< GetFmsHotChDB()<<"\n"<<endl;
    cout << "\033[1;31mTrigger Definition file: \033[0m"<< GetTrigDefFile()<<"\n"<<endl;
    cout << "\033[1;31mProduction data path: \033[0m"<< GetProdPath()<<"\n"<<endl;
    cout << "\033[1;31mStream prefix: \033[0m"<< GetStreamPrefix()<<"\n"<<endl;
    cout << "--- Nano DST Branch Status: ----"<<endl;
    cout << "Event:"<< EnableEvent()<<endl;
    cout << "TPC:"<< EnableTpc()<<endl;
    cout << "EMC:"<< EnableEmc()<<endl;
    cout << "FMS:"<< EnableFms()<<endl;
    cout << "RPS:"<< EnableRps()<<endl;
    cout << "EEMC:"<< EnableEEmc()<<endl;
    cout << "==================================================" <<endl;    
}

void TStConfig::CheckValidity()
{

}
