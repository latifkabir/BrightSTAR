// Filename: TStEEmcEvtDispMaker.cxx
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

#include "TStEEmcEvtDispMaker.h"

ClassImp(TStEEmcEvtDispMaker);

TStEEmcEvtDispMaker:: TStEEmcEvtDispMaker(StMuDstMaker* muDstMaker,  TH1F **hist1d, int sector, const char* name)  
    : StMaker(name)
    , mFile(0)
{
    muMaker = muDstMaker;
    muDst = muMaker->muDst();
    mSector = sector;

    mHist1d[0] = new TH1F("dist0", (TString)"Tower raw ADC Distribution, sec = " + (TString)to_string(mSector), 60, 60.0, 120.0);
    mHist1d[1] = new TH1F("dist1", (TString)"Tower Energy Distribution, sec = " + (TString)to_string(mSector), 60, 60.0, 120.0);
    mHist1d[2] = new TH1F("dist2", (TString)"SMD U plane raw ADC, sec = " + (TString)to_string(mSector), 288, 0.0, 288.0);
    mHist1d[3] = new TH1F("dist3", (TString)"SMD U plane energy, sec = " + (TString)to_string(mSector), 288, 0.0, 288.0);
    mHist1d[4] = new TH1F("dist4", (TString)"SMD V plane ADC, sec = " + (TString)to_string(mSector), 288, 0.0, 288.0);
    mHist1d[5] = new TH1F("dist5", (TString)"SMD V plane energy, sec = " + (TString)to_string(mSector), 288, 0.0, 288.0);

    hist1d[0] = mHist1d[0];
    hist1d[1] = mHist1d[1];
    hist1d[2] = mHist1d[2];
    hist1d[3] = mHist1d[3];
    hist1d[4] = mHist1d[4];
    hist1d[5] = mHist1d[5];
}
TStEEmcEvtDispMaker::~TStEEmcEvtDispMaker() {}

int TStEEmcEvtDispMaker::Init()
{
    muMaker->SetStatus("*",0);
    muMaker->SetStatus("MuEvent",1);
    muMaker->SetStatus("Emc*",1);
    muMaker->SetStatus("EEmc*",1);
    
    eeDb = (StEEmcDb*)this->GetDataSet("StEEmcDb");
    assert(eeDb);
    geomTw= new EEmcGeomSimple(); // tower geomtry
    geomSmd =  StEEmcSmdGeom::instance(); //strip geometry, do NOT call new StEEmcSmdGeom()
    
    // assert(!mFileName.IsNull());
    // mFile = TFile::Open(mFileName,"recreate");
    // assert(mFile);
    
    return StMaker::Init();
}

int TStEEmcEvtDispMaker::InitRun(int runNumber)
{
    return StMaker::InitRun(runNumber);
}

int TStEEmcEvtDispMaker::Make()
{
    // Get MuDst
    if (!GetDataSet("MuDst"))
    {
	LOG_WARN << "No MuDst" << endm;
	return kStWarn;
    }
    for(int i = 0; i < 6; ++i)
	mHist1d[i]->Reset();
    int sector_t;			// 1-12
    int subsector_t;	        	// 1-5
    int etabin_t;			// 1-12
    int rawAdc_t;
    int towerIndex;
    
    Int_t sec_u, strip_u, adc_u, X, id_u, rawAdc_u;
    Int_t sec_v, strip_v, adc_v, Y, id_v, rawAdc_v;
    StMuEmcCollection *emcMuColl;
    StMuEmcHit *hit;

    emcMuColl = muDst->muEmcCollection();	
    if(!emcMuColl)
    {
	cout<<"No EMC data for this event"<<endl;
	return -1;
    }
    //-------------------- Tower-------------------------------------
    for (int ii = 0; ii < emcMuColl->getNEndcapTowerADC(); ++ii)
    {	    
	emcMuColl->getEndcapTowerADC(ii, rawAdc_t, sector_t, subsector_t, etabin_t);
	towerIndex = (sector_t -1) * 60 + (subsector_t - 1) * 12 + (etabin_t - 1);

	assert(1 <= sector_t && sector_t <= 12);
	assert(1 <= subsector_t && subsector_t <= 5);
	assert(1 <= etabin_t && etabin_t <= 12);

	// sector_t = 1-12, subsector_t = A-E, etabin_t = 1-12, type=T,P-R

	const EEmcDbItem* x_t = eeDb->getTile(sector_t, 'A' + subsector_t - 1, etabin_t, 'T');

	assert(x_t);

	float adc_t = (float)rawAdc_t - x_t->ped;
	double energy_t = adc_t / x_t->gain;
	if(x_t->gain < 0)
	    energy_t = 0.0;
	if(sector_t == mSector)
	{
	    mHist1d[0]->SetBinContent(towerIndex + 1 - (sector_t-1)*60, rawAdc_t);
	    mHist1d[1]->SetBinContent(towerIndex + 1 - (sector_t-1)*60, energy_t);
	}
    }	
    //------------------- SMD ----------------------------------------- 
    Int_t nh_u = emcMuColl->getNEndcapSmdHits('U');
    Int_t nh_v = emcMuColl->getNEndcapSmdHits('V');

    for (Int_t i = 0; i < nh_u; i++)
    {
	hit = emcMuColl->getEndcapSmdHit('U', i, sec_u, strip_u);
	rawAdc_u = hit->getAdc();
	id_u = hit->getId();
	const EEmcDbItem *x_u = eeDb->getByStrip(sec_u, 'U', strip_u);
	assert(x_u); 
	if(x_u->fail) continue;
	float adc_u = rawAdc_u - x_u->ped;
	double energy_u = adc_u / x_u->gain;
	
	if(sec_u == mSector)
	{
	    mHist1d[2]->SetBinContent(strip_u, rawAdc_u); //You should probably also match u and v hit ids
	    mHist1d[3]->SetBinContent(strip_u, energy_u); //You should probably also match u and v hit ids
	}
    }
    for (Int_t i = 0; i < nh_v; i++)
    {		
	hit = emcMuColl->getEndcapSmdHit('V', i, sec_v, strip_v);
	rawAdc_v = hit->getAdc();
	id_v = hit->getId();
	const EEmcDbItem *x_v = eeDb->getByStrip(sec_v, 'V', strip_v);
	assert(x_v); 
	
	if(x_v->fail) continue;
	float adc_v = rawAdc_v - x_v->ped;
	double energy_v = adc_v / x_v->gain;	
	
	if(sec_v == mSector)
	{
	    mHist1d[4]->SetBinContent(strip_v, rawAdc_v); //You should probably also match u and v hit ids
	    mHist1d[5]->SetBinContent(strip_v, energy_v); //You should probably also match u and v hit ids
	}
    }		

    return kStOk;
}

void TStEEmcEvtDispMaker::setOutFile(const char* filename)
{
    //mFileName = filename;
}

int TStEEmcEvtDispMaker::Finish()
{
    // mFile->Write();
    // mFile->Close();

    return kStOk;
}

int TStEEmcEvtDispMaker::FinishRun(int runNumber)
{
    return StMaker::FinishRun(runNumber);
}
