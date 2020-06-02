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
    
    ClassDef(TStFillNoDB,1)
};
#endif
