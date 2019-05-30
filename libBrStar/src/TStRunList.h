// Filename: TStRunList.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed May 29 00:26:00 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTRUNLIST_H
#define TSTRUNLIST_H

#include <TObject.h>
#include <string>

class TStRunList: public TObject
{
public:
    
    TStRunList();
    virtual ~TStRunList();

    static void PrintRunList();
    static void PrintRunList(Int_t firstRun, Int_t lastRun);
    static void PrintRunList(Int_t minEvents);
    static void DumpRunList();
    static Int_t GetFirstRun();
    static Int_t GetLastRun();
    static void MakeFileList(Int_t firstRun, Int_t lastRunOrNfiles = -1);
    static void MakeFileListWithEvents(Int_t minEvents);
    
    ClassDef(TStRunList,0)
};
#endif
