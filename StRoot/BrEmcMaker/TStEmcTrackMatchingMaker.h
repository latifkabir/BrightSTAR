// Filename: TStEmcTrackMatchingMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTEMCTRACKMATCHINGMAKER_H
#define TSTEMCTRACKMATCHINGMAKER_H

#include "StMaker.h"

class StEvent;
class StMuDst;
class StEmcCollection;
class StEmcPosition;

class TStEmcTrackMatchingMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent* mEvent;
    StEmcCollection *mEmcCollection;
protected:
    Int_t MatchToTracks(StEvent *event);    
public: 
    TStEmcTrackMatchingMaker(const char *name  = "TStEmcTrackMatchingMaker");
    virtual ~TStEmcTrackMatchingMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 


    ClassDef(TStEmcTrackMatchingMaker,1) 
};

#endif

