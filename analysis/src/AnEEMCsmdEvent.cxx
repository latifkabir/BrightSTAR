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

void AnEEMCsmdEvent(Int_t runNo, Int_t maxFiles, Int_t eventNo)
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
    
    TTree *ch = muDstMaker->chain();
    TH1F *dist1 = new TH1F("dist1", "dist1", 288, 0.0, 288.0);
    TH1F *dist2 = new TH1F("dist2", "dist2", 288, 0.0, 288.0);

    Int_t iEvent = 0;

    chain->Init();

    Int_t nEvents = ch->GetEntries();  

    Int_t sec_u, strip_u, adc_u, X;
    Int_t sec_v, strip_v, adc_v, Y;
    StMuEmcHit *hit;
    
    chain->Clear();
    chain->Make(eventNo);
    emcMuColl = mDst->muEmcCollection();

    if(!emcMuColl)
    {
	cout<<"No EMC data for this event"<<endl;
	return;
    }
    // ------------------- SMD ----------------------------------------- 
    Int_t nh_u = emcMuColl->getNEndcapSmdHits('U');
    Int_t nh_v = emcMuColl->getNEndcapSmdHits('V');

    for (Int_t i = 0; i < nh_u; i++)
    {
	hit = emcMuColl->getEndcapSmdHit('U', i, sec_u, strip_u);
	adc_u = hit->getAdc();		
	dist1->Fill(strip_u,adc_u);
    }

    for (Int_t i = 0; i < nh_v; i++)
    {		
	hit = emcMuColl->getEndcapSmdHit('V', i, sec_v, strip_v);
	adc_v = hit->getAdc();
	dist2->Fill(strip_v, adc_v);
    }		
	   
    TCanvas *c1 = new TCanvas();
    c1->Divide(1, 2);
    c1->cd(1);
    dist1->Draw();
    c1->cd(2);
    dist2->Draw();
    
    chain->Finish();
    st.Stop();
    st.Print();
}			
