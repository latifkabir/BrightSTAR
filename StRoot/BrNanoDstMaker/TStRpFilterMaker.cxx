
//Based on StTriggerFilterMaker

#include "TStRpFilterMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"

ClassImp(TStRpFilterMaker)

TStRpFilterMaker::TStRpFilterMaker(const char *name) : StMaker(name), mPrint(0) { }

TStRpFilterMaker::~TStRpFilterMaker()
{
    mGoodTriggers.clear();
    mVetoTriggers.clear();
}

Int_t TStRpFilterMaker::Init()
{
    // this allows us to skip an event for other Makers
    SetAttr(".Privilege",1);
    if(!hEvtCount)
	hEvtCount = new TH1D("hEvtCount", "Event Count", 100, 0, 0);
    return kStOk;
}

Int_t TStRpFilterMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    hEvtCount->Fill(1);

    if(mPrint)
    {
	vector<unsigned int> ids = StMuDst::event()->triggerIdCollection().nominal().triggerIds();
	LOG_INFO << "Offline Trigger Id = ";
	for(unsigned i=0; i<ids.size(); ++i) LOG_INFO << ids[i] << " ";
    }
    for(unsigned i=0; i<mVetoTriggers.size(); ++i)
    {
	if(StMuDst::event()->triggerIdCollection().nominal().isTrigger(mVetoTriggers[i]))
	{
	    if(mPrint)  LOG_INFO << " Veto" << endm;
	    return kStSkip;
	}
    }
    for(unsigned i=0; i<mGoodTriggers.size(); ++i)
    {
	if(StMuDst::event()->triggerIdCollection().nominal().isTrigger(mGoodTriggers[i]))
	{
	    if(mPrint)   LOG_INFO << " Accept" << endm;
	    hEvtCount->Fill(3);
	    //RP No Track events filter  
	    mRpsMuColl = mMuDst->RpsCollection();
	    if(!mRpsMuColl)
	    {
		LOG_INFO << "No RP data for this event" << endm;
		return kStSkip;
	    }    
	    if(mRpsMuColl->numberOfTracks() < 1)
	    {
		LOG_INFO << "Zero RP track for this event. To be skipped ... " << endm;
		return kStSkip;
	    }
	    hEvtCount->Fill(5);
	    return kStOk;
	}
    }
    if(mPrint)    LOG_INFO << " Skip" << endm;
    return kStSkip;
}
