// This is an example of how to read the MuDst and do the cluster finding 
// for the BEMC

void RunEmcTreeMaker(TString infile = "~/pwg/data/st_fms_16066050_raw_5000002.MuDst.root", TString outFile = "", const Int_t n_event=100)
{
    infile = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/072/16072047/st_physics_16072047_raw_1000014.MuDst.root";
    //gROOT->Macro("loadMuDst.C");
    gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    loadSharedLibraries();
    // Load St_db_Maker and co
    gSystem->Load("StDbLib.so");
    gSystem->Load("StDbBroker.so");
    gSystem->Load("St_db_Maker");

    // Load Emc libraries
    gSystem->Load("StDaqLib");
    gSystem->Load("StEmcRawMaker");
    gSystem->Load("StEmcADCtoEMaker");
    gSystem->Load("StPreEclMaker");
    gSystem->Load("BrEmcMaker");

    gSystem->Load("StTriggerFilterMaker");
	
    StChain *chain=new StChain;
    StMuDstMaker *mudst_mk = new StMuDstMaker(0,0,"",infile,"",999);

    // Need St_db_Maker for Emc calibration
    St_db_Maker *db1 = new St_db_Maker("db","$HOME/StarDb","MySQL:StarDb","$STAR/StarDb");
       
    // Maker to apply calibration
    StEmcADCtoEMaker *adc_to_e = new StEmcADCtoEMaker();
    adc_to_e->setPrint(kFALSE);
    // Makers for cluster finding
    StPreEclMaker *pre_ecl = new StPreEclMaker();
    pre_ecl->setPrint(kFALSE);
    StEpcMaker *epc = new StEpcMaker();
    epc->setPrint(kFALSE);
    epc->setFillHisto(kTRUE);
    
    chain->SetDEBUG(0);
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

    Double_t pairE;
    Double_t pairM;
    Double_t zgg;
    Double_t theta;
    StThreeVectorF v1;
    StThreeVectorF v2;
    Double_t E1;
    Double_t E2;
    Int_t Q1;
    Int_t Q2;
    TH1D *massDist = new TH1D("massDist", "massDist", 200, 0.0, 1.0);
    TH1D *photonE = new TH1D("photonE", "photonE", 200, 0.0, 0.0);
    
    Int_t i_event = 0;
    while (i_event < n_event)
    {
	chain->Make();
	StEmcCollection *emcCollection = mudst_mk->muDst()->emcCollection();	
	StSPtrVecEmcPoint& emcPoints = emcCollection->barrelPoints();
	//cout <<"Number of points: "<< emcPoints.size() <<endl;
	Int_t nPoints = emcPoints.size();
	
	for(Int_t i = 0; i < nPoints; ++i)
	{
	    StEmcPoint *point1 = emcPoints[i];
	    // cout << "--------> Energy: "<< point1->energy() <<endl;
	    //cout << "--------> Position: "<< point1->position().x() <<"\t" << point1->position().y() <<"\t" << point1->position().z() <<endl;
	    // StPtrVecTrack& tr = point1->track();
	    // cout <<"num of tracks: " << tr.size() <<endl;
	    photonE->Fill(point1->energy());	
	    //---------- Calculate and fill Pi0 invariant mass --------------
	    for(Int_t j = i + 1; j < nPoints; ++j)
	    {
		if(j>=nPoints)
		    continue;
		
		StEmcPoint *point2 = emcPoints[j];
	        v1 = point1->position();
		//cout << v1.x() <<"\t"<< v1.y() << "\t" <<v1.z() <<endl;

		v2 = point2->position();
		E1 = point1->energy();
		E2 = point2->energy();
		Q1 = point1->quality();
		Q2 = point2->quality();
		theta = v1.angle(v2);
		zgg = fabs(E1 - E2) / (E1 + E2);

		pairM = TMath::Sqrt(2*E1*E2*(1 - TMath::Cos(theta)));

		//cout << E1 << "\t" << E2 <<"\t" <<theta << "\t" << zgg << "\t" << pairM <<endl;

		if ((E1 + E2) < 1.0)
		    continue;
		// if (pairM  > 0.5)
		//     continue;
		// if(theta < 0.05)
		//     continue;
		if (zgg > 0.7)
		    continue;
		
		//if((Q1&1) && (Q1&4) && (Q1&8) && (Q2&1) && (Q2&4) && (Q2&8))		
		    massDist->Fill(pairM);
		
	    }	
	}

	if(i_event %1000 ==0)
	    cout << "Events processed: "<< i_event <<endl;

	i_event++;
	chain->Clear();
    }

    massDist->Draw();
    //photonE->Draw();
    chain->Finish();
}
