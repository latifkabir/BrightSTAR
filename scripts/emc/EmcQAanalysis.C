// Filename: EmcQAanalysis.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Sep  2 18:48:47 2019 (-0400)
// URL: jlab.org/~latif

void EmcQAanalysis()
{
    gSystem->Load("EmcTreeReader_cxx.so");

    const Int_t size = 32;
    const Int_t size_r = size;
    Int_t runList[size] = {16072006, 16072007, 16072008, 16072009, 16072010, 16072012, 16072013, 16072014, 16072021, 16072022, 16072023, 16072024, 16072025, 16072026, 16072033, 16072034, 16072035, 16072036, 16072038, 16072039, 16072040, 16072041, 16072042, 16072043, 16072046, 16072047, 16072057, 16072058, 16072059, 16072060, 16072061, 16072062};
        
    TString fPath = "/star/u/kabir/GIT/BrightSTAR/dst/RunEmcTreeMaker/RunEmcTreeMaker_";

    TChain *chain = new TChain("T");
    cout << "Total files to be added:"<< size_r <<" out of "<< size <<endl;
    for(Int_t i = 0; i < size_r; ++i)
    {
	TString fileName = fPath + Form("%i", runList[i]) + ".root/T";
	chain->Add(fileName);
    }
    
    cout << "Total events: " << chain->GetEntries() <<endl;
    
    EmcTreeReader * emc = new EmcTreeReader(chain);

    Int_t nEvents = emc->fChain->GetEntries();
    cout << "Total Entries: "<<nEvents<<endl;

    // TH1D *hist = new TH1D("hist", "hist", 6, 0, 6);
    TH2D *hist_p = new TH2D("hist_p", "hist_p", 100, 0.0, 30.0, 100, 0.0, 30.0);
    TH2D *hist_p2 = new TH2D("hist_p2", "hist_p2", 100, 0.0, 30.0, 100, 0.0, 30.0);
    TH2D *hist_ratio = new TH2D("hist_r", "hist_r", 100, 0.0, 30.0, 100, 0.0, 2.0);
    TH2D *hist_beta = new TH2D("hist_beta", "hist_beta", 100, 0.0, 30.0, 100, 0.7, 1.2);
    
    for(Int_t evt = 0; evt < nEvents; ++evt)
    {
	if(evt % 1000 == 0)
	    cout<< "Events proessed: "<< evt << endl;
	
	emc->GetEntry(evt);
	//hist_p->Fill(emc->point);

	Int_t C1 = 0;
	Int_t Q1;
	Int_t C2 = 0;
	Int_t Q2;
		
	for(Int_t k = 0; k < emc->point; ++k)
	{
	    if(emc->point_nTracks[k] != 1)
		continue;

	    hist_p2->Fill(emc->pid_trait_p[k][0], emc->point_E[k]);
	    
	    if(emc->point_E[k] < 3.0)
		continue;

	    if(emc->pid_trait_p[k][0] < 3.0)
		continue;
			       			
	    hist_p->Fill(emc->pid_trait_p[k][0], emc->point_E[k]);			    
	    hist_ratio->Fill(emc->pid_trait_p[k][0], emc->point_E[k] / emc->pid_trait_p[k][0]);		     hist_beta->Fill(emc->pid_trait_p[k][0], emc->pid_trait_beta[k][0]);			    	    
	}	
    }
    
    TCanvas *c1 = new TCanvas();
    hist_p->Draw("colz");

    TCanvas *c2 = new TCanvas();
    hist_ratio->Draw("colz");

    TCanvas *c3 = new TCanvas();
    hist_beta->Draw("colz");

    TCanvas *c4 = new TCanvas();
    hist_p2->Draw("colz");
    
}

