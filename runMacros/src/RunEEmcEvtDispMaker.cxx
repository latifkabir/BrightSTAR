// Filename: RunEEmcEvtDispMaker.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun 23 21:23:37 2019 (-0400)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrEEmcMaker/TStEEmcEvtDispMaker.h"

void RunEEmcEvtDispMaker(TString InputFileList, TString outputfile, Int_t eventNo, Int_t maxEvents = -1)
{
    if(InputFileList == "")
	//InputFileList = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/090/16090026/st_physics_16090026_raw_5500035.MuDst.root";	
	InputFileList = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/066/16066033/st_physics_16066033_raw_0000006.MuDst.root";	
	//InputFileList = "/star/u/kabir/pwg/data/st_physics_adc_7136033_raw_1060001.MuDst.root";	

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
    TCanvas *c3 = new TCanvas("c3", "c3");
    TCanvas *c4 = new TCanvas("c4", "c4");
    c4->Divide(1, 2);
    TCanvas *c5 = new TCanvas("c5", "c5");
    c5->Divide(1, 2);
    TCanvas *c6 = new TCanvas("c6", "c6");
    c6->Divide(1, 2);    
    TH1F *hist1d[12];
    
    TStEEmcEvtDispMaker* EEmcEvtDispMaker = new TStEEmcEvtDispMaker(muDstMaker, hist1d);
    //EEmcDistMaker->setOutFile(outputfile);
    TChain *ch = muDstMaker->chain();
    
    chain->Init();
    Int_t iEvent;
    Int_t lEvent;
    if(eventNo == -1)
    {
	iEvent = 1;
	if(maxEvents == -1)
	    lEvent = ch->GetEntries() - 1;
	else
	    lEvent = maxEvents;
    }
    else
    {
	iEvent = eventNo;
	lEvent = eventNo;
    }
    TPaveText *t = new TPaveText(.05,.3,.95,.6);
    t->AddText("File: st_physics_16066033_raw_0000006");
    c3->cd();
    t->Draw();
    c3->SaveAs("jobResults/EEmcEvtDisp.pdf(", "pdf");
    t->Clear();
    // Event loop
    for (Int_t event = iEvent; event <= lEvent; ++event)
    {
	//cout << "Ntuple event no.: "<<event<<endl;

	chain->Clear();
	Int_t status = chain->Make(event);
	//muDstMaker->chain()->GetEntry(event);
	//ch->GetEntry(event);
	t->Clear();
	t->AddText("Event no: " + (TString) to_string(event));
	c3->cd();
	c3->Draw();
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

	c4->cd(1);
	hist1d[6]->Draw();
	c4->cd(2);
	hist1d[9]->Draw();

	c5->cd(1);
	hist1d[7]->Draw();
	c5->cd(2);
	hist1d[10]->Draw();

	c6->cd(1);
	hist1d[8]->Draw();
	c6->cd(2);
	hist1d[11]->Draw();
	
	c0->Update();
	c1->Update();
	c2->Update();
	c3->Update();
	c4->Update();
	c5->Update();
	c6->Update();
	sleep(1);

	if(hist1d[3]->GetMaximum() > 0.001 || hist1d[5]->GetMaximum() > 0.001)
	{
	    cout << "Non-trivial Event No.:"<<event<<endl;
	    c3->SaveAs("jobResults/EEmcEvtDisp.pdf", "pdf");	    
	    c0->SaveAs("jobResults/EEmcEvtDisp.pdf", "pdf");	    
	    c1->SaveAs("jobResults/EEmcEvtDisp.pdf", "pdf");	    
	    c2->SaveAs("jobResults/EEmcEvtDisp.pdf", "pdf");	    
	    c4->SaveAs("jobResults/EEmcEvtDisp.pdf", "pdf");	    
	    c5->SaveAs("jobResults/EEmcEvtDisp.pdf", "pdf");	    
	    c6->SaveAs("jobResults/EEmcEvtDisp.pdf", "pdf");	    
	}
    }
    c3->SaveAs("jobResults/EEmcEvtDisp.pdf)", "pdf");	          
    chain->Finish();
}
