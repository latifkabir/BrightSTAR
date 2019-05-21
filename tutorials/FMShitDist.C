#include <TFile.h>
#include <TH1F.h>
#include <TTree.h>
#include <TBranch.h>
#include <TObjArray.h>
#include <TClonesArray.h>
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StChain/StChain.h"
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuDstMaker.h"
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuEvent.h"
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuDst.h"
// #include "/afs/rhic.bnl.gov/star/packages/SL18h/StRoot/StMuDSTMaker/COMMON/StMuTrack.h"


void FMShitDist()
{ 
    TFile *mFile = new TFile("/star/u/kabir/pwg/data/st_physics_18053099_raw_0000004.MuDst.root");
    TTree *tree = (TTree*) mFile->Get("MuDst");	
    TClonesArray *array = new TClonesArray("StMuFmsHit",10000);
    StMuFmsHit *hit;
    
    // tree->SetBranchStatus("*",0);
    // tree->SetBranchStatus("FmsHit*",1);
    TBranch *br = tree->GetBranch("FmsHit");   // Select only FMS branch for quick access
    tree->SetBranchAddress("FmsHit",&array);	
    TH1F *adcDist = new TH1F("adcDist","adcDist",100,0.0,0.0);
    int iEvent = 0;
    
    while( br->GetEntry(iEvent++) )     // Doing tree->GetEntry(iEvent++) will be super slow
    {
	int nHits = array->GetEntriesFast();
	for(int i = 0; i < nHits; i++)
	{	     
	    hit = (StMuFmsHit*) array->At(i);
	    adcDist->Fill(hit->detectorId());
	} 
    }

    adcDist->Draw();
}			
		
		
	
