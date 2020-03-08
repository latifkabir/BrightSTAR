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
#include "StEvent/StEvent.h"
#include "StChain/StChain.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuTofHit.h"
#include "TStar.h"
#include "TStRunList.h"

/*

This script do not work. As the data is not fille in the MuDST.
People generally loop over TPC primary tracks and get corresponding BtofPidTrait and
then extrack relevant beta, tof, pathlength.

*/


void TofData(TString fileList, TString outFile)
{
    StChain *chain = new StChain;
    StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);       

    // muDstMaker->SetStatus("*", 0);
    // muDstMaker->SetStatus("MuEvent", 1);
    // muDstMaker->SetStatus("*Track*", 1);
    // muDstMaker->SetStatus("*Vert*", 1);
    
    StMuDst *mDst = muDstMaker->muDst();  
    
    TFile *file = new TFile(outFile, "RECREATE"); 
    TH1D *hist1 = new TH1D("tof", "TOF", 100, 0.0, 0.0);
    TH1D *hist2 = new TH1D("beta", "Beta", 100, 0.0, 0.0);
    TH1D *hist3 = new TH1D("pathLength", "Path Length", 100, 0.0, 0.0);
    TH1D *hist4 = new TH1D("eta", "Eta", 100, 0.0, 0.0);
    TH1D *hist5 = new TH1D("phi", "Phi", 100, 0.0, 0.0);
        
    chain->Init();
    Int_t nEvents = 1000;//muDstMaker->chain()->GetEntries();
    StMuTofHit *tofHit;
    Int_t cellIndex;
    Float_t beta;
    Float_t pathLength;
    Float_t tof;
    StThreeVectorF position;
    
    cout << "Total events to be processed: "<< nEvents <<endl;
	
    for (int iev = 0; iev < nEvents; iev++)
    {
	chain->Clear();
	int iret = chain->Make(iev); 

	for(Int_t t = 0; t < mDst->numberOfTofHit(); ++t) //<--------- this infrmation is not available in MuDST. So the size is zero.
	{
	    tofHit = mDst->tofHit(t);
	    //For all available field variables accessible for StMuTofHit, see class reference
	    
	    cellIndex = tofHit->cellIndex();
	    tof = tofHit->timeOfFlight();
	    pathLength = tofHit->pathLength();
	    beta = tofHit->beta();
	    position = tofHit->projectedPoint();

	    hist1->Fill(tof);
	    hist2->Fill(beta);
	    hist3->Fill(pathLength);
	    hist4->Fill(position.pseudoRapidity());
	    hist5->Fill(position.phi());
	}
		
	if(iev % 100 == 0)
	    cout << "Events processed: "<<iev <<endl;
    } 
    chain->Finish();

    file->Write();
    file->Close();
    delete chain;
}
