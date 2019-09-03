//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Sep  2 18:28:30 2019 by ROOT version 5.34/38
// from TChain T/
//////////////////////////////////////////////////////////

#ifndef EmcTreeReader_h
#define EmcTreeReader_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class EmcTreeReader
{
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           evt_no;
   Int_t           trig_MB;
   Int_t           trig_HT1;
   Int_t           trig_HT2;
   Int_t           point;
   Double_t        point_x[25];   //[point]
   Double_t        point_y[25];   //[point]
   Double_t        point_z[25];   //[point]
   Double_t        point_px[25];   //[point]
   Double_t        point_py[25];   //[point]
   Double_t        point_pz[25];   //[point]
   Double_t        point_E[25];   //[point]
   Int_t           point_quality[25];   //[point]
   Int_t           point_nTracks[25];   //[point]
   Int_t           pi0;
   Double_t        pi0_x[305];   //[pi0]
   Double_t        pi0_y[305];   //[pi0]
   Double_t        pi0_z[305];   //[pi0]
   Double_t        pi0_px[305];   //[pi0]
   Double_t        pi0_py[305];   //[pi0]
   Double_t        pi0_pz[305];   //[pi0]
   Double_t        pi0_E[305];   //[pi0]
   Double_t        pi0_M[305];   //[pi0]
   Double_t        pi0_theta[305];   //[pi0]
   Double_t        pi0_zgg[305];   //[pi0]
   Double_t        pi0_dgg[305];   //[pi0]
   Int_t           pi0_Q1[305];   //[pi0]
   Int_t           pi0_Q2[305];   //[pi0]
   Int_t           pi0_nTracks1[305];   //[pi0]
   Int_t           pi0_nTracks2[305];   //[pi0]

   // List of branches
   TBranch        *b_evt_no;   //!
   TBranch        *b_trig_MB;   //!
   TBranch        *b_trig_HT1;   //!
   TBranch        *b_trig_HT2;   //!
   TBranch        *b_point;   //!
   TBranch        *b_point_x;   //!
   TBranch        *b_point_y;   //!
   TBranch        *b_point_z;   //!
   TBranch        *b_point_px;   //!
   TBranch        *b_point_py;   //!
   TBranch        *b_point_pz;   //!
   TBranch        *b_point_E;   //!
   TBranch        *b_point_quality;   //!
   TBranch        *b_point_nTracks;   //!
   TBranch        *b_pi0;   //!
   TBranch        *b_pi0_x;   //!
   TBranch        *b_pi0_y;   //!
   TBranch        *b_pi0_z;   //!
   TBranch        *b_pi0_px;   //!
   TBranch        *b_pi0_py;   //!
   TBranch        *b_pi0_pz;   //!
   TBranch        *b_pi0_E;   //!
   TBranch        *b_pi0_M;   //!
   TBranch        *b_pi0_theta;   //!
   TBranch        *b_pi0_zgg;   //!
   TBranch        *b_pi0_dgg;   //!
   TBranch        *b_pi0_Q1;   //!
   TBranch        *b_pi0_Q2;   //!
   TBranch        *b_pi0_nTracks1;   //!
   TBranch        *b_pi0_nTracks2;   //!

   EmcTreeReader(TTree *tree=0);
   virtual ~EmcTreeReader();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EmcTreeReader_cxx
EmcTreeReader::EmcTreeReader(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0)
   {
      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("T","");
      tree = chain;

   }
   Init(tree);
}

EmcTreeReader::~EmcTreeReader()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t EmcTreeReader::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t EmcTreeReader::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void EmcTreeReader::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("evt_no", &evt_no, &b_evt_no);
   fChain->SetBranchAddress("trig_MB", &trig_MB, &b_trig_MB);
   fChain->SetBranchAddress("trig_HT1", &trig_HT1, &b_trig_HT1);
   fChain->SetBranchAddress("trig_HT2", &trig_HT2, &b_trig_HT2);
   fChain->SetBranchAddress("point", &point, &b_point);
   fChain->SetBranchAddress("point_x", point_x, &b_point_x);
   fChain->SetBranchAddress("point_y", point_y, &b_point_y);
   fChain->SetBranchAddress("point_z", point_z, &b_point_z);
   fChain->SetBranchAddress("point_px", point_px, &b_point_px);
   fChain->SetBranchAddress("point_py", point_py, &b_point_py);
   fChain->SetBranchAddress("point_pz", point_pz, &b_point_pz);
   fChain->SetBranchAddress("point_E", point_E, &b_point_E);
   fChain->SetBranchAddress("point_quality", point_quality, &b_point_quality);
   fChain->SetBranchAddress("point_nTracks", point_nTracks, &b_point_nTracks);
   fChain->SetBranchAddress("pi0", &pi0, &b_pi0);
   fChain->SetBranchAddress("pi0_x", pi0_x, &b_pi0_x);
   fChain->SetBranchAddress("pi0_y", pi0_y, &b_pi0_y);
   fChain->SetBranchAddress("pi0_z", pi0_z, &b_pi0_z);
   fChain->SetBranchAddress("pi0_px", pi0_px, &b_pi0_px);
   fChain->SetBranchAddress("pi0_py", pi0_py, &b_pi0_py);
   fChain->SetBranchAddress("pi0_pz", pi0_pz, &b_pi0_pz);
   fChain->SetBranchAddress("pi0_E", pi0_E, &b_pi0_E);
   fChain->SetBranchAddress("pi0_M", pi0_M, &b_pi0_M);
   fChain->SetBranchAddress("pi0_theta", pi0_theta, &b_pi0_theta);
   fChain->SetBranchAddress("pi0_zgg", pi0_zgg, &b_pi0_zgg);
   fChain->SetBranchAddress("pi0_dgg", pi0_dgg, &b_pi0_dgg);
   fChain->SetBranchAddress("pi0_Q1", pi0_Q1, &b_pi0_Q1);
   fChain->SetBranchAddress("pi0_Q2", pi0_Q2, &b_pi0_Q2);
   fChain->SetBranchAddress("pi0_nTracks1", pi0_nTracks1, &b_pi0_nTracks1);
   fChain->SetBranchAddress("pi0_nTracks2", pi0_nTracks2, &b_pi0_nTracks2);
   Notify();
}

Bool_t EmcTreeReader::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EmcTreeReader::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t EmcTreeReader::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EmcTreeReader_cxx
