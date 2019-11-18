//Based on StTriggerFilterMaker

#ifndef TST_FMSRP_FILTER_MAKER_HH
#define TST_FMSRP_FILTER_MAKER_HH


#ifndef StMaker_H
#include "StMaker.h"
#endif

#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include <vector>
using std::vector;

class StMuDst;


class TStFmsRpFilterMaker : public StMaker
{
    StMuDst *mMuDst;
    StMuRpsCollection *mRpsMuColl;
public:
    TStFmsRpFilterMaker(const char *name = "TStFmsRpFilterMaker");
    virtual ~TStFmsRpFilterMaker();
    
    //virtual void Clear(const char *option="");
    virtual Int_t Init();
    //virtual Int_t InitRun(int runnumber);
    virtual Int_t Make();
    //virtual Int_t Finish();
    
    void addTrigger(unsigned int trigId) { mGoodTriggers.push_back(trigId); }
    void addVetoTrigger(unsigned int trigId) { mVetoTriggers.push_back(trigId); }
    void printTriggerId(int v=1) {mPrint=v;}

    const vector<unsigned int> getTriggers() const {return mGoodTriggers;}
    const vector<unsigned int> getVetoTriggers() const {return mGoodTriggers;}

    virtual const char* GetCVS() const
    {static const char cvs[]="Tag $Name:  $ $Id: TStFmsRpFilterMaker.h,v 1.4 2015/09/09 20:29:39 akio Exp $ built " __DATE__ " " __TIME__; return cvs;}
    
private:
    vector<unsigned int> mGoodTriggers; //!
    vector<unsigned int> mVetoTriggers; //!
    int mPrint; //!

    ClassDef(TStFmsRpFilterMaker,1)
};

#endif

