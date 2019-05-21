// Filename: FMSdist.C
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Mon May 20 15:04:13 2019 (-0400)
// URL: jlab.org/~latif

void FMSdist()
{
    StChain *chain = new StChain;
    //StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", "/star/u/kabir/pwg/data/st_physics_18053099_raw_0000004.MuDst.root", "");
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", "resources/fileList.list", "");
    TTree * tree = muDstMaker->tree();
    //TChain * tree = muDstMaker->chain();
    TClonesArray *array = new TClonesArray("StMuFmsHit",10000);
    StMuFmsHit *hit;
    
    // tree->SetBranchStatus("*",0);
    // tree->SetBranchStatus("FmsHit*",1);
    TBranch *br = tree->GetBranch("FmsHit");   // Select only FMS branch for quick access
    tree->SetBranchAddress("FmsHit",&array);	
    TH1F *adcDist = new TH1F("adcDist","adcDist",100,0.0,0.0);
    int iEvent = 0;

    chain->Init();
    while( chain->Make(iEvent++) )     // Doing tree->GetEntry(iEvent++) will be super slow
    {
	int nHits = array->GetEntriesFast();
	for(int i = 0; i < nHits; i++)
	{	     
	    hit = (StMuFmsHit*) array->At(i);
	    adcDist->Fill(hit->detectorId());
	} 
    }
    chain->Finish();
    adcDist->Draw();					
}			
