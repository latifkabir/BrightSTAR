#include "StRootInclude.h"
#include "RootInclude.h"
#include "cppInclude.h"

using namespace std;

void FmsQA(TString fileList, TString outFileName)
{
    // No need for compiled code and if you load the library from rootlogon.C
    // gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    // loadSharedLibraries();

    struct DetChPair
    {
	Int_t det;
	Int_t ch;
    } det_ch;
    
    TFile *f = new TFile(outFileName,"recreate"); //Creates a root file that will hold the histograms.
	
    StChain *chain = new StChain;
    StMuDstMaker *mMaker = new StMuDstMaker(0,0,"", fileList, "", 10000); //Opens the STAR data to be used.
    //connect to STAR FMS database
    St_db_Maker *stDb = new St_db_Maker("StarDb", "MySQL:StarDb");    
    StFmsDbMaker *fmsDBMaker = new StFmsDbMaker("FmsDbMk");
    fmsDBMaker->setDebug(0);
    fmsDBMaker->Init();
    
    StMuDst *mDst =  mMaker->muDst();  //Get the Tree from MuDst.
    TChain *ch = mMaker->chain();
    ch->SetBranchStatus("*", 0);
    ch->SetBranchStatus("Fms*", 1);
    
    TClonesArray *array = new TClonesArray("StMuFmsHit",10000); //Create an array from StMuFmsHit.
    StMuFmsHit *hit;                          // Look at StMuFmsHit documentaion.
    TBranch *br = ch->GetBranch("FmsHit");  //Get the FmsHit branch.
    ch->SetBranchAddress("FmsHit",&array);  // Setting the branch address.

    Int_t iEvent = 0; // Event number.
    Int_t channel; 
    Int_t detID;
    const Int_t oMaxCh = 571; 
    const Int_t iMaxCh = 288;
    TH1F *adcDist[4][oMaxCh]; //Initilize the 1D histogram.

    vector < DetChPair > deadChList;
    vector < DetChPair > badChList;
    vector < DetChPair > hotChList;
    vector < DetChPair > bitShChList;
    
    for(Int_t i = 0; i < 4; ++i)
    {
	Int_t MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (Int_t l = 0; l < MaxCh; l++) 
	{
	    TString title = "adcDist_";
	    title += (i + 8);        
	    title += "_";        
	    title += (l +1);        
	    adcDist[i][l] = new TH1F(title, title, 300, 0.0, 500); //Creating 1D histograms for each channel.
	}   
    }

    Int_t nEntries = ch->GetEntries();  
    cout << "Total events to be processed: "<< nEntries <<endl;

    while(ch->GetEntry(iEvent++))
    {
	if(iEvent % 1000 == 0)
	    cout << "Events processed:"<< iEvent <<endl;
	
	Int_t nHits = array->GetEntriesFast();

	for(Int_t j = 0; j < nHits; j++) //For loop to fill each channel histogram.
	{	     
	    hit = (StMuFmsHit*) array->At(j); //Look at documentaion for hit.
	    detID = hit->detectorId();        //Gets detector ID for each event.
	    channel = hit->channel();         //Gets channel for each event. 288 for small/oMaxCh for large.
	    if(detID >= 8 && detID <= 11)     // Exclude detector IDs corresponding to FPOST, FPS, FPD etc.
		adcDist[detID - 8][channel - 1]->Fill(hit->adc()); //fill the histogram with adc data. Note: channel number starts from 1.	    
	}
    }

    //The following 3 lines were moved here to avoid crate map printout for every event. This block is  necessary to initialize BD maker
    ch->SetBranchStatus("MuEvent*", 1);
    chain->Init();
    chain->EventLoop(1);
    //Check if database is working fine
    cout << "------->Max channel for det 8:"<< fmsDBMaker->maxChannel(8) <<endl;
    cout  <<"------->Max channel for det 11:"<< fmsDBMaker->maxChannel(11) <<endl;

    f->cd();
    
    for(Int_t i = 0; i < 4; ++i)
    {
	Int_t MaxCh;
	if(i == 0 || i == 1)
	    MaxCh = oMaxCh;
	else
	    MaxCh = iMaxCh;
	for (Int_t l = 0; l < MaxCh; l++)
	{
	    if(fmsDBMaker->getGain(i + 8, l + 1) != 0.0)
		adcDist[i][l]->Write();
	    if(adcDist[i][l]->GetEntries() == 0 && fmsDBMaker->getGain(i + 8, l + 1) != 0.0)
	    {
		det_ch.det = i + 8;
		det_ch.ch = (l + 1);
		deadChList.push_back(det_ch);
	    }
	    else if(adcDist[i][l]->GetEntries() < 1000 && adcDist[i][l]->GetEntries() > 0 && fmsDBMaker->getGain(i + 8, l + 1) != 0.0)
	    {
		det_ch.det = i + 8;
		det_ch.ch = (l + 1);
		badChList.push_back(det_ch);
	    }
	}
    }

    vector <DetChPair>::iterator it;    
    cout << "List of dead channels:" <<endl;
    for(it = deadChList.begin(); it != deadChList.end(); ++ it)
	cout << it->det << "\t"<< it->ch<<endl;
    
    cout << "List of bad channels:" <<endl;
    for(it = badChList.begin(); it != badChList.end(); ++ it)
	cout << it->det << "\t"<< it->ch <<endl;
        
    f->Close();    
}			
	
