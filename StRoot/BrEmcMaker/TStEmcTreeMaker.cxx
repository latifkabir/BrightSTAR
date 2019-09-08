// Filename: TStEmcTreeMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif

#include "TStEmcTreeMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StEmcPoint.h"
#include "StEvent/StEmcPoint.h"
#include "StEvent/StEmcCollection.h"

ClassImp(TStEmcTreeMaker)

//_____________________________________________________________________________ 
TStEmcTreeMaker::TStEmcTreeMaker(const char *name):StMaker(name)
{
    mX = new Double_t[kMaxPoints];
    mY = new Double_t[kMaxPoints];
    mZ = new Double_t[kMaxPoints];
    mPx= new Double_t[kMaxPoints];
    mPy= new Double_t[kMaxPoints];
    mPz= new Double_t[kMaxPoints];
    mE = new Double_t[kMaxPoints];
    mQ = new Int_t[kMaxPoints];
    mNtracks = new Int_t[kMaxPoints];
    
    mPi0X = new Double_t[kMaxPi0];
    mPi0Y = new Double_t[kMaxPi0];
    mPi0Z = new Double_t[kMaxPi0];
    mPi0Px = new Double_t[kMaxPi0];
    mPi0Py = new Double_t[kMaxPi0];
    mPi0Pz = new Double_t[kMaxPi0];
    mPi0E = new Double_t[kMaxPi0];
    mPi0Pt = new Double_t[kMaxPi0];
    mPi0M = new Double_t[kMaxPi0];
    mPi0theta = new Double_t[kMaxPi0];
    mPi0zgg = new Double_t[kMaxPi0];
    mPi0dgg = new Double_t[kMaxPi0];
    mPi0Q1 = new Int_t[kMaxPi0];
    mPi0Q2 = new Int_t[kMaxPi0];
    mPi0nTracks1 = new Int_t[kMaxPi0];
    mPi0nTracks2 = new Int_t[kMaxPi0];
}

//_____________________________________________________________________________ 
TStEmcTreeMaker::~TStEmcTreeMaker()
{
    //
    delete[] mX; 
    delete[] mY; 
    delete[] mZ; 
    delete[] mPx; 
    delete[] mPy; 
    delete[] mPz; 
    delete[] mE; 
    delete[] mQ; 
    delete[] mNtracks;

    delete[] mPi0X;
    delete[] mPi0Y;
    delete[] mPi0Z;
    delete[] mPi0Px;
    delete[] mPi0Py;
    delete[] mPi0Pz;
    delete[] mPi0E;
    delete[] mPi0Pt;
    delete[] mPi0M;
    delete[] mPi0theta;
    delete[] mPi0zgg;
    delete[] mPi0dgg;
    delete[] mPi0Q1;
    delete[] mPi0Q2;
    delete[] mPi0nTracks1;
    delete[] mPi0nTracks2;
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
    mTree->Branch("evt_no", &mEvtNo, "evt_no/I");
    mTree->Branch("trig_MB", &mMB, "trig_MB/I");
    mTree->Branch("trig_HT1", &mHT1, "trig_HT1/I");
    mTree->Branch("trig_HT2", &mHT2, "trig_HT2/I");

    mTree->Branch("vz", &mVz, "vz/D");
    
    mTree->Branch("point", &mNpoints, "point/I");
    mTree->Branch("point_x", mX, "point_x[point]/D");
    mTree->Branch("point_y", mY, "point_y[point]/D");
    mTree->Branch("point_z", mZ, "point_z[point]/D");
    mTree->Branch("point_px", mPx, "point_px[point]/D");
    mTree->Branch("point_py", mPy, "point_py[point]/D");
    mTree->Branch("point_pz", mPz, "point_pz[point]/D");
    mTree->Branch("point_E", mE, "point_E[point]/D");
    mTree->Branch("point_quality", mQ, "point_quality[point]/I");
    mTree->Branch("point_nTracks", mNtracks, "point_nTracks[point]/I");

    mTree->Branch("pid_trait_q", mTraits->q, "pid_q[point][point_nTracks[point]]/I");
    mTree->Branch("pid_trait_p", mTraits->p, "pid_p[point][point_nTracks[point]]/D");
    mTree->Branch("pid_trait_pt", mTraits->pt, "pid_pt[point][point_nTracks[point]]/D");
    mTree->Branch("pid_trait_dca", mTraits->dca, "pid_dca[point][point_nTracks[point]]/D");
    mTree->Branch("pid_trait_beta", mTraits->beta, "pid_beta[point][point_nTracks[point]]/D");
    
    //------- Not sure if I will keep the following for the final version. SInce it's redundent --------------
    mTree->Branch("pi0", &mNpi0, "pi0/I");
    mTree->Branch("pi0_x", mPi0X, "pi0_x[pi0]/D");
    mTree->Branch("pi0_y", mPi0Y, "pi0_y[pi0]/D");
    mTree->Branch("pi0_z", mPi0Z, "pi0_z[pi0]/D");
    mTree->Branch("pi0_px", mPi0Px, "pi0_px[pi0]/D");
    mTree->Branch("pi0_py", mPi0Py, "pi0_py[pi0]/D");
    mTree->Branch("pi0_pz", mPi0Pz, "pi0_pz[pi0]/D");
    mTree->Branch("pi0_E", mPi0E, "pi0_E[pi0]/D");    
    mTree->Branch("pi0_pt", mPi0Pt, "pi0_pt[pi0]/D");    
    mTree->Branch("pi0_M", mPi0M, "pi0_M[pi0]/D");    
    mTree->Branch("pi0_theta", mPi0theta, "pi0_theta[pi0]/D");    
    mTree->Branch("pi0_zgg", mPi0zgg, "pi0_zgg[pi0]/D");    
    mTree->Branch("pi0_dgg", mPi0dgg, "pi0_dgg[pi0]/D");    
    mTree->Branch("pi0_Q1", mPi0Q1, "pi0_Q1[pi0]/I");    
    mTree->Branch("pi0_Q2", mPi0Q2, "pi0_Q2[pi0]/I");
    mTree->Branch("pi0_nTracks1", mPi0nTracks1, "pi0_nTracks1[pi0]/I");    
    mTree->Branch("pi0_nTracks2", mPi0nTracks2, "pi0_nTracks2[pi0]/I");
}

//_____________________________________________________________________________
void TStEmcTreeMaker::ResetBuffer()
{
    mEvtNo = -1;
    mMB = -1;
    mHT1 = -1;
    mHT2 = -1;

    mVz = -999;
    
    mNpoints = -1;
    memset(mX, -1, kMaxPoints*sizeof(Double_t));
    memset(mY, -1, kMaxPoints*sizeof(Double_t));
    memset(mZ, -1, kMaxPoints*sizeof(Double_t));
    memset(mPx, -1, kMaxPoints*sizeof(Double_t));
    memset(mPy, -1, kMaxPoints*sizeof(Double_t));
    memset(mPz, -1, kMaxPoints*sizeof(Double_t));
    memset(mE, -1, kMaxPoints*sizeof(Double_t));
    memset(mQ, -1, kMaxPoints*sizeof(Int_t));
    memset(mNtracks, -1, kMaxPoints*sizeof(Int_t));

    mNpi0 = -1;
    memset(mPi0X, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Y, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Z, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Px, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Py, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Pz, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0E, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Pt, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0M, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0theta, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0zgg, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0dgg, -1, kMaxPoints*sizeof(Double_t));
    memset(mPi0Q1, -1, kMaxPoints*sizeof(Int_t));
    memset(mPi0Q2, -1, kMaxPoints*sizeof(Int_t));
    memset(mPi0nTracks1, -1, kMaxPoints*sizeof(Int_t));
    memset(mPi0nTracks2, -1, kMaxPoints*sizeof(Int_t));
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
    mTrkMatchingMkr = (TStEmcTrackMatchingMaker*)GetMaker("EmcTrkMatching");
    if(!mTrkMatchingMkr)
    {
	cout << "TStEmcTreeMaker::Make- Unable to retrieve EmcTrackMatchingMaker. Can Not proceed" <<endl;
	return kStFatal;
    }
    // if(mTrigIDs.size() > 0)
    // {
    // 	if(!Accept(mMuDst->event()))
    // 	    return kStOK;
    // }    
    ResetBuffer();

    mEvtNo = mMuDst->event()->eventNumber(); // To be checked if this is invariant accross production
    if(HasTrigger(mMuDst->event(), 480003)) //MB
	mMB = 1;
    if(HasTrigger(mMuDst->event(), 480204)) //HT1
	mHT1 = 1;
    if(HasTrigger(mMuDst->event(), 480205)) //HT2
	mHT2 = 1;
    //Process event only if either MB, HT1 or HT2 present
    if(mMB == -1 && mHT1 == -1 && mHT2 == -1)  
	return kStOK;

    mVertex = mMuDst->primaryVertex(0)->position();
    //---------- Access EMC data ---------------
    mEmcCollection = mMuDst->emcCollection();
    if(!mEmcCollection)
    {
	cout<<"No EMC data for this event"<<endl;
	return kStSkip;
    }
    StSPtrVecEmcPoint& mEmcPoints = mEmcCollection->barrelPoints();
    mNpoints = mEmcPoints.size();
    mNpi0 = 0;
    mVz = mVertex.z();
    mTraits = mTrkMatchingMkr->GetPidTraits();
    //memcpy(&q[0][0], &mTraits->q[0][0], 1000*1000*sizeof(Int_t));
    for(Int_t i = 0; i < mNpoints; ++i)
    {
	mPoint1 = mEmcPoints[i];
	mV1 = mPoint1->position();  //Check if primary vertex position to be subtracted ???
	mE1 = mPoint1->energy();
	mQ1 = mPoint1->quality();

	mX[i] = mV1.x();
	mY[i] = mV1.y();
	mZ[i] = mV1.z();
	mPV = (mV1 - mVertex).unit();
	mPV *= mE1;
	mPx[i] = mPV.x(); 
	mPy[i] = mPV.y(); 
	mPz[i] = mPV.z(); 
	mE[i] = mE1;
	mQ[i] = mQ1;
	mNtracks[i] = mPoint1->nTracks();
	
	mLV1.SetPxPyPzE(mPx[i], mPy[i], mPz[i], mE[i]);

	//---------- Calculate and fill Pi0 kinematic varibles --------------
	for(Int_t j = i + 1; j < mNpoints; ++j)
	{
	    if(j >= mNpoints)
		continue;
		
	    mPoint2 = mEmcPoints[j];
	    mV2 = mPoint2->position(); // Check if primary vertex to be subtracted ???
	    mE2 = mPoint2->energy();
	    mQ2 = mPoint2->quality();
	    mTheta = (mV1 - mVertex).angle((mV2 - mVertex));
	    mZgg = fabs(mE1 - mE2) / (mE1 + mE2);
	    
	    mPi0X[mNpi0] = (mE1*mV1.x() + mE2*mV2.x()) / (mE1 + mE2);
	    mPi0Y[mNpi0] = (mE1*mV1.y() + mE2*mV2.y()) / (mE1 + mE2);
	    mPi0Z[mNpi0] = (mE1*mV1.z() + mE2*mV2.z()) / (mE1 + mE2);

	    mPV = (mV2 - mVertex).unit();
	    mPV *= mE2;
	    
	    mPx2 = mPV.x(); 
	    mPy2 = mPV.y(); 
	    mPz2 = mPV.z(); 
	    
	    mLV2.SetPxPyPzE(mPx2, mPy2, mPz2, mE2);
	    mLV = (mLV1 + mLV2);	  

	    //pairM = TMath::Sqrt(2*mE1*mE2*(1 - TMath::Cos(mTheta)));	 //Method-1
	    mPairM = mLV.M();  //Method-2

	    mPi0Px[mNpi0] = mLV.Px();
	    mPi0Py[mNpi0] = mLV.Py();
	    mPi0Pz[mNpi0] = mLV.Pz();
	    mPi0E[mNpi0] = mLV.E();
	    mPi0Pt[mNpi0] = mLV.Pt();
	    
	    mPi0M[mNpi0] = mPairM;
	    mPi0theta[mNpi0] = mTheta;
	    mPi0zgg[mNpi0] = mZgg;
	    mPi0dgg[mNpi0] = (mV1 - mV2).mag();
	    mPi0Q1[mNpi0] = mQ1;
	    mPi0Q2[mNpi0] = mQ2;
	    mPi0nTracks1[mNpi0] = mPoint1->nTracks();
	    mPi0nTracks2[mNpi0] = mPoint2->nTracks();
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



//_____________________________________________________________________________
Bool_t TStEmcTreeMaker::HasTrigger(StMuEvent *muEvent, Int_t triggerId)
{
    mTrigMuColl = muEvent->triggerIdCollection();
    const StTriggerId trgIDs = mTrigMuColl.nominal();
    if (trgIDs.isTrigger(triggerId))
	return kTRUE;
    else
	return kFALSE;
}
