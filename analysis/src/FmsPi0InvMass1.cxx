// Filename: FmsPi0InvMass.C
// Description: Pi0 (two photons) Invariant Mass directly from MuDST files.
// Author: Latif Kabir < latif@jlab.org >
// Created: Mon May 20 15:04:13 2019 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "StRootInclude.h"
#include "BrightStInclude.h"

void FmsPi0InvMass1(TString fileList)
{
    // TStRunList::MakeFileList(16072047, 3);
    // TString fileList = TStar::Config->GetFileList();
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "");
    muDstMaker->SetStatus("*",0);
    muDstMaker->SetStatus("Fms*",1);
    
    StMuDst *mDst = muDstMaker->muDst();   // Get StMuDst
    StMuFmsCollection *fmsMuColl;
    StMuFmsHit *fmsHit;
    StMuFmsCluster *fmsCluster;
    StMuFmsPoint *fmsPoint1;
    StMuFmsPoint *fmsPoint2;
    
    TChain *ch = muDstMaker->chain();
    TH1D *massDist = new TH1D("massDist", "massDist", 200, 0.0, 1.0);
    TH2D *xyDist = new TH2D("xyDist", "xyDist", 300, -3.0, 3.0, 300, -3.0, 3.0);
    Double_t pairE;
    Double_t pairM;
    Double_t zgg;
    StLorentzVectorF LV;
    StLorentzVectorF LV1;
    StLorentzVectorF LV2;
    Int_t nPoints;
    Double_t pi0_x;
    Double_t pi0_y;

    chain->Init();
    
    Int_t nEvents = ch->GetEntries(); 
    
    for (Int_t iev = 0; iev < nEvents; iev++)
    {
	chain->Clear();
	Int_t iret = chain->Make(iev);
	fmsMuColl = mDst->muFmsCollection();

	if(!fmsMuColl)
	{
	    cout<<"No FMS data for this event"<<endl;
	    continue;
	}

	nPoints = fmsMuColl->numberOfPoints();
	for(Int_t i = 0; i < nPoints; ++i)
	{
	    for(Int_t j = i + 1; j < nPoints; ++j)
	    {
		if(j>=nPoints)
		    continue;
		fmsPoint1 = fmsMuColl->getPoint(i);
		fmsPoint2 = fmsMuColl->getPoint(j);
		LV1 = fmsPoint1->fourMomentum();
		LV2 = fmsPoint2->fourMomentum();
		zgg = fabs(LV1.e() - LV2.e()) / (LV1.e() + LV2.e());		
		LV = LV1 + LV2;
		pairM = LV.m();
		pairE = LV.e();
		pi0_x = (LV1.e()*LV1.x() + LV2.e()*LV2.x()) / (LV1.e() + LV2.e()); //From StFmsPointPair code
		pi0_y = (LV1.e()*LV1.y() + LV2.e()*LV2.y()) / (LV1.e() + LV2.e()); 
		
		if (pairE < 20.0)
		    continue;
		if (pairM  > 1.0)
		    continue;
		if (zgg > 0.7)
		    continue;
		    
		massDist->Fill(pairM);
		xyDist->Fill(pi0_x, pi0_y); //Position of pi0 in FMS coordinate. You need to convert to STAR coordinate using DB to get expected distribution.
	    }	
	}	
    }
    chain->Finish();
    TCanvas *c1 = new TCanvas();
    massDist->Draw();
    TCanvas *c2 = new TCanvas();
    xyDist->Draw("colz");
}			
