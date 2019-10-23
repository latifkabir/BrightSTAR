#include "StMessMgr.h"

#include "StEmcEvent.h"

ClassImp(StEmcEvent)

void StEmcEvent::Clear( const char* opt ) {

        mNhits = -1;
        mVhitE.clear();
        mVhitEt.clear();
        mVhitEta.clear();
        mVhitPhi.clear();
	mVhitPedestal.clear();
	mVhitPedrms.clear();
	mVhitStatus.clear();
	mVhitDetId.clear();
	mVhitId.clear();

}

void StEmcEvent::Print( const char* opt ) const {

        LOG_INFO << "StEmcEvent: "
                 << "\n\tmNhits =\t" << mNhits << "\thit array size =\t" << mVhitE.size()
                 << endm;
}
