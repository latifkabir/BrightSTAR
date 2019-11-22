// Filename: AnFmsRpQA.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Nov 21 13:00:15 2019 (-0500)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "TAnFmsRpTreeReader.h"
#include "TStRunList.h"
using namespace std;

void AnFmsRpQA(Int_t lastRun)
{
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList();
    Int_t nRuns = runList->GetN();
    if (lastRun == -1)
	lastRun = runList->GetEntry(nRuns -1);
    Int_t run;
    TString basePath = "/star/u/kabir/GIT/BrightSTAR/dst/FmsRpTreeMaker_ucr/RunFmsRpTreeMaker_";
    
    Int_t nEntries = 0;
    Int_t trk_i[2] = { -1, -1};
    Int_t nTrk = 0;

    Bool_t isValid = true;
    Int_t evtCount = 0;

    TFile *fout = new TFile("FmsRpQA.root", "RECREATE");
    TH1D *hist1 = new TH1D("trkP", "trk P", 200, 60, 150);
    TH1D *hist2 = new TH1D("trkPt", "trk Pt", 200, 0, 10);
    TH1D *hist3 = new TH1D("trkEta", "trk Eta", 200, -10, 10);
    TH1D *hist4 = new TH1D("trkPhi", "trk Phi", 200, -4, 4);
    TH1D *hist5 = new TH1D("trkXi", "trk Xi", 200, 0, 0);

    for(Int_t r = 0; r < nRuns; ++r)
    {
	run = runList->GetEntry(r);

	//For now do have only one third of total runs
	if(run > lastRun)
	    break;
	
	TString fileName;
	fileName += basePath;
	fileName += run;
	 fileName += ".root";
	if(gSystem->AccessPathName(fileName))
	    continue;

	cout << "Processing run# "<< run << endl;
	
	TChain *chain = new TChain("T");
	chain->Add(fileName);
	TAnFmsRpTreeReader *reader = new TAnFmsRpTreeReader(chain);
	nEntries = chain->GetEntries();
	for(Int_t evt = 0; evt < nEntries; ++evt)
	{
	    reader->GetEntry(evt);
	    //reader->Show();

	    //------------------- RP Cuts --------------------------------
	    trk_i[0] = -1;
	    trk_i[1] = -1;
	    nTrk = 0;
	    isValid = true;
	    for(Int_t t = 0; t < reader->rp_nTracks; ++t)
	    {
		if (reader->rp_trackType[t] != 1)
		    continue;
		if(!(reader->rp_trackTheta_x[t] > -1.5 && reader->rp_trackTheta_x[t] < 5.0) ) 
		    continue;
		if( !(fabs(reader->rp_trackTheta_y[t]) > 1.0 &&  fabs(reader->rp_trackTheta_y[t]) < 5.5) )
		    continue;
		if(reader->rp_trackNplanes[t] < 7)
		    continue;

		++nTrk;
		trk_i[nTrk - 1] = t;
		if(nTrk > 2)
		{
		    trk_i[0] = -1;
		    trk_i[1] = -1;
		    isValid = false;
		    break;
		}
	    }

	    if(!(nTrk > 0 && nTrk <=2 && isValid))
		continue;

	    for(Int_t trk = 0; trk < nTrk; ++trk)
	    {
		hist1->Fill(reader->rp_trackP[trk_i[trk]]);
		hist2->Fill(reader->rp_trackPt[trk_i[trk]]);
		hist3->Fill(reader->rp_trackEta[trk_i[trk]]);
		hist4->Fill(reader->rp_trackPhi[trk_i[trk]]);
		hist5->Fill(reader->rp_trackXi[trk_i[trk]]);		
	    }

	    ++evtCount;

	    if(evt%1000 == 0)
		cout << "Events processed in this run:"<< (int)(100.0*evt / nEntries) <<" %"<<endl;
	}

	//delete chain;
	//delete reader;
    }
    cout<<"Total events passed the cut:" << evtCount <<endl; 
    
    fout->Write();
    //fout->Close();
}
