// Filename: StjEEmcSMD.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jan 31 22:52:39 2021 (-0500)
// URL: jlab.org/~latif

#ifndef STJEEMCSMD_H
#define STJEEMCSMD_H

#include <TObject.h>

#include "StjTowerEnergyList.h"

class StjEEmcSMD : public TObject {

public:
  StjEEmcSMD() { }
  virtual ~StjEEmcSMD() { }

  virtual void Init() { }

  virtual StjTowerEnergyList getEnergyList() = 0;

  ClassDef(StjEEmcSMD, 1)

};

#endif // STJEEMCSMD_H
