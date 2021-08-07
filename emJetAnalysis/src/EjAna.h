// Filename: EjAna.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Oct 18 12:40:20 2020 (-0400)
// URL: jlab.org/~latif

#ifndef EJANA_H
#define EJANA_H

#include "TString.h"

class EjAna
{
public:
    static TString kDefaultDet;
        
    static Double_t kFmsEtaMin;
    static Double_t kFmsEtaMax;

    static Double_t kEEmcEtaMin;
    static Double_t kEEmcEtaMax;
    
    static Double_t kPtMin;
    static Double_t kFmsEngMin; // Fms Photon energy cut
    static Double_t kFmsEngMax; // Fms Photon energy cut
    static Double_t kEEmcEtMin; // EEmc tower energy_t cut
    static Double_t kRMax;      // Anti kT radius

    static Int_t kFMS_JP0;
    static Int_t kFMS_JP1;
    static Int_t kFMS_JP2;
    static Int_t kFMS_SM_BS1;
    static Int_t kFMS_SM_BS2;
    static Int_t kFMS_SM_BS3;
    static Int_t kFMS_LG_BS1;
    static Int_t kFMS_LG_BS2;
    static Int_t kFMS_LG_BS3;

    static Int_t kEHTO;
    static Int_t kJP1;
    static Int_t kJP2;
    static Int_t kEHT0_EJP1_L2Egamma;

    EjAna(){};
    ~EjAna(){};
    static void Print();

    ClassDef(EjAna, 1) 
};
#endif
