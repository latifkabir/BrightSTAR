// Filename: EmcPi0Analysis.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Sep  2 18:48:47 2019 (-0400)
// URL: jlab.org/~latif

void EmcPi0Analysis()
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

    TH1D *hist0 = new TH1D("hist", "hist", 200, 0, 1.0);
    TH1D *hist = new TH1D("eta", "hist", 200, 0.0, 1.2);
    TH2D *hist2d = new TH2D("hist2d", "hist2d", 315, 0, 3.14, 120, 0, 1.2);
    TH1D *hist1 = new TH1D("hist1", "hist1", 120, 0, 1.2);
    TH1D *hist2 = new TH1D("hist2", "hist2", 120, 0, 1.2);
    TH1D *hist3 = new TH1D("hist3", "hist3", 120, 0, 1.2);
    TH1D *hist4 = new TH1D("hist4", "hist4", 120, 0, 1.2);
    TH1D *hist5 = new TH1D("hist5", "hist5", 120, 0, 1.2);
    TH1D *hist6 = new TH1D("hist6", "hist6", 120, 0, 1.2);
    TH1D *hist7 = new TH1D("hist7", "hist7", 120, 0, 1.2);
    TH1D *hist8 = new TH1D("hist8", "hist8", 120, 0, 1.2);
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
	if(evt % 1000 == 0)
	    cout<<"Events processed: "<< evt<<endl;
	
	emc->GetEntry(evt);	
	//hist->Fill(emc->point);

	Int_t C1 = 0;
	Int_t Q1;
	Int_t C2 = 0;
	Int_t Q2;

	//------------ Point Analysis -------------------
		
	// for(Int_t k = 0; k < emc->point; ++k)
	// {
	//     if(emc->point_nTracks[k] > 0)
	// 	continue;
	//     Q1 = emc->point_quality[k];
	//     //cout << "Q1:"<<Q1 <<endl;

	//     if( (Q1&1) )
	// 	C1 = 1; // only tower
	//     if( (Q1&1) && (Q1&4))
	// 	C1 = 2; // tower + smd eta
	//     if( (Q1&1) && (Q1&8))
	// 	C1 = 3; // tower + smd phi
	//     if( (Q1&1) && (Q1&4) && (Q1&8))
	// 	C1 = 4; // tower + smd eta + smd phi
	//     hist0->Fill(C1);
	// }
		
		
	for(Int_t k = 0; k < emc->point; ++k)
	{
	    if(emc->point_nTracks[k] > 0)
		continue;

	    //eta threshold cut
	    if(emc->point_E[k] < 3.5)
		continue;
	    
	    for(Int_t l = k + 1; l < emc->point; ++l)
	    {
		if(l >= emc->point)
		    continue;
		
		if(emc->point_nTracks[l] > 0)
		    continue;

		//eta threshold cut
		if(emc->point_E[l] < 3.5)
		    continue;
		
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
		//     C2 = 4; // tower + smd eta + smd phi

		// if(!(C1 == 4 && C2 == 4))
		//     continue;
		
		LV1.SetPxPyPzE(emc->point_px[k], emc->point_py[k], emc->point_pz[k], emc->point_E[k]);
		LV2.SetPxPyPzE(emc->point_px[l], emc->point_py[l], emc->point_pz[l], emc->point_E[l]);
		V1.SetXYZ(emc->point_x[k], emc->point_y[k], emc->point_z[k]);
		V2.SetXYZ(emc->point_x[l], emc->point_y[l], emc->point_z[l]);
		
		mass = (LV1+LV2).M();
		pt = (LV1+LV2).Pt();

		dgg = (V1 - V2).Mag();
		zgg = fabs(LV1.E() - LV2.E()) / (LV1.E() + LV2.E());
		//theta = V1.Angle(V2);
		theta = TMath::RadToDeg()*std::atan2(V1.Cross(V2), V1.Dot(V2));
		
		// if( dgg > 100)
		//     continue;

		// if(theta  > 0.1)
		//     continue;
		
		// if( zgg > 0.7)
		//     continue;		

		hist0->Fill(zgg);
		hist->Fill(mass);
		// hist2d->Fill(theta, mass);
		// if(pt >0 && pt < 1.0)
		//     hist1->Fill(mass);
		// else if(pt > 1.0 && pt< 2.0)
		//     hist2->Fill(mass);
		// else if(pt > 2.0 && pt< 3.0)
		//     hist3->Fill(mass);
		// else if(pt > 3.0 && pt< 4.0)
		//     hist4->Fill(mass);
		// else if(pt > 4.0 && pt< 5.0)
		//     hist5->Fill(mass);
		// else if(pt > 5.0 && pt< 6.0)
		//     hist6->Fill(mass);
		// else if(pt > 6.0 && pt< 7.0)
		//     hist7->Fill(mass);
		// else if(pt > 7.0 && pt< 8.0)
		//     hist8->Fill(mass);
	    }
	}
	
	//---------- Pion Analysis ---------------------
	/*
	for(Int_t i = 0; i < emc->pi0; ++i)
	{
	    // LV.SetPxPyPzE(emc->pi0_px[i], emc->pi0_py[i], emc->pi0_pz[i], emc->pi0_E[i]);
	    // cout << emc->pi0_px[i] <<"\t" << emc->pi0_E[i] <<endl;
	    // cout << "LV M: "<<LV.M()<<endl;

	    if(emc->pi0_nTracks1[i] > 0 || emc->pi0_nTracks2[i] > 0)
		continue;
	    if(emc->pi0_zgg[i] > 0.7)
		continue;
	    // if(emc->pi0_theta[i] > 0.5)
	    // 	continue;
	    if(emc->pi0_E[i] < 2.0)
		continue;
	    
	    Q1 = emc->pi0_Q1[i];
	    Q2 = emc->pi0_Q2[i];

	    if( (Q1&1) )
		C1 = 1; // only tower
	    if( (Q1&1) && (Q1&4))
		C1 = 2; // tower + smd eta
	    if( (Q1&1) && (Q1&8))
		C1 = 3; // tower + smd phi
	    if( (Q1&1) && (Q1&4) && (Q1&8))
		C1 = 4; // tower + smd eta + smd phi

	    if( (Q2&1) )
		C2 = 1; // only tower
	    if( (Q2&1) && (Q2&4))
		C2 = 2; // tower + smd eta
	    if( (Q2&1) && (Q2&8))
		C2 = 3; // tower + smd phi
	    if( (Q2&1) && (Q2&4) && (Q2&8))
		C2 = 4; // tower + smd eta + smd phi

	    if(C1 == 4 && C2 == 4)
		//hist->Fill(4);	    
	    hist->Fill(emc->pi0_M[i]);
	}	
	*/
    }
    
    // TCanvas *c1 = new TCanvas();
    // c1->Divide(2, 2);
    // c1->cd(1);
    // hist1->Draw();
    // c1->cd(2);
    // hist2->Draw();
    // c1->cd(3);
    // hist3->Draw();
    // c1->cd(4);
    // hist4->Draw();

    // TCanvas *c2 = new TCanvas();
    // c2->Divide(2, 2);
    // c2->cd(1);
    // hist5->Draw();
    // c2->cd(2);
    // hist6->Draw();
    // c2->cd(3);
    // hist7->Draw();
    // c2->cd(4);
    // hist8->Draw();
    
    TCanvas *c3 = new TCanvas();
    hist->Draw();

    TCanvas *c4 = new TCanvas();
    // //hist2d->Draw("colz");
    hist0->Draw();


    TFile *file = new TFile("eta.root", "RECREATE");
    hist->Write();
    file->Close();
}

//M_{#pi^{0}} [GeV/c^{2}]
