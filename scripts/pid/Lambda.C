// Filename: Lambda.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep 29 00:03:19 2019 (-0400)
// URL: jlab.org/~latif


void Lambda()
{
    gSystem->Load("PidTreeReader_cxx.so");

    TFile *file = new TFile("~/GIT/BrightSTAR/results/root/RunPidTreeMaker/RunPidTreeMaker_16072057.root");
    TTree *t = (TTree*)file->Get("T");
    PidTreeReader *pid = new PidTreeReader(t);
    //pid->Show(1);
    TLorentzVector LVp; // Plus particle
    TLorentzVector LVm; //Minus partice
    TLorentzVector LV;
    Double_t pi_m = 0.13957; //GeV/c^2
    Double_t pr_m = 0.93827; //GeV/c^2
    Int_t prp = -1; //Proton plus
    Int_t pim = -1; //Pion minus
    Int_t nEvents = t->GetEntries();
    TH1D *hist = new TH1D("hist", "hist; M_{p#pi^{-}} [GeV/c^{2}]", 200.0, 0.9, 1.4);
    cout << "Total events to be analyzed: "<< nEvents <<endl;

    for(Int_t evt = 0; evt < nEvents; ++evt)
    {
	pid->GetEntry(evt);
	if(evt % 1000 == 0)
	    cout << "Events analyzed: "<<evt<<endl;

	for(Int_t ntrk1 = 0; ntrk1 < pid->prN; ++ntrk1)
	{
	    if(pid->prQ[ntrk1] == 1)
		prp = ntrk1;
	    else
		continue;
	    
	    for(Int_t ntrk2 = 0; ntrk2 < pid->piN; ++ntrk2)
	    {
		if(pid->piQ[ntrk2] == -1)
		    pim = ntrk2;
		else 
		    continue;

		LVp.SetXYZM(pid->prP[prp][0], pid->prP[prp][1], pid->prP[prp][2], pr_m);
		LVm.SetXYZM(pid->piP[pim][0], pid->piP[pim][1], pid->piP[pim][2], pi_m);

		LV = (LVp + LVm);
		hist->Fill(LV.M());
	    }   
	}
    }

    hist->Draw();    
}
