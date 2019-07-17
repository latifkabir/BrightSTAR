void FmsChannelStatus()
{
    gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    loadSharedLibraries();
    TFile *f = new TFile("FmsQA.root","recreate"); //Creates a root file that will hold the histograms.
	
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0,0,"","resources/temp/FmsFileList.list","", 43); //Opens the STAR data to be used.
    //connect to STAR FMS database
    St_db_Maker *stDb = new St_db_Maker("StarDb", "MySQL:StarDb");    
    StFmsDbMaker *fmsDBMaker = new StFmsDbMaker("FmsDbMk");
    fmsDBMaker->Init();
    chain->Init();
    chain->EventLoop(1);
    
    StMuDst *mDst =  mMaker->muDst();  //Get the Tree from MuDst.
    TChain *tree = mMaker->chain();
	
    TClonesArray *array = new TClonesArray("StMuFmsHit",10000); //Create an array from StMuFmsHit.
    StMuFmsHit *hit;                          // Look at StMuFmsHit documentaion.    
    TBranch *br = tree->GetBranch("FmsHit");  //Get the FmsHit branch.
    tree->SetBranchAddress("FmsHit",&array);  // Setting the branch address.

    int iEvent = 0; // Event number.
    int channel; 
    int detID;
    const oMaxCh = 571; 
    const iMaxCh = 288;
    TH1F *adcDist[4][oMaxCh]; //Initilize the 1D histogram.
    
    for(int i = 0; i < 4; ++i)
    {
	int MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (int l = 0; l < MaxCh; l++) 
	{
	    TString title = "adcDist_";
	    title += (i + 8);        
	    title += "_";        
	    title += (l + 1);        
	    adcDist[i][l] = new TH1F(title, title, 300, 0.0, 500); //Creating 1D histograms for each channel.
	}   
    }

    //while(br->GetEntry(iEvent++))
    for(int evt = 0; evt < tree->GetEntries(); ++evt)
    {
	br->GetEntry(evt);
	int nHits = array->GetEntriesFast();

	for(int j = 0; j < nHits; j++) //For loop to fill each channel histogram.
	{	     
	    hit = (StMuFmsHit*) array->At(j); //Look at documentaion for hit.
	    detID = hit->detectorId();        //Gets detector ID for each event.
	    channel = hit->channel();         //Gets channel for each event. 288 for small/oMaxCh for large.
	    if(detID >= 8 && detID <= 11)     // Exclude detector IDs corresponding to FPOST, FPS, FPD etc.
		adcDist[detID - 8][channel - 1]->Fill(hit->adc()); //fill the histogram with adc data. Note: channel number starts from 1.	    
	}
    }

    chain->Init();
    chain->EventLoop(1);
    f->cd();
    
    for(int i = 0; i < 4; ++i)
    {
	int MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (int l = 0; l < MaxCh; l++)
	{
	    adcDist[i][l]->Write();
	    if(adcDist[i][l]->GetEntries() == 0 && fmsDBMaker->getGain(i + 8, l + 1) != 0.0)
		cout << "Dead channel:: det id:" << (i + 8) << " channel:" << (l + 1)<<endl;
	    else if(adcDist[i][l]->GetEntries() < 100 && fmsDBMaker->getGain(i + 8, l + 1) != 0.0)
		cout << "Bad channel:: det id:" << (i + 8) << " channel:" << (l + 1)<<endl;
	}
    }
    f->Close();    
}			
	
