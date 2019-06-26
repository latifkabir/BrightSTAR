// Filename: TStEEmcDistMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun 23 21:19:50 2019 (-0400)
// URL: jlab.org/~latif

// MuDst stuff
#include <TFile.h>
#include <TH1.h>
#include "TROOT.h"
#include "TFile.h"
#include "StEventTypes.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
// BEMC stuff
#include "StDaqLib/EMC/StEmcDecoder.h"
#include "StEmcUtil/database/StBemcTables.h"
#include "StEmcRawMaker/defines.h"
// EEMC stuff
#include "StEEmcDbMaker/StEEmcDbMaker.h"
#include "StEEmcUtil/database/StEEmcDb.h"
#include "StEEmcUtil/database/EEmcDbItem.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"
// Local stuff

#include "TStEEmcDistMaker.h"

ClassImp(TStEEmcDistMaker);

TStEEmcDistMaker:: TStEEmcDistMaker(StMuDstMaker* muDstMaker, const char* name)  
    : StMaker(name)
    , mFile(0)
{
    muMaker = muDstMaker;
    muDst = muMaker->muDst();
}
TStEEmcDistMaker::~TStEEmcDistMaker() {}

int TStEEmcDistMaker::Init()
{
    muMaker->SetStatus("*",0);
    muMaker->SetStatus("MuEvent",1);
    muMaker->SetStatus("Emc*",1);
    muMaker->SetStatus("EEmc*",1);
    
    eeDb = (StEEmcDb*)this->GetDataSet("StEEmcDb");
    assert(eeDb);
    geomTw= new EEmcGeomSimple(); // tower geomtry
    geomSmd =  StEEmcSmdGeom::instance(); //strip geometry, do NOT call new StEEmcSmdGeom()
    
    assert(!mFileName.IsNull());
    mFile = TFile::Open(mFileName,"recreate");
    assert(mFile);

    mHist1[0] = new TH1D("h0", "Endcap tower index", 730, 0.0, 730.);
    mHist1[1] = new TH1D("h1", "Endcap tower raw ADC", 200, 0.0, 0.0);
    mHist1[2] = new TH1D("h2", "Endcap tower energy", 1000, -0.5, 0.5);
    mHist1[3] = new TH1D("h3", "Endcap SMD raw ADC U", 600, 0.0, 0.0);
    mHist1[4] = new TH1D("h4", "Endcap SMD raw ADC V", 600, 0.0, 0.0);
    mHist1[5] = new TH1D("h5", "Endcap SMD energy U", 600, -0.001, 0.005);
    mHist1[6] = new TH1D("h6", "Endcap SMD energy V", 600, -0.001, 0.005);

    mHist2[0] = new TH2D("h2d1", "Tower x vs y", 100, 0.0, 0.0, 100, 0.0, 0.0);
    mHist2[1] = new TH2D("h2d2", "SMD x vs y", 100, 0.0, 0.0, 100, 0.0, 0.0);

    return StMaker::Init();
}

int TStEEmcDistMaker::InitRun(int runNumber)
{
    return StMaker::InitRun(runNumber);
}

int TStEEmcDistMaker::Make()
{
    // Get MuDst

    if (!GetDataSet("MuDst"))
    {
	LOG_WARN << "No MuDst" << endm;
	return kStWarn;
    }

    // Get run number and event number

    // LOG_INFO << "runNumber=" << GetRunNumber() << ", eventNumber" << GetEventNumber() << endm;

    StMuEmcCollection* emc = muDst->muEmcCollection();

    for (int i = 0; i < emc->getNEndcapTowerADC(); ++i)
    {
	int sector;			// 1-12
	int subsector;		        // 1-5
	int etabin;			// 1-12
	int rawAdc;
	int towerIndex;

	emc->getEndcapTowerADC(i, rawAdc, sector, subsector, etabin);
	towerIndex = (sector -1) * 60 + (subsector - 1) * 12 + (etabin - 1);
	TVector3 r= geomTw-> getTowerCenter(sector - 1, subsector - 1, etabin -1);
	if(rawAdc == 0)
	    continue;
	mHist1[0]->Fill(towerIndex);
	mHist2[0]->Fill(r.x(), r.y());
	// Sanity checks

	assert(1 <= sector && sector <= 12);
	assert(1 <= subsector && subsector <= 5);
	assert(1 <= etabin && etabin <= 12);

	// sector=1-12, subsector=A-E, etabin=1-12, type=T,P-R

	const EEmcDbItem* x = eeDb->getTile(sector, 'A'+subsector-1, etabin, 'T');

	assert(x);

	float adc = (float)rawAdc - x->ped;
	mHist1[1]->Fill(rawAdc);
	double energy = adc/x->gain;

	if(x->gain < 0) energy = 0.0;
	mHist1[2]->Fill(energy);
    }

    char uv = 'U';
    StMuEmcHit *hit;
    StThreeVectorD smdIntSect;
    
    int sec_u,strip_u, id_u;
    int sec_v,strip_v, id_v;
    int nh_u = emc->getNEndcapSmdHits('U');
    int nh_v = emc->getNEndcapSmdHits('V');
    Int_t nh = (nh_u < nh_v) ? nh_u : nh_v;
    if(nh_u != nh_v)
	cout << "nHits did not match" <<endl;
       
    for (int i = 0; i < nh; i++)
    {
	hit = emc->getEndcapSmdHit('U', i, sec_u, strip_u);
	float rawAdc_u = hit->getAdc();
	id_u = hit->getId();
	const EEmcDbItem *x_u = eeDb->getByStrip(sec_u, 'U', strip_u);
	assert(x_u); 

	if(x_u->fail) continue;
	float adc_u = rawAdc_u - x_u->ped;
	int iuv_u = x_u->plane -'U';
	int istr_u = x_u->strip - 1;
	int isec_u = sec_u - 1;

	assert(iuv_u >= 0 && iuv_u < 2);
	assert(istr_u >= 0 && istr_u < 288);
	if(x_u->gain <= 0) continue;
	
	hit = emc->getEndcapSmdHit('V', i, sec_v, strip_v);
	float rawAdc_v = hit->getAdc();
	id_v = hit->getId();
	const EEmcDbItem *x_v = eeDb->getByStrip(sec_v, 'V', strip_v);
	assert(x_v); 
	
	if(x_v->fail) continue;
	float adc_v = rawAdc_v - x_v->ped;
	int iuv_v = x_v->plane -'U';
	int istr_v = x_v->strip - 1;
	int isec_v = sec_v - 1;

	assert(iuv_v >= 0 && iuv_v < 2);
	assert(istr_v >= 0 && istr_v < 288);
	if(x_v->gain <= 0) continue;

	if(isec_u == isec_v && adc_u > 0 && adc_v > 0 && id_u == id_v)
	{
	   smdIntSect = geomSmd->getIntersection(isec_u, istr_u, istr_v);
	   mHist2[1]->Fill(smdIntSect.x(), smdIntSect.y());
	}
	
	double energy_u = adc_u / x_u->gain;
	double energy_v = adc_v / x_v->gain;
	mHist1[3]->Fill(adc_u);
	mHist1[4]->Fill(adc_v);
	mHist1[5]->Fill(energy_u);
	mHist1[6]->Fill(energy_v);
    }
    
    return kStOk;
}

void TStEEmcDistMaker::setOutFile(const char* filename)
{
    mFileName = filename;
}

int TStEEmcDistMaker::Finish()
{
    mFile->Write();
    mFile->Close();

    return kStOk;
}

int TStEEmcDistMaker::FinishRun(int runNumber)
{
    return StMaker::FinishRun(runNumber);
}
