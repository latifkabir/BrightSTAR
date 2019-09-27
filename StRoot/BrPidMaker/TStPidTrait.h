// Filename: TStPidTrait.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep 22 01:04:16 2019 (-0400)
// URL: jlab.org/~latif

#ifndef TSTPIDTRAIT_H
#define TSTPIDTRAIT_H

#include "TStPidTagger.h"

struct TStPidTrait
{
    Int_t Q[TStPidTagger::kMaxTracks];         // Charge 
    Double_t P[TStPidTagger::kMaxTracks][3];   // Momentum Px, Py, Pz
    Double_t X[TStPidTagger::kMaxTracks][3];   // Position of projection on detector EMC, EEMC, FMS: x, y, z   
};

#endif
