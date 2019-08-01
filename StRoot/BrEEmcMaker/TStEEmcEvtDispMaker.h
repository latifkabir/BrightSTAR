// Filename: TStEEmcEvtDispMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun 23 21:19:08 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTEEMCEVTDISPMCMAKER_H
#define TSTEEMCEVTDISPMCMAKER_H

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

class TStEEmcEvtDispMaker : public StMaker
{
public:
    TStEEmcEvtDispMaker(StMuDstMaker* muDstMaker, TH1F **hist1d, int sector = 2, const char* name = "TStEEmcEvtDispMaker");  
    virtual ~TStEEmcEvtDispMaker();

    int Init();
    int InitRun(int runNumber);
    int Make();
    int Finish();
    int FinishRun(int runNumber);

    void setOutFile(const char* filename);
private:
    EEDB *eeDb;

    TH1F *mHist1d[12];
    TH2F *mHist2d[2];
    int mSector;
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

    ClassDef(TStEEmcEvtDispMaker,1);
};

#endif 
