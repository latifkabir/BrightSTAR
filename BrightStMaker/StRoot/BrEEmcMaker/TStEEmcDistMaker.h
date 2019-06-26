// Filename: TStEEmcDistMaker.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun 23 21:19:08 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTEEDISTMCMAKER_H
#define TSTEEDISTMCMAKER_H

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

class TStEEmcDistMaker : public StMaker
{
public:
    TStEEmcDistMaker(StMuDstMaker* muDstMaker, const char* name = "TStEEmcDistMaker");  
    virtual ~TStEEmcDistMaker();

    int Init();
    int InitRun(int runNumber);
    int Make();
    int Finish();
    int FinishRun(int runNumber);

    void setOutFile(const char* filename);

    EEDB *eeDb;
    TH1D *mHist1[10];
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

    ClassDef(TStEEmcDistMaker,1);
};

#endif 
