void loadLibraries(bool fmsLibs = true);
void RunFmsTreeMaker(const char*  inFile = "inFiles_fms.list", const char* outFile = "fmsCalib.root")
{
	TStopwatch sw;
	sw.Start();

	//loadLibraries();
	//gSystem->Load("StFmsCalibMaker");

	gMessMgr->SetLimit("I", 0);
	gMessMgr->SetLimit("Q", 0);
	gMessMgr->SetLimit("W", 0);

	StChain* chain = new StChain("StChain");
	chain->SetDEBUG(0);

	//-------------------------------------------

	//Arguments(default): mode, nameMode, directory (./), file, filter (.), max files (10), and name (MuDst)
	StMuDstMaker* muDstMk = new StMuDstMaker(0, 0, "", inFile, ".MuDst.root", 1000, "MuDst");
	cout <<Form("\nEntries in %s: %i", inFile, muDstMk->tree()->GetEntries()) <<endl;

	St_db_Maker* dbMk = new St_db_Maker("db", "MySQL:StarDb", "$STAR/StarDb");
	dbMk->SetAttr("blacklist", "emc");
	dbMk->SetAttr("blacklist", "eemc");
	dbMk->SetAttr("blacklist", "ftpc");
	dbMk->SetAttr("blacklist", "ist");
	dbMk->SetAttr("blacklist", "mtd");
	dbMk->SetAttr("blacklist", "pmd");
	dbMk->SetAttr("blacklist", "pp2pp");
	dbMk->SetAttr("blacklist", "pxl");
	dbMk->SetAttr("blacklist", "ssd");
	dbMk->SetAttr("blacklist", "svt");
	dbMk->SetAttr("blacklist", "tof");
	dbMk->SetAttr("blacklist", "tpc");
	dbMk->SetDebug();

	StFmsDbMaker* fmsDb = new StFmsDbMaker("fmsDb");
	fmsDb->setDebug(0);
	//fmsDb->readGainCorrFromText(true); cout <<"WARNING! External gain correction factors are being used!\n" <<endl;

	StEventMaker*    eventMk    = new StEventMaker();
	StFmsHitMaker*   fmsHitMk   = new StFmsHitMaker();
	StFmsPointMaker* fmsPointMk = new StFmsPointMaker();
	//fmsHitMk  ->SetReadMuDst();
	//fmsPointMk->SetReadMuDst();

	//-------------------------------------------

	StFmsCalibMaker* fmsCalib = new StFmsCalibMaker();
	TString outName(outFile);
	if (!strcmp(outFile, "")) { outName = inFile; outName.ReplaceAll("MuDst", "fmsCalib"); }
	fmsCalib->SetOutputName((const char*)outName);
	//fmsCalib->SetReadBadChannel("FmsCellStat.txt");
	fmsCalib->GetQaTree();
	//fmsCalib->GetMap();

	//-------------------------------------------

	chain->Init();
	cout<<"----------------------------> Here!!!!! <--------------------------"<<endl;
	chain->EventLoop();
	chain->Finish();
	delete chain;

	sw.Stop();
	sw.Print();
	std::cout <<"Done!" <<endl;
	return;
}//Main

//==============================
void loadLibraries(bool fmsLibs)
{
    //STAR libraries for chain, MuDST, logger etc
    gROOT->Macro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();
    gROOT->Macro("loadMuDst.C");
    //gROOT->Macro("LoadLogger.C");

    gSystem->Load("StTpcDb");
    gSystem->Load("St_db_Maker");
    gSystem->Load("StDaqLib");
    gSystem->Load("StDbBroker");
    gSystem->Load("StDbUtilities");
    gSystem->Load("StDetectorDbMaker");
    gSystem->Load("StEEmcDbMaker");
    gSystem->Load("StEEmcUtil");
    gSystem->Load("StEmcRawMaker");
    gSystem->Load("StEmcADCtoEMaker");
    gSystem->Load("StEmcTriggerMaker");
    gSystem->Load("StEpcMaker");
    gSystem->Load("StEventMaker");
    gSystem->Load("StMCAsymMaker");
    gSystem->Load("StPreEclMaker");
    gSystem->Load("StRandomSelector");
    gSystem->Load("StSpinDbMaker");
    gSystem->Load("StTriggerFilterMaker");
    gSystem->Load("StTriggerUtilities");
    if (fmsLibs)
	{
        gSystem->Load("libMinuit.so");
        gSystem->Load("StFmsUtil");
        gSystem->Load("StFmsDbMaker");
        gSystem->Load("StFmsHitMaker");
        gSystem->Load("StFmsPointMaker");
        gSystem->Load("StFmsFpsMaker");
    }

	return;
}//loadLibraries
