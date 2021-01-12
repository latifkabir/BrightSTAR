// Filename: ReadDst.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Apr 27 02:00:44 2020 (-0400)
// URL: jlab.org/~latif

void ReadDst()
{
    gSystem->Load("BrContainers.so");

   
    TChain *ch = new TChain("T");
    ch->Add("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/R15CepNanoDst/CepNanoDstMaker_16064079.root");
     //Add other files here or can use wildcat in the file name
    
    TFile *file = new TFile("cepDstQA.root", "recreate");
    TH1D *h_nVertices = new TH1D("nVertices", "Number of vertices", 20, 0, 20);
    TH1D *h_VtxZ = new TH1D("vtxZ", "vertex z", 100, -200, 200);
    TH1D *h_nTracks = new TH1D("nTracks", "Number of RP tracks", 20, 0, 20);
    TH1D *h_westP = new TH1D("trkPwest", "West RP trk P; RP track P [GeV/c]", 200, 60, 150);
    TH1D *h_westPt = new TH1D("trkPtWest", "West RP trk Pt; RP track P_{T} [GeV/c]", 200, 0, 2);

    TH1D *h_nPions = new TH1D("nPions", "Number of pions", 10, 0, 10);
    TH1D *h_pionPt = new TH1D("pionPt", "Pt of pions", 50, 0, 5);
    
    TClonesArray *rpsArr = new TClonesArray("TStRpsTrackData");
    TClonesArray *emcArr = new TClonesArray("TStEmcPointData");
    TClonesArray *eArr = new TClonesArray("TStChargedPidData");
    TClonesArray *piArr = new TClonesArray("TStChargedPidData");
    TClonesArray *prArr = new TClonesArray("TStChargedPidData");
    TClonesArray *kaArr = new TClonesArray("TStChargedPidData");
    TClonesArray *ukArr = new TClonesArray("TStChargedPidData");

    TStEventData *event = new TStEventData();

    ch->SetBranchAddress("event", &event);
    ch->SetBranchAddress("rpTrack", &rpsArr);
    ch->SetBranchAddress("emcPoint", &emcArr);
    ch->SetBranchAddress("e", &eArr);   //electrons
    ch->SetBranchAddress("pi", &piArr); // pions
    ch->SetBranchAddress("pr", &prArr); // protons
    ch->SetBranchAddress("ka", &kaArr); // kaons
    ch->SetBranchAddress("uk", &ukArr); // unknown particles

    TStRpsTrackData *rpsTrack;
    Int_t nRpsTracks;   
    TStEmcPointData *emcPhoton;
    TStChargedPidData *chargedPart;
    Int_t nChargedPart;

    
    Int_t nEntries = ch->GetEntries(); //Number of events

    //Loap over all events
    for(Int_t evt = 0; evt < nEntries; ++evt)
    {
	ch->GetEntry(evt); //Load event

	//--------- Read Event Information ------------------
	h_nVertices->Fill(event->GetNvertices());
	h_VtxZ->Fill(event->GetVz());

	//Check TStEventData.h for all available Get() methods
	
	//----------- Read RP Information --------------------------
	nRpsTracks = rpsArr->GetEntriesFast();
	h_nTracks->Fill(nRpsTracks);
	for(Int_t trk = 0; trk < nRpsTracks; ++trk)
	{
	    rpsTrack = (TStRpsTrackData*)rpsArr->At(trk);

	    if(rpsTrack->GetBranch() == 2 || rpsTrack->GetBranch() == 3) //East RP :: O East Up RP, 1: East Down RP
	    {		    
		h_westP->Fill(rpsTrack->GetP());		
		h_westPt->Fill(rpsTrack->GetPt());
		
		//Check TStRpsTrackData.h file for all available Get() methods 		
	    }	    
	}


	//------------- Read Charged Particle Information ---------------------------
	//Read pion info
	nChargedPart = piArr->GetEntriesFast();
	h_nPions->Fill(nChargedPart);	
	for(Int_t pi = 0; pi < nChargedPart; ++pi)
	{
	    chargedPart = (TStChargedPidData*) piArr->At(pi);
	    h_pionPt->Fill(chargedPart->GetPt());
	   //Check TStChargedPidData.h file for all available Get() methods 			    
	}

	//Access other charged particles (pr, e, ka) in the same way
	
    }
    
    file->Write();
}
