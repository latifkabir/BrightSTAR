
#include "TAnEEmcRpTreeReader.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include "StEEmcPool/EEmcTreeContainers/EEmc2ParticleCandidate.h"
#include "StEEmcPool/EEmcTreeContainers/EEmcParticleCandidate.h"

ClassImp(TAnEEmcRpTreeReader)

TAnEEmcRpTreeReader::TAnEEmcRpTreeReader(TChain *eemc_ch1, TChain *eemc_ch3, TChain *rp_ch) 
{
    //------------ EEMC Tree Part 3 -----------------------------------
    photon_array = new TClonesArray("EEmcParticleCandidate_t");
    pion_array = new TClonesArray("EEmc2ParticleCandidate_t");
        
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
    delete photon_array;
    delete pion_array;
   
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
    // std::cout << photon_array <<std::endl;
    // std::cout << pion_array <<std::endl;
    // photon_array->Clear();
    // pion_array->Clear();
    // Read contents of entry.
    if (!fChain_eemc1 || !fChain_eemc3 || !fChain_rp) return 0;

    fChain_eemc1->GetEntry(entry);
    fChain_eemc3->GetEntry(entry);
    fChain_rp->GetEntry(entry);

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
    // fChain_eemc3->SetMakeClass(1);
    // fChain_rp->SetMakeClass(1);

    //-------------------------- EEMC Tree part 1 --------------------------------------------------
    fChain_eemc1->SetBranchAddress("mRunNumber", &mRunNumber, &b_evtHddr_mRunNumber);
    fChain_eemc1->SetBranchAddress("mEventNumber", &mEventNumber, &b_evtHddr_mEventNumber);
    fChain_eemc1->SetBranchAddress("mSpin4", &mSpin4, &b_spinInfo_mSpin4);
    
    //-------------------------- EEMC Tree part 3 --------------------------------------------------
    fChain_eemc3->SetBranchAddress("photon", &photon_array);
    fChain_eemc3->SetBranchAddress("pi0", &pion_array);


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
