// Filename: TStPidTrait.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep 22 01:04:16 2019 (-0400)
// URL: jlab.org/~latif

struct TStPidTrait
{
    Int_t Q[20];         // Charge 
    Double_t P[20][3];   // Momentum Px, Py, Pz
    Double_t X[20][3];   // Position of projection on detector EMC, EEMC, FMS: x, y, z   
};
