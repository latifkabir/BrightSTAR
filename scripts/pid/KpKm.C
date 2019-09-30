// Filename: KpKm.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep 29 00:03:19 2019 (-0400)
// URL: jlab.org/~latif


void KpKm()
{
    gSystem->Load("PidTreeReader_cxx.so");

    TFile *file = new TFile("~/GIT/BrightSTAR/results/root/RunPidTreeMaker/RunPidTreeMaker_16072057.root");
    TTree *t = (TTree*)file->Get("T");
    PidTreeReader *pid = new PidTreeReader(t);
    //pid->Show(1);
    TLorentzVector LVp;
    TLorentzVector LVm;
    TLorentzVector LV;
    Double_t ka_m = 0.493677; //GeV/c^2
    Int_t kap = -1;
    Int_t kam = -1;
    Int_t nEvents = t->GetEntries();
    TH1D *hist = new TH1D("hist", "hist; M_{K^{+}K^{-}} [GeV/c^{2}]", 200.0, 0.9, 1.4);
    cout << "Total events to be analyzed: "<< nEvents <<endl;

    for(Int_t evt = 0; evt < nEvents; ++evt)
    {
	pid->GetEntry(evt);
	if(evt % 1000 == 0)
	    cout << "Events analyzed: "<<evt<<endl;

	for(Int_t nka1 = 0; nka1 < pid->kaN; ++nka1)
	{
	    if(pid->kaQ[nka1] == 1)
		kap = nka1;
	    else
		continue;
	    
	    for(Int_t nka2 = 0; nka2 < pid->kaN; ++nka2)
	    {
		if(pid->kaQ[nka2] == -1)
		    kam = nka2;
		else 
		    continue;

		LVp.SetXYZM(pid->kaP[kap][0], pid->kaP[kap][1], pid->kaP[kap][2], ka_m);
		LVm.SetXYZM(pid->kaP[kam][0], pid->kaP[kam][1], pid->kaP[kam][2], ka_m);

		LV = (LVp + LVm);
		hist->Fill(LV.M());
	    }   
	}
    }

    hist->Draw();    
}
