// Filename: ExAnalysis3.C
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Sun May 12 16:19:03 2019 (-0400)
// URL: jlab.org/~latif

#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TObjArray.h>


void TpcVertex(TString fileList = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/093/16093018/st_physics_16093018_raw_1000005.MuDst.root", TString outFile = "TpCVertex.root")
{
    StChain *chain = new StChain;
    StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);       
    
    StMuDst *mDst = muDstMaker->muDst();  
    StMuEvent *ev = 0;
    StMuPrimaryVertex *pVtx;

    
    TFile *file = new TFile(outFile, "RECREATE"); 
    TH1D *hNgTrk = new TH1D("hNgTrk", "Number of Global Tracks Per Event; No. of Global Tracks Per Event", 200, 0, 2000);
    TH1D *hNpTrk = new TH1D("hNpTrk", "Number of Primary Tracks Per Event; No. of Primary Tracks Per Event", 100, 0, 100);
    TH1D *hNvtx = new TH1D("hNvtx", "Number of Primary Vertices Per Event; No. of Primary Vertices Per Event", 20, 0, 10);
    TH1D *hNvtxTof = new TH1D("hNvtxTof", "Number of Primary Vertices with TOF Match Per Event; No. of Primary Vertices Per Event", 20, 0, 10);

    
    chain->Init();
    Int_t nEvents = muDstMaker->chain()->GetEntries();
    Int_t nTracks;
    Int_t nVtxTof;
    
    cout << "Total events to be processed: "<< nEvents <<endl;
	
    for (int iev = 0; iev < nEvents; iev++)
    {
	if(iev % 100 == 0)
	    cout << "Events processed: "<<iev <<endl;
	
	chain->Clear();
	chain->Make(iev); 

	hNgTrk->Fill(mDst->numberOfGlobalTracks());
	hNpTrk->Fill(mDst->numberOfPrimaryTracks());
	hNvtx->Fill(mDst->numberOfPrimaryVertices());
	
	nVtxTof = 0;
	for(Int_t i = 0; i < mDst->numberOfPrimaryVertices(); ++i)
	{
	    pVtx = mDst->primaryVertex(i);
	    if(!pVtx)
		continue;
	    if(pVtx->nBTOFMatch () > 0)
		++nVtxTof;
	}
	hNvtxTof->Fill(nVtxTof);
	
	// Vertex multiplicity cut for CEP Analysis 
	if(nVtxTof != 1)  
	    continue;

	//Process Your event here

	
    } 
    chain->Finish();

    file->Write();
    file->Close();
    delete chain;
}
