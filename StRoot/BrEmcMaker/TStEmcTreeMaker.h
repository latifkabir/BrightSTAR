// Filename: TStEmcTreeMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:38:04 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSEMCTREEMAKER_H
#define TSEMCTREEMAKER_H

#include "StMaker.h"
#include "TLorentzVector.h"
#include "StEmcPoint.h"
#include "StEpcMaker/StEpcMaker.h"
#include "StEvent/StEmcPoint.h"
#include "StEvent/StEmcCollection.h"
#include "StEmcADCtoEMaker/StEmcADCtoEMaker.h"
#include "StPreEclMaker/StPreEclMaker.h"

class StEvent;
class StMuDst;
 
class TStEmcTreeMaker : public StMaker
{
private:
    StMuDst *mMuDst;  
    StEvent* mEvent;
    // Maker to apply calibration
    StEmcADCtoEMaker *mAdc2E;
    // Makers for cluster finding
    StPreEclMaker *mPreEcl;
    // Collection of points maker
    StEpcMaker *mEpc;

    StEmcCollection *mEmcCollection;
    StSPtrVecEmcPoint mEmcPoints;
    StEmcPoint *mPoint1;
    StEmcPoint *mPoint2;

    Int_t mNpoints;
    Double_t mPairE;
    Double_t mPairM;
    Double_t mZgg;
    Double_t mTheta;
    StThreeVectorF mV1;
    StThreeVectorF mV2;
    Double_t mE1;
    Double_t mE2;
    Int_t mQ1;
    Int_t mQ2;
    Double_t mPi0_x;
    Double_t mPi0_y;
    Double_t mPi0_z;
    TLorentzVector mLV1;
    TLorentzVector mLV2;
    TLorentzVector mLV;
    
protected:

    
public: 
    TStEmcTreeMaker(const char *name  = "EmcTreeMaker");
    virtual ~TStEmcTreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    // virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
    // virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 


    ClassDef(TStEmcTreeMaker, 1) 
};

#endif

