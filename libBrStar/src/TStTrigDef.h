// Filename: TStTrigDef.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed May 29 00:26:00 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTTRIGDEF_H
#define TSTTRIGDEF_H

#include <TObject.h>
#include <string>

class TStTrigDef: public TObject
{
public:
    
    TStTrigDef();
    virtual ~TStTrigDef();

    static void MakeTrigIdDB(); 
    static Int_t GetTrigId(Int_t runNumber, TString trigger_name);
    static Int_t GetNearestTrigId(Int_t runNumber, TString trigger_name);

    ClassDef(TStTrigDef,1)
};
#endif
