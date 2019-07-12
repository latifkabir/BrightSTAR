// Filename: EEmcReadDemo.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon May 20 15:04:13 2019 (-0400)
// URL: jlab.org/~latif
/*
Exclusively access all EEMC fields using StRoot classes.

*/

#include "RootInclude.h"
#include "TStRunList.h"
#include "TStar.h"
#include "StRootInclude.h"

void EEmcReadDemo(Int_t runNo, Int_t maxFiles)
{
    TStopwatch st;
    st.Start();
    Int_t nFiles = TStRunList::MakeFileList(runNo, maxFiles);
    TString fileList = TStar::Config->GetFileList();
    TStar::ExitIfInvalid(fileList);
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", nFiles);

    muDstMaker->SetStatus("*",0);
    muDstMaker->SetStatus("Emc*",1);
    muDstMaker->SetStatus("EEmc*",1);

    StMuDst *mDst = muDstMaker->muDst();   // Get StMuDst
    StMuEmcCollection *emcMuColl;

    StMuEmcHit  *eemcHit;
    StMuEmcPoint *eemcPoint;

    TTree *ch = muDstMaker->chain();
    TH1F *dist = new TH1F("dist", "dist", 100, 0.0, 0.0);
    TH1F *engDist = new TH1F("edist", "edist", 100, 0.0, 0.0);
    int iEvent = 0;

    chain->Init();

    int nEvents = ch->GetEntries();  // Use muDstMaker->chain() if more than one root file is involved.

    for (int iev = 0; iev < nEvents; iev++)
    {
	chain->Clear();
	chain->Make(iev);
	emcMuColl = mDst->muEmcCollection();

	if(!emcMuColl)
	{
	    cout<<"No EMC data for this event"<<endl;
	    continue;
	}

	Int_t nClusEEMC = emcMuColl->getNClusters(5);  // 5 is the detector id for eemc
	//std::cout<<nClusEEMC<<std::endl;
	dist->Fill(nClusEEMC);
	for(Int_t i = 0; i < nClusEEMC; ++i)
	{
	    engDist->Fill(emcMuColl->getCluster(i, 5)->getEnergy());
	}
    }

    TCanvas *c1 = new TCanvas();
    dist->Draw();
    TCanvas *c2 = new TCanvas();
    engDist->Draw();	

    chain->Finish();
    st.Stop();
    st.Print();
}			
