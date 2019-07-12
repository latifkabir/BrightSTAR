void FmsQA()
{
    gROOT->ProcessLine(".x /afs/rhic.bnl.gov/star/packages/SL19b/StRoot/macros/loadMuDst.C"); // Calls the libraries.
    TFile *f = new TFile("FmsQA.root","recreate"); //Creates a root file that will hold the histograms.
	
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0,0,"","resources/temp/FmsFileList.list","", 10000); //Opens the STAR data to be used.
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
	    TString title = "adcDist:";
	    title += (i + 8);        
	    title += "-";        
	    title += l;        
	    adcDist[i][l] = new TH1F(title, title, 300, 0.0, 500); //Creating 1D histograms for each channel.
	}   
    }
    int maxx[4] = {0};    
    while(br->GetEntry(iEvent++))
    {
	int nHits = array->GetEntriesFast();

	for(int j = 0; j < nHits; j++) //For loop to fill each channel histogram.
	{	     
	    hit = (StMuFmsHit*) array->At(j); //Look at documentaion for hit.
	    detID = hit->detectorId();        //Gets detector ID for each event.
	    channel = hit->channel();         //Gets channel for each event. 288 for small/oMaxCh for large.
	    if(detID >= 8 && detID <= 11)     // Exclude detector IDs corresponding to FPOST, FPS, FPD etc.
	    {
		adcDist[detID - 8][channel - 1]->Fill(hit->adc()); //fill the histogram with adc data. Note: channel number starts from 1.
		if(channel > maxx[detID - 8])
		    maxx[detID - 8] = channel;
	    }
	}
    }

    f->cd();
    
    for(int i = 0; i < 4; ++i)
    {
	int MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (int l = 0; l < MaxCh; l++) 	
	    adcDist[i][l]->Write();	   
    }
    f->Close();    
}			
	
