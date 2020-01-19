// Filename: RpStreamDstQA.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Jan 15 18:47:02 2020 (-0500)
// URL: jlab.org/~latif

void RpStreamDstQA()
{
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList();
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    TString fileName;
    TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/R15RpStream/AnRunAnTreeMaker_";

    //Histograms
    TFile *outFile = new TFile("AnRpsStreamDstQA.root", "RECREATE");
    TH1D *hist1West = new TH1D("trkPwest", "West RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2West = new TH1D("trkPtWest", "West RP trk Pt; RP track P_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3West = new TH1D("trkEtaWest", "West RP trk Eta; RP Track #Eta", 200, -10, 10);
    TH1D *hist4West = new TH1D("trkPhiWest", "West RP trk Phi; RP track #phi [mrad]", 200, -3, 3);
    TH1D *hist5West = new TH1D("trkXiWest", "West RP trk Xi; RP track #xi", 200, 0, 0);

    TH1D *hist1East = new TH1D("trkPeast", "East RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist2East = new TH1D("trkPtEast", "East RP trk Pt; RP track P_{T} [GeV/c]", 200, 0, 2);
    TH1D *hist3East = new TH1D("trkEtaEast", "East RP trk Eta; RP Track #eta", 200, -10, 10);
    TH1D *hist4East = new TH1D("trkPhiEast", "East RP trk Phi; RP track #phi [mrad]", 200, -3, 3);
    TH1D *hist5East = new TH1D("trkXiEast", "East RP trk Xi; RP track #xi", 200, 0, 0);
    
    TH1D *hist9 = new TH1D("trkP", " RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *hist10 = new TH1D("trkPt", " RP trk Pt; RP track Pt [GeV/c]", 200, 0, 2);

    TH1D *hist6 = new TH1D("pi0M", "Diphoton mass with highest energy pair; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);
    TH1D *hist11 = new TH1D("pi0M", "Diphoton mass; M_{#gamma#gamma} [GeV/c^{2}]", 200, 0, 1.0);
    TH1D *hist12 = new TH1D("pi0E", "Diphoton Energy; E_{#gamma#gamma} [GeV]", 200, 10, 100);
    TH2D *hist2d5 = new TH2D("pionXY", "#pi^{0} position; X [cm]; Y[cm]", 100, -100, 100, 100, -100, 100);
    TH2D *hist2d6 = new TH2D("pionXY", "#pi^{0} position with highest energy pair; X [cm]; Y[cm]", 100, -100, 100, 100, -100, 100);
    
    TH1D* hist13 = new TH1D("tofMult", "TOF Multiplicity", 250, 0, 500);
    TH1D* hist14 = new TH1D("bbcSumL_west", "West Large BBC ADC Sum", 250, 0, 8000);
    TH1D* hist15 = new TH1D("bbcSumS_west", "West Small BBC ADC Sum", 250, 0, 45000);
    TH1D* hist16 = new TH1D("trigger", "Trigger Distribution", 1000, 480000, 481000);
    
    
    //Input data     
    TTree *tree;
    Int_t nEntries = 0;
    TClonesArray *fmsArr = new TClonesArray("TStFmsPointPairData");
    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");
    TStEventData *event = new TStEventData();
    TStFmsPointPairData *pion;
    TStRpsTrackData *rpsTrack;
    Int_t nPions = 0;
    Int_t nRpsTracks = 0;
        
    cout << "Total number of files to be processed: "<< maxRuns <<endl;

    //------------ Loop over runs --------------------------
    for (Int_t run_i = 0; run_i < maxRuns; ++run_i)
    {
	run = runList->GetEntry(run_i);
	fileName = filePrefix;
	fileName += run;
	fileName += ".root";

	if(gSystem->AccessPathName(fileName))
	{
	    cout << "No DST file for run number "<< run <<" ... SKIPPED."<<endl;
	    continue;
	}
	cout << "Processing run number: "<< run <<endl;
	cout << "Percent completed: "<< (Int_t)(nRunsDone*100.0 / maxRuns) << "%"<<endl;
	
	TFile *file = new TFile(fileName);
	tree = (TTree*)file->Get("T");
	nEntries = tree->GetEntries();
	cout << "Total number of entries: "<< nEntries <<endl;

	tree->SetBranchAddress("event", &event);
	tree->SetBranchAddress("fmsPointPair", &fmsArr);
	tree->SetBranchAddress("rpTrack", &rpsArr);
	//----------- Loop over Events ---------------------
	for(Int_t evt = 0; evt < nEntries; ++evt)
	{
	    // fmsArr->Clear(); //This is automatically called unless you turn off auto delete. 
	    // rpsArr->Clear(); //This is automatically called unless you turn off auto delete.
	    // Also Clear Event  buffer if it contains TClonesArray as member

	    tree->GetEntry(evt);

	    //----- Event Info ---------
	    hist13->Fill(event->mTofMultiplicity);
	    hist14->Fill(event->mBbcADCSumLarge[1]); //West Large BBC
	    hist15->Fill(event->mBbcADCSum[1]);      //West small BBC

	    for(Int_t t = 0; t < event->mNtrig; ++t)
		hist16->Fill(event->mTriggers[t]);
	    
	    //----- RP Tracks ----------
	    nRpsTracks = rpsArr->GetEntriesFast();	    
	    for(Int_t trk = 0; trk < nRpsTracks; ++trk)
	    {
		rpsTrack = (TStRpsTrackData*)rpsArr->At(trk);

		hist9->Fill(rpsTrack->mP);
		hist10->Fill(rpsTrack->mPt);
		
		if(rpsTrack->mBranch == 0 || rpsTrack->mBranch == 1) //East RP :: O East Up RP, 1: East Down RP
		{
		    // ++nTrkEast;
		    // eastTrk_i = t;
		
		    hist1East->Fill(rpsTrack->mP);		
		    hist2East->Fill(rpsTrack->mPt);
		    hist3East->Fill(rpsTrack->mEta);
		    hist4East->Fill(rpsTrack->mPhi);
		    hist5East->Fill(rpsTrack->mXi);		
		}

		if(rpsTrack->mBranch == 2 || rpsTrack->mBranch == 3) //West RP :: 2: West Up RP, 3: West Down RP
		{
		    // ++nTrkWest;
		    // westTrk_i = t;

		    hist1West->Fill(rpsTrack->mP);		
		    hist2West->Fill(rpsTrack->mPt);
		    hist3West->Fill(rpsTrack->mEta);
		    hist4West->Fill(rpsTrack->mPhi);
		    hist5West->Fill(rpsTrack->mXi);
		}
		
	    }
	    //---------- FMS Pion candidates ---------------
	    nPions = fmsArr->GetEntriesFast();
	    for(Int_t pi = 0; pi < nPions; ++pi)
	    {
		pion = (TStFmsPointPairData)fmsArr->At(pi);

		if(pion->mZgg > 0.8)
		    continue;
	    
		if(pion->mE < 12 || pion->mE > 70)
		    continue;

		hist11->Fill(pion->mM);
		hist12->Fill(pion->mE);
		hist2d5->Fill(pion->mX, pion->mY);

		if(pi == 0)
		{
		    hist6->Fill(pion->mM);
		    hist2d6->Fill(pion->mX, pion->mY);
		}
	    }
	}
		
	//-----------
	++nRunsDone;
	file->Close();
	delete file;

	// ------- For testing ---------
	if(nRunsDone > 0)
	    break;
    }

    outFile->cd();
    outFile->Write();
}
