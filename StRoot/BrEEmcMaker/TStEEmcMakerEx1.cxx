// Sample MuDst reader

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

#include "TStEEmcMakerEx1.h"

ClassImp(TStEEmcMakerEx1);

int TStEEmcMakerEx1::Init()
{
  printf("##########################################\n");
  printf("                  Init()                  \n");
  printf("##########################################\n");

  eeDb = (StEEmcDb*)this->GetDataSet("StEEmcDb");
  assert(eeDb);

  assert(!mFileName.IsNull());

  mFile = TFile::Open(mFileName,"recreate");
  assert(mFile);

  hist_tower_energy= new TH1D("hist_tower_energy","Endcap Energy",300,0,30.);
  hist_smd_energy= new TH1D("hist_smd_energy","Endcap Energy",100,0,0.1);

  return StMaker::Init();
}

int TStEEmcMakerEx1::InitRun(int runNumber)
{

  printf("##########################################\n");
  printf("                  InitRun()               \n");
  printf("##########################################\n");

  return StMaker::InitRun(runNumber);
}

int TStEEmcMakerEx1::Make()
{

  // Get MuDst

  if (!GetDataSet("MuDst")) {
    LOG_WARN << "No MuDst" << endm;
    return kStWarn;
  }

  // Get run number and event number

  // LOG_INFO << "runNumber=" << GetRunNumber() << ", eventNumber" << GetEventNumber() << endm;

  StMuEmcCollection* emc = StMuDst::muEmcCollection();

  for (int i = 0; i < emc->getNEndcapTowerADC(); ++i) {
    int sector;			// 1-12
    int subsector;		// 1-5
    int etabin;			// 1-12
    int rawAdc;

    emc->getEndcapTowerADC(i, rawAdc, sector, subsector, etabin);

    // Sanity checks

    assert(1 <= sector && sector <= 12);
    assert(1 <= subsector && subsector <= 5);
    assert(1 <= etabin && etabin <= 12);

    // sector=1-12, subsector=A-E, etabin=1-12, type=T,P-R

    const EEmcDbItem* x = eeDb->getTile(sector, 'A'+subsector-1, etabin, 'T');

    assert(x);

    float adc= (float)rawAdc - x->ped;
    double energy = adc/x->gain;

    if(x->gain < 0) energy=0.0;

    hist_tower_energy->Fill(energy);

  }

  char uv='U';
  for(uv='U'; uv <='V'; uv++) {
  int sec,strip;
  int nh= emc->getNEndcapSmdHits(uv);
  int i;
  for (i=0; i < nh; i++) {
    StMuEmcHit *hit=emc->getEndcapSmdHit(uv,i,sec,strip);
    float rawAdc=hit->getAdc();
    const EEmcDbItem *x=eeDb->getByStrip(sec,uv,strip);
    assert(x); 

    if(x->fail) continue;
    float adc=rawAdc-x->ped;
    int iuv=x->plane-'U';
    int istr=x->strip -1;
    int isec=sec -1;

    assert(iuv>=0 && iuv<2);
    assert(istr>=0 && istr<288);

    if(x->gain<=0) continue;

    double energy =adc/x->gain;
    hist_smd_energy->Fill(energy);
    }
  }
  return kStOk;
}

void TStEEmcMakerEx1::setOutFile(const char* filename)
{
  mFileName = filename;
}

int TStEEmcMakerEx1::Finish()
{

  printf("##########################################\n");
  printf("                  Finish()                \n");
  printf("##########################################\n");

  mFile->Write();
  mFile->Close();

  return kStOk;
}

int TStEEmcMakerEx1::FinishRun(int runNumber)
{

  printf("##########################################\n");
  printf("                  FinishRun()             \n");
  printf("##########################################\n");

  return StMaker::FinishRun(runNumber);
}
