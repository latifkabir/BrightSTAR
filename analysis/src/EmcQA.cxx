// Filename: EmcQA.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Sep 16 20:34:03 2019 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "StRootInclude.h"

void EmcQA(TString infile, TString outFile, const Int_t n_event)
{	
    StChain *chain = new StChain;
    StMuDstMaker *mudst_mk = new StMuDstMaker(0, 0, "", infile, "", 999);

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

    TFile *file = new TFile(outFile, "RECREATE");   
    TH1D *pointN = new TH1D("pointN", "pointN", 200, 0.0, 50.0);
    TH1D *pointE = new TH1D("pointE", "pointE; Energy [GeV]", 200, 0.0, 50.0);
    TH1D *pointEta = new TH1D("pointEta", "pointEta", 200, -1.1, 1.1);
    TH1D *pointPhi = new TH1D("pointPhi", "pointPhi", 200, -3.2, 3.2);
    TH1D *pointDeltaEta = new TH1D("pointDeltaEta", "pointDeltaEta", 200, 0.0, 0.1);
    TH1D *pointDeltaPhi = new TH1D("pointDeltaPhi", "pointDeltaPhi", 200, 0.0, 0.05);
    TH1D *pointEtaError = new TH1D("pointEtaError", "pointEtaError", 200, -1.5, 1.5);
    TH1D *pointPhiError = new TH1D("pointPhiError", "pointPhiError", 200, -3.2, 3.2);
    TH1D *pointQuality = new TH1D("pointQuality", "pointQuality", 200, 0, 50);
    
    Int_t nPoints;
    
    StEmcCollection *emcCollection;
    StSPtrVecEmcPoint emcPoints;
    StEmcPoint *point;
       
    StMuDst *muDst;
    chain->Init();

    Int_t nEvents = (n_event == -1)? mudst_mk->chain()->GetEntries() : n_event;
    cout << "Total events to be processed: "<<nEvents<<endl;

    for( Int_t evt = 0; evt < nEvents; ++evt)
    {
	chain->Make(evt);

	muDst = mudst_mk->muDst();    
	//---------- Access EMC data ---------------
	emcCollection = muDst->emcCollection();	
	emcPoints = emcCollection->barrelPoints();
	nPoints = emcPoints.size();
	pointN->Fill(nPoints);
	for(Int_t i = 0; i < nPoints; ++i)
	{
	    point = emcPoints[i];

	    pointE->Fill(point->energy());		    
	    pointEta->Fill(point->position().pseudoRapidity());		    
	    pointPhi->Fill(point->position().phi());
	    pointDeltaEta->Fill(point->size().x());
	    pointDeltaPhi->Fill(point->size().y());
	    pointQuality->Fill(point->quality());
	    pointEtaError->Fill(point->positionError().pseudoRapidity()); //<--- Value does NOT make sense
	    pointPhiError->Fill(point->positionError().phi());  //<--- Value does NOT make sense


	    // cout << "--------> Energy: "<< point->energy() <<endl;
	    // cout << "--------> Position: "<< point->position().x() <<"\t" << point->position().y() <<"\t" << point->position().z() <<endl;
	    // cout << "--------> Point eta:"<< point->position().pseudoRapidity()<<"\t"<<"phi:"<< point->position().phi() <<endl;
	    // cout << "--------> Point delta_eta:"<< point->size().x()<<"\t"<<"delta_phi:"<< point->size().y() <<endl;

	    // cout << "--------> Position Error: "<< point->positionError().x() <<"\t" << point->positionError().y() <<"\t" << point->positionError().z() <<endl;
	    // cout << "--------> Point eta error:"<< point->positionError().pseudoRapidity()<<"\t"<<"phi error:"<< point->positionError().phi() <<endl;
	    
	    // //--------- The following fields are not set and do NOT provide sensible value ----------------
	    // cout << "--------->Detector: "<<point->detector() <<endl;
	    // cout << "--------->Quality: "<<point->quality() <<endl;
	    // cout << "--------->id: "<<point->id()<<" charge:"<<point->charge() <<endl;
	    // cout << "--------->deltaEta: "<<point->deltaEta()<<" deltaPhi:"<<point->deltaPhi() <<endl;
	    // cout << "--------->num of tracks: "<<point->nTracks()<<endl;
	    // // cout << "--------->print: "<<point->print() <<endl; // This will make it crash at some point

	    // StPtrVecTrack& tr = point->track();
	    // cout <<"------->num of tracks: " << tr.size() <<endl;
	    
	}
	if(evt % 1000 == 0)
	    cout << "Events processed:" << evt <<endl;
	chain->Clear();
    }

    file->Write();
    file->Close();
    chain->Finish();
}

