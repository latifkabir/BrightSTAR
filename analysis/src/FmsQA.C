void FmsQA()
{
    gROOT->ProcessLine(".x /afs/rhic.bnl.gov/star/packages/SL19b/StRoot/macros/loadMuDst.C"); // Calls the libraries.
    TFile *f = new TFile("FmsQA.root","recreate"); //Creates a root file that will hold the histograms.
	
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0,0,"","resources/temp/FmsFileList.list",""); //Opens the STAR data to be used.
    StMuDst *mDst =  mMaker->muDst();  //Get the Tree from MuDst.
    TChain *tree = mMaker->chain();
	
    TClonesArray *array = new TClonesArray("StMuFmsHit",10000); //Create an array from StMuFmsHit.
    StMuFmsHit *hit;                          // Look at StMuFmsHit documentaion.    
    TBranch *br = tree->GetBranch("FmsHit");  //Get the FmsHit branch.
    tree->SetBranchAddress("FmsHit",&array);  // Setting the branch address.

    int iEvent = 0; // Event number.
    int channel; 
    int detID;
    const oMaxCh = 600; 
    TH1F *adcDist[oMaxCh]; //Initilize the 1D histogram.
    
    for (int l = 0; l < oMaxCh; l++) //Use a for loop to create histograms for each channel.
    {
	TString title = "adcDist";
	title += l;        
	adcDist[l] = new TH1F(title,title,300,0.0,500); //Creating 1D histograms for each channel.
    }   

    while(br->GetEntry(iEvent++))
    {
	int nHits = array->GetEntriesFast();
	for(int j = 0; j < nHits; j++) //For loop to fill each channel histogram.
	{	     
	    hit = (StMuFmsHit*) array->At(j); //Look at documentaion for hit.
	    detID = hit->detectorId();        //Gets detector ID for each event.
	    channel = hit->channel();         //Gets channel for each event. 288 for small/oMaxCh for large.
	    if (detID == 8)                   //Only take data from detector 8.
		adcDist[channel]->Fill(hit->adc()); //fill the histogram with adc data.
	} 
    }
    f->cd();
    for(int k = 0; k < oMaxCh; k++)
	adcDist[k]->Write(); //write each histograms.

    f->Close();    
}			
	
