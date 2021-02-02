// Filename: StjEEmcSmd.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jan 31 22:52:39 2021 (-0500)
// URL: jlab.org/~latif

#ifndef STJEEMCSMD_H
#define STJEEMCSMD_H

#include <TObject.h>

#include "StjTowerEnergyList.h"

class StjEEmcSmd : public TObject {

public:
  StjEEmcSmd() { }
  virtual ~StjEEmcSmd() { }

  virtual void Init() { }

  virtual StjTowerEnergyList getEnergyList() = 0;

  ClassDef(StjEEmcSmd, 1)

};

#endif // STJEEMCSMD_H
