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

TStEEmcEvtDispMaker:: TStEEmcEvtDispMaker(StMuDstMaker* muDstMaker,  TH1F **hist1d, Int_t sector, const char* name)  
    : StMaker(name)
    , mFile(0)
{
    muMaker = muDstMaker;
    muDst = muMaker->muDst();
    mSector = sector;

    mHist1d[0] = new TH1F("dist0", (TString)"Tower raw ADC Distribution, Sector = " + (TString)to_string(mSector) + ";Tower index; ADC count", 60, 60.0, 120.0);
    mHist1d[1] = new TH1F("dist1", (TString)"Tower Energy Distribution, Sector = " + (TString)to_string(mSector) + ";Tower index; Energy [GeV]", 60, 60.0, 120.0);
    mHist1d[2] = new TH1F("dist2", (TString)"SMD U plane raw ADC, Sector = " + (TString)to_string(mSector) + ";Strip no.; ADC count", 288, 0.0, 288.0);
    mHist1d[3] = new TH1F("dist3", (TString)"SMD U plane energy, Sector = " + (TString)to_string(mSector) + ";Strip no.; Energy [GeV]", 288, 0.0, 288.0);
    mHist1d[4] = new TH1F("dist4", (TString)"SMD V plane ADC, Sector = " + (TString)to_string(mSector) + ";Strip no.; ADC count", 288, 0.0, 288.0);
    mHist1d[5] = new TH1F("dist5", (TString)"SMD V plane energy, Sector = " + (TString)to_string(mSector) + ";Strip no.; Energy [GeV]", 288, 0.0, 288.0);
    mHist1d[6] = new TH1F("dist6", (TString)"Preshower-1 ADC, Sector = " + (TString)to_string(mSector) + ";Tile index; ADC count", 60, 60.0, 120.0);
    mHist1d[7] = new TH1F("dist7", (TString)"Preshower-2 ADC, Sector = " + (TString)to_string(mSector) + ";Tile index; ADC count", 60, 60.0, 120.0);
    mHist1d[8] = new TH1F("dist8", (TString)"Postshower ADC, Sector = " + (TString)to_string(mSector) + ";Tile index; ADC count", 60, 60.0, 120.0);    
    mHist1d[9] = new TH1F("dist9", (TString)"Preshower-1 energy, Sector = " + (TString)to_string(mSector) + ";Tile index; energy [GeV]", 60, 60.0, 120.0);
    mHist1d[10] = new TH1F("dist10", (TString)"Preshower-2 energy, Sector = " + (TString)to_string(mSector) + ";Tile index; energy [GeV]", 60, 60.0, 120.0);    
    mHist1d[11] = new TH1F("dist11", (TString)"Postshower energy, Sector = " + (TString)to_string(mSector) + ";Tile index; energy [GeV]", 60, 60.0, 120.0);
    
    for(Int_t i = 0; i < 12; ++i)
	hist1d[i] = mHist1d[i];
}
TStEEmcEvtDispMaker::~TStEEmcEvtDispMaker() {}

Int_t TStEEmcEvtDispMaker::Init()
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

Int_t TStEEmcEvtDispMaker::InitRun(Int_t runNumber)
{
    return StMaker::InitRun(runNumber);
}

Int_t TStEEmcEvtDispMaker::Make()
{
    // Get MuDst
    if (!GetDataSet("MuDst"))
    {
	LOG_WARN << "No MuDst" << endm;
	return kStWarn;
    }
    for(Int_t i = 0; i < 6; ++i)
	mHist1d[i]->Reset();
    Int_t sector_t;			// 1-12
    Int_t subsector_t;	        	// 1-5
    Int_t etabin_t;			// 1-12
    Int_t rawAdc_t;
    Int_t towerIndex;
    
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
    for (Int_t ii = 0; ii < emcMuColl->getNEndcapTowerADC(); ++ii)
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
    //-------------------------Pre and Post shower -----------------------------------------
    Int_t sec_p,eta_p,sub_p, pre, rawAdc_p;
    Int_t nh = emcMuColl->getNEndcapPrsHits();
    //cout<<"\nTotal "<<nh<<" hits in pre1+2+post\n"<<endl;
    for (Int_t i = 0; i < nh; i++)
    {
	hit = emcMuColl->getEndcapPrsHit(i,sec_p, sub_p, eta_p, pre);
	rawAdc_p = hit->getAdc();
	const EEmcDbItem* x_p;
	if(pre == 1) //Pre-shower-1
	    x_p = eeDb->getTile(sec_p, 'A' + sub_p - 1, eta_p, 'P');
	else if(pre == 2) //Pre-shower-2
	    x_p = eeDb->getTile(sec_p, 'A' + sub_p - 1, eta_p, 'Q');
	else if(pre == 3) //Post-shower
	    x_p = eeDb->getTile(sec_p, 'A' + sub_p - 1, eta_p, 'R');
	assert(x_p);
	assert(pre>=1 && pre<=3);
	float adc_p =  rawAdc_p - x_p->ped;
	double energy_p = adc_p / x_p->gain;       
	Int_t index_p = (sec_p - 1) * 60 + (sub_p - 1) * 12 + (eta_p - 1);

	mHist1d[5 + pre]->SetBinContent(index_p, adc_p);
	mHist1d[8 + pre]->SetBinContent(index_p, energy_p);
	//printf("i=%d  pre/post(%d) %2.2d%c%c%2.2d : energy=%f  adc=%d\n", i, pre, sec_p, pre+'P'-1, sub_p+'A'-1, eta_p, hit->getEnergy(), adc_p);
    }        
    //cout <<"Event number:"<< muDst->event()->eventNumber()<<endl;
    return kStOk;
}

void TStEEmcEvtDispMaker::setOutFile(const char* filename)
{
    //mFileName = filename;
}

Int_t TStEEmcEvtDispMaker::Finish()
{
    // mFile->Write();
    // mFile->Close();
    return kStOk;
}

Int_t TStEEmcEvtDispMaker::FinishRun(Int_t runNumber)
{
    return StMaker::FinishRun(runNumber);
}
