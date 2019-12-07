//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec  4 13:25:09 2019 by ROOT version 5.34/38
// from TChain tree/
//////////////////////////////////////////////////////////

#ifndef TAnEEmcRpTreeReader_h
#define TAnEEmcRpTreeReader_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>
#include <TVector3.h>

class TAnEEmcRpTreeReader
{
    // Fixed size dimensions of array or collections stored in the TTree if any.
    static const Int_t kMaxphoton = 100;
    static const Int_t kMaxpi0 = 500;

    static const Int_t kMaxTracks = 1000;

public :
    TChain          *fChain_eemc1;   //!pointer to the analyzed TTree or TChain
    TChain          *fChain_eemc3;   //!pointer to the analyzed TTree or TChain
    TChain          *fChain_rp;      //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain


    //-------------------------- EEMC Tree part 1 --------------------------------------------------
    Int_t           mEventNumber;
    Int_t           mRunNumber;
    UShort_t        mSpin4;

    TBranch        *b_evtHddr_mRunNumber;   //!
    TBranch        *b_evtHddr_mEventNumber;   //!
    TBranch        *b_spinInfo_mSpin4;   //!
    //-------------------------- EEMC Tree part 3 --------------------------------------------------
    // Declaration of leaf types
    TClonesArray *photon_array;
    TClonesArray *pion_array;

    // List of branches
    TBranch        *b_photon_array;    
    TBranch        *b_pion_array;   //!


    //------------- RP Tree -----------------------------------------
    // Declaration of leaf types
    Int_t           evt_run;
    Int_t           evt_id;
    Short_t         evt_bSpin;
    Short_t         evt_ySpin;
    Int_t           evt_nTrig;
    Int_t           evt_Triggers[9];   //[evt_nTrig]
    Int_t           evt_bbcADCSum[2];
    Int_t           evt_bbcADCSumLarge[2];
    Int_t           evt_bbcEarliestTDC[2];
    Int_t           evt_bbcEarliestTDCLarge[2];
    Int_t           evt_zdcADCSum[2];
    Int_t           evt_vpdADCSum[2];
    Int_t           evt_tofMultiplicity;

    Int_t            rp_nTracks;
    Int_t*           rp_trackType;   //[rp_nTracks]
    Int_t*           rp_trackNplanes;   //[rp_nTracks]    
    Int_t*           rp_trackBranch;   //[rp_nTracks]
    Double_t*        rp_trackTheta;   //[rp_nTracks]
    Double_t*        rp_trackTheta_x;   //[rp_nTracks]
    Double_t*        rp_trackTheta_y;   //[rp_nTracks]
    Double_t*        rp_trackEta;   //[rp_nTracks]
    Double_t*        rp_trackPhi;   //[rp_nTracks]
    Double_t*        rp_trackXi;   //[rp_nTracks]
    Double_t*        rp_trackMt;   //[rp_nTracks]
    Double_t*        rp_trackP;   //[rp_nTracks]
    Double_t*        rp_trackPt;   //[rp_nTracks]

    // List of branches
    TBranch        *b_evt_run;   //!
    TBranch        *b_evt_id;   //!
    TBranch        *b_evt_bSpin;   //!
    TBranch        *b_evt_ySpin;   //!
    TBranch        *b_evt_nTrig;   //!
    TBranch        *b_evt_Triggers;   //!
    TBranch        *b_evt_bbcADCSum;   //!
    TBranch        *b_evt_bbcADCSumLarge;   //!
    TBranch        *b_evt_bbcEarliestTDC;   //!
    TBranch        *b_evt_bbcEarliestTDCLarge;   //!
    TBranch        *b_evt_zdcADCSum;   //!
    TBranch        *b_evt_vpdADCSum;   //!
    TBranch        *b_evt_tofMultiplicity;   //!

    TBranch        *b_rp_nTracks;   //!
    TBranch        *b_rp_trackType;   //!
    TBranch        *b_rp_trackNplanes;   //!    
    TBranch        *b_rp_trackBranch;   //!
    TBranch        *b_rp_trackTheta;   //!
    TBranch        *b_rp_trackTheta_x;   //!
    TBranch        *b_rp_trackTheta_y;   //!
    TBranch        *b_rp_trackEta;   //!
    TBranch        *b_rp_trackPhi;   //!
    TBranch        *b_rp_trackXi;   //!
    TBranch        *b_rp_trackMt;   //!
    TBranch        *b_rp_trackP;   //!
    TBranch        *b_rp_trackPt;   //!
    
    TAnEEmcRpTreeReader(TChain *eemc_ch1, TChain *eemc_ch3, TChain *rp_ch);
    virtual ~TAnEEmcRpTreeReader();
    virtual Int_t    Cut(Long64_t entry);
    virtual Int_t    GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void     Init(TChain *eemc_ch1, TChain *eemc_ch3, TChain *rp_ch);
    virtual void     Loop();
    virtual Bool_t   Notify();
    virtual void     Show(Long64_t entry = -1);

    ClassDef(TAnEEmcRpTreeReader,1)
};

#endif

