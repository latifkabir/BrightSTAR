// Filename: AnFmsPhotonAngDist.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Jul 16 17:35:56 2020 (-0400)
// URL: jlab.org/~latif


#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"

class FmsPhotonAngDistMaker: public StMaker
{
    StMuDst *mDst;   // Get StMuDst
    StEvent* event;
    StFmsDbMaker* mFmsDbMk;
    TString name;
    StFmsCollection *fmsColl;    
    StMuFmsHit *fmsHit;
    StMuFmsCluster *fmsCluster;
    StMuFmsPoint *fmsPoint;
    Int_t nPoints;
    Double_t pointEta;
    Double_t pointPhi;
    Double_t x, y;
    TH1D *hist1d[3];
    TH2D *hist2d;
    
public:
    FmsPhotonAngDistMaker(TString name): StMaker(name)
    {	
	Init();
    }    
    ~FmsPhotonAngDistMaker()
    {
	
    }
    Int_t Init()
    {
    	mFmsDbMk = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
	if (!mFmsDbMk) { LOG_ERROR <<"TStFmsTreeMaker::InitRun - !StFmsDbMaker" <<endl; return kStFatal; }

	return kStOk;	
    }
    void Set1dHist(TH1D *h1d, Int_t index)
    {
	hist1d[index] = h1d;	
    }
    void Set2dHist(TH2D *h2d)
    {
	hist2d = h2d;	
    }
    Int_t Make()
    {
	mDst = (StMuDst*)GetInputDS("MuDst");
	event = (StEvent*)GetInputDS("StEvent");	

	if (!mDst || !event)
	{
	    cout <<"TStFmsTreeMaker::Make - !MuDst or !StEvent" <<endl;
	    return kStErr;
	}
	
	fmsColl = (StFmsCollection*)event->fmsCollection();

	if(!fmsColl)
	{
	    cout<<"No FMS data for this event"<<endl;
	    return kStSkip;
	}
	
	StSPtrVecFmsPoint& points = fmsColl->points();	    
        nPoints = points.size();
	for (Int_t i = 0; i < nPoints; ++i)
	{
	    StFmsPoint* point = points[i];
	    pointEta = point->XYZ().pseudoRapidity();
	    pointPhi = point->XYZ().phi();	      
	    x = point->XYZ().x();
	    y = point->XYZ().y();
	    
	    hist1d[0]->Fill(pointEta);
	    hist1d[1]->Fill(pointPhi);
	    hist1d[2]->Fill(sqrt(x*x + y*y));
	    
	    hist2d->Fill(x, y);
	}
	return kStOk;
    }
};			

void AnFmsPhotonAngDist(TString fileList, TString outName)
{
    // TStRunList::MakeFileList(16072057, 1);
    // TString fileList = TStar::Config->GetFileList();

    TFile *f = new TFile(outName, "RECREATE");
    TH1D *etaDist = new TH1D("etaDist","Eta Dist; #eta", 100, 2, 4.5);
    TH1D *phiDist = new TH1D("phiDist","Phi Dist; #phi [rad]",100, -3.2, 3.2);
    TH1D *radDist = new TH1D("radDist","Radial Dist; r [cm]", 15, 20.0, 115.0);
    TH2D *xyDist = new TH2D("xyDist", "xyDist", 200, -100.0, 100.0, 200, -100.0, 100.0);
    
    StChain *chain = new StChain;
    StMuDstMaker *muDstMaker = new StMuDstMaker(0, 0, "", fileList, "", 1000);
    St_db_Maker* dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
    StFmsDbMaker* fmsDb = new StFmsDbMaker("fmsDb");

    fmsDb->SetAttr("fmsGainCorr","fmsGainCorr-BNL-C");
    Bool_t isHotCh[4][571] = {0};
    TStFmsHotChDB *fmsHotChDb = new TStFmsHotChDB();

    Int_t runNumber = TStRunList::GetRunFromFileName((string)fileList);
    if(runNumber < 1)
    {
    	cout << "Unable to get run number" <<endl;
    	return;
    }
    
    fmsHotChDb->GetHotChList(runNumber, isHotCh);
    cout << "The following FMS cells are masked:" <<endl;
    for(int i = 0; i < 4; ++i)
    {
	for(int j = 0; j < 571; ++j)
	    if(isHotCh[i][j])
		cout << "det "<< (i + 1)<< " ch "<< (j+1) << " hot/bad status:"<< isHotCh[i][j] <<endl;
    }
    fmsDb->maskChannels(isHotCh);
    
    muDstMaker->SetStatus("*", 0);
    muDstMaker->SetStatus("Fms*", 1);
    muDstMaker->SetStatus("MuEvent*", 1);
    StEventMaker* eventMk = new StEventMaker();
    StFmsHitMaker*   fmsHitMk   = new StFmsHitMaker();
    StFmsPointMaker* fmsPointMk = new StFmsPointMaker();
    // FPS
    StFmsFpsMaker * fpsMk = new StFmsFpsMaker();
    fpsMk->setReadMuDST(1);      // (1) // (reverse 0<->1 convention as FmsHitMaker and FmsPointMaker)
    
    FmsPhotonAngDistMaker *phMkr = new FmsPhotonAngDistMaker("Pi0Maker");
    phMkr->Set1dHist(etaDist, 0);
    phMkr->Set1dHist(phiDist, 1);
    phMkr->Set1dHist(radDist, 2);
    phMkr->Set2dHist(xyDist);
    
    chain->Init();    
    chain->EventLoop();  // Run specified number of events
    chain->Finish();

    // TCanvas *c1 = new TCanvas();
    // etaDist->Draw();
    // TCanvas *c2 = new TCanvas();
    // xyDist->Draw("colz");
    f->Write();
}


