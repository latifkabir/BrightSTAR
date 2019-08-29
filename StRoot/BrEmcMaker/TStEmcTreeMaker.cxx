// Filename: TStEmcTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif

#include "TStEmcTreeMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"

#include "StEmcPoint.h"
#include "StEvent/StEmcPoint.h"
#include "StEvent/StEmcCollection.h"

ClassImp(TStEmcTreeMaker)

//_____________________________________________________________________________ 
TStEmcTreeMaker::TStEmcTreeMaker(const char *name):StMaker(name)
{

}

//_____________________________________________________________________________ 
TStEmcTreeMaker::~TStEmcTreeMaker()
{
    //
}


//_____________________________________________________________________________ 
Int_t TStEmcTreeMaker::Init()
{   
    mFile = new TFile(mOutName, "RECREATE");
    mTree = new TTree("T", "BEMC Tree");
    SetBranches();
    return kStOK;
}
//_____________________________________________________________________________
void TStEmcTreeMaker::SetOutName(TString outName)
{
    mOutName = outName;   
}

//_____________________________________________________________________________
void TStEmcTreeMaker::SetBranches()
{
    mTree->Branch("point", &mNpoints, "point/I");
    mTree->Branch("point_x", mX, "point_x[point]/D");
    mTree->Branch("point_y", mY, "point_y[point]/D");
    mTree->Branch("point_z", mZ, "point_z[point]/D");
    mTree->Branch("point_px", mPx, "point_px[point]/D");
    mTree->Branch("point_py", mPy, "point_py[point]/D");
    mTree->Branch("point_pz", mPz, "point_pz[point]/D");
    mTree->Branch("point_E", mE, "point_E[point]/D");
    mTree->Branch("point_quality", mQ, "point_quality[point]/I");

    //------- Not sure if I will keep the following for the final version. SInce it's redundent --------------
    mTree->Branch("pi0", &mNpi0, "pi0/I");
    mTree->Branch("pi0_x", mPi0X, "pi0_x[pi0]/D");
    mTree->Branch("pi0_y", mPi0Y, "pi0_y[pi0]/D");
    mTree->Branch("pi0_z", mPi0Z, "pi0_z[pi0]/D");
    mTree->Branch("pi0_px", mPi0Px, "pi0_px[pi0]/D");
    mTree->Branch("pi0_py", mPi0Py, "pi0_py[pi0]/D");
    mTree->Branch("pi0_pz", mPi0Pz, "pi0_pz[pi0]/D");
    mTree->Branch("pi0_E", mPi0E, "pi0_E[pi0]/D");    
    mTree->Branch("pi0_M", mPi0M, "pi0_M[pi0]/D");    
    mTree->Branch("pi0_theta", mPi0theta, "pi0_theta[pi0]/D");    
    mTree->Branch("pi0_zgg", mPi0zgg, "pi0_zgg[pi0]/D");    
    mTree->Branch("pi0_dgg", mPi0dgg, "pi0_dgg[pi0]/D");    
    mTree->Branch("pi0_Q1", mPi0Q1, "pi0_Q1[pi0]/I");    
    mTree->Branch("pi0_Q2", mPi0Q2, "pi0_Q2[pi0]/I");
}

//_____________________________________________________________________________
void TStEmcTreeMaker::ResetBuffer()
{
    mNpoints = -1;
    memset(mX, -1, kMaxPoints*sizeof(Double_t));
    memset(mY, -1, kMaxPoints*sizeof(Double_t));
    memset(mZ, -1, kMaxPoints*sizeof(Double_t));
    memset(mPx, -1, kMaxPoints*sizeof(Double_t));
    memset(mPy, -1, kMaxPoints*sizeof(Double_t));
    memset(mPz, -1, kMaxPoints*sizeof(Double_t));
    memset(mE, -1, kMaxPoints*sizeof(Double_t));
    memset(mQ, -1, kMaxPoints*sizeof(Int_t));

    mNpi0 = -1;
    memset(mPi0X, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Y, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Z, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Px, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Py, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Pz, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0E, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0M, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0theta, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0zgg, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0dgg, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Q1, -1, kMaxPoints*sizeof(Int_t));
    memset(mPi0Q2, -1, kMaxPoints*sizeof(Int_t));
}

//_____________________________________________________________________________
Int_t TStEmcTreeMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    if(!mMuDst)
    {
	cout << "TStEmcTreeMaker::Make- Unable to retrieve MuDst" <<endl;
	return kStFatal;
    }
    if(mTrigIDs.size() > 0)
    {
	if(!Accept(mMuDst->event()))
	    return kStOK;
    }
    ResetBuffer();
    //---------- Access EMC data ---------------
    mEmcCollection = mMuDst->emcCollection();
    if(!mEmcCollection)
    {
	cout<<"No EMC data for this event"<<endl;
	return kStSkip;
    }
    StSPtrVecEmcPoint mEmcPoints = mEmcCollection->barrelPoints();
    mNpoints = mEmcPoints.size();
    mNpi0 = 0;
    for(Int_t i = 0; i < mNpoints; ++i)
    {
	mPoint1 = mEmcPoints[i];
	mV1 = mPoint1->position();
	mE1 = mPoint1->energy();
	mQ1 = mPoint1->quality();

	mX[i] = mV1.x();
	mY[i] = mV1.y();
	mZ[i] = mV1.z();
	mPx[i] = mE1*mV1.x() / mV1.mag();
	mPy[i] = mE1*mV1.y() / mV1.mag();
	mPz[i] = mE1*mV1.z() / mV1.mag();
	mE[i] = mE1;
	mQ[i] = mQ1;
	
	mLV1.SetPxPyPzE(mPx[i], mPy[i], mPz[i], mE[i]);
	//cout<< "Total tracks: " << mPoint1->nTracks() <<endl;
	//---------- Calculate and fill Pi0 invariant mass --------------
	for(Int_t j = i + 1; j < mNpoints; ++j)
	{
	    if(j >= mNpoints)
		continue;
		
	    mPoint2 = mEmcPoints[j];
	    mV2 = mPoint2->position();
	    mE2 = mPoint2->energy();
	    mQ2 = mPoint2->quality();
	    mTheta = mV1.angle(mV2);
	    mZgg = fabs(mE1 - mE2) / (mE1 + mE2);
	    
	    mPi0X[mNpi0] = (mE1*mV1.x() + mE2*mV2.x()) / ( mE1 + mE2);
	    mPi0Y[mNpi0] = (mE1*mV1.y() + mE2*mV2.y()) / ( mE1 + mE2);
	    mPi0Z[mNpi0] = (mE1*mV1.z() + mE2*mV2.z()) / ( mE1 + mE2);
	    
	    mPi0Px[mNpi0] = mE2*mV2.x() / mV2.mag();
	    mPi0Py[mNpi0] = mE2*mV2.y() / mV2.mag();
	    mPi0Pz[mNpi0] = mE2*mV2.z() / mV2.mag();
	    mPi0E[mNpi0] = mE2;

	    mLV2.SetPxPyPzE(mPi0Px[mNpi0], mPi0Py[mNpi0], mPi0Pz[mNpi0], mE2);
	    mLV = (mLV1 + mLV2);	  
	    //pairM = TMath::Sqrt(2*mE1*mE2*(1 - TMath::Cos(mTheta)));	 //Method-1
	    mPairM = mLV.M();  //Method-2

	    mPi0M[mNpi0] = mPairM;
	    mPi0theta[mNpi0] = mTheta;
	    mPi0zgg[mNpi0] = mZgg;
	    mPi0dgg[mNpi0] = (mV1 - mV2).mag();
	    mPi0Q1[mNpi0] = mQ1;
	    mPi0Q2[mNpi0] = mQ2;
	    
	    ++mNpi0;
	}	
    }
    mTree->Fill();
    return kStOK;
}

//_____________________________________________________________________________
Int_t TStEmcTreeMaker::Finish()
{
    mFile->Write();
    mFile->Close();    
    return kStOK;
}

//_____________________________________________________________________________
Bool_t TStEmcTreeMaker::Accept(StMuEvent *muEvent)
{
    mTrigMuColl = muEvent->triggerIdCollection();
    const StTriggerId trgIDs = mTrigMuColl.nominal();
	
    for(Int_t i = 0; i < mTrigIDs.size(); i++)
    {
	if (trgIDs.isTrigger(mTrigIDs[i]))
	    return kTRUE;
    }
    return kFALSE;
}

//_____________________________________________________________________________
void TStEmcTreeMaker::AddTrigger(Int_t triggerId)
{
    mTrigIDs.push_back(triggerId);
}
