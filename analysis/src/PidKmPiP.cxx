// Filename: KpKm.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep 29 00:03:19 2019 (-0400)
// URL: jlab.org/~latif

// Can give D0 meson
// Need lot more statistics to see the peak
#include "PidTreeReader.h"
#include "RootInclude.h"
#include <iostream>

using namespace std;

void PidKmPiP()
{
    gSystem->Load("PidTreeReader_cxx.so");

    TFile *file = new TFile("~/GIT/BrightSTAR/dst/RunPidTreeMaker/RunPidTreeMaker_16072057.root");
    TTree *t = (TTree*)file->Get("T");
    PidTreeReader *pid = new PidTreeReader(t);
    //pid->Show(1);
    TLorentzVector LVp; //Positive particle
    TLorentzVector LVm; //Negative particle
    TLorentzVector LV;
    Double_t ka_m = 0.493677; //GeV/c^2
    Double_t pi_m = 0.13957; //GeV/c^2
    Int_t pip = -1;
    Int_t kam = -1;
    Int_t nEvents = t->GetEntries();
    TH1D *hist = new TH1D("hist", "hist; M_{K^{-}#pi^{+}} [GeV/c^{2}]", 200.0, 1.5, 2.5);
    cout << "Total events to be analyzed: "<< nEvents <<endl;

    for(Int_t evt = 0; evt < nEvents; ++evt)
    {
	pid->GetEntry(evt);
	if(evt % 1000 == 0)
	    cout << "Events analyzed: "<<evt<<endl;

	for(Int_t nka1 = 0; nka1 < pid->kaN; ++nka1)
	{
	    if(pid->kaQ[nka1] == -1)
		kam = nka1;
	    else
		continue;
	    
	    for(Int_t npi2 = 0; npi2 < pid->piN; ++npi2)
	    {
		if(pid->piQ[npi2] == 1)
		    pip = npi2;
		else 
		    continue;

		LVp.SetXYZM(pid->piP[pip][0], pid->piP[pip][1], pid->piP[pip][2], pi_m);
		LVm.SetXYZM(pid->kaP[kam][0], pid->kaP[kam][1], pid->kaP[kam][2], ka_m);

		LV = (LVp + LVm);
		hist->Fill(LV.M());
	    }   
	}
    }

    hist->Draw();    
}
