// This is an example of how to read the MuDst and do the cluster finding 
// for the BEMC

// #include "StEmcADCtoEMaker/StEmcADCtoEMaker.h"
// #include "StPreEclMaker/StPreEclMaker.h"
// #include "StEpcMaker/StEpcMaker.h"

void RunEmcTreeMaker(TString infile = "~/pwg/data/st_fms_16066050_raw_5000002.MuDst.root", TString outFile = "", const Int_t n_events = -1)
{
    infile = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/072/16072047/st_physics_16072047_raw_1000014.MuDst.root";
	
    StChain *chain = new StChain;
    StMuDstMaker *mudst_mk = new StMuDstMaker(0,0,"",infile,"",999);
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
    
    TStEmcTreeMaker *emcTreeMaker = new TStEmcTreeMaker();
    //emcTreeMaker->SetOutName();
    
    emcTreeMaker->AddTrigger(480003); //MB
    //emcTreeMaker->AddTrigger(480201);
    //emcTreeMaker->AddTrigger(480202);
    //emcTreeMaker->AddTrigger(480203);
    emcTreeMaker->AddTrigger(480204);   // HT1
    emcTreeMaker->AddTrigger(480205);   // HT2
    //emcTreeMaker->AddTrigger(480206);

    chain->Init();

    StMuDst *muDst = mudst_mk->muDst();
    Int_t nEvents =  mudst_mk->chain()->GetEntries();
    if(n_events != -1)
	nEvents = n_events;
    for (i = 0;  i < nEvents; ++i)
    {
	chain->Make(i);
	if(i %100 ==0)
	    cout << "Events processed: "<< i_event <<endl;	   
	chain->Clear();
    }
    chain->Finish();
    delete chain;
}

