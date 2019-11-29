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
#include "TStar.h"

#include<sstream>
#include<string>
#include "TString.h"



using namespace std;

ClassImp(TStar)

TStar::TStar()
{
    
}

TStar::~TStar()
{
    
}

TStConfig* TStar::Config = new TStConfig();

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


Int_t TStar::GetCounter()
{
    TString filePath = Config->GetFilePath();
    filePath += "FileCounter.txt";    
    fstream counterFile(filePath,ios::in | ios::out);
    Int_t fileNo = -1;
    if(!counterFile)
    {
	cout << "WARNING: The vault file counter NOT found" <<endl;
	return -1;
    }

    counterFile>>fileNo;	
    ++fileNo;
    counterFile.seekg(0,ios::beg);
    counterFile<<fileNo;
    counterFile.close();

    TString rootFilePath = Config->GetFilePath() + "root/";
    rootFilePath += fileNo;
    rootFilePath += ".root";

    if(!gSystem->AccessPathName(rootFilePath))
    {
	cout << "WARNING: The vault-file already exists. Overwriting NOT permitted." <<endl;
	return -1;
    }
    return fileNo;
}

void TStar::SetCounter(Int_t count)
{
    TString filePath = Config->GetFilePath();
    filePath += "FileCounter.txt";    
    fstream counterFile(filePath, ios::out);
    Int_t fileNo = count;
    if(!counterFile)
    {
	cout << "WARNING: The vault file counter NOT found" <<endl;
	return;
    }
    counterFile<<fileNo;
    cout << "Counter set to: "<<fileNo <<endl;
    counterFile.close();
}

void TStar::PrintCounter()
{
    TString filePath = Config->GetFilePath();
    filePath += "FileCounter.txt";    
    fstream counterFile(filePath, ios::in);
    Int_t fileNo = -1;
    if(!counterFile)
    {
	cout << "WARNING: The vault file-counter NOT found" <<endl;
	return;
    }

    counterFile>>fileNo;	
    counterFile.close();
    cout << "File counter in the vault: "<< fileNo <<endl;
}


void TStar::FileViewer(Int_t fileNumber)
{
    //Open the file for reading
    TString fileName = Config->GetFilePath() + "root/";
    fileName += fileNumber;
    fileName += ".root";
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
	if (obj->IsA()->InheritsFrom("TH1") || obj->IsA()->InheritsFrom("TGraph"))
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

void TStar::FileViewer(TString fileName)
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
	if (obj->IsA()->InheritsFrom("TH1") || obj->IsA()->InheritsFrom("TGraph"))
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

void TStar::FilePrint(Int_t lowerRange, Int_t upperRange)
{
    for(int i = lowerRange; i <= upperRange; ++i)
    {
	//Open the file for reading
	TString fileName = Config->GetFilePath() + "root/"; 
	fileName += i;
	fileName += ".root";
	if(gSystem->AccessPathName(fileName))
	    continue;
	cout <<"\n============================================================================="<<endl;
	cout << "\t\t\t File Number: "<<i<<endl;

	cout <<"============================================================================="<<endl;
	TFile *file = new TFile(fileName);
	file->ls();
	file->Close();
    }
}

void TStar::FileListing(Int_t lowerRange, Int_t upperRange)
{
    for(int i = lowerRange; i <= upperRange; ++i)
    {
	//Open the file for reading
	TString fileName = Config->GetFilePath() + "root/";
	fileName += i;
	fileName += ".root";
	if(gSystem->AccessPathName(fileName))
	    continue;
	TFile *file = new TFile(fileName);
	file->Print();
	file->Close();
    }
}

void TStar::FileListing()
{
    TString filePath = Config->GetFilePath();
    filePath += "FileCounter.txt";    
    fstream counterFile(filePath, ios::in);
    Int_t fileNo = -1;
    if(!counterFile)
    {
	cout << "WARNING: The file having the file-counter NOT found" <<endl;
	return;
    }

    counterFile>>fileNo;	
    counterFile.close();

    for(int i = 0; i <= fileNo; ++i)
    {
	//Open the file for reading
	TString fileName = Config->GetFilePath() + "root/";
	fileName += i;
	fileName += ".root";
	if(gSystem->AccessPathName(fileName))
	    continue;
	TFile *file = new TFile(fileName);
	file->Print();
	file->Close();
    }
}


void TStar::help()
{
    string str;
    TString lineStr;
    str = TStar::Config->GetStarHome() + (string)"/libBrStar/src/";
	
    str = str + "LinkDef.h";
    //cout << str <<endl;

    ifstream myFile(str.c_str());
    if(!myFile)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\n\t\t=========================================================================" <<endl;

    cout << "\t\t|\t\t\t   Classes/Functions inside libBrStar        \t|" <<endl;
    cout << "\t\t=========================================================================" <<endl;
    while(getline(myFile,str))
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

    myFile.close();
    cout<< "-------------------------------------------------------------------------------------------" <<endl;  
    cout << "| Type 'help(\"function or class name\")' for details of any specific function or class     |"<<endl;
    cout<<  "-------------------------------------------------------------------------------------------" <<endl;  

    str =  TStar::Config->GetStarHome() + (string)"/analysis/src/";
    str = str + "LinkDef.h";
    //cout << str <<endl;

    ifstream myFile2(str.c_str());
    if(!myFile2)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\t\t=================================================================================" <<endl;

    cout << "\t\t|\t\t\t   Functions inside Analysis   \t\t\t\t|" <<endl;
    cout << "\t\t=================================================================================" <<endl;
  
    while(getline(myFile2,str))
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
    myFile2.close();

    str =  TStar::Config->GetStarHome() + (string)"/runMacros/src/";
    str = str + "LinkDef.h";
    //cout << str <<endl;

    ifstream myFile3(str.c_str());
    if(!myFile3)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\t\t=================================================================================" <<endl;

    cout << "\t\t|\t\t\t   Functions inside Run Macros   \t\t\t|" <<endl;
    cout << "\t\t=================================================================================" <<endl;
  
    while(getline(myFile3,str))
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
    myFile3.close();


    str =  TStar::Config->GetStarHome() + (string)"/diffAnalysis/src/";
    str = str + "LinkDef.h";
    ifstream myFile4(str.c_str());
    if(!myFile4)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    cout << "\t\t=================================================================================" <<endl;

    cout << "\t\t|\t\t\t   Classes/Functions inside diffAnalysis   \t\t|" <<endl;
    cout << "\t\t=================================================================================" <<endl;
  
    while(getline(myFile4,str))
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
    myFile4.close();
    
    cout<< "-------------------------------------------------------------------------------------------" <<endl;  
    cout << "| Type 'help(\"function or class name\")' for details of any specific function or class     |"<<endl;
    cout<<  "-------------------------------------------------------------------------------------------" <<endl;  

}


void TStar::help(string file)
{
    string str =  TStar::Config->GetStarHome() + (string)"/libBrStar/src/";
    str = str + file + ".h";
    ifstream myFile(str.c_str());

    if(!myFile)
    {
	str =  TStar::Config->GetStarHome() + (string)"/analysis/src/";
	str = str+file + ".h";
        myFile.open(str.c_str());	
    }

    if(!myFile)
    {
	str =  TStar::Config->GetStarHome() + (string)"/runMacros/src/";
	str = str+file + ".h";
        myFile.open(str.c_str());	
    }
    
    if(!myFile)
    {
	str =  TStar::Config->GetStarHome() + (string)"/diffAnalysis/src/";
	str = str+file + ".h";
        myFile.open(str.c_str());	
    }
    
    if(!myFile)
    {
	cout << "Source file NOT found" <<endl;
	return;
    }
    while(getline(myFile,str))       
	cout << str <<endl;

    myFile.close();
}

void TStar::history()
{
    string str;
    TString lineStr;
    str = TStar::Config->GetStarHome() + (string)"/.root_history";
	
    ifstream myFile(str.c_str());
    if(!myFile)
    {
	cout << "Source file NOT found:"<<str<<endl;
	return;
    }
    while(getline(myFile,str))
	cout << str <<endl;

    myFile.close();
}


