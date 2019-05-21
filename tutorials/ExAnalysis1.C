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


void ExAnalysis1()
{ // Plots delta-Phi
    TFile mFile("/star/u/kabir/pwg/data/st_physics_12114059_raw_3010001.MuDst.root");
    TTree *tree = (TTree*) mFile.Get("MuDst");	
    TClonesArray *array = new TClonesArray("StMuTrack",10000); //First argument is class name.

    TBranch *brTracks = tree->GetBranch("PrimaryTracks");
    tree->SetBranchAddress("PrimaryTracks",&array);	
    TH1F *dPhi = new TH1F("dPhi","dPhi",100,-3.15,3.15);
    int iEvent = 0;
    
    while( brTracks->GetEntry(iEvent++) )
    {
	int nTracks = array->GetEntriesFast();
	for(int i = 0; i < nTracks-1; i++)
	{
	    StMuTrack *track1 = (StMuTrack*) array->At(i);
	    if(track1->pt() > 2.0 && track1->flag() > 0)
	    {
		for(int j = i+1; j < nTracks; j++)
		{
		    StMuTrack *track2 = (StMuTrack*) array->At(j);
		    if(track2->pt() > 2.0 && track2->flag() > 0)
		    {
			dPhi->Fill(track1->phi() - track2->phi());
			cout<<track1->phi()<<"\t"<<track2->phi()<<endl;
		    }
		}
	    }
	} 
    }

    dPhi->DrawCopy();
    dPhi->Delete();					
    mFile.Close();
}			
		
		
	
