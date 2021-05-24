//
// Run bfc.C with FMS fast simulator in the chain to import FPD/FMS hits
// from GEANT to StEvent to MuDst.
//


void FmsSimRunBfc(int nevents, const char* fzfile, int filterThreshold = 1) 
{
    // const char* chainopt = "DbV20150923 y2015 MakeEvent ITTF NoSsdIt NoSvtIt Idst BAna l0 Tree logger Sti VFPPVnoCTB beamLine tpcDB TpcHitMover TpxClu fmsDb fmsSim fmspoint  tags emcY2 EEfs evout -dstout IdTruth geantout big fzin MiniMcMk clearmem  sdt20150417.193427"; //Mriganka

    const char* chainopt = "ry2015a agml usexgeom MakeEvent McEvent vfmce Idst BAna l0 l3 Tree logger fmsSim fmspoint evout -dstout IdTruth bigbig fzin geantout clearmem sdt20150417.193427"; //MMM's jobs
    
    //const char* chainopt = "ry2011b agml usexgeom MakeEvent ITTF NoSsdIt NoSvtIt Idst BAna l0 l3 Tree logger Sti  fmsSim   IdTruth big fzin clearmem sdt20110417.193427"  //Zhanwen

    gROOT->LoadMacro("bfc.C");  //This is how everyone else in STAR does it. bfc.C must be under ROOT macros path in this case

    bfc(-1,chainopt,fzfile);

    //-------------------------------------------
    if(!TStar::gBrDebug)
    {
    	gMessMgr->SetLimit("I", 0);
    	gMessMgr->SetLimit("Q", 0);
    	gMessMgr->SetLimit("W", 0);
    }
    //-------------------------------------------

    //-------- Mask FMS hot/dead cells here ------------
    StFmsDbMaker* fmsDb = (StFmsDbMaker*) chain->GetMaker("fmsDb");
    fmsDb->SetAttr("fmsGainCorr","fmsGainCorr-BNL-C");
    Bool_t isHotCh[4][571] = {0};
    Int_t nHotCh = 0;
    TStFmsHotChDB *fmsHotChDb = new TStFmsHotChDB();
    fmsHotChDb->GetHotChList(16080020, isHotCh);
    cout << "The following FMS cells are masked:" <<endl;
    for(int i = 0; i < 4; ++i)
    {
	for(int j = 0; j < 571; ++j)
	{
	    if(isHotCh[i][j])
	    {
		cout << "det "<< (i + 1)<< " ch "<< (j+1) << " hot/bad status:"<< isHotCh[i][j] <<endl;
		++nHotCh;
	    }
	}
    }
    cout << "Total manually masked bad / hot channels: "<< nHotCh <<endl;

    fmsDb->maskChannels(isHotCh);
    
    //fmsdb->readGainFromText();
   
    StPythiaEventMaker* pythia = new StPythiaEventMaker("PythiaEvent");
    TString pyfile = gSystem->BaseName(fzfile);
    pyfile.ReplaceAll(".fzd",".pythia.root");
    pythia->SetPythiaFile(pyfile);
   
    //---------> Need to find out appropriate threshold <----------------
    //StFmsFilterMaker* fmsFilter = new StFmsFilterMaker("fmsFilter"); // Energy sum per quadrant or pie
    StFmsEtFilterMaker* fmsFilter = new StFmsEtFilterMaker("fmsFilter"); // Transverse Energy sum per quadrant or pie
    fmsFilter->setEThres(filterThreshold); //Initial value was E = 30. It was too high to produce MuDst. Use 12 GeV in E or 1 GeV in E_T.
    cout << "-----------------> !!! The FMS Filter Maker Threshold is set to:  "<< filterThreshold << " !!!<----------------------------"<<endl;
    
    // Delay write until after filter
    chain->AddAfter("fmsSim",fmsFilter);
    chain->AddAfter("geant",pythia);

    chain->Init();

    if(nevents == -1)
	chain->EventLoop();
    else
	chain->EventLoop(nevents);
}


void FmsSimRunBfc(int cycle, int nevents, int filterThreshold = 1)
{
    FmsSimRunBfc(-1, Form("FmsSim_Run15_%i_evt%i.fzd", cycle, nevents), filterThreshold);     
}
