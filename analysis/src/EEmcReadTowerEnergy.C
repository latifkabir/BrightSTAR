void ReadEEmcEnergyTree(){

    // gSystem->Load("StEEmcPoolEEmcTreeContainers"); 
    // gSystem->Load("StEEmcTreeMaker");
    
    TString filePath_eemc = "/star/u/ananyapaul/Analysis/EEmcAnalysis/BrightSTAR/MuDstP1P2P3/eemcTreeP1_16090026.root";
    TChain *tr_eemc = new TChain("tree");
    tr_eemc->Add(filePath_eemc);
    cout<<"EEmc Chain Entries:"<<tr_eemc->GetEntries()<<endl;

    tr_eemc->Print();
    
    EEmcEnergy_t *eemcE = new EEmcEnergy_t(); 
    tr_eemc->SetBranchAddress("eemcEnergy", &eemcE);		

    //Declare histograms here
    TH1D *h1eemcTowE = new TH1D("h1eemcTowE", "h1eemcTowE", 100, 0, 0);
    TH1D *h1eemcSmdE = new TH1D("h1eemcSmdE", "h1eemcSmdE", 100, 0, 0);
    

    ETowEnergy_t eTow;
    ETowEnergy_t ePre1;
    ETowEnergy_t ePre2;
    ETowEnergy_t ePost;
    ESmdEnergy_t eSmd;
    UInt_t nTowers, nStrips;
    

    const int Entries = tr_eemc->GetEntries();

    for(Int_t iEvent=0; iEvent<Entries; iEvent++)
    {
	tr_eemc->GetEntry(iEvent);
	
	if(iEvent % 1000 == 0)
	    cout<<" Events processed = "<<iEvent<<endl;

	eTow = eemcE->eTow;
	ePre1 = eemcE->ePre1;
	ePre2 = eemcE->ePre2;
	ePre2 = eemcE->ePost;
	eSmd = eemcE->eSmd;
	nTowers = eemcE->nTowers;
	nStrips = eemcE->nStrips;

	for(int i = 0; i < 720; ++i)
	    h1eemcTowE->Fill(eTow.getByIdx(i).energy);	           
	
    }

    h1eemcTowE->Draw();
 

}
