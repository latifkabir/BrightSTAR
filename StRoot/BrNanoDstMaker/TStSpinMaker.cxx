// Filename: TStSpinMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Apr 12 03:51:42 2021 (-0400)
// URL: jlab.org/~latif

#include "TStSpinMaker.h"
#include "StChain/StChain.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"
#include "StEvent/StTriggerData.h"


ClassImp(TStSpinMaker)

TStSpinMaker::TStSpinMaker(TString name): StMaker(name)
{	
    Init();
}
TStSpinMaker::~TStSpinMaker()
{
	
}
Int_t TStSpinMaker::Init()
{	
    return kStOk;	
}
Int_t TStSpinMaker::InitRun(Int_t runNumber)
{
    mSpinDbMaker = static_cast<StSpinDbMaker*>(GetMaker("spinDb"));
    mSpinDbMaker->print();

    return kStOK;
}
    
Int_t TStSpinMaker::Make()
{
    mSpinY = 0;
    mSpinB = 0;
    
    mDst = (StMuDst*)GetInputDS("MuDst");
    if (!mDst)
    {
	cout <<"TStSpinMaker::Make - !MuDst" <<endl;
	return kStErr;
    }
    mMuEvent = mDst->event();
	
    mBunchid7Bit = mMuEvent->triggerData()->bunchId7Bit();
    mSpin4Bit = mSpinDbMaker->spin4usingBX7(mBunchid7Bit); 
	
    /*
      Spin info to be interpreted based on: https://drupal.star.bnl.gov/STAR/blog/oleg/spin-patterns-and-polarization-direction
      spin = 10:  STAR pol B+ Y+
      =  9:  STAR pol B+ Y-
      =  6:  STAR pol B- Y+
      =  5:  STAR pol B- Y-
      (Note: Bunches can also be unpolarized, i.e. other possible values for the 4-spin bits are 1, 2, 4, and 8.)
    */

    // Let's use the convention (our convention) that -1 is down spin, +1 is up spin and 0 is invalid spin. 
    switch(mSpin4Bit) 
    {
    case 5:
    {
	mSpinB = -1;
	mSpinY = -1;
	break;
    }
    case 6:
    {
	mSpinB = -1;
	mSpinY = +1;
	break;
    }
    case 9:
    {
	mSpinB = +1;
	mSpinY = -1;
	break;
    }
    case 10:
    {
	mSpinB = +1;
	mSpinY = +1;
	break;
    }
    default: //either one is unpolarized or undefined
	mSpinB = 0;
	mSpinY = 0;
    }

    return kStOk;
}

Int_t TStSpinMaker::Finish()
{
    return kStOk;
}
