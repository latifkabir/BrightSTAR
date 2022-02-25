// Filename: EjRePlotAn.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Jul 26 02:03:35 2021 (-0400)
// URL: jlab.org/~latif

#include "TString.h"
#include "TCanvas.h"

void EjCanvasPartition(TCanvas *C,const Int_t Nx = 2,const Int_t Ny = 2,
                     Float_t lMargin = 0.15, Float_t rMargin = 0.05,
                     Float_t bMargin = 0.15, Float_t tMargin = 0.05);

void EjRePlotAn(TString fileName, TString det = "fms",  Int_t isMerged = 0);
