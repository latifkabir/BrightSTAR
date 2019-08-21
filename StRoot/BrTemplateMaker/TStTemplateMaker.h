// Filename: TStTemplateMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTTEMPLATEMAKER_H
#define TSTTEMPLATEMAKER_H

#include "StMaker.h"

class StEvent;
class StMuDst;
//class StFmsDbMaker;  

class TStTemplateMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent* mEvent;
    //StFmsDbMaker* mDbMaker; // Example DB Maker, replace with desired detector.
protected:

    
public: 
    TStTemplateMaker(const char *name  = "TemplateMaker");
    virtual ~TStTemplateMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 


    ClassDef(TStTemplateMaker,1) 
};

#endif

