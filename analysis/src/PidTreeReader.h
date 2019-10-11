//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Sep 28 23:02:08 2019 by ROOT version 5.34/38
// from TTree T/Pid Tree
// found on file: /star/u/kabir/GIT/BrightSTAR/results/root/RunPidTreeMaker/RunPidTreeMaker_16072057.root
//////////////////////////////////////////////////////////

#ifndef PidTreeReader_h
#define PidTreeReader_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <iostream>
// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class PidTreeReader
{
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           event;
   Double_t        vz;
   Int_t           eN;
   Int_t           eQ[100];   //[eN]
   Double_t        eP[100][3];   //[eN]
   Double_t        eX[100][3];   //[eN]
   Int_t           piN;
   Int_t           piQ[100];   //[piN]
   Double_t        piP[100][3];   //[piN]
   Double_t        piX[100][3];   //[piN]
   Int_t           muN;
   Int_t           muQ[100];   //[muN]
   Double_t        muP[100][3];   //[muN]
   Double_t        muX[100][3];   //[muN]
   Int_t           kaN;
   Int_t           kaQ[100];   //[kaN]
   Double_t        kaP[100][3];   //[kaN]
   Double_t        kaX[100][3];   //[kaN]
   Int_t           prN;
   Int_t           prQ[100];   //[prN]
   Double_t        prP[100][3];   //[prN]
   Double_t        prX[100][3];   //[prN]

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_eN;   //!
   TBranch        *b_eQ;   //!
   TBranch        *b_eP;   //!
   TBranch        *b_eX;   //!
   TBranch        *b_piN;   //!
   TBranch        *b_piQ;   //!
   TBranch        *b_piP;   //!
   TBranch        *b_piX;   //!
   TBranch        *b_muN;   //!
   TBranch        *b_muQ;   //!
   TBranch        *b_muP;   //!
   TBranch        *b_muX;   //!
   TBranch        *b_kaN;   //!
   TBranch        *b_kaQ;   //!
   TBranch        *b_kaP;   //!
   TBranch        *b_kaX;   //!
   TBranch        *b_prN;   //!
   TBranch        *b_prQ;   //!
   TBranch        *b_prP;   //!
   TBranch        *b_prX;   //!

   PidTreeReader(TTree *tree=0);
   virtual ~PidTreeReader();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef PidTreeReader_cxx
PidTreeReader::PidTreeReader(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0)
   {
       std::cout << "You must privide a valid TTree or TChain" <<std::endl;
   }
   Init(tree);
}

PidTreeReader::~PidTreeReader()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t PidTreeReader::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t PidTreeReader::LoadTree(Long64_t entry)
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

void PidTreeReader::Init(TTree *tree)
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

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("vz", &vz, &b_vz);
   fChain->SetBranchAddress("eN", &eN, &b_eN);
   fChain->SetBranchAddress("eQ", eQ, &b_eQ);
   fChain->SetBranchAddress("eP", eP, &b_eP);
   fChain->SetBranchAddress("eX", eX, &b_eX);
   fChain->SetBranchAddress("piN", &piN, &b_piN);
   fChain->SetBranchAddress("piQ", piQ, &b_piQ);
   fChain->SetBranchAddress("piP", piP, &b_piP);
   fChain->SetBranchAddress("piX", piX, &b_piX);
   fChain->SetBranchAddress("muN", &muN, &b_muN);
   fChain->SetBranchAddress("muQ", &muQ, &b_muQ);
   fChain->SetBranchAddress("muP", &muP, &b_muP);
   fChain->SetBranchAddress("muX", &muX, &b_muX);
   fChain->SetBranchAddress("kaN", &kaN, &b_kaN);
   fChain->SetBranchAddress("kaQ", kaQ, &b_kaQ);
   fChain->SetBranchAddress("kaP", kaP, &b_kaP);
   fChain->SetBranchAddress("kaX", kaX, &b_kaX);
   fChain->SetBranchAddress("prN", &prN, &b_prN);
   fChain->SetBranchAddress("prQ", prQ, &b_prQ);
   fChain->SetBranchAddress("prP", prP, &b_prP);
   fChain->SetBranchAddress("prX", prX, &b_prX);
   Notify();
}

Bool_t PidTreeReader::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void PidTreeReader::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t PidTreeReader::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef PidTreeReader_cxx
