// This is an example of how to read the MuDst and do the cluster finding 
// for the BEMC

void EmcReadPoint(TString infile = "~/pwg/data/st_fms_16066050_raw_5000002.MuDst.root", TString outFile = "", const Int_t n_event = 100)
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

    TH1D *photonE = new TH1D("photonE", "photonE", 200, 0.0, 0.0);
    StEmcCollection *emcCollection;
    StSPtrVecEmcPoint emcPoints;
    StEmcPoint *point;

       
    StMuDst *muDst;
    chain->Init();
    
    Int_t i_event = 0;
    Int_t status = 1;
    while (i_event < n_event)
    {
	status = chain->Make();
	hasTrigger = kFALSE;
	muDst = mudst_mk->muDst();
	
	cout << "Processing event no.:" << i_event <<endl;
	
	//---------- Access EMC data ---------------
	emcCollection = muDst->emcCollection();	
	emcPoints = emcCollection->barrelPoints();
	//cout <<"Number of points: "<< emcPoints.size() <<endl;
	Int_t nPoints = emcPoints.size();
	
	for(Int_t i = 0; i < nPoints; ++i)
	{
	    point = emcPoints[i];
	    cout << "--------> Energy: "<< point->energy() <<endl;
	    cout << "--------> Position: "<< point->position().x() <<"\t" << point->position().y() <<"\t" << point->position().z() <<endl;
	    cout << "--------> Point eta:"<< point->position().pseudoRapidity()<<"\t"<<"phi:"<< point->position().phi() <<endl;
	    cout << "--------> Point delta_eta:"<< point->size().x()<<"\t"<<"delta_phi:"<< point->size().y() <<endl;

	    cout << "--------> Position Error: "<< point->positionError().x() <<"\t" << point->positionError().y() <<"\t" << point->positionError().z() <<endl;
	    cout << "--------> Point eta error:"<< point->positionError().pseudoRapidity()<<"\t"<<"phi error:"<< point->positionError().phi() <<endl;
	    
	    //--------- The following fields are not set and do NOT provide sensible value ----------------
	    cout << "--------->Detector: "<<point->detector() <<endl;
	    cout << "--------->Quality: "<<point->quality() <<endl;
	    cout << "--------->id: "<<point->id()<<" charge:"<<point->charge() <<endl;
	    cout << "--------->deltaEta: "<<point->deltaEta()<<" deltaPhi:"<<point->deltaPhi() <<endl;
	    cout << "--------->num of tracks: "<<point->nTracks()<<endl;
	    // cout << "--------->print: "<<point->print() <<endl; // This will make it crash at some point

	    StPtrVecTrack& tr = point->track();
	    cout <<"------->num of tracks: " << tr.size() <<endl;

	    photonE->Fill(point->energy());		    
	}

	if(i_event %1000 ==0)
	    cout << "Events processed: "<< i_event <<endl;

	i_event++;
	chain->Clear();
    }

    photonE->Draw();
    chain->Finish();
}

