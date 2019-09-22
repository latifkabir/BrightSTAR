// This is an example of how to read the MuDst and do the cluster finding 
// for the BEMC

#include "RootInclude.h"
#include "StRootInclude.h"
#include "BrEmcMaker/TStEmcTreeMaker.h"
#include "BrEmcMaker/TStEmcTrackMatchingMaker.h"

void RunEmcTreeMaker(TString infile, TString outFile, const Int_t n_events)
{	
    StChain *chain = new StChain;
    StMuDstMaker *mudst_mk = new StMuDstMaker(0, 0, "", infile, "", 1000);
    // Need St_db_Maker for Emc calibration
    St_db_Maker *db1 = new St_db_Maker("db","$HOME/StarDb","MySQL:StarDb","$STAR/StarDb");
    chain->SetDEBUG(0);

    // Maker to apply calibration
    StEmcADCtoEMaker *adc_to_e = new StEmcADCtoEMaker();
    adc_to_e->setPrint(kFALSE);
    // Makers for cluster finding
    StPreEclMaker *pre_ecl = new StPreEclMaker();
    pre_ecl->setPrint(kFALSE);
    StEpcMaker *epc = new StEpcMaker();
    epc->setPrint(kFALSE);
    epc->setFillHisto(kTRUE);

    TStEmcTrackMatchingMaker *trackMatchingMkr = new TStEmcTrackMatchingMaker();
    
    TStEmcTreeMaker *emcTreeMaker = new TStEmcTreeMaker();
    emcTreeMaker->SetOutName(outFile);
    
    // emcTreeMaker->AddTrigger(480003); //MB
    //emcTreeMaker->AddTrigger(480201);
    //emcTreeMaker->AddTrigger(480202);
    //emcTreeMaker->AddTrigger(480203);
    // emcTreeMaker->AddTrigger(480204);   // HT1
    // emcTreeMaker->AddTrigger(480205);   // HT2
    //emcTreeMaker->AddTrigger(480206);
    
    chain->Init();

    StMuDst *muDst = mudst_mk->muDst();
    Int_t nEvents =  mudst_mk->chain()->GetEntries();
    if(n_events != -1)
	nEvents = n_events;
    cout << "Total events to be processed: " << nEvents << endl;
    for (Int_t i = 0;  i < nEvents; ++i)
    {
	chain->Make(i);
	//cout << "-----> Number of primary tracks: "<< mudst_mk->muDst()->numberOfPrimaryTracks() <<endl;
	if(i %100 ==0)
	    cout << "Events processed: "<< i <<endl;	   
	chain->Clear();
    }
    chain->Finish();

    // TCanvas *c1 = new TCanvas();
    // trackMatchingMkr->h1Eta_p->Draw();
    // TCanvas *c2 = new TCanvas();
    // trackMatchingMkr->h1Phi_p->Draw();
    
    delete chain;
}

