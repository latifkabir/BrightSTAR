// Filename: EjAnalysis.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Oct 18 12:40:20 2020 (-0400)
// URL: jlab.org/~latif

class EjAnalysis
{
public:
    const static Double_t kFmsEtaMin = 2.8;
    const static Double_t kFmsEtaMax = 3.8;

    const static Double_t kEemcEtaMin = 1.0;
    const static Double_t kEemcEtaMax = 2.0;

    const static Int_t kFMS_JP0 = 0;
    const static Int_t kFMS_JP1 = 1;
    const static Int_t kFMS_JP2 = 2;
    const static Int_t kFMS_SM_BS1 = 3;
    const static Int_t kFMS_SM_BS2 = 4;
    const static Int_t kFMS_SM_BS3 = 5;
    const static Int_t kFMS_LG_BS1 = 6;
    const static Int_t kFMS_LG_BS2 = 7;
    const static Int_t kFMS_LG_BS3 = 8;

    const static Int_t kEHTO = 0;
    const static Int_t kJP1 = 1;
    const static Int_t kJP2 = 2;
    const static Int_t kEHT0_EJP1_L2Egamma = 3;

    const static Double_t kFmsTrigPt[9];
    
};
