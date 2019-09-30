// Filename: ePeM.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep 29 00:03:19 2019 (-0400)
// URL: jlab.org/~latif


void ePeM()
{
    gSystem->Load("PidTreeReader_cxx.so");

    TFile *file = new TFile("~/GIT/BrightSTAR/results/root/RunPidTreeMaker/RunPidTreeMaker_16072057.root");
    TTree *t = (TTree*)file->Get("T");
    PidTreeReader *pid = new PidTreeReader(t);
    //pid->Show(1);
    TLorentzVector LVp; // Plus particle
    TLorentzVector LVm; //Minus partice
    TLorentzVector LV;
    Double_t e_m = 0.000510; //GeV/c^2
    Int_t ep = -1; //Proton plus
    Int_t em = -1; //Pion minus
    Int_t nEvents = t->GetEntries();
    TH1D *hist = new TH1D("hist", "hist; M_{e^{+}e^{-}} [GeV/c^{2}]", 160.0, 0.0, 0.08);
    cout << "Total events to be analyzed: "<< nEvents <<endl;

    for(Int_t evt = 0; evt < nEvents; ++evt)
    {
	pid->GetEntry(evt);
	if(evt % 1000 == 0)
	    cout << "Events analyzed: "<<evt<<endl;

	for(Int_t ntrk1 = 0; ntrk1 < pid->eN; ++ntrk1)
	{
	    if(pid->eQ[ntrk1] == 1)
		ep = ntrk1;
	    else
		continue;
	    
	    for(Int_t ntrk2 = 0; ntrk2 < pid->eN; ++ntrk2)
	    {
		if(pid->eQ[ntrk2] == -1)
		    em = ntrk2;
		else 
		    continue;

		LVp.SetXYZM(pid->eP[ep][0], pid->eP[ep][1], pid->eP[ep][2], e_m);
		LVm.SetXYZM(pid->eP[em][0], pid->eP[em][1], pid->eP[em][2], e_m);

		LV = (LVp + LVm);
		hist->Fill(LV.M());
	    }   
	}
    }

    hist->Draw();    
}
