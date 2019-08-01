// Filename: TStEEmcQAMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun 23 21:19:08 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTEEMCQAMAKER_H
#define TSTEEMCQAMAKER_H

#include "TH2D.h"
#include "StMaker.h"
#include "StChain.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"
#include "StEEmcUtil/StEEmcSmd/StEEmcSmdGeom.h"
// BEMC stuff

class StEmcDecoder;
class StBemcTables;
class StMuDstMaker;
class StMuDst;
// EEMC stuff

class StEEmcDbMaker;
class StEEmcDb;
typedef StEEmcDb EEDB;

#include "StMaker.h"
#include "TDatime.h"

class TStEEmcQAMaker : public StMaker
{
public:
    TStEEmcQAMaker(StMuDstMaker* muDstMaker, const char* name = "TStEEmcQAMaker");  
    virtual ~TStEEmcQAMaker();

    Int_t Init();
    Int_t InitRun(Int_t runNumber);
    Int_t Make();
    Int_t Finish();
    Int_t FinishRun(Int_t runNumber);

    void setOutFile(const char* filename);

    EEDB *eeDb;
    TH1D *mHist1[20];
    TH2D *mHist2[2];

private:
    StMuDstMaker* muMaker;
    StMuDst* muDst;
    // Barrel stuff

    StEmcDecoder* mEmcDecoder;
    StBemcTables* mBemcTables;
    EEmcGeomSimple* geomTw;
    StEEmcSmdGeom* geomSmd;
    // Endcap stuff

    StEEmcDbMaker* mEEmcDb;

    TString mFileName;
    TFile* mFile;

    ClassDef(TStEEmcQAMaker,1);
};

#endif 
