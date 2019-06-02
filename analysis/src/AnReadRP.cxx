// Filename: AnReadRP.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon May 20 15:04:13 2019 (-0400)
// URL: jlab.org/~latif
/*
Exclusively access all RP fields using StRoot classes.
Check the class references for the classes: StMuRpsCollection, StMuRpsTrackPoint,  StMuRpsTrack
*/

#include "RootInclude.h"
#include "TStRunList.h"
#include "TStar.h"
#include "StRootInclude.h"

void AnReadRP(Int_t runNo, Int_t maxFiles)
{
    TStopwatch st;
    st.Start();
    TStRunList::MakeFileList(runNo, maxFiles);
    TString fileList = TStar::Config->GetFileList();
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "");
    muDstMaker->SetStatus("*",0);
    muDstMaker->SetStatus("pp2pp*",1);
    
    StMuDst *mDst = muDstMaker->muDst();   // Get StMuDst
    StMuRpsCollection *rpsMuColl;
    StMuRpsTrackPoint  *rpsTrkPoint;
    StMuRpsTrack *rpsTrk;
    
    TChain *ch = muDstMaker->chain();
    TH1F *dist = new TH1F("dist", "dist", 100, 0.0, 0.0);
    TH1F *engDist = new TH1F("edist", "edist", 100, 0.0, 0.0);
    int iEvent = 0;

    chain->Init();
    
    int nEvents = ch->GetEntries();  // Use muDstMaker->chain() if more than one root file is involved.    
    for (int iev = 0; iev < nEvents; iev++)
    {
	chain->Clear();
	int iret = chain->Make(iev);
	
	rpsMuColl = mDst->RpsCollection();

	if(!rpsMuColl)
	{
	    cout<<"No RP data for this event"<<endl;
	    continue;
	}

	for(Int_t i = 0; i < rpsMuColl->numberOfTrackPoints(); ++i)
	{
	    rpsTrkPoint = rpsMuColl->trackPoint(i);
	    dist->Fill(rpsTrkPoint->x());
	}
	
	for(Int_t j = 0; j < rpsMuColl->numberOfTracks(); ++j)
	{
	    rpsTrk = rpsMuColl->track(j);
            //Call getter function of rpsTrk
	}

	for(Int_t i = 0; i < rpsMuColl->numberOfRomanPots(); ++i)
	{
	    for(Int_t j = 0; j < rpsMuColl->numberOfPlanes(); ++j)
	    {
		for(Int_t k = 0; k < rpsMuColl->numberOfClusters(i,j); ++k)
		{
		    engDist->Fill(rpsMuColl->energyCluster(i, j, k));		   
		}
	    }
	}
    }
    chain->Finish();
    TCanvas *c1 = new TCanvas(); 
    dist->Draw();
    TCanvas *c2 = new TCanvas(); 
    engDist->Draw();
    st.Stop();
    st.Print();
}			
