// Filename: AnReadEEMC2.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun  2 03:06:14 2019 (-0400)
// URL: jlab.org/~latif

class StMuEmcCollection;

void AnReadEEMC2(Int_t runNo = 16066033, Int_t maxFiles = -1);
void printEEtower( StMuEmcCollection* emc );
void printEEpre( StMuEmcCollection* emc );
void printEEsmd( StMuEmcCollection* emc );
