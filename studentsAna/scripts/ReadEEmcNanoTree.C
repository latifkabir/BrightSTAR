// Filename: ReadEEmcNanoTree.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Dec  4 13:09:18 2019 (-0500)
// URL: jlab.org/~latif

void ReadEEmcNanoTree()
{
    TString filePath_eemc = "/gpfs/mnt/gpfs01/star/pwg/kabir/dst/EEmcNanoDst/Run15trans/RunEEmcNanoDstMaker_*.root";
    TChain *ch_eemc = new TChain("T");
    ch_eemc->Add(filePath_eemc);
    cout <<"EEmc Chain Entries: "<< ch_eemc->GetEntries()<<endl;


    TFile *file = new TFile("EEmcNanoTreeQA.root", "RECREATE");
    TStEventData *event = new TStEventData;
    TClonesArray *array_ph = new TClonesArray("EEmcParticleCandidate_t");
    TClonesArray *array_pi = new TClonesArray("EEmc2ParticleCandidate_t");
    ch_eemc->SetBranchAddress("event", &event);
    ch_eemc->SetBranchAddress("eemcPhoton", &array_ph);
    ch_eemc->SetBranchAddress("eemcPion", &array_pi);

    TH1F *phE = new TH1F("phE","phE", 100, 0, 100.0);
    TH1F *pi0M = new TH1F("pi0M","pi0M", 100, 0, 1.0);
    TH1F *pi0E = new TH1F("pi0E","pi0E", 100, 0, 100.0);
    TH1F *pi0Z = new TH1F("pi0Z","pi0Z", 100, 0, 1.0);
    TH1F *pi0D = new TH1F("pi0D","pi0D", 100, 0, 0.0);
    TH2D *pi0Pos = new TH2D("pi0Pos", "pi0Pos", 250, -250, 250, 250, -250, 250);
    
    for(Int_t iEvent= 0; iEvent < ch_eemc->GetEntries(); ++iEvent)
    {
	if(iEvent % 1000 == 0)
	    cout << "Events processed: "<< iEvent <<endl;

	 ch_eemc->GetEntry(iEvent);

	 //Access Event information
	 // cout << "Run number: " << event->GetRunNumber() <<endl;
	 //cout << "Fill number: " << event->GetFillNumber() <<endl;

	 
	int nPhotons = array_ph->GetEntriesFast();
	for(int i = 0; i < nPhotons; i++)
	{
	    EEmcParticleCandidate_t *photon = (EEmcParticleCandidate_t*) array_ph->At(i);

	    phE->Fill(photon->E);
	}
	 
	int nPions = array_pi->GetEntriesFast();

	for(int i = 0; i < nPions; i++)
	{
	    EEmc2ParticleCandidate_t *pion = (EEmc2ParticleCandidate_t*) array_pi->At(i);

	    pi0M->Fill(pion->M);
	    pi0E->Fill(pion->E);
	    pi0Z->Fill(pion->Z);
	    pi0D->Fill(pion->D);
	    pi0Pos->Fill(pion->position.X(), pion->position.Y());
	}	
    }

    file->Write();
}
