#include "StMessMgr.h"

#include "StFmsEventHeader.h"

ClassImp(StFmsEventHeader)

void StFmsEventHeader::Clear( const char* opt ) {

        mFillnum = -1;
        mRnum = -1;
        mIevt = -1;
	mSpinbit = -1;
        mBunchId = -1;
        mBbcCoincidenceRate = -1;
	mBbcTimeBin = 0;
        mTriggerIds.clear();
}

void StFmsEventHeader::Print( const char* opt ) const {

        LOG_INFO << "StFmsEventHeader: " <<" fill = " << mFillnum <<"run = " <<mRnum <<" event = "<<mIevt
                 << "\n\tmBunchId =\t" << mBunchId << " spinbit= "<< mSpinbit
                 << "\n\tmBbcCoincidenceRate =\t" << mBbcCoincidenceRate
                 << "\n\tNTriggers = \t" << mTriggerIds.size()
                 << endm;
}

