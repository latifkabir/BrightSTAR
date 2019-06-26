// Filename: AnReadEEMC.C
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

void AnEEMCdist(Int_t runNo, Int_t maxFiles)
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
    TH1F *dist1 = new TH1F("dist1", "dist1", 100, 0.0, 0.0);
    TH1F *dist2 = new TH1F("dist2", "dist2", 100, 0.0, 0.0);
    TH1F *dist3 = new TH1F("dist3", "dist3", 100, 0.0, 0.0);
    TH2F *h2dist = new TH2F("h2dist", "h2dist", 100, 0.0, 0.0, 100, 0.0, 0.0);
    TH1F *engDist = new TH1F("edist", "edist", 100, 0.0, 0.0);
    Int_t iEvent = 0;

    chain->Init();

    Int_t nEvents = ch->GetEntries();  

    Int_t sec_u, strip_u, adc_u, X;
    Int_t sec_v, strip_v, adc_v, Y;
    StMuEmcHit *hit;
    
    for (Int_t iev = 0; iev < nEvents; iev++)
    {
	chain->Clear();
	chain->Make(iev);
	emcMuColl = mDst->muEmcCollection();

	if(!emcMuColl)
	{
	    cout<<"No EMC data for this event"<<endl;
	    continue;
	}
	// ------------------- SMD ----------------------------------------- 
	Int_t nh_u = emcMuColl->getNEndcapSmdHits('U');
	Int_t nh_v = emcMuColl->getNEndcapSmdHits('V');
	Int_t nh = (nh_u < nh_v) ? nh_u : nh_v;
	if(nh_u != nh_v)
	    cout << "nHits did not match" <<endl;

	for (Int_t i = 0; i < nh; i++)
	{
	    hit = emcMuColl->getEndcapSmdHit('U', i, sec_u, strip_u);
	    adc_u = hit->getAdc();
	    X = 1000 + strip_u -1  + (sec_u-1)*300;  //Check if u to be associated with X??
		
	    hit = emcMuColl->getEndcapSmdHit('V', i, sec_v, strip_v);
	    adc_v = hit->getAdc();
	    Y = 1000 + strip_v -1  + (sec_v-1)*300;  //Check if v to be associated with Y??

	    if(sec_u == 2 && sec_u == sec_v)
	    {
		h2dist->Fill(X, Y);
		dist1->Fill(adc_u);
		dist2->Fill(adc_v);
	    }
	    engDist->Fill(sec_u);
	}		
    }

    TCanvas *c1 = new TCanvas();
    h2dist->Draw("colz");
    TCanvas *c2 = new TCanvas();
    engDist->Draw();
    TCanvas *c3 = new TCanvas();
    dist1->Draw();
    TCanvas *c4 = new TCanvas();
    dist2->Draw();
    
    chain->Finish();
    st.Stop();
    st.Print();
}			
