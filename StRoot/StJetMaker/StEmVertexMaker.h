// Filename: StEmVertexMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef STEMVERTEXMAKER_H
#define STEMVERTEXMAKER_H

#include "StMaker.h"

class StMuDst;
class StTriggerData;

class StEmVertexMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    Double_t mEmVertexZ;
protected:
    bool mReadBbcSlewing = false; //Turn to true if ReadBbcSlewing is invoked
    float mBbcSlew[2][16][3]; //Oleg, [east/west][pmt][parameter]
    
public: 
    StEmVertexMaker(const char *name  = "StEmVertexMaker");
    virtual ~StEmVertexMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();

    Int_t ReadBbcSlewing(const char* filename_bbc); //BBC slewing correction function by Oleg Eyser
    Double_t GetBbcZCorr(const StTriggerData* triggerData);

    Double_t GetEmVertexZ(){return mEmVertexZ;}
    
    ClassDef(StEmVertexMaker,1) 
};

#endif

