// Filename: EmcJpsiAnalysis.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Sep  2 18:48:47 2019 (-0400)
// URL: jlab.org/~latif

void EmcJpsiAnalysis()
{
    gSystem->Load("EmcTreeReader_cxx.so");

    const Int_t size = 32;
    Int_t runList[size] = {16072006, 16072007, 16072008, 16072009, 16072010, 16072012, 16072013, 16072014, 16072021, 16072022, 16072023, 16072024, 16072025, 16072026, 16072033, 16072034, 16072035, 16072036, 16072038, 16072039, 16072040, 16072041, 16072042, 16072043, 16072046, 16072047, 16072057, 16072058, 16072059, 16072060, 16072061, 16072062};
        
    TString fPath = "/star/u/kabir/GIT/BrightSTAR/results/root/RunEmcTreeMaker/RunEmcTreeMaker_";

    TChain *chain = new TChain("T");
    cout << "Total files to be added:"<< size <<endl;
    for(Int_t i = 0; i < size; ++i)
    {
	TString fileName = fPath + Form("%i", runList[i]) + ".root/T";
	chain->Add(fileName);
    }
    
    cout << "Total events: " << chain->GetEntries() <<endl;
    
    EmcTreeReader * emc = new EmcTreeReader(chain);

    Int_t nEvents = emc->fChain->GetEntries();
    cout << "Total Entries: "<<nEvents<<endl;

    // TH1D *hist = new TH1D("hist", "hist", 6, 0, 6);
    TH1D *hist = new TH1D("hist", "hist", 200, 0.0, 1.0);

    TLorentzVector LV1;
    TLorentzVector LV2;
    TVector3 V1;
    TVector3 V2;
    Double_t mass;
    Double_t pt;
    Double_t dgg;
    Double_t zgg;
    Double_t theta;

    
    for(Int_t evt = 0; evt < nEvents; ++evt)
    {
	if(evt % 5000 == 0)
	    cout<< "Events proessed: "<< evt << endl;
	
	emc->GetEntry(evt);
	//hist->Fill(emc->point);

	Int_t C1 = 0;
	Int_t Q1;
	Int_t C2 = 0;
	Int_t Q2;

		
	for(Int_t k = 0; k < emc->point; ++k)
	{
	    if(emc->point_nTracks[k] != 1)
		continue;
	    
	    for(Int_t l = k + 1; l < emc->point; ++l)
	    {
		if(l >= emc->point)
		    continue;
		if(emc->point_nTracks[l] != 1)
		    continue;

		if(!((emc->pid_trait_q[k][0] == 1 && emc->pid_trait_q[l][0] == -1) || (emc->pid_trait_q[k][0] == -1 && emc->pid_trait_q[l][0] == 1)))
		    continue;

		// if( emc->point_E[k] / emc->pid_trait_p[k][0] > 1.5 ||  emc->point_E[k] / emc->pid_trait_p[k][0] < 0.5)
		//     continue;
		
		// if( emc->point_E[l] / emc->pid_trait_p[l][0] > 1.5 ||  emc->point_E[l] / emc->pid_trait_p[l][0] < 0.5)
		//     continue;

		//Pion cut
		 if(emc->point_E[k] > 3.0 ||  emc->pid_trait_p[k][0] > 3.0)
		     continue;

		// if(emc->point_E[l] > 3.0 ||  emc->pid_trait_p[l][0] > 3.0)
		//     continue;

		
		// if(emc->pid_trait_beta[k][0] == -999 || emc->pid_trait_beta[l][0] == -999)
		//   continue;
		
		// if(!(emc->pid_trait_beta[k][0] > 0.95 && emc->pid_trait_beta[l][0] > 0.95))
		//   continue;
		
		// Q1 = emc->point_quality[k];
		// Q2 = emc->point_quality[l];

		// if( (Q1&1) )
		//     C1 = 1; // only tower
		// if( (Q1&1) && (Q1&4))
		//     C1 = 2; // tower + smd eta
		// if( (Q1&1) && (Q1&8))
		//     C1 = 3; // tower + smd phi
		// if( (Q1&1) && (Q1&4) && (Q1&8))
		//     C1 = 4; // tower + smd eta + smd phi

		// if( (Q2&1) )
		//     C2 = 1; // only tower
		// if( (Q2&1) && (Q2&4))
		//     C2 = 2; // tower + smd eta
		// if( (Q2&1) && (Q2&8))
		//     C2 = 3; // tower + smd phi
		// if( (Q2&1) && (Q2&4) && (Q2&8))
		//    C2 = 4; // tower + smd eta + smd phi

		// if(!(C1 == 4 && C2 == 4))
		//     continue;
		
		LV1.SetPxPyPzE(emc->point_px[k], emc->point_py[k], emc->point_pz[k], emc->point_E[k]);
		LV2.SetPxPyPzE(emc->point_px[l], emc->point_py[l], emc->point_pz[l], emc->point_E[l]);
		V1.SetXYZ(emc->point_x[k], emc->point_y[k], emc->point_z[k]);
		V2.SetXYZ(emc->point_x[l], emc->point_y[l], emc->point_z[l]);
		
		mass = (LV1+LV2).M();
		//pt = (LV1+LV2).Pt();
		hist->Fill(mass);		
	    }
	}
	
    }
    
    TCanvas *c3 = new TCanvas();
    hist->Draw();
}

