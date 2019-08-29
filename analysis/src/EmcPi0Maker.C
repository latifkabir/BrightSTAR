// This is an example of how to read the MuDst and do the cluster finding 
// for the BEMC

void EmcPi0Maker(TString infile = "~/pwg/data/st_fms_16066050_raw_5000002.MuDst.root", TString outFile = "", const Int_t n_event = 100)
{
    infile = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/072/16072047/st_physics_16072047_raw_1000014.MuDst.root";
	
    StChain *chain = new StChain;
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

    StMuTriggerIdCollection *trigMuColl;
    vector <int> exptTrigID;
    exptTrigID.push_back(480003); //MB
    //exptTrigID.push_back(480201);
    // exptTrigID.push_back(480202);
    // exptTrigID.push_back(480203);
    exptTrigID.push_back(480204);   // HT1
    exptTrigID.push_back(480205);   // HT2
    // exptTrigID.push_back(480206);
    Bool_t hasTrigger = kFALSE;
    StEmcCollection *emcCollection;
    StSPtrVecEmcPoint emcPoints;
    StEmcPoint *point1;
    StEmcPoint *point2;
    
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
    Double_t pi0_x;
    Double_t pi0_y;
    Double_t pi0_z;
    TH1D *massDist = new TH1D("massDist", "massDist", 200, 0.0, 1.0);
    TH1D *photonE = new TH1D("photonE", "photonE", 200, 0.0, 0.0);
    TLorentzVector LV1;
    TLorentzVector LV2;
    TLorentzVector LV;
    
    StMuDst *muDst;
    chain->Init();
    
    Int_t i_event = 0;
    Int_t status = 1;
    while (i_event < n_event)
    {
	status = chain->Make();
	hasTrigger = kFALSE;
	muDst = mudst_mk->muDst();
	//-------- Filter out undesirable triggers ---------
	trigMuColl = muDst->event()->triggerIdCollection();
	const StTriggerId trgIDs = trigMuColl->nominal();
	
	for(Int_t i = 0; i < exptTrigID.size(); i++)
        {
	    if (trgIDs.isTrigger(exptTrigID[i]))
		hasTrigger = kTRUE;
	}
	
	if(!hasTrigger)
	    continue;
	
	cout << "Processing event no.:" << i_event <<endl;
	
	//---------- Access EMC data ---------------
	emcCollection = muDst->emcCollection();	
	emcPoints = emcCollection->barrelPoints();
	//cout <<"Number of points: "<< emcPoints.size() <<endl;
	Int_t nPoints = emcPoints.size();
	
	for(Int_t i = 0; i < nPoints; ++i)
	{
	    point1 = emcPoints[i];
	    // cout << "--------> Energy: "<< point1->energy() <<endl;
	    // cout << "--------> Position: "<< point1->position().x() <<"\t" << point1->position().y() <<"\t" << point1->position().z() <<endl;
	    // cout << "--------->Detector: "<<point1->detector() <<endl;
	    // cout << "--------->id: "<<point1->id()<<" charge:"<<point1->charge() <<endl;
	    // cout << "--------->print: "<<point1->print() <<endl;

	    //StPtrVecTrack& tr = point1->track();
	    //cout <<"------->num of tracks: " << tr.size() <<endl;

	    photonE->Fill(point1->energy());	
	    //---------- Calculate and fill Pi0 invariant mass --------------
	    for(Int_t j = i + 1; j < nPoints; ++j)
	    {
		if(j >= nPoints)
		    continue;
		
		point2 = emcPoints[j];
	        v1 = point1->position();
		//cout << v1.x() <<"\t"<< v1.y() << "\t" <<v1.z() <<endl;
		v2 = point2->position();

		E1 = point1->energy();
		E2 = point2->energy();
		Q1 = point1->quality();
		Q2 = point2->quality();
		theta = v1.angle(v2);
		zgg = fabs(E1 - E2) / (E1 + E2);
		pi0_x = (E1*v1.x() + E2*v2.x()) / ( E1 + E2);
		pi0_y = (E1*v1.y() + E2*v2.y()) / ( E1 + E2);
		pi0_z = (E1*v1.z() + E2*v2.z()) / ( E1 + E2);
		
		//----------------------- Method 1 --------------------------
		//pairM = TMath::Sqrt(2*E1*E2*(1 - TMath::Cos(theta)));
		//cout << E1 << "\t" << E2 <<"\t" <<theta << "\t" << zgg << "\t" << pairM <<endl;

		// ----------------------- Method 2 ------------------------------------------------
		LV1.SetPxPyPzE(E1*v1.x() / v1.mag(), E1*v1.y() / v1.mag(), E1*v1.z() / v1.mag(), E1);
		LV2.SetPxPyPzE(E2*v2.x() / v2.mag(), E2*v2.y() / v2.mag(), E2*v2.z() / v2.mag(), E2);
		LV = (LV1 + LV2);
		pairM = LV.M();

		
		if ((E1 + E2) < 1.0)
		    continue;
		// if (pairM  > 0.5)
		//     continue;
		// if(theta < 0.05)
		//     continue;
		if (zgg > 0.7)
		    continue;

		if(LV.Pt() < 2.0)
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

