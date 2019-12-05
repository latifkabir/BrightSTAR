// Filename: AnEEmcRpChain.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Dec  4 13:09:18 2019 (-0500)
// URL: jlab.org/~latif

void AnEEmcRpChain()
{
    TString filePath_eemc = "/star/u/kabir/GIT/BrightSTAR/dst/R15EEmcRpTree/eemcTreeP3_EEmcRpTree_160900*.root";
    TChain *ch_eemc = new TChain("tree");
    ch_eemc->Add(filePath_eemc);
    // //ch_eemc->MakeClass("AnEEmcRpChainReader");
    cout <<"EEmc Chain Entries: "<< ch_eemc->GetEntries()<<endl;

    // TString filePath_rp = "/star/u/kabir/GIT/BrightSTAR/dst/R15EEmcRpTree/RpTree_*.root";
    // TChain *ch_rp = new TChain("T");
    // ch_rp->Add(filePath_rp);
    // cout <<"RpChain Entries: "<< ch_rp->GetEntries()<<endl;

    // ch_eemc->AddFriend(ch_rp);

    // ch_eemc->MakeClass("AnEEmcRpChainReader");

    // TString fName = "/star/u/kabir/GIT/BrightSTAR/dst/R15EEmcRpTree/eemcTreeP1_EEmcRpTree_16093017.root";
    // TChain *chain =  new TChain("tree");
    // chain->Add(fName);
    // chain->MakeClass("AnEEmcTreePart1Reader");

    TFile *file = new TFile("EEmcQA.root", "RECREATE");
    TClonesArray *array_ph = new TClonesArray("EEmcParticleCandidate_t", 100);
    TClonesArray *array_pi = new TClonesArray("EEmc2ParticleCandidate_t", 100);
    //TBranch *brPion = ch_eemc->GetBranch("pi0");
    ch_eemc->SetBranchAddress("photon", &array_ph);
    ch_eemc->SetBranchAddress("pi0", &array_pi);

    TH1F *phE = new TH1F("phE","phE", 100, 0, 100.0);
    TH1F *pi0M = new TH1F("pi0M","pi0M", 100, 0, 1.0);
    TH1F *pi0E = new TH1F("pi0E","pi0E", 100, 0, 100.0);
    TH1F *pi0Z = new TH1F("pi0Z","pi0Z", 100, 0, 1.0);
    TH1F *pi0D = new TH1F("pi0D","pi0D", 100, 0, 0.0);
    TH2D *pi0Pos = new TH2D("pi0Pos", "pi0Pos", 250, -250, 250, 250, -250, 250);
    
    for(Int_t iEvent= 0; iEvent < ch_eemc->GetEntries(); ++iEvent)
    {
	// brPion->GetEntry(iEvent);
	 ch_eemc->GetEntry(iEvent);
	 //cout << "Entry:" << iEvent <<endl;

	 
	int nPhotons = array_ph->GetEntriesFast();
	for(int i = 0; i < nPhotons; i++)
	{
	    EEmcParticleCandidate_t *photon = (EEmcParticleCandidate_t*) array_ph->At(i);

	    phE->Fill(photon->E);
	}
	 
	int nPions = array_pi->GetEntriesFast();

	for(int i = 0; i < nPions; i++)
	{
	    //EEmc2ParticleCandidate_t is a derived class of EEmcarticleCandidate_t
	    EEmc2ParticleCandidate_t *pion = (EEmc2ParticleCandidate_t*) array_pi->At(i);

	    pi0M->Fill(pion->M);
	    //pi0E->Fill(pion->momentum.X());
	    pi0E->Fill(pion->E);
	    pi0Z->Fill(pion->Z);
	    pi0D->Fill(pion->D);
	    pi0Pos->Fill(pion->position.X(), pion->position.Y());
	}


	
    }

    file->Write();
}
