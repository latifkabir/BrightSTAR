// Sample MuDst reader
//

#ifndef TSTEEMCMAKEREX1_H
#define TSTEEMCMAKEREX1_H

#ifndef StMaker_H
#include "StMaker.h"
#endif
#include<StChain.h>
// BEMC stuff

class StEmcDecoder;
class StBemcTables;

// EEMC stuff

class StEEmcDbMaker;
class StEEmcDb;
typedef StEEmcDb EEDB;

#include "StMaker.h"
#include "TDatime.h"

class TStEEmcMakerEx1 : public StMaker {
public:
  TStEEmcMakerEx1(const char* name = "TStEEmcMakerEx1")  
  : StMaker(name)
  , mFile(0)
  {
  }

  ~TStEEmcMakerEx1() {}

  int Init();
  int InitRun(int runNumber);
  int Make();
  int Finish();
  int FinishRun(int runNumber);

  void setOutFile(const char* filename);

  EEDB *eeDb;
  TH1D *hist_tower_energy;
  TH1D *hist_smd_energy;

private:
  // Barrel stuff

  StEmcDecoder* mEmcDecoder;
  StBemcTables* mBemcTables;

  // Endcap stuff

  StEEmcDbMaker* mEEmcDb;

  TString mFileName;
  TFile* mFile;

  ClassDef(TStEEmcMakerEx1,1);
};

#endif 
