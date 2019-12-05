//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec  4 22:59:18 2019 by ROOT version 5.34/38
// from TChain tree/
//////////////////////////////////////////////////////////

#ifndef AnEEmcTreePart1Reader_h
#define AnEEmcTreePart1Reader_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include ".sl73_gcc485/obj/StRoot/StChain/StEvtHddr.h"
#include <TObject.h>
#include <TNamed.h>
#include <TDataSet.h>
#include <TDatime.h>
#include ".sl73_gcc485/obj/StRoot/StEEmcPool/EEmcTreeContainers/StSpinInfo.h"
#include <TVector3.h>
#include ".sl73_gcc485/obj/StRoot/StEEmcPool/EEmcTreeContainers/TrigSet.h"
#include ".sl73_gcc485/obj/StRoot/StEEmcPool/EEmcTreeContainers/EEmcEnergy.h"
#include ".sl73_gcc485/obj/StRoot/StEEmcPool/EEmcTreeContainers/EEmcEnergy.h"
#include ".sl73_gcc485/obj/StRoot/StEEmcPool/EEmcTreeContainers/EEmcEnergy.h"

// Fixed size dimensions of array or collections stored in the TTree if any.

class AnEEmcTreePart1Reader {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
 //StEvtHddr       *evtHddr;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   TString         fName;
   TString         fTitle;
   Int_t           mRunNumber;
   Int_t           mId;
   UInt_t          mInputTriggerMask;
   UInt_t          mTriggerMask;
   Float_t         mCenterOfMassEnergy;
   Int_t           mAEast;
   Int_t           mZEast;
   Int_t           mAWest;
   Int_t           mZWest;
   Float_t         mLuminosity;
   Float_t         mBeamPolarizationEast[3];
   Float_t         mBeamPolarizationWest[3];
   Float_t         mBImpact;
   Float_t         mPhImpact;
   Int_t           mGenerType;
   UInt_t          mBunchCrossingNumber[2];
   Int_t           mIventNumber;
   Int_t           mEventSize;
   Int_t           mEventNumber;
   UInt_t          mEventTime_fDatime;
   UInt_t          mProdTime_fDatime;
   TString         mEventType;
 //StSpinInfo_t    *spinInfo;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   Bool_t          mValidDB;
   Bool_t          mbXingIsMasked;
   UShort_t        mSpin4;
   UShort_t        mBunchCrossing7bit;
   UShort_t        mBunchCrossing48bit;
   UShort_t        mBunchCrossingStar;
   UShort_t        mDsmVertex;
   Int_t           mPolarizationType;
 //TVector3        *vertex;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   Double_t        fX;
   Double_t        fY;
   Double_t        fZ;
   Float_t         vertexRank;
 //TrigSet         *hardwareTriggers;
   Int_t           nTrigs;
   UInt_t          mTrigArray[10];   //[nTrigs]
 //TrigSet         *softwareTriggers;
   Int_t           nTrigs;
   UInt_t          mTrigArray[10];   //[nTrigs]
   UInt_t          bbcOnlineTimeDiff;
 //EEmcEnergy_t    *eemcEnergy;
   UInt_t          fUniqueID;
   UInt_t          fBits;
   EEmcElement_t   eTow_element[720];
   EEmcElement_t   ePre1_element[720];
   EEmcElement_t   ePre2_element[720];
   EEmcElement_t   ePost_element[720];
   ESmdSector_t    eSmd_sec[12];
   UInt_t          nTowers;
   UInt_t          nStrips;

   // List of branches
   TBranch        *b_evtHddr_fUniqueID;   //!
   TBranch        *b_evtHddr_fBits;   //!
   TBranch        *b_evtHddr_fName;   //!
   TBranch        *b_evtHddr_fTitle;   //!
   TBranch        *b_evtHddr_mRunNumber;   //!
   TBranch        *b_evtHddr_mId;   //!
   TBranch        *b_evtHddr_mInputTriggerMask;   //!
   TBranch        *b_evtHddr_mTriggerMask;   //!
   TBranch        *b_evtHddr_mCenterOfMassEnergy;   //!
   TBranch        *b_evtHddr_mAEast;   //!
   TBranch        *b_evtHddr_mZEast;   //!
   TBranch        *b_evtHddr_mAWest;   //!
   TBranch        *b_evtHddr_mZWest;   //!
   TBranch        *b_evtHddr_mLuminosity;   //!
   TBranch        *b_evtHddr_mBeamPolarizationEast;   //!
   TBranch        *b_evtHddr_mBeamPolarizationWest;   //!
   TBranch        *b_evtHddr_mBImpact;   //!
   TBranch        *b_evtHddr_mPhImpact;   //!
   TBranch        *b_evtHddr_mGenerType;   //!
   TBranch        *b_evtHddr_mBunchCrossingNumber;   //!
   TBranch        *b_evtHddr_mIventNumber;   //!
   TBranch        *b_evtHddr_mEventSize;   //!
   TBranch        *b_evtHddr_mEventNumber;   //!
   TBranch        *b_evtHddr_mEventTime_fDatime;   //!
   TBranch        *b_evtHddr_mProdTime_fDatime;   //!
   TBranch        *b_evtHddr_mEventType;   //!
   TBranch        *b_spinInfo_fUniqueID;   //!
   TBranch        *b_spinInfo_fBits;   //!
   TBranch        *b_spinInfo_mValidDB;   //!
   TBranch        *b_spinInfo_mbXingIsMasked;   //!
   TBranch        *b_spinInfo_mSpin4;   //!
   TBranch        *b_spinInfo_mBunchCrossing7bit;   //!
   TBranch        *b_spinInfo_mBunchCrossing48bit;   //!
   TBranch        *b_spinInfo_mBunchCrossingStar;   //!
   TBranch        *b_spinInfo_mDsmVertex;   //!
   TBranch        *b_spinInfo_mPolarizationType;   //!
   TBranch        *b_vertex_fUniqueID;   //!
   TBranch        *b_vertex_fBits;   //!
   TBranch        *b_vertex_fX;   //!
   TBranch        *b_vertex_fY;   //!
   TBranch        *b_vertex_fZ;   //!
   TBranch        *b_vertexRank;   //!
   TBranch        *b_hardwareTriggers_nTrigs;   //!
   TBranch        *b_mTrigArray;   //!
   TBranch        *b_softwareTriggers_nTrigs;   //!
   TBranch        *b_mTrigArray;   //!
   TBranch        *b_bbcOnlineTimeDiff;   //!
   TBranch        *b_eemcEnergy_fUniqueID;   //!
   TBranch        *b_eemcEnergy_fBits;   //!
   TBranch        *b_eemcEnergy_eTow_element;   //!
   TBranch        *b_eemcEnergy_ePre1_element;   //!
   TBranch        *b_eemcEnergy_ePre2_element;   //!
   TBranch        *b_eemcEnergy_ePost_element;   //!
   TBranch        *b_eemcEnergy_eSmd_sec;   //!
   TBranch        *b_eemcEnergy_nTowers;   //!
   TBranch        *b_eemcEnergy_nStrips;   //!

   AnEEmcTreePart1Reader(TTree *tree=0);
   virtual ~AnEEmcTreePart1Reader();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef AnEEmcTreePart1Reader_cxx
AnEEmcTreePart1Reader::AnEEmcTreePart1Reader(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("tree",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("tree","");
      chain->Add("/star/u/kabir/GIT/BrightSTAR/dst/R15EEmcRpTree/eemcTreeP1_EEmcRpTree_16093017.root/tree");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

AnEEmcTreePart1Reader::~AnEEmcTreePart1Reader()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t AnEEmcTreePart1Reader::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t AnEEmcTreePart1Reader::LoadTree(Long64_t entry)
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

void AnEEmcTreePart1Reader::Init(TTree *tree)
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

//    fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_evtHddr_fUniqueID);
//    fChain->SetBranchAddress("fBits", &fBits, &b_evtHddr_fBits);
   fChain->SetBranchAddress("fName", &fName, &b_evtHddr_fName);
   fChain->SetBranchAddress("fTitle", &fTitle, &b_evtHddr_fTitle);
   fChain->SetBranchAddress("mRunNumber", &mRunNumber, &b_evtHddr_mRunNumber);
   fChain->SetBranchAddress("mId", &mId, &b_evtHddr_mId);
   fChain->SetBranchAddress("mInputTriggerMask", &mInputTriggerMask, &b_evtHddr_mInputTriggerMask);
   fChain->SetBranchAddress("mTriggerMask", &mTriggerMask, &b_evtHddr_mTriggerMask);
   fChain->SetBranchAddress("mCenterOfMassEnergy", &mCenterOfMassEnergy, &b_evtHddr_mCenterOfMassEnergy);
   fChain->SetBranchAddress("mAEast", &mAEast, &b_evtHddr_mAEast);
   fChain->SetBranchAddress("mZEast", &mZEast, &b_evtHddr_mZEast);
   fChain->SetBranchAddress("mAWest", &mAWest, &b_evtHddr_mAWest);
   fChain->SetBranchAddress("mZWest", &mZWest, &b_evtHddr_mZWest);
   fChain->SetBranchAddress("mLuminosity", &mLuminosity, &b_evtHddr_mLuminosity);
   fChain->SetBranchAddress("mBeamPolarizationEast[3]", mBeamPolarizationEast, &b_evtHddr_mBeamPolarizationEast);
   fChain->SetBranchAddress("mBeamPolarizationWest[3]", mBeamPolarizationWest, &b_evtHddr_mBeamPolarizationWest);
   fChain->SetBranchAddress("mBImpact", &mBImpact, &b_evtHddr_mBImpact);
   fChain->SetBranchAddress("mPhImpact", &mPhImpact, &b_evtHddr_mPhImpact);
   fChain->SetBranchAddress("mGenerType", &mGenerType, &b_evtHddr_mGenerType);
   fChain->SetBranchAddress("mBunchCrossingNumber[2]", mBunchCrossingNumber, &b_evtHddr_mBunchCrossingNumber);
   fChain->SetBranchAddress("mIventNumber", &mIventNumber, &b_evtHddr_mIventNumber);
   fChain->SetBranchAddress("mEventSize", &mEventSize, &b_evtHddr_mEventSize);
   fChain->SetBranchAddress("mEventNumber", &mEventNumber, &b_evtHddr_mEventNumber);
   fChain->SetBranchAddress("mEventTime.fDatime", &mEventTime_fDatime, &b_evtHddr_mEventTime_fDatime);
   fChain->SetBranchAddress("mProdTime.fDatime", &mProdTime_fDatime, &b_evtHddr_mProdTime_fDatime);
   fChain->SetBranchAddress("mEventType", &mEventType, &b_evtHddr_mEventType);
   fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_spinInfo_fUniqueID);
   fChain->SetBranchAddress("fBits", &fBits, &b_spinInfo_fBits);
   fChain->SetBranchAddress("mValidDB", &mValidDB, &b_spinInfo_mValidDB);
   fChain->SetBranchAddress("mbXingIsMasked", &mbXingIsMasked, &b_spinInfo_mbXingIsMasked);
   fChain->SetBranchAddress("mSpin4", &mSpin4, &b_spinInfo_mSpin4);
   fChain->SetBranchAddress("mBunchCrossing7bit", &mBunchCrossing7bit, &b_spinInfo_mBunchCrossing7bit);
   fChain->SetBranchAddress("mBunchCrossing48bit", &mBunchCrossing48bit, &b_spinInfo_mBunchCrossing48bit);
   fChain->SetBranchAddress("mBunchCrossingStar", &mBunchCrossingStar, &b_spinInfo_mBunchCrossingStar);
   fChain->SetBranchAddress("mDsmVertex", &mDsmVertex, &b_spinInfo_mDsmVertex);
   fChain->SetBranchAddress("mPolarizationType", &mPolarizationType, &b_spinInfo_mPolarizationType);
//    fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_vertex_fUniqueID);
//    fChain->SetBranchAddress("fBits", &fBits, &b_vertex_fBits);
   fChain->SetBranchAddress("fX", &fX, &b_vertex_fX);
   fChain->SetBranchAddress("fY", &fY, &b_vertex_fY);
   fChain->SetBranchAddress("fZ", &fZ, &b_vertex_fZ);
   fChain->SetBranchAddress("vertexRank", &vertexRank, &b_vertexRank);
   fChain->SetBranchAddress("nTrigs", &nTrigs, &b_hardwareTriggers_nTrigs);
   fChain->SetBranchAddress("mTrigArray", mTrigArray, &b_mTrigArray);
//    fChain->SetBranchAddress("nTrigs", &nTrigs, &b_softwareTriggers_nTrigs);
//    fChain->SetBranchAddress("mTrigArray", mTrigArray, &b_mTrigArray);
   fChain->SetBranchAddress("bbcOnlineTimeDiff", &bbcOnlineTimeDiff, &b_bbcOnlineTimeDiff);
//    fChain->SetBranchAddress("fUniqueID", &fUniqueID, &b_eemcEnergy_fUniqueID);
//    fChain->SetBranchAddress("fBits", &fBits, &b_eemcEnergy_fBits);
   fChain->SetBranchAddress("eTow.element[720]", eTow_element, &b_eemcEnergy_eTow_element);
   fChain->SetBranchAddress("ePre1.element[720]", ePre1_element, &b_eemcEnergy_ePre1_element);
   fChain->SetBranchAddress("ePre2.element[720]", ePre2_element, &b_eemcEnergy_ePre2_element);
   fChain->SetBranchAddress("ePost.element[720]", ePost_element, &b_eemcEnergy_ePost_element);
   fChain->SetBranchAddress("eSmd.sec[12]", eSmd_sec, &b_eemcEnergy_eSmd_sec);
   fChain->SetBranchAddress("nTowers", &nTowers, &b_eemcEnergy_nTowers);
   fChain->SetBranchAddress("nStrips", &nStrips, &b_eemcEnergy_nStrips);
   Notify();
}

Bool_t AnEEmcTreePart1Reader::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void AnEEmcTreePart1Reader::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t AnEEmcTreePart1Reader::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef AnEEmcTreePart1Reader_cxx
