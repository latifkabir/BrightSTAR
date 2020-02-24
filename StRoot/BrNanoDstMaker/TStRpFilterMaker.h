//Based on StTriggerFilterMaker

#ifndef TST_RP_FILTER_MAKER_HH
#define TST_RP_FILTER_MAKER_HH


#ifndef StMaker_H
#include "StMaker.h"
#endif

#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include "TH1D.h"
#include <vector>
using std::vector;

class StMuDst;


class TStRpFilterMaker : public StMaker
{
    StMuDst *mMuDst;
    StMuRpsCollection *mRpsMuColl;
    TH1D* hEvtCount;
public:
    TStRpFilterMaker(const char *name = "TStRpFilterMaker");
    virtual ~TStRpFilterMaker();
    
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
    {static const char cvs[]="Tag $Name:  $ $Id: TStRpFilterMaker.h,v 1.4 2015/09/09 20:29:39 akio Exp $ built " __DATE__ " " __TIME__; return cvs;}

    void SetHist1d(TH1D* hist1d){ hEvtCount = hist1d;}
private:
    vector<unsigned int> mGoodTriggers; //!
    vector<unsigned int> mVetoTriggers; //!
    int mPrint; //!

    ClassDef(TStRpFilterMaker,1)
};

#endif

