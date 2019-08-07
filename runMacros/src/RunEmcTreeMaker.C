// This is an example of how to read the MuDst and do the cluster finding 
// for the BEMC

void RunEmcTreeMaker(TString infile = "~/pwg/data/st_fms_16066050_raw_5000002.MuDst.root", const Int_t n_event=100)
{
    infile = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/072/16072047/st_physics_16072047_raw_1000014.MuDst.root";
    //gROOT->Macro("loadMuDst.C");

    // Load St_db_Maker and co
    gSystem->Load("StDbLib.so");
    gSystem->Load("StDbBroker.so");
    gSystem->Load("St_db_Maker");

    // Load Emc libraries
    gSystem->Load("StDaqLib");
    gSystem->Load("StEmcRawMaker");
    gSystem->Load("StEmcADCtoEMaker");
    gSystem->Load("StPreEclMaker");
    gSystem->Load("StEpcMaker");

    StChain *chain=new StChain;
    StMuDstMaker *mudst_mk=new StMuDstMaker(0,0,"",infile,"",999);

    // Need St_db_Maker for Emc calibration
    St_db_Maker *db1 = new St_db_Maker("db","$HOME/StarDb","MySQL:StarDb","$STAR/StarDb");

    // Maker to apply calibration
    StEmcADCtoEMaker *adc_to_e=new StEmcADCtoEMaker();
    adc_to_e->setPrint(kFALSE);
    // Makers for cluster finding
    StPreEclMaker *pre_ecl=new StPreEclMaker();
    pre_ecl->setPrint(kFALSE);
    StEpcMaker *epc=new StEpcMaker();
    epc->setPrint(kFALSE);

    chain->Init();
   
    // This is how you can set alternative clustering parameters
    /*
      pre_ecl->SetClusterConditions("bemc",  4, 0.2, 0.1, 0.05, kFALSE);
  
      Int_t sizeMax = 5;
      Float_t energySeed = 0.2;
      Float_t energyAdd  = 0.1;
      Float_t minTotE  = 0.05;
  
      pre_ecl->SetClusterConditions("bsmde", sizeMax,energySeed, energyAdd, 
      minTotE, kFALSE);
      pre_ecl->SetClusterConditions("bsmdp", sizeMax,energySeed, energyAdd, 
      minTotE, kFALSE);
    */	  

    Int_t i_event = 0;
    while (i_event < n_event)
    {
	chain->Make();
	
	StEmcCollection *emcCollection = mudst_mk->muDst()->emcCollection();

	if (emcCollection)
	{
	    cout << emcCollection->barrelPoints().size() << " points in barrel" << endl;

	    StEmcDetector *barrel = emcCollection->detector(kBarrelEmcTowerId);
	    if (barrel->cluster()) 
	    	cout << barrel->cluster()->clusters().size() << " barrel tower clusters" << endl;
	    StEmcDetector *smde = emcCollection->detector(kBarrelSmdEtaStripId);
	    if (smde) 
	    	cout << smde->cluster()->clusters().size() << " smd eta clusters" << endl;
	    StEmcDetector *smdp = emcCollection->detector(kBarrelSmdPhiStripId);
	    if (smdp) 
	    	cout << smdp->cluster()->clusters().size() << " smd phi clusters" << endl;
	}
	else
	{
	    cout << "No emc collection!" << endl;
	}
    
	i_event++;
	chain->Clear();
    }
}
