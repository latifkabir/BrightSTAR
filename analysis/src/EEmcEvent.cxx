// Filename: EEmcEvent.cxx
// Description: Event display for EEMC
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon May 20 15:04:13 2019 (-0400)
// URL: jlab.org/~latif


#include "RootInclude.h"
#include "TStRunList.h"
#include "TStar.h"
#include "StRootInclude.h"

void EEmcEvent(Int_t runNo, Int_t maxFiles, Int_t eventNo)
{
    TStopwatch st;
    st.Start();
    Int_t sector = 2;
    Int_t nFiles = TStRunList::MakeFileList(runNo, maxFiles);
    TString fileList = TStar::Config->GetFileList();
    TStar::ExitIfInvalid(fileList);
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", nFiles);

    muDstMaker->SetStatus("*",0);
    muDstMaker->SetStatus("MuEvent*",1);
    muDstMaker->SetStatus("Emc*",1);
    muDstMaker->SetStatus("EEmc*",1);

    StMuDst *mDst = muDstMaker->muDst();   // Get StMuDst
    StMuEmcCollection *emcMuColl;
    StMuTriggerIdCollection *trigMuColl;
    
    TChain *ch = muDstMaker->chain();
    TH1F *dist0 = new TH1F("dist0", (TString)"Tower ADC Distribution, sec = " + (TString)to_string(sector), 60, 60.0, 120.0);
    TH1F *dist1 = new TH1F("dist1", (TString)"SMD U plane sec = " + (TString)to_string(sector), 288, 0.0, 288.0);
    TH1F *dist2 = new TH1F("dist2", (TString)"SMD V plane sec = " + (TString)to_string(sector), 288, 0.0, 288.0);

    Int_t iEvent = 0;
    vector <int> exptTrigID;
    exptTrigID.push_back(480301); // 480301 ---> EHT0 Trigger

    Bool_t foundTrigger = kFALSE;    
    chain->Init();

    Int_t nEvents = ch->GetEntries();  
    int sector_t;			// 1-12
    int subsector_t;	        	// 1-5
    int etabin_t;			// 1-12
    int rawAdc_t;
    int towerIndex;
    
    Int_t sec_u, strip_u, adc_u, X;
    Int_t sec_v, strip_v, adc_v, Y;
    StMuEmcHit *hit;
    TCanvas *c0 = new TCanvas();
    TCanvas *c1 = new TCanvas();
    c1->Divide(1, 2);
    
    for(Int_t ii = 1; ii < nEvents; ++ii)
    {
	foundTrigger = kFALSE;
	chain->Clear();
	//chain->Make(ii);
	muDstMaker->chain()->GetEntry(ii);

	// trigMuColl = &mDst->event()->triggerIdCollection();
	// if(!trigMuColl)
	// {
	//     cout<<"No Trigger data for this event"<<endl;
	//     continue;
	// }
	// const StTriggerId trgIDs = trigMuColl->nominal();
	// for(Int_t i = 0; i < exptTrigID.size(); i++)
        // {
	//     if (trgIDs.isTrigger(exptTrigID[i]))
	// 	foundTrigger = kTRUE;
	// }
	// if(!foundTrigger)
	//     continue;
	
	emcMuColl = mDst->muEmcCollection();	
	if(!emcMuColl)
	{
	    cout<<"No EMC data for this event"<<endl;
	    return;
	}
	//-------------------- Tower-------------------------------------
	for (int ii = 0; ii < emcMuColl->getNEndcapTowerADC(); ++ii)
	{	    
	    emcMuColl->getEndcapTowerADC(ii, rawAdc_t, sector_t, subsector_t, etabin_t);
	    towerIndex = (sector_t -1) * 60 + (subsector_t - 1) * 12 + (etabin_t - 1);
	    if(sector_t == sector)
		dist0->SetBinContent(towerIndex + 1 - (sector_t-1)*60, rawAdc_t);
	}	
	//------------------- SMD ----------------------------------------- 
	Int_t nh_u = emcMuColl->getNEndcapSmdHits('U');
	Int_t nh_v = emcMuColl->getNEndcapSmdHits('V');

	for (Int_t i = 0; i < nh_u; i++)
	{
	    hit = emcMuColl->getEndcapSmdHit('U', i, sec_u, strip_u);
	    adc_u = hit->getAdc();

	    if(sec_u == sector)
	    {
		cout << "Hit:"<<i<<" Strip: "<<strip_u<<" ADC: "<<adc_u <<endl;
		dist1->SetBinContent(strip_u, adc_u);
	    }
	}
	for (Int_t i = 0; i < nh_v; i++)
	{		
	    hit = emcMuColl->getEndcapSmdHit('V', i, sec_v, strip_v);
	    adc_v = hit->getAdc();
	    if(sec_v == sector)
	    {
		cout << "Hit:"<<i<<" Strip: "<<strip_v<<" ADC: "<<adc_v <<endl;
		dist2->SetBinContent(strip_v, adc_v);
	    }
	}		
	c0->cd();
	dist0->Draw();
	c1->cd(1);
	dist1->Draw();
	c1->cd(2);
	dist2->Draw();
	c0->Update();
	c1->Update();
	sleep(1);
	dist0->Reset();
	dist1->Reset();
	dist2->Reset();
    }
    chain->Finish();
    st.Stop();
    st.Print();
    //delete chain;
}			
