// Filename: PipPiM.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep 29 00:03:19 2019 (-0400)
// URL: jlab.org/~latif

// Can provide Rho, K_S^0, etc 

#include "PidTreeReader.h"
#include "RootInclude.h"
#include <iostream>

using namespace std;

void PidPiPpiM()
{
    TFile *file = new TFile("~/GIT/BrightSTAR/dst/RunPidTreeMaker/RunPidTreeMaker_16072057.root");
    TTree *t = (TTree*)file->Get("T");
    PidTreeReader *pid = new PidTreeReader(t);
    //pid->Show(1);
    TLorentzVector LVp;
    TLorentzVector LVm;
    TLorentzVector LV;
    Double_t pi_m = 0.13957; //GeV/c^2
    Int_t pip = -1;
    Int_t pim = -1;
    Int_t nEvents = t->GetEntries();
    TH1D *hist = new TH1D("hist", "hist; M_{#pi^{+}#pi^{-}} [GeV/c^{2}]", 200.0, 0.2, 1.0);
    cout << "Total events to be analyzed: "<< nEvents <<endl;

    for(Int_t evt = 0; evt < nEvents; ++evt)
    {
	pid->GetEntry(evt);
	//---> Check if the max particle number exceeds the array size in the buffer <---
	if(evt % 1000 == 0)
	    cout << "Events analyzed: "<<evt<<endl;

	for(Int_t npi1 = 0; npi1 < pid->piN; ++npi1)
	{
	    if(pid->piQ[npi1] == 1)
		pip = npi1;
	    else
		continue;
	    
	    for(Int_t npi2 = 0; npi2 < pid->piN; ++npi2)
	    {
		if(pid->piQ[npi2] == -1)
		    pim = npi2;
		else 
		    continue;

		LVp.SetXYZM(pid->piP[pip][0], pid->piP[pip][1], pid->piP[pip][2], pi_m);
		LVm.SetXYZM(pid->piP[pim][0], pid->piP[pim][1], pid->piP[pim][2], pi_m);

		LV = (LVp + LVm);
		hist->Fill(LV.M());
	    }   
	}
    }

    hist->Draw();    
}
