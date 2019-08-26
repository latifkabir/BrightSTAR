// Filename: TStEmcTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStEmcTreeMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"

#include "StEmcPoint.h"
#include "StEpcMaker/StEpcMaker.h"
#include "StEvent/StEmcPoint.h"
#include "StEvent/StEmcCollection.h"
#include "StEmcADCtoEMaker/StEmcADCtoEMaker.h"
#include "StPreEclMaker/StPreEclMaker.h"

ClassImp(TStEmcTreeMaker)

//_____________________________________________________________________________ 
TStEmcTreeMaker::TStEmcTreeMaker(const char *name):StMaker(name)
{
    // Maker to apply calibration
    mAdc2E = new StEmcADCtoEMaker();
    // Makers for cluster finding
    mPreEcl = new StPreEclMaker();
    // Collection of points maker
    mEpc = new StEpcMaker();
}

//_____________________________________________________________________________ 
TStEmcTreeMaker::~TStEmcTreeMaker()
{
    //
}


//_____________________________________________________________________________ 
Int_t TStEmcTreeMaker::Init()
{
    // Create tables
    
    // Create Histograms
    
    // Get pointer to database
    // mDbMaker = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    // if (!mDbMaker)
    // {
    // 	LOG_ERROR <<"TStFmsTreeMaker::InitRun - !StFmsDbMaker" <<endl;
    // 	return kStFatal;
    // }
    mAdc2E->setPrint(kFALSE);
    mPreEcl->setPrint(kFALSE);
    mEpc->setPrint(kFALSE);
    mEpc->setFillHisto(kFALSE);
    
    return kStOK;
}


//_____________________________________________________________________________
Int_t TStEmcTreeMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");
    // Get St<det>MuCollection
    // Or Get St<det>Colelction
    // Check if mMuDst or mEvent is valid

    //---------- Access EMC data ---------------
    mEmcCollection = mMuDst->emcCollection();	
    StSPtrVecEmcPoint mEmcPoints = mEmcCollection->barrelPoints();

    mNpoints = mEmcPoints.size();
	
    for(Int_t i = 0; i < mNpoints; ++i)
    {
	mPoint1 = mEmcPoints[i];	
	//---------- Calculate and fill Pi0 invariant mass --------------
	for(Int_t j = i + 1; j < mNpoints; ++j)
	{
	    if(j >= mNpoints)
		continue;
		
	    mPoint2 = mEmcPoints[j];
	    mV1 = mPoint1->position();
	    //cout << mV1.x() <<"\t"<< mV1.y() << "\t" <<mV1.z() <<endl;
	    mV2 = mPoint2->position();

	    mE1 = mPoint1->energy();
	    mE2 = mPoint2->energy();
	    mQ1 = mPoint1->quality();
	    mQ2 = mPoint2->quality();
	    mTheta = mV1.angle(mV2);
	    mZgg = fabs(mE1 - mE2) / (mE1 + mE2);
	    mPi0_x = (mE1*mV1.x() + mE2*mV2.x()) / ( mE1 + mE2);
	    mPi0_y = (mE1*mV1.y() + mE2*mV2.y()) / ( mE1 + mE2);
	    mPi0_z = (mE1*mV1.z() + mE2*mV2.z()) / ( mE1 + mE2);
		
	    //----------------------- Method 1 --------------------------
	    //pairM = TMath::Sqrt(2*mE1*mE2*(1 - TMath::Cos(mTheta)));

	    // ----------------------- Method 2 ------------------------------------------------
	    mLV1.SetPxPyPzE(mE1*mV1.x() / mV1.mag(), mE1*mV1.y() / mV1.mag(), mE1*mV1.z() / mV1.mag(), mE1);
	    mLV2.SetPxPyPzE(mE2*mV2.x() / mV2.mag(), mE2*mV2.y() / mV2.mag(), mE2*mV2.z() / mV2.mag(), mE2);
	    mLV = (mLV1 + mLV2);
	    mPairM = mLV.M();					
	}	
    }
        
    return kStOK;
}

//_____________________________________________________________________________
Int_t TStEmcTreeMaker::Finish()
{
    //Write histograms to root file etc.

    return kStOK;
}
