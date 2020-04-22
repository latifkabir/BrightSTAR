// Filename: TStFmsHotChDB.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Apr 10 15:22:21 2020 (-0400)
// URL: jlab.org/~latif

#ifndef TSTFMSHOTCHDB_H
#define TSTFMSHOTCHDB_H

#include <TObject.h>
#include <string>
#include <vector>

class TStFmsHotChDB: public TObject
{
public:
    
    TStFmsHotChDB();
    virtual ~TStFmsHotChDB();

    void GetHotChList(Int_t runNo, Bool_t isHotCh[][571]);
    void PrintHotChDB();
    
    ClassDef(TStFmsHotChDB,1)
};
#endif
