
#ifndef TSTEEMCMAKERGEOMEX_H
#define TSTEEMCMAKERGEOMEX_H


#include "StMaker.h"
#include "TH2D.h"


class StMuDstMaker;
class EEmcGeomSimple;
class StEEmcSmdGeom;
class StEEmcDb;

class TStEEmcMakerGeomEx : public StMaker
{
private:
    StMuDstMaker* mMuDstMaker;
    EEmcGeomSimple *geomTw;
    StEEmcSmdGeom  *geomSmd;
    StEEmcDb *eeDb;
    TString mFileName;
    TFile* mFile;
    TH2D *mHist2d[3];
public: 
    TStEEmcMakerGeomEx(const char *self="MuEEDemo", const char* muDstMakerName="muDstMaker");
    virtual       ~TStEEmcMakerGeomEx();
    virtual Int_t Init();
    virtual Int_t  Make();
    virtual Int_t Finish();
    void SetOutFile(const char* fileName);
    
    /// Displayed on session exit, leave it as-is please ...
    virtual const char *GetCVS() const
    {
	static const char cvs[]="Tag $Name:  $ $Id: TStEEmcMakerGeomEx.h,v 1.3 2014/08/06 11:43:03 jeromel Exp $ built " __DATE__ " " __TIME__ ; 
	return cvs;
    }

    ClassDef(TStEEmcMakerGeomEx, 1)   //StAF chain virtual base class for Makers
};

#endif

