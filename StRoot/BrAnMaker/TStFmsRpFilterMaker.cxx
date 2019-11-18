
//Based on StTriggerFilterMaker

#include "TStFmsRpFilterMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"

ClassImp(TStFmsRpFilterMaker)

TStFmsRpFilterMaker::TStFmsRpFilterMaker(const char *name) : StMaker(name), mPrint(0) { }

TStFmsRpFilterMaker::~TStFmsRpFilterMaker()
{
    mGoodTriggers.clear();
    mVetoTriggers.clear();
}

Int_t TStFmsRpFilterMaker::Init()
{
    // this allows us to skip an event for other Makers
    SetAttr(".Privilege",1);
    return kStOk;
}

Int_t TStFmsRpFilterMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    //RP No Track events filter  
    mRpsMuColl = mMuDst->RpsCollection();
    if(!mRpsMuColl)
    {
	cout<<"No RP data for this event"<<endl;
	return kStSkip;
    }    
    if(mRpsMuColl->numberOfTracks() < 1)
	return kStSkip;
    
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
      return kStOk;
    }
  }
  if(mPrint)    LOG_INFO << " Skip" << endm;
  return kStSkip;
}
