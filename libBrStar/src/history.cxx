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

void history()
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


