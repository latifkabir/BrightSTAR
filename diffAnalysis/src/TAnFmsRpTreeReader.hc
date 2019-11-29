//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 20 21:14:01 2019 by ROOT version 5.34/38
// from TTree T/RP + FMS Tree
// found on file: /star/u/kabir/GIT/BrightSTAR/dst/FmsRpTreeMaker/RunFmsRpTreeMaker_16079017.root
//////////////////////////////////////////////////////////

#ifndef TAnFmsRpTreeReader_h
#define TAnFmsRpTreeReader_h

#include <iostream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class TAnFmsRpTreeReader
{
    static const Int_t kMaxPairs = 1000;
    static const Int_t kMaxTracks = 1000;
public :
    TChain          *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t           fCurrent; //!current Tree number in a TChain

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
    Int_t           fms_nPairs;
    Double_t*        fms_pairE;   //[fms_nPairs]
    Double_t*        fms_pairM;   //[fms_nPairs]
    Double_t*        fms_pairPt;   //[fms_nPairs]
    Double_t*        fms_pairEta;   //[fms_nPairs]
    Double_t*        fms_pairPhi;   //[fms_nPairs]
    Double_t*        fms_pairZgg;   //[fms_nPairs]
    Double_t*        fms_pairDgg;   //[fms_nPairs]
    Double_t*        fms_pairX;   //[fms_nPairs]
    Double_t*        fms_pairY;   //[fms_nPairs]
    Int_t           rp_nTracks;
    Int_t           *rp_trackType;   //[rp_nTracks]
    Int_t           *rp_trackNplanes;   //[rp_nTracks]    
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
    TBranch        *b_fms_nPairs;   //!
    TBranch        *b_fms_pairE;   //!
    TBranch        *b_fms_pairM;   //!
    TBranch        *b_fms_pairPt;   //!
    TBranch        *b_fms_pairEta;   //!
    TBranch        *b_fms_pairPhi;   //!
    TBranch        *b_fms_pairZgg;   //!
    TBranch        *b_fms_pairDgg;   //!
    TBranch        *b_fms_pairX;   //!
    TBranch        *b_fms_pairY;   //!
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

    TAnFmsRpTreeReader(TString fileName);
    virtual ~TAnFmsRpTreeReader();
    virtual Int_t    Cut(Long64_t entry);
    virtual Int_t    GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void     Init();
    virtual void     Loop();
    virtual Bool_t   Notify();
    virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TAnFmsRpTreeReader_cxx
TAnFmsRpTreeReader::TAnFmsRpTreeReader(TString fileName)
{
    fms_pairE = new Double_t[kMaxPairs];   //[fms_nPairs]
    fms_pairM = new Double_t[kMaxPairs];   //[fms_nPairs]
    fms_pairPt = new Double_t[kMaxPairs];   //[fms_nPairs]
    fms_pairEta = new Double_t[kMaxPairs];   //[fms_nPairs]
    fms_pairPhi = new Double_t[kMaxPairs];   //[fms_nPairs]
    fms_pairZgg = new Double_t[kMaxPairs];   //[fms_nPairs]
    fms_pairDgg = new Double_t[kMaxPairs];   //[fms_nPairs]
    fms_pairX = new Double_t[kMaxPairs];   //[fms_nPairs]
    fms_pairY = new Double_t[kMaxPairs];   //[fms_nPairs]

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
   
    fChain = new TChain("T");
    fChain->Add(fileName);
    
    Init();
}

TAnFmsRpTreeReader::~TAnFmsRpTreeReader()
{
    delete[] fms_pairE;
    delete[] fms_pairM;
    delete[] fms_pairPt;
    delete[] fms_pairEta;
    delete[] fms_pairPhi;
    delete[] fms_pairZgg;
    delete[] fms_pairDgg;
    delete[] fms_pairX;
    delete[] fms_pairY;

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

    delete fChain;
    // if (!fChain) return;
    // delete fChain->GetCurrentFile();
}

Int_t TAnFmsRpTreeReader::GetEntry(Long64_t entry)
{
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}
Long64_t TAnFmsRpTreeReader::LoadTree(Long64_t entry)
{
    // Set the environment to read one entry
    if (!fChain) return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent)
    {
	fCurrent = fChain->GetTreeNumber();
	Notify();
    }
    return centry;
}

void TAnFmsRpTreeReader::Init()
{
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if(!fChain)
    {
	std::cout << "Invalid TTree" <<std::endl;
	return;
    }
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("evt_run", &evt_run, &b_evt_run);
    fChain->SetBranchAddress("evt_id", &evt_id, &b_evt_id);
    fChain->SetBranchAddress("evt_bSpin", &evt_bSpin, &b_evt_bSpin);
    fChain->SetBranchAddress("evt_ySpin", &evt_ySpin, &b_evt_ySpin);
    fChain->SetBranchAddress("evt_nTrig", &evt_nTrig, &b_evt_nTrig);
    fChain->SetBranchAddress("evt_Triggers", evt_Triggers, &b_evt_Triggers);
    fChain->SetBranchAddress("evt_bbcADCSum", evt_bbcADCSum, &b_evt_bbcADCSum);
    fChain->SetBranchAddress("evt_bbcADCSumLarge", evt_bbcADCSumLarge, &b_evt_bbcADCSumLarge);
    fChain->SetBranchAddress("evt_bbcEarliestTDC", evt_bbcEarliestTDC, &b_evt_bbcEarliestTDC);
    fChain->SetBranchAddress("evt_bbcEarliestTDCLarge", evt_bbcEarliestTDCLarge, &b_evt_bbcEarliestTDCLarge);
    fChain->SetBranchAddress("evt_zdcADCSum", evt_zdcADCSum, &b_evt_zdcADCSum);
    fChain->SetBranchAddress("evt_vpdADCSum", evt_vpdADCSum, &b_evt_vpdADCSum);
    fChain->SetBranchAddress("evt_tofMultiplicity", &evt_tofMultiplicity, &b_evt_tofMultiplicity);
    fChain->SetBranchAddress("fms_nPairs", &fms_nPairs, &b_fms_nPairs);
    fChain->SetBranchAddress("fms_pairE", fms_pairE, &b_fms_pairE);
    fChain->SetBranchAddress("fms_pairM", fms_pairM, &b_fms_pairM);
    fChain->SetBranchAddress("fms_pairPt", fms_pairPt, &b_fms_pairPt);
    fChain->SetBranchAddress("fms_pairEta", fms_pairEta, &b_fms_pairEta);
    fChain->SetBranchAddress("fms_pairPhi", fms_pairPhi, &b_fms_pairPhi);
    fChain->SetBranchAddress("fms_pairZgg", fms_pairZgg, &b_fms_pairZgg);
    fChain->SetBranchAddress("fms_pairDgg", fms_pairDgg, &b_fms_pairDgg);
    fChain->SetBranchAddress("fms_pairX", fms_pairX, &b_fms_pairX);
    fChain->SetBranchAddress("fms_pairY", fms_pairY, &b_fms_pairY);
    fChain->SetBranchAddress("rp_nTracks", &rp_nTracks, &b_rp_nTracks);
    fChain->SetBranchAddress("rp_trackType", rp_trackType, &b_rp_trackType);
    fChain->SetBranchAddress("rp_trackNplanes", rp_trackNplanes, &b_rp_trackNplanes);   
    fChain->SetBranchAddress("rp_trackBranch", rp_trackBranch, &b_rp_trackBranch);
    fChain->SetBranchAddress("rp_trackTheta", rp_trackTheta, &b_rp_trackTheta);
    fChain->SetBranchAddress("rp_trackTheta_x", rp_trackTheta_x, &b_rp_trackTheta_x);
    fChain->SetBranchAddress("rp_trackTheta_y", rp_trackTheta_y, &b_rp_trackTheta_y);
    fChain->SetBranchAddress("rp_trackEta", rp_trackEta, &b_rp_trackEta);
    fChain->SetBranchAddress("rp_trackPhi", rp_trackPhi, &b_rp_trackPhi);
    fChain->SetBranchAddress("rp_trackXi", rp_trackXi, &b_rp_trackXi);
    fChain->SetBranchAddress("rp_trackMt", rp_trackMt, &b_rp_trackMt);
    fChain->SetBranchAddress("rp_trackP", rp_trackP, &b_rp_trackP);
    fChain->SetBranchAddress("rp_trackPt", rp_trackPt, &b_rp_trackPt);
    Notify();
}

Bool_t TAnFmsRpTreeReader::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

void TAnFmsRpTreeReader::Show(Long64_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}
Int_t TAnFmsRpTreeReader::Cut(Long64_t entry)
{
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}
#endif // #ifdef TAnFmsRpTreeReader_cxx
