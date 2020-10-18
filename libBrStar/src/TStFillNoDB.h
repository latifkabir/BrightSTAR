// Filename: TStFillNoDB.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Apr 10 15:22:21 2020 (-0400)
// URL: jlab.org/~latif

#ifndef TSTFILLNODB_H
#define TSTFILLNODB_H

#include <TObject.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

class TStFillNoDB: public TObject
{
public:
    
    TStFillNoDB();
    virtual ~TStFillNoDB();

    void GenerateFillDB(TString inFile);
    void ReadFillNoDB();
    void PrintFillNoDB();
    map <int, vector<int> > GetFillNoDB();
    vector <int> GetRunsWithFill(Int_t fillNo);
    vector <int> GetAllFillNos();
    Int_t GetFillNo(Int_t runNo);
    
    void GetFillPolarization(Int_t fill, Double_t &energy, Int_t &startTime, Int_t &stopTime, Double_t &p_b, Double_t &dp_b, Double_t &dpdt_b, Double_t &edpdt_b, Double_t &p_y, Double_t &dp_y, Double_t &dpdt_y, Double_t &edpdt_y);
    
    ClassDef(TStFillNoDB,1)
};
#endif
