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
    TClonesArray   *photon_array;
    Int_t           photon_;
    UInt_t          *photon_fUniqueID;   //[photon_]
    UInt_t          *photon_fBits;   //[photon_]
    Int_t           *photon_PID;   //[photon_]
    Int_t           *photon_hitIdx1;   //[photon_]
    Float_t         *photon_E;   //[photon_]
    Float_t         *photon_M;   //[photon_]
    Float_t         *photon_PT;   //[photon_]
    UInt_t          *photon_position_fUniqueID;   //[photon_]
    UInt_t          *photon_position_fBits;   //[photon_]
    Double_t        *photon_position_fX;   //[photon_]
    Double_t        *photon_position_fY;   //[photon_]
    Double_t        *photon_position_fZ;   //[photon_]
    UInt_t          *photon_momentum_fUniqueID;   //[photon_]
    UInt_t          *photon_momentum_fBits;   //[photon_]
    Double_t        *photon_momentum_fX;   //[photon_]
    Double_t        *photon_momentum_fY;   //[photon_]
    Double_t        *photon_momentum_fZ;   //[photon_]

    TClonesArray    *pi0_array;
    Int_t           pi0_;
    UInt_t          *pi0_fUniqueID;   //[pi0_]
    UInt_t          *pi0_fBits;   //[pi0_]
    Int_t           *pi0_PID;   //[pi0_]
    Int_t           *pi0_hitIdx1;   //[pi0_]
    Float_t         *pi0_E;   //[pi0_]
    Float_t         *pi0_M;   //[pi0_]
    Float_t         *pi0_PT;   //[pi0_]
    UInt_t          *pi0_position_fUniqueID;   //[pi0_]
    UInt_t          *pi0_position_fBits;   //[pi0_]
    Double_t        *pi0_position_fX;   //[pi0_]
    Double_t        *pi0_position_fY;   //[pi0_]
    Double_t        *pi0_position_fZ;   //[pi0_]
    UInt_t          *pi0_momentum_fUniqueID;   //[pi0_]
    UInt_t          *pi0_momentum_fBits;   //[pi0_]
    Double_t        *pi0_momentum_fX;   //[pi0_]
    Double_t        *pi0_momentum_fY;   //[pi0_]
    Double_t        *pi0_momentum_fZ;   //[pi0_]
    Int_t           *pi0_hitIdx2;   //[pi0_]
    Float_t         *pi0_Z;   //[pi0_]
    Float_t         *pi0_D;   //[pi0_]

    // List of branches
    TBranch        *b_photon_array;
    TBranch        *b_photon_;   //!
    TBranch        *b_photon_fUniqueID;   //!
    TBranch        *b_photon_fBits;   //!
    TBranch        *b_photon_PID;   //!
    TBranch        *b_photon_hitIdx1;   //!
    TBranch        *b_photon_E;   //!
    TBranch        *b_photon_M;   //!
    TBranch        *b_photon_PT;   //!
    TBranch        *b_photon_position_fUniqueID;   //!
    TBranch        *b_photon_position_fBits;   //!
    TBranch        *b_photon_position_fX;   //!
    TBranch        *b_photon_position_fY;   //!
    TBranch        *b_photon_position_fZ;   //!
    TBranch        *b_photon_momentum_fUniqueID;   //!
    TBranch        *b_photon_momentum_fBits;   //!
    TBranch        *b_photon_momentum_fX;   //!
    TBranch        *b_photon_momentum_fY;   //!
    TBranch        *b_photon_momentum_fZ;   //!
    
    TBranch        *b_pi0_array;   //!
    TBranch        *b_pi0_;   //!
    TBranch        *b_pi0_fUniqueID;   //!
    TBranch        *b_pi0_fBits;   //!
    TBranch        *b_pi0_PID;   //!
    TBranch        *b_pi0_hitIdx1;   //!
    TBranch        *b_pi0_E;   //!
    TBranch        *b_pi0_M;   //!
    TBranch        *b_pi0_PT;   //!
    TBranch        *b_pi0_position_fUniqueID;   //!
    TBranch        *b_pi0_position_fBits;   //!
    TBranch        *b_pi0_position_fX;   //!
    TBranch        *b_pi0_position_fY;   //!
    TBranch        *b_pi0_position_fZ;   //!
    TBranch        *b_pi0_momentum_fUniqueID;   //!
    TBranch        *b_pi0_momentum_fBits;   //!
    TBranch        *b_pi0_momentum_fX;   //!
    TBranch        *b_pi0_momentum_fY;   //!
    TBranch        *b_pi0_momentum_fZ;   //!
    TBranch        *b_pi0_hitIdx2;   //!
    TBranch        *b_pi0_Z;   //!
    TBranch        *b_pi0_D;   //!


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

