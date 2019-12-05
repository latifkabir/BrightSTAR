
#include "TAnEEmcRpTreeReader.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

ClassImp(TAnEEmcRpTreeReader)

TAnEEmcRpTreeReader::TAnEEmcRpTreeReader(TChain *eemc_ch1, TChain *eemc_ch3, TChain *rp_ch) 
{

    //------------ EEMC Tree Part 3 -----------------------------------
    photon_array = new TClonesArray("Int_t", 100);
    photon_fUniqueID = new UInt_t[kMaxphoton];   //[photon_]
    photon_fBits = new UInt_t[kMaxphoton];   //[photon_]
    photon_PID = new Int_t[kMaxphoton];   //[photon_]
    photon_hitIdx1 = new Int_t[kMaxphoton];   //[photon_]
    photon_E = new Float_t[kMaxphoton];   //[photon_]
    photon_M = new Float_t[kMaxphoton];   //[photon_]
    photon_PT = new Float_t[kMaxphoton];   //[photon_]
    photon_position_fUniqueID = new UInt_t[kMaxphoton];   //[photon_]
    photon_position_fBits = new UInt_t[kMaxphoton];   //[photon_]
    photon_position_fX = new Double_t[kMaxphoton];   //[photon_]
    photon_position_fY = new Double_t[kMaxphoton];   //[photon_]
    photon_position_fZ = new Double_t[kMaxphoton];   //[photon_]
    photon_momentum_fUniqueID = new UInt_t[kMaxphoton];   //[photon_]
    photon_momentum_fBits = new UInt_t[kMaxphoton];   //[photon_]
    photon_momentum_fX = new Double_t[kMaxphoton];   //[photon_]
    photon_momentum_fY = new Double_t[kMaxphoton];   //[photon_]
    photon_momentum_fZ = new Double_t[kMaxphoton];   //[photon_]


    pi0_array = new TClonesArray("Int_t", 500);
    pi0_fUniqueID = new UInt_t[kMaxpi0];   //[pi0_]
    pi0_fBits = new UInt_t[kMaxpi0];   //[pi0_]
    pi0_PID = new Int_t[kMaxpi0];   //[pi0_]
    pi0_hitIdx1 = new Int_t[kMaxpi0];   //[pi0_]
    pi0_E = new Float_t[kMaxpi0];   //[pi0_]
    pi0_M = new Float_t[kMaxpi0];   //[pi0_]
    pi0_PT = new Float_t[kMaxpi0];   //[pi0_]
    pi0_position_fUniqueID = new UInt_t[kMaxpi0];   //[pi0_]
    pi0_position_fBits = new UInt_t[kMaxpi0];   //[pi0_]
    pi0_position_fX = new Double_t[kMaxpi0];   //[pi0_]
    pi0_position_fY = new Double_t[kMaxpi0];   //[pi0_]
    pi0_position_fZ = new Double_t[kMaxpi0];   //[pi0_]
    pi0_momentum_fUniqueID = new UInt_t[kMaxpi0];   //[pi0_]
    pi0_momentum_fBits = new UInt_t[kMaxpi0];   //[pi0_]
    pi0_momentum_fX = new Double_t[kMaxpi0];   //[pi0_]
    pi0_momentum_fY = new Double_t[kMaxpi0];   //[pi0_]
    pi0_momentum_fZ = new Double_t[kMaxpi0];   //[pi0_]
    pi0_hitIdx2 = new Int_t[kMaxpi0];   //[pi0_]
    pi0_Z = new Float_t[kMaxpi0];   //[pi0_]
    pi0_D = new Float_t[kMaxpi0];   //[pi0_]
    
    
    //--------------- RP Tree ---------------------------------
    rp_trackType = new Int_t[kMaxTracks];   //[rp_nTracks]
    rp_trackNplanes = new Int_t[kMaxTracks];   //[rp_nTracks]
    rp_trackBranch = new Int_t[kMaxTracks];   //[rp_nTracks]
    rp_trackTheta = new Double_t[kMaxTracks];   //[rp_nTracks]
    rp_trackTheta_x = new Double_t[kMaxTracks];   //[rp_nTracks]
    rp_trackTheta_y = new Double_t[kMaxTracks];   //[rp_nTracks]
    rp_trackEta = new Double_t[kMaxTracks];   //[rp_nTracks]
    rp_trackPhi = new Double_t[kMaxTracks];   //[rp_nTracks]
    rp_trackXi = new Double_t[kMaxTracks];   //[rp_nTracks]
    rp_trackMt = new Double_t[kMaxTracks];   //[rp_nTracks]
    rp_trackP = new Double_t[kMaxTracks];   //[rp_nTracks]
    rp_trackPt = new Double_t[kMaxTracks];   //[rp_nTracks]

    
    Init(eemc_ch1, eemc_ch3, rp_ch);
}

TAnEEmcRpTreeReader::~TAnEEmcRpTreeReader()
{
    //---------------- EEMC Part 3 ------------------
    //delete photon_array;
    delete[] photon_fUniqueID;
    delete[] photon_fBits;
    delete[] photon_PID;
    delete[] photon_hitIdx1;
    delete[] photon_E;
    delete[] photon_M;
    delete[] photon_PT;
    delete[] photon_position_fUniqueID;
    delete[] photon_position_fBits;
    delete[] photon_position_fX;
    delete[] photon_position_fY;
    delete[] photon_position_fZ;
    delete[] photon_momentum_fUniqueID;
    delete[] photon_momentum_fBits;
    delete[] photon_momentum_fX;
    delete[] photon_momentum_fY;
    delete[] photon_momentum_fZ;

    //delete pi0_array;
    delete[] pi0_fUniqueID;
    delete[] pi0_fBits;
    delete[] pi0_PID;
    delete[] pi0_hitIdx1;
    delete[] pi0_E;
    delete[] pi0_M;
    delete[] pi0_PT;
    delete[] pi0_position_fUniqueID;
    delete[] pi0_position_fBits;
    delete[] pi0_position_fX;
    delete[] pi0_position_fY;
    delete[] pi0_position_fZ;
    delete[] pi0_momentum_fUniqueID;
    delete[] pi0_momentum_fBits;
    delete[] pi0_momentum_fX;
    delete[] pi0_momentum_fY;
    delete[] pi0_momentum_fZ;
    delete[] pi0_hitIdx2;
    delete[] pi0_Z;
    delete[] pi0_D;

    
    //------------ RP Tree ------------------
    delete[] rp_trackType;
    delete[] rp_trackNplanes;
    delete[] rp_trackBranch;
    delete[] rp_trackTheta;
    delete[] rp_trackTheta_x;
    delete[] rp_trackTheta_y;
    delete[] rp_trackEta;
    delete[] rp_trackPhi;
    delete[] rp_trackXi;
    delete[] rp_trackMt;
    delete[] rp_trackP;
    delete[] rp_trackPt;
    
}

Int_t TAnEEmcRpTreeReader::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!fChain_eemc1 || !fChain_eemc3 || !fChain_rp) return 0;

    fChain_eemc1->GetEntry(entry);
    fChain_eemc3->GetEntry(entry);
    fChain_rp->GetEntry(entry);
    if(photon_array)
	photon_ = photon_array->GetEntries();
    else
	photon_ = 0;
    if(pi0_array)
	pi0_ = pi0_array->GetEntries();
    else
	pi0_ = 0;
    return 1;
	
}
Long64_t TAnEEmcRpTreeReader::LoadTree(Long64_t entry)
{
    // Set the environment to read one entry
    if (!fChain_eemc1 || !fChain_eemc3 || !fChain_rp) return -5;
    Long64_t centry = fChain_eemc3->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain_eemc3->GetTreeNumber() != fCurrent)
    {
	fCurrent = fChain_eemc3->GetTreeNumber();
	Notify();
    }
    return centry;
}

void TAnEEmcRpTreeReader::Init(TChain *eemc_ch1, TChain *eemc_ch3, TChain *rp_ch)
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if (!eemc_ch1 || ! eemc_ch3 || !rp_ch) return;
    fChain_eemc1 = eemc_ch1;
    fChain_eemc3 = eemc_ch3;
    fChain_rp = rp_ch;
    
    fCurrent = -1;
    fChain_eemc1->SetMakeClass(1);
    fChain_eemc3->SetMakeClass(1);
    fChain_rp->SetMakeClass(1);

    //-------------------------- EEMC Tree part 1 --------------------------------------------------
    fChain_eemc1->SetBranchAddress("mRunNumber", &mRunNumber, &b_evtHddr_mRunNumber);
    fChain_eemc1->SetBranchAddress("mEventNumber", &mEventNumber, &b_evtHddr_mEventNumber);
    fChain_eemc1->SetBranchAddress("mSpin4", &mSpin4, &b_spinInfo_mSpin4);
    
    //-------------------------- EEMC Tree part 3 --------------------------------------------------
    fChain_eemc3->SetBranchAddress("photon", &photon_array, &b_photon_array);
    fChain_eemc3->SetBranchAddress("photon.fUniqueID", photon_fUniqueID, &b_photon_fUniqueID);
    fChain_eemc3->SetBranchAddress("photon.fBits", photon_fBits, &b_photon_fBits);
    fChain_eemc3->SetBranchAddress("photon.PID", photon_PID, &b_photon_PID);
    fChain_eemc3->SetBranchAddress("photon.hitIdx1", photon_hitIdx1, &b_photon_hitIdx1);
    fChain_eemc3->SetBranchAddress("photon.E", photon_E, &b_photon_E);
    fChain_eemc3->SetBranchAddress("photon.M", photon_M, &b_photon_M);
    fChain_eemc3->SetBranchAddress("photon.PT", photon_PT, &b_photon_PT);
    fChain_eemc3->SetBranchAddress("photon.position.fUniqueID", photon_position_fUniqueID, &b_photon_position_fUniqueID);
    fChain_eemc3->SetBranchAddress("photon.position.fBits", photon_position_fBits, &b_photon_position_fBits);
    fChain_eemc3->SetBranchAddress("photon.position.fX", photon_position_fX, &b_photon_position_fX);
    fChain_eemc3->SetBranchAddress("photon.position.fY", photon_position_fY, &b_photon_position_fY);
    fChain_eemc3->SetBranchAddress("photon.position.fZ", photon_position_fZ, &b_photon_position_fZ);
    fChain_eemc3->SetBranchAddress("photon.momentum.fUniqueID", photon_momentum_fUniqueID, &b_photon_momentum_fUniqueID);
    fChain_eemc3->SetBranchAddress("photon.momentum.fBits", photon_momentum_fBits, &b_photon_momentum_fBits);
    fChain_eemc3->SetBranchAddress("photon.momentum.fX", photon_momentum_fX, &b_photon_momentum_fX);
    fChain_eemc3->SetBranchAddress("photon.momentum.fY", photon_momentum_fY, &b_photon_momentum_fY);
    fChain_eemc3->SetBranchAddress("photon.momentum.fZ", photon_momentum_fZ, &b_photon_momentum_fZ);
    fChain_eemc3->SetBranchAddress("pi0", &pi0_array, &b_pi0_array);
    fChain_eemc3->SetBranchAddress("pi0.fUniqueID", pi0_fUniqueID, &b_pi0_fUniqueID);
    fChain_eemc3->SetBranchAddress("pi0.fBits", pi0_fBits, &b_pi0_fBits);
    fChain_eemc3->SetBranchAddress("pi0.PID", pi0_PID, &b_pi0_PID);
    fChain_eemc3->SetBranchAddress("pi0.hitIdx1", pi0_hitIdx1, &b_pi0_hitIdx1);
    fChain_eemc3->SetBranchAddress("pi0.E", pi0_E, &b_pi0_E);
    fChain_eemc3->SetBranchAddress("pi0.M", pi0_M, &b_pi0_M);
    fChain_eemc3->SetBranchAddress("pi0.PT", pi0_PT, &b_pi0_PT);
    fChain_eemc3->SetBranchAddress("pi0.position.fUniqueID", pi0_position_fUniqueID, &b_pi0_position_fUniqueID);
    fChain_eemc3->SetBranchAddress("pi0.position.fBits", pi0_position_fBits, &b_pi0_position_fBits);
    fChain_eemc3->SetBranchAddress("pi0.position.fX", pi0_position_fX, &b_pi0_position_fX);
    fChain_eemc3->SetBranchAddress("pi0.position.fY", pi0_position_fY, &b_pi0_position_fY);
    fChain_eemc3->SetBranchAddress("pi0.position.fZ", pi0_position_fZ, &b_pi0_position_fZ);
    fChain_eemc3->SetBranchAddress("pi0.momentum.fUniqueID", pi0_momentum_fUniqueID, &b_pi0_momentum_fUniqueID);
    fChain_eemc3->SetBranchAddress("pi0.momentum.fBits", pi0_momentum_fBits, &b_pi0_momentum_fBits);
    fChain_eemc3->SetBranchAddress("pi0.momentum.fX", pi0_momentum_fX, &b_pi0_momentum_fX);
    fChain_eemc3->SetBranchAddress("pi0.momentum.fY", pi0_momentum_fY, &b_pi0_momentum_fY);
    fChain_eemc3->SetBranchAddress("pi0.momentum.fZ", pi0_momentum_fZ, &b_pi0_momentum_fZ);
    fChain_eemc3->SetBranchAddress("pi0.hitIdx2", pi0_hitIdx2, &b_pi0_hitIdx2);
    fChain_eemc3->SetBranchAddress("pi0.Z", pi0_Z, &b_pi0_Z);
    fChain_eemc3->SetBranchAddress("pi0.D", pi0_D, &b_pi0_D);


    //-------------------------- RP Tree -----------------------
    fChain_rp->SetBranchAddress("evt_run", &evt_run, &b_evt_run);
    fChain_rp->SetBranchAddress("evt_id", &evt_id, &b_evt_id);
    fChain_rp->SetBranchAddress("evt_bSpin", &evt_bSpin, &b_evt_bSpin);
    fChain_rp->SetBranchAddress("evt_ySpin", &evt_ySpin, &b_evt_ySpin);
    fChain_rp->SetBranchAddress("evt_nTrig", &evt_nTrig, &b_evt_nTrig);
    fChain_rp->SetBranchAddress("evt_Triggers", evt_Triggers, &b_evt_Triggers);
    fChain_rp->SetBranchAddress("evt_bbcADCSum", evt_bbcADCSum, &b_evt_bbcADCSum);
    fChain_rp->SetBranchAddress("evt_bbcADCSumLarge", evt_bbcADCSumLarge, &b_evt_bbcADCSumLarge);
    fChain_rp->SetBranchAddress("evt_bbcEarliestTDC", evt_bbcEarliestTDC, &b_evt_bbcEarliestTDC);
    fChain_rp->SetBranchAddress("evt_bbcEarliestTDCLarge", evt_bbcEarliestTDCLarge, &b_evt_bbcEarliestTDCLarge);
    fChain_rp->SetBranchAddress("evt_zdcADCSum", evt_zdcADCSum, &b_evt_zdcADCSum);
    fChain_rp->SetBranchAddress("evt_vpdADCSum", evt_vpdADCSum, &b_evt_vpdADCSum);
    fChain_rp->SetBranchAddress("evt_tofMultiplicity", &evt_tofMultiplicity, &b_evt_tofMultiplicity);
    fChain_rp->SetBranchAddress("rp_nTracks", &rp_nTracks, &b_rp_nTracks);
    fChain_rp->SetBranchAddress("rp_trackType", rp_trackType, &b_rp_trackType);
    fChain_rp->SetBranchAddress("rp_trackNplanes", rp_trackNplanes, &b_rp_trackNplanes);   
    fChain_rp->SetBranchAddress("rp_trackBranch", rp_trackBranch, &b_rp_trackBranch);
    fChain_rp->SetBranchAddress("rp_trackTheta", rp_trackTheta, &b_rp_trackTheta);
    fChain_rp->SetBranchAddress("rp_trackTheta_x", rp_trackTheta_x, &b_rp_trackTheta_x);
    fChain_rp->SetBranchAddress("rp_trackTheta_y", rp_trackTheta_y, &b_rp_trackTheta_y);
    fChain_rp->SetBranchAddress("rp_trackEta", rp_trackEta, &b_rp_trackEta);
    fChain_rp->SetBranchAddress("rp_trackPhi", rp_trackPhi, &b_rp_trackPhi);
    fChain_rp->SetBranchAddress("rp_trackXi", rp_trackXi, &b_rp_trackXi);
    fChain_rp->SetBranchAddress("rp_trackMt", rp_trackMt, &b_rp_trackMt);
    fChain_rp->SetBranchAddress("rp_trackP", rp_trackP, &b_rp_trackP);
    fChain_rp->SetBranchAddress("rp_trackPt", rp_trackPt, &b_rp_trackPt);

    
    Notify();
}

Bool_t TAnEEmcRpTreeReader::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

void TAnEEmcRpTreeReader::Show(Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain_eemc1 || !fChain_eemc3 || !fChain_rp) return;
    fChain_eemc1->Show(entry);
    fChain_eemc3->Show(entry);
    fChain_rp->Show(entry);
}
Int_t TAnEEmcRpTreeReader::Cut(Long64_t entry)
{
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}


void TAnEEmcRpTreeReader::Loop()
{

}
