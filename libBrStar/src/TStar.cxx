// Filename: TStar.cc
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Mon Jul  9 15:22:01 2018 (-0400)
// URL: jlab.org/~latif

#include <iostream>
#include <fstream>
#include "TSystem.h"
#include "TApplication.h"
#include "TFile.h"
#include "TObject.h"
#include "TKey.h"
#include "TIterator.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TClass.h"
#include "TDictionary.h"
#include "TBrowser.h"
#include "TStar.h"

#include<sstream>
#include<string>
#include "TString.h"
#include "TStRunList.h"


using namespace std;

ClassImp(TStar)

TStar::TStar()
{
    
}

TStar::~TStar()
{
    
}

TStConfig* TStar::Config = new TStConfig();
TStConfig* TStar::gConfig = Config;

//TStRunList uses TStar::Config, so that should be initialized first. 
//This will make library loading slow (to read large json file)
Int_t TStar::gFirstRun = TStRunList::GetFirstRun();  
Int_t TStar::gLastRun = TStRunList::GetLastRun();

Bool_t TStar::gBrDebug = kFALSE;
Bool_t TStar::gBrInfo = kTRUE;

void TStar::Exit()
{
    gApplication->Terminate();
}

void TStar::ExitIfInvalid(TString filePath)
{
    if(gSystem->AccessPathName(filePath))
    {
	cout << "Invalid path: "<<filePath<<endl;
	Exit();
    }
}

Bool_t TStar::IsValid(TString filePath)
{
    if(gSystem->AccessPathName(filePath))
    {
	cout << "Invalid path: "<<filePath<<endl;
	return kFALSE;
    }
    return kTRUE;
}

void TStar::view(TString fileName)
{
    //Open the file for reading
    if(!IsValid(fileName))
	return;
    TFile *file = new TFile(fileName);
    Int_t hist_counter = 0;

    //Create an iterator
    TIter nextkey (file->GetListOfKeys());
    TKey *key;

    while ( (key = (TKey*) nextkey() ))
    {
	//Read object from first source file
	TObject *obj = key->ReadObj();

	//Check that the object the key points to is a histogram
	if (obj->IsA()->InheritsFrom("TH2"))
	{
	    //Cast the TObject pointer to a histogram one
	    // TH1 * hist = (TH1*) (obj);
	    
	    TCanvas *c = new TCanvas();
	    obj->Draw("colz");
	}
	else if (obj->IsA()->InheritsFrom("TH1") || obj->IsA()->InheritsFrom("TGraph"))
	{
	    //Cast the TObject pointer to a histogram one
	    // TH1 * hist = (TH1*) (obj);
	    
	    TCanvas *c = new TCanvas();
	    obj->Draw();
	}
	else if(obj->IsA()->InheritsFrom("TCanvas"))
	    obj->Draw();
    }    
}



void TStar::help()
{
    string str;
    TString lineStr;
    str = TStar::Config->GetStarHome() + (string)"/libBrStar/src/";
	
    str = str + "LinkDef.h";
    //cout << str <<endl;

    ifstream inFile(str.c_str());
    if(!inFile)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\n\t\t=========================================================================" <<endl;

    cout << "\t\t|\t\t\t   Classes/Functions inside libBrStar        \t|" <<endl;
    cout << "\t\t=========================================================================" <<endl;
    while(getline(inFile,str))
    {
	//cout << str <<endl;
	lineStr = str.c_str();
	if(str[0]=='/' && str[1]=='/')
	    continue;
	if(str == "#endif")
	    continue;
	if(str == "#ifdef __CINT__")
	    continue;

	cout << lineStr.ReplaceAll(";", "").ReplaceAll("#pragma link C++ ", "").ReplaceAll("//", " : ") <<endl;
    }

    inFile.close();
    cout<< "-------------------------------------------------------------------------------------------" <<endl;  
    cout << "| Type 'help(\"function or class name\")' for details of any specific function or class     |"<<endl;
    cout<<  "-------------------------------------------------------------------------------------------" <<endl;  

    str =  TStar::Config->GetStarHome() + (string)"/analysis/src/";
    str = str + "LinkDef.h";
    //cout << str <<endl;

    ifstream inFile2(str.c_str());
    if(!inFile2)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\t\t=================================================================================" <<endl;

    cout << "\t\t|\t\t\t   Functions inside Analysis   \t\t\t\t|" <<endl;
    cout << "\t\t=================================================================================" <<endl;
  
    while(getline(inFile2,str))
    {
	lineStr = str.c_str();
	if(str[0]=='/' && str[1]=='/')
	    continue;
	if(str == "#endif")
	    continue;
	if(str == "#ifdef __CINT__")
	    continue;
	//cout << str <<endl;
	cout << lineStr.ReplaceAll(";", "").ReplaceAll("#pragma link C++ ", "").ReplaceAll("//", " : ") <<endl;
    }
    inFile2.close();

    str =  TStar::Config->GetStarHome() + (string)"/runMacros/src/";
    str = str + "LinkDef.h";
    //cout << str <<endl;

    ifstream inFile3(str.c_str());
    if(!inFile3)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\t\t=================================================================================" <<endl;

    cout << "\t\t|\t\t\t   Functions inside Run Macros   \t\t\t|" <<endl;
    cout << "\t\t=================================================================================" <<endl;
  
    while(getline(inFile3,str))
    {
	lineStr = str.c_str();
	if(str[0]=='/' && str[1]=='/')
	    continue;
	if(str == "#endif")
	    continue;
	if(str == "#ifdef __CINT__")
	    continue;
	//cout << str <<endl;
	cout << lineStr.ReplaceAll(";", "").ReplaceAll("#pragma link C++ ", "").ReplaceAll("//", " : ") <<endl;
    }
    inFile3.close();


    str =  TStar::Config->GetStarHome() + (string)"/diffAnalysis/src/";
    str = str + "LinkDef.h";
    ifstream inFile4(str.c_str());
    if(!inFile4)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\t\t=================================================================================" <<endl;

    cout << "\t\t|\t\t\t   Classes/Functions inside diffAnalysis   \t\t|" <<endl;
    cout << "\t\t=================================================================================" <<endl;
  
    while(getline(inFile4,str))
    {
	lineStr = str.c_str();
	if(str[0]=='/' && str[1]=='/')
	    continue;
	if(str == "#endif")
	    continue;
	if(str == "#ifdef __CINT__")
	    continue;
	//cout << str <<endl;
	cout << lineStr.ReplaceAll(";", "").ReplaceAll("#pragma link C++ ", "").ReplaceAll("//", " : ") <<endl;
    }
    inFile4.close();

    str =  TStar::Config->GetStarHome() + (string)"/emJetAnalysis/src/";
    str = str + "LinkDef.h";
    ifstream inFile5(str.c_str());
    if(!inFile5)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\t\t=================================================================================" <<endl;

    cout << "\t\t|\t\t\t   Classes/Functions inside emJetAnalysis   \t\t|" <<endl;
    cout << "\t\t=================================================================================" <<endl;
  
    while(getline(inFile5,str))
    {
	lineStr = str.c_str();
	if(str[0]=='/' && str[1]=='/')
	    continue;
	if(str == "#endif")
	    continue;
	if(str == "#ifdef __CINT__")
	    continue;
	//cout << str <<endl;
	cout << lineStr.ReplaceAll(";", "").ReplaceAll("#pragma link C++ ", "").ReplaceAll("//", " : ") <<endl;
    }
    inFile5.close();

    str =  TStar::Config->GetStarHome() + (string)"/starSim/src/";
    str = str + "LinkDef.h";
    ifstream inFile6(str.c_str());
    if(!inFile6)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\t\t=================================================================================" <<endl;

    cout << "\t\t|\t\t\t   Classes/Functions inside StarSim       \t\t|" <<endl;
    cout << "\t\t=================================================================================" <<endl;
  
    while(getline(inFile6,str))
    {
	lineStr = str.c_str();
	if(str[0]=='/' && str[1]=='/')
	    continue;
	if(str == "#endif")
	    continue;
	if(str == "#ifdef __CINT__")
	    continue;
	//cout << str <<endl;
	cout << lineStr.ReplaceAll(";", "").ReplaceAll("#pragma link C++ ", "").ReplaceAll("//", " : ") <<endl;
    }
    inFile6.close();
    
    cout<< "-------------------------------------------------------------------------------------------" <<endl;  
    cout << "| Type 'help(\"function or class name\")' for details of any specific function or class     |"<<endl;
    cout<<  "-------------------------------------------------------------------------------------------" <<endl;  

}


void TStar::help(string file)
{
    string str =  TStar::Config->GetStarHome() + (string)"/libBrStar/src/";
    str = str + file + ".h";
    ifstream inFile(str.c_str());

    if(!inFile)
    {
	str =  TStar::Config->GetStarHome() + (string)"/analysis/src/";
	str = str+file + ".h";
        inFile.open(str.c_str());	
    }

    if(!inFile)
    {
	str =  TStar::Config->GetStarHome() + (string)"/runMacros/src/";
	str = str+file + ".h";
        inFile.open(str.c_str());	
    }
    
    if(!inFile)
    {
	str =  TStar::Config->GetStarHome() + (string)"/diffAnalysis/src/";
	str = str+file + ".h";
        inFile.open(str.c_str());	
    }

    if(!inFile)
    {
	str =  TStar::Config->GetStarHome() + (string)"/emJetAnalysis/src/";
	str = str+file + ".h";
        inFile.open(str.c_str());	
    }

    if(!inFile)
    {
	str =  TStar::Config->GetStarHome() + (string)"/starSim/src/";
	str = str+file + ".h";
        inFile.open(str.c_str());	
    }

    
    if(!inFile)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    while(getline(inFile,str))       
	cout << str <<endl;

    inFile.close();
}

void TStar::history()
{
    string str;
    TString lineStr;
    str = TStar::Config->GetStarHome() + (string)"/.root_history";
	
    ifstream inFile(str.c_str());
    if(!inFile)
    {
	cout << "Source file NOT found:"<<str<<endl;
	return;
    }
    while(getline(inFile,str))
	cout << str <<endl;

    inFile.close();
}

void TStar::open(TString fileName)
{
    if(!IsValid(fileName))
    {
	cout<<"input file NOT found: "<< fileName<<endl;
	return;
    }
    TFile *file = new TFile(fileName);

    TBrowser *b = new TBrowser();    
}
