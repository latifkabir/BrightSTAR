// Filename: EjAna.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Apr 24 17:44:22 2021 (-0400)
// URL: jlab.org/~latif

#include "EjAna.h"
#include <iostream>

using namespace std;

ClassImp(EjAna)

Double_t EjAna::kBeamEnergy = 510; //For Run 17
// Double_t EjAna::kBeamEnergy = 200; //For Run 15

TString EjAna::kDefaultDet = "fms";

Double_t EjAna:: kFmsEtaMin = 2.8;
Double_t EjAna:: kFmsEtaMax = 3.8;

Double_t EjAna:: kEEmcEtaMin = 1.0;
Double_t EjAna:: kEEmcEtaMax = 2.0;

Double_t EjAna::kPtMin = 2.0;
Double_t EjAna::kRMax = 0.7; 

// FMS Photon energy cut
// 3 GeV cut was determined by RUN15 calibration condition: Zgg < 0.7 + pairE > 20 GeV
// Updated based on Simulation to 1.0 GeV
// FMS Photon energy cut: 1.0 for my analysis, 0.5 to compare with Zhanwen.
Double_t EjAna::kFmsEngMin = 1.0; 
Double_t EjAna::kFmsEngMax = 250.0; 

// EEmc Tower Et cut
// 0.2 is the default in all other full jet analysis 
Double_t EjAna::kEEmcEtMin = 0.2; 


Int_t EjAna:: kFMS_JP0 = 0;
Int_t EjAna:: kFMS_JP1 = 1;
Int_t EjAna:: kFMS_JP2 = 2;
Int_t EjAna:: kFMS_SM_BS1 = 3;
Int_t EjAna:: kFMS_SM_BS2 = 4;
Int_t EjAna:: kFMS_SM_BS3 = 5;
Int_t EjAna:: kFMS_LG_BS1 = 6;
Int_t EjAna:: kFMS_LG_BS2 = 7;
Int_t EjAna:: kFMS_LG_BS3 = 8;

Int_t EjAna:: kEHTO = 0;
Int_t EjAna:: kJP1 = 1;
Int_t EjAna:: kJP2 = 2;
Int_t EjAna:: kEHT0_EJP1_L2Egamma = 3;

void EjAna::Print()
{
    cout << "-------> Setting for EM Jet Analysis <----------" <<endl;
    cout << "Default Detector: "<< kDefaultDet <<endl;
    cout << "FMS Eta Min: "<< kFmsEtaMin <<endl;
    cout << "FMS Eta Max: "<< kFmsEtaMax <<endl;
    cout << "EEmc Eta Min: "<< kEEmcEtaMin <<endl;
    cout << "EEmc Eta Max: "<< kEEmcEtaMax <<endl;
    cout << "jet Reconstruction Radius: "<< kRMax <<endl;
    cout << "jet Reconstruction Pt cut: "<< kPtMin <<endl;
    cout << "jet Reconstruction Fms Photon Energy cut: "<< kFmsEngMin <<endl;
    cout << "jet Reconstruction Fms Photon Energy Max cut: "<< kFmsEngMax <<endl;
    cout << "jet Reconstruction EEmc tower Energy_t cut: "<< kEEmcEtMin <<endl;    
}
