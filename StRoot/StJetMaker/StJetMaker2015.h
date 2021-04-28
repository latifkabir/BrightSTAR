#ifndef StJetMaker2015_H
#define StJetMaker2015_H

#include "StJetMaker2012.h"
#include "StEmVertexMaker.h"

class StTriggerData;

class StJetMaker2015 : public StJetMaker2012
{
public:

    StJetMaker2015(const char* name = "StJetMaker2015") : StJetMaker2012(name) {}
    ~StJetMaker2015() {}

    Int_t Init();
    Int_t Finish();
    Int_t Make();

    void setMCmode(Bool_t isMCmode){mIsMCmode = isMCmode;}
protected:
    StEmVertexMaker *mEmVertexMkr;
    int mEvent = 0;
    Bool_t mIsMCmode = false;
    
    ClassDef(StJetMaker2015, 1);
};

#endif
