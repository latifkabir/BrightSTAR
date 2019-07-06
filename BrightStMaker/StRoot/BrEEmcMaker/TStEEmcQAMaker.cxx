// Filename: TStEEmcQAMaker.cxx
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

#include "TStEEmcQAMaker.h"

ClassImp(TStEEmcQAMaker);

TStEEmcQAMaker:: TStEEmcQAMaker(StMuDstMaker* muDstMaker, const char* name)  
    : StMaker(name)
    , mFile(0)
{
    muMaker = muDstMaker;
    muDst = muMaker->muDst();
}
TStEEmcQAMaker::~TStEEmcQAMaker() {}

Int_t TStEEmcQAMaker::Init()
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
    mHist1[1] = new TH1D("h1", "Endcap tower raw ADC; ADC count", 200, 0.0, 0.0);
    mHist1[2] = new TH1D("h2", "Endcap tower energy; Energy [GeV]", 1000, 0.0, 0.0);
    mHist1[3] = new TH1D("h3", "Endcap SMD raw ADC U; ADC count", 600, 0.0, 0.0);
    mHist1[4] = new TH1D("h4", "Endcap SMD raw ADC V; ADC count", 600, 0.0, 0.0);
    mHist1[5] = new TH1D("h5", "Endcap SMD energy U; Energy [GeV]", 600, -0.001, 0.005);
    mHist1[6] = new TH1D("h6", "Endcap SMD energy V; Energy [GeV]", 600, -0.001, 0.005);
    mHist1[7] = new TH1D("h7", "Endcap Preshower-1 ADC; ADC count", 600, 0.0, 0.0);
    mHist1[8] = new TH1D("h8", "Endcap Preshower-1 Energy; Energy [GeV]", 600, 0.0, 0.0);
    mHist1[9] = new TH1D("h9", "Endcap Preshower-2 ADC; ADC count", 600, 0.0, 0.0);
    mHist1[10] = new TH1D("h10", "Endcap Preshower-2 Energy; Energy [GeV]", 600, 0.0, 0.0);
    mHist1[11] = new TH1D("h11", "Endcap Postshower ADC; ADC count", 600, 0.0, 0.0);
    mHist1[12] = new TH1D("h12", "Endcap Postshower Energy; Energy [GeV]", 600, 0.0, 0.0);
    mHist1[13] = new TH1D("h13", "Trigger Distribution; Trigger ID", 500, 480001.0, 480501.0);
    
    mHist2[0] = new TH2D("h2d1", "Tower x vs y; x [cm]; y[cm]", 28, -250.0, 250.0, 28, -250.0, 250.0);
    mHist2[1] = new TH2D("h2d2", "SMD x vs y; x[cm]; y[cm]", 100, 0.0, 0.0, 100, 0.0, 0.0);

    return StMaker::Init();
}

Int_t TStEEmcQAMaker::InitRun(Int_t runNumber)
{
    return StMaker::InitRun(runNumber);
}

Int_t TStEEmcQAMaker::Make()
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
    StMuTriggerIdCollection *trigMuColl = &muDst->event()->triggerIdCollection();
    //----------------------- Tower ----------------------------------------
    for (Int_t i = 0; i < emc->getNEndcapTowerADC(); ++i)
    {
	Int_t sector;			// 1-12
	Int_t subsector;		        // 1-5
	Int_t etabin;			// 1-12
	Int_t rawAdc;
	Int_t towerIndex;

	emc->getEndcapTowerADC(i, rawAdc, sector, subsector, etabin);
	towerIndex = (sector -1) * 60 + (subsector - 1) * 12 + (etabin - 1);
	TVector3 r= geomTw-> getTowerCenter(sector - 1, subsector - 1, etabin -1);
	if(rawAdc == 0)
	    continue;
	mHist1[0]->Fill(towerIndex);
	//mHist2[0]->Fill(r.x(), r.y(), rawAdc);
	// Sanity checks

	assert(1 <= sector && sector <= 12);
	assert(1 <= subsector && subsector <= 5);
	assert(1 <= etabin && etabin <= 12);

	// sector=1-12, subsector=A-E, etabin=1-12, type=T,P-R

	const EEmcDbItem* x = eeDb->getTile(sector, 'A' + subsector - 1, etabin, 'T');
	assert(x);

	Float_t adc = (Float_t)rawAdc - x->ped;
	mHist1[1]->Fill(rawAdc);
	Double_t energy = adc/x->gain;

	if(x->gain < 0) energy = 0.0;
	mHist2[0]->Fill(r.x(), r.y(), energy);
	mHist1[2]->Fill(energy);
    }
    //----------------------- SMD ----------------------------------------
    char uv = 'U';
    StMuEmcHit *hit;
    StThreeVectorD smdIntSect;
    
    Int_t sec_u,strip_u, id_u;
    Int_t sec_v,strip_v, id_v;
    Int_t iuv_u, istr_u, isec_u;
    Int_t iuv_v, istr_v, isec_v;
    Double_t energy_u, max_energy_u;
    Double_t energy_v, max_energy_v;
    Float_t rawAdc_u, adc_u;
    Float_t rawAdc_v, adc_v;
    const EEmcDbItem *x_u, *x_v;
    Int_t nh_u = emc->getNEndcapSmdHits('U');
    Int_t nh_v = emc->getNEndcapSmdHits('V');
    Int_t nh = (nh_u < nh_v) ? nh_u : nh_v;
    if(nh_u != nh_v)
	cout << "nHits did not match" <<endl;
    max_energy_u = 0.0;
    max_energy_v = 0.0;
    for (Int_t i = 0; i < nh; i++)
    {
	hit = emc->getEndcapSmdHit('U', i, sec_u, strip_u);
	rawAdc_u = hit->getAdc();
	id_u = hit->getId();
	x_u = eeDb->getByStrip(sec_u, 'U', strip_u);
	assert(x_u); 

	if(x_u->fail) continue;
	adc_u = rawAdc_u - x_u->ped;
	iuv_u = x_u->plane -'U';
	istr_u = x_u->strip - 1;
	isec_u = sec_u - 1;

	assert(iuv_u >= 0 && iuv_u < 2);
	assert(istr_u >= 0 && istr_u < 288);
	if(x_u->gain <= 0) continue;
	
	hit = emc->getEndcapSmdHit('V', i, sec_v, strip_v);
	rawAdc_v = hit->getAdc();
	id_v = hit->getId();
	x_v = eeDb->getByStrip(sec_v, 'V', strip_v);
	assert(x_v); 
	
	if(x_v->fail) continue;
	adc_v = rawAdc_v - x_v->ped;
	iuv_v = x_v->plane -'U';
	istr_v = x_v->strip - 1;
	isec_v = sec_v - 1;

	assert(iuv_v >= 0 && iuv_v < 2);
	assert(istr_v >= 0 && istr_v < 288);
	if(x_v->gain <= 0) continue;

	energy_u = adc_u / x_u->gain;
	energy_v = adc_v / x_v->gain;
	if(energy_u > max_energy_u)
	    max_energy_u = energy_u;
	if(energy_v > max_energy_v)
	    max_energy_v = energy_v;
	mHist1[3]->Fill(adc_u);
	mHist1[4]->Fill(adc_v);
	mHist1[5]->Fill(energy_u);
	mHist1[6]->Fill(energy_v);

	if(isec_u == isec_v && adc_u > 0 && adc_v > 0 && id_u == id_v)
	{
	   smdIntSect = geomSmd->getIntersection(isec_u, istr_u, istr_v);
	   if(energy_u > 0.002 && energy_v > 0.002)
	       mHist2[1]->Fill(smdIntSect.x(), smdIntSect.y());
	}		
    }
    
    //-------------------------Pre and Post shower -----------------------------------------
    Int_t sec_p,eta_p,sub_p, pre, rawAdc_p;
    Double_t adc_p, energy_p;
    Int_t index_p;
    const EEmcDbItem* x_p;
    Int_t nh_p = emc->getNEndcapPrsHits();
    //cout<<"\nTotal "<<nh<<" hits in pre1+2+post\n"<<endl;
    for (Int_t i = 0; i < nh_p; i++)
    {
	hit = emc->getEndcapPrsHit(i,sec_p, sub_p, eta_p, pre);
	rawAdc_p = hit->getAdc();

	if(pre == 1) //Pre-shower-1
	    x_p = eeDb->getTile(sec_p, 'A' + sub_p - 1, eta_p, 'P');
	else if(pre == 2) //Pre-shower-2
	    x_p = eeDb->getTile(sec_p, 'A' + sub_p - 1, eta_p, 'Q');
	else if(pre == 3) //Post-shower
	    x_p = eeDb->getTile(sec_p, 'A' + sub_p - 1, eta_p, 'R');
	assert(x_p);
	assert(pre>=1 && pre<=3);
	adc_p =  rawAdc_p - x_p->ped;
	energy_p = adc_p / x_p->gain;       
	index_p = (sec_p - 1) * 60 + (sub_p - 1) * 12 + (eta_p - 1);

	if(pre == 1)
	{
	    mHist1[7]->Fill(adc_p);
	    mHist1[8]->Fill(energy_p);
	}
	else if(pre == 2)
	{
	    mHist1[9]->Fill(adc_p);
	    mHist1[10]->Fill(energy_p);
	}	    
	else if(pre == 3)
	{
	    mHist1[11]->Fill(adc_p);
	    mHist1[12]->Fill(energy_p);
	}	    
	//prInt_tf("i=%d  pre/post(%d) %2.2d%c%c%2.2d : energy=%f  adc=%d\n", i, pre, sec_p, pre+'P'-1, sub_p+'A'-1, eta_p, hit->getEnergy(), adc_p);
    }        
    //----------------- Trigger Distribution ----------------------------    
    if(!trigMuColl)
    {
	cout<<"No Trigger data for this event"<<endl;
	return -1;
    }

    const StTriggerId trgIDs = trigMuColl->nominal();
    if(max_energy_u >= 0.001 && max_energy_v >= 0.001)
    {
	for(Int_t i = 0; i < trgIDs.triggerIds().size(); i++)
	    mHist1[13]->Fill(trgIDs.triggerIds().at(i));
    }
    
    return kStOk;
}

void TStEEmcQAMaker::setOutFile(const char* filename)
{
    mFileName = filename;
}

Int_t TStEEmcQAMaker::Finish()
{
    mFile->Write();
    mFile->Close();

    return kStOk;
}

Int_t TStEEmcQAMaker::FinishRun(Int_t runNumber)
{
    return StMaker::FinishRun(runNumber);
}
