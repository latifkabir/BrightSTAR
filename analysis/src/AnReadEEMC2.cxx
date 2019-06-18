// Filename: AnReadEEMC2.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun May 26 15:24:41 2019 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "TStRunList.h"
#include "TStar.h"
#include "StRootInclude.h"
#include "AnReadEEMC2.h"

void AnReadEEMC2(Int_t runNo, Int_t maxFiles)
{
    TStopwatch st;
    st.Start();
    Int_t nFiles = TStRunList::MakeFileList(runNo, maxFiles);
    if(nFiles < 1)
    {
    	cout << "Data-files for run: "<< runNo <<" not found on local disk." <<endl;
    	TStar::Exit();
    }
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
    
    TChain *ch = muDstMaker->chain();
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
	
	printEEtower(emcMuColl);
	printEEpre(emcMuColl);
	printEEsmd(emcMuColl);      	
    }
    
    chain->Finish();
    st.Stop();
    st.Print();    
}


//======================================================================================
void printEEtower( StMuEmcCollection* emc )
{
    int sec,eta,sub,adc;
    StMuEmcHit *hit;
  
    int i, nh;
  
    printf("\tTotal %d hits in Tower (only ADC>0)\n",emc->getNEndcapTowerADC());
    nh=0;
    for (i=0; i< emc->getNEndcapTowerADC(); i++)
    {
	emc->getEndcapTowerADC(i,adc,sec,sub,eta);
	if (adc<=0) continue; // print only non-zero values
	nh++;
	printf("i=%d  Tower %2.2dT%c%2.2d   adc=%4d\n",i,sec,sub+'A'-1,eta,adc );
	//    printf("  Tower isec=%d ieta=%d isub=%d    adc=%4d\n",sec,eta, sub,adc );
	int adcX=1000+ (eta-1) + (sub-1)*12 +(sec-1)*60;
	//    assert(adc==adcX );
    }
    printf("\tTotal %d towers with ADC>0\n",nh);
}

//======================================================================================
void printEEpre( StMuEmcCollection* emc )
{
    int sec,eta,sub,pre,adc;
    StMuEmcHit *hit;
  
    int i, nh;
    nh= emc->getNEndcapPrsHits();
    printf("\nTotal %d hits in pre1+2+post\n",nh);
    for (i=0; i<nh; i++)
    {
	hit=emc->getEndcapPrsHit(i,sec,sub,eta,pre);
	int ss=sub + 5*(pre-1);
	adc=hit->getAdc();
	printf("i=%d  pre/post(%d) %2.2d%c%c%2.2d : energy=%f  adc=%d\n",i,pre,sec,pre+'P'-1,sub+'A'-1,eta,hit->getEnergy(),adc);
	int adcX=      (eta-1) + (sub-1) *12 +(sec-1)*60 + 1000*pre;
    
	//    assert(adc==adcX  );
    }
}


//======================================================================================
void printEEsmd( StMuEmcCollection* emc )
{
    int sec,strip,adc;
    char uv='U';
    StMuEmcHit *hit;
    
    for(uv='U'; uv<='V'; uv++)
    {
	int nh= emc->getNEndcapSmdHits(uv);
	printf("\nTotal %d hits in SMD-%c\n",nh,uv);
	for (int i=0; i<nh; i++) {
	    hit=emc->getEndcapSmdHit(uv,i,sec,strip);
	    adc=hit->getAdc();
	    printf("  SMD-%c  %2.2d%c%3.3d : energy=%f  adc=%d\n",uv,sec,uv,strip,hit->getEnergy(),adc);
	    int adcX= 1000 + strip-1  +(sec-1)*300;
	    //       assert(adc==adcX  );
	}
    }
}
