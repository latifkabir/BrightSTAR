// Filename: help.cpp
// Description: 
// Author: Latiful Kabir < siplukabir@gmail.com >
// Created: Mon Mar 28 15:12:34 2016 (-0400)
// URL: latifkabir.github.io

#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include "TString.h"
#include "TStar.h"

using namespace std;

void help()
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
    cout<< "-------------------------------------------------------------------------------------------" <<endl;  
    cout << "| Type 'help(\"function or class name\")' for details of any specific function or class     |"<<endl;
    cout<<  "-------------------------------------------------------------------------------------------" <<endl;  

}


void help(string file)
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
	cout << "Source file NOT found" <<endl;
	return;
    }
    while(getline(myFile,str))       
	cout << str <<endl;

    myFile.close();
}

// int main(int argc, char *argv[])
// {
//     help();
//     help("TStRun");
//     return 0;
// }
