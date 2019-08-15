#include <iostream>
#include "RootInclude.h"
#include "StRootInclude.h"
#include "BrFmsMaker/TStFmsTreeMaker.h"

/*
In order to calibrated with gain correction table from a text file, you need the following two files in the current directory:
FmsGainCorr.txt
FmsCellStat.txt

These files should be under current directory. It is hard coded in FmsDBMaker and must remain in top level directory. 

 */

void RunFmsTreeMaker(const char*  inFile, const char* outFile)
{
	TStopwatch sw;
	sw.Start();
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
	fmsDb->readGainCorrFromText(true); cout <<"WARNING! External gain correction factors are being used!\n" <<endl;

	StEventMaker*    eventMk    = new StEventMaker();
	StFmsHitMaker*   fmsHitMk   = new StFmsHitMaker();
	StFmsPointMaker* fmsPointMk = new StFmsPointMaker();
	//fmsHitMk  ->SetReadMuDst();
	//fmsPointMk->SetReadMuDst();

	//-------------------------------------------

	TStFmsTreeMaker* fmsTreeMaker = new TStFmsTreeMaker();
	TString outName(outFile);
	if (!strcmp(outFile, "")) { outName = inFile; outName.ReplaceAll("MuDst", "fmsTreeMaker"); }
	fmsTreeMaker->SetOutputName((const char*)outName);
	//fmsTreeMaker->SetReadBadChannel("/star/u/kabir/GIT/BrightSTAR/FmsCellStat.txt");
	fmsTreeMaker->GetQaTree();
	//fmsTreeMaker->GetMap();

	//-------------------------------------------

	chain->Init();

	chain->EventLoop();
	chain->Finish();
	delete chain;

	sw.Stop();
	sw.Print();
	std::cout <<"Done!" <<endl;
	return;
}
