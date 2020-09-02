#ifndef StJetMaker2015_H
#define StJetMaker2015_H

#include "StJetMaker2012.h"
class StTriggerData;

class StJetMaker2015 : public StJetMaker2012
{
public:

    StJetMaker2015(const char* name = "StJetMaker2015") : StJetMaker2012(name) {}
    ~StJetMaker2015() {}

    Int_t Init();
    Int_t Finish();
    Int_t Make();

    Int_t ReadBbcSlewing(const char* filename_bbc); //BBC slewing correction function by Oleg Eyser
    Float_t GetBbcZCorr(const StTriggerData* triggerData);
    void setMCmode(Bool_t isMCmode){mIsMCmode = isMCmode;}
protected:

    bool mReadBbcSlewing = false; //Turn to true if ReadBbcSlewing is invoked
    float mBbcSlew[2][16][3]; //Oleg, [east/west][pmt][parameter]
    int mEvent = 0;
    Bool_t mIsMCmode = false;
    
    ClassDef(StJetMaker2015, 1);
};

#endif
