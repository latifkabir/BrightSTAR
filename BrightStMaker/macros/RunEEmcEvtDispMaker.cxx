// Filename: RunEEmcEvtDispMaker.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun 23 21:23:37 2019 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrEEmcMaker/TStEEmcEvtDispMaker.h"
#include "TString.h"

void RunEEmcEvtDispMaker(TString InputFileList, TString outputfile, Int_t nEntries)
{
    if(InputFileList == "")
	//InputFileList = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/090/16090026/st_physics_16090026_raw_5500035.MuDst.root";	
	InputFileList = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/066/16066033/st_physics_16066033_raw_0000006.MuDst.root";	

    StChain* chain = new StChain;
    StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", InputFileList, ".", 1000, "MuDst");
    StMuDbReader* muDB = StMuDbReader::instance();

    St_db_Maker* dbMaker = new St_db_Maker("StarDb", "MySQL:StarDb");     // STAR database
    StEEmcDbMaker* eemcDb = new StEEmcDbMaker;     // EEMC database

    TCanvas *c0 = new TCanvas("c0", "c0");
    c0->Divide(1, 2);
    TCanvas *c1 = new TCanvas("c1", "c1");
    c1->Divide(1, 2);
    TCanvas *c2 = new TCanvas("c2", "c2");
    c2->Divide(1, 2);
    TH1F *hist1d[6];
    
    TStEEmcEvtDispMaker* EEmcEvtDispMaker = new TStEEmcEvtDispMaker(muDstMaker, hist1d);
    //EEmcDistMaker->setOutFile(outputfile);
    TChain *ch = muDstMaker->chain();
    
    chain->Init();
    if(nEntries == -1)
	nEntries = ch->GetEntries();
      
    // Event loop
    for (int iEvent = 1; iEvent <= nEntries; ++iEvent)
    {
	chain->Clear();
	int status = chain->Make(iEvent);
	
	c0->cd(1);
	hist1d[0]->Draw();
	c0->cd(2);
	hist1d[1]->Draw();
	
	c1->cd(1);
	hist1d[2]->Draw();
	c1->cd(2);
	hist1d[3]->Draw();

	c2->cd(1);
	hist1d[4]->Draw();
	c2->cd(2);
	hist1d[5]->Draw();
	
	c0->Update();
	c1->Update();
	c2->Update();
	sleep(1);
	// for(int i = 0; i< 6; ++i)
	//     hist1d[i]->Reset();
    }
    chain->Finish();
}
