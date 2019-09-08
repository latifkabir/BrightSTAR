#include "iostream.h"
#include "string.h"
#include <algorithm>
#include <stdio.h>
#include "ctype.h"


// Forward the class declarations for StChain and St_db_Maker
class     StChain;
class     St_db_Maker;

void loadLibs();

void RunFemtoDstMaker( const Char_t * fileList = "./filelist.lis" ){
	cout << "FileList: " << fileList << endl;
	
	// Maximums
	Int_t nEvents = 50000000;
	Int_t nfiles = 1000;
	
	// nEvents = 1000;


	StChain  *chain = 0;
	St_db_Maker *dbMk = 0;
	Int_t iEvt = 0, istat = 0;

	// First load some shared libraries we need
	loadLibs();

	// create the chain
	chain  = new StChain("StChain");

	// create the StMuDstMaker
	StMuDstMaker *muDstMaker = new StMuDstMaker(  	0,
													0,
													"",
													fileList,
													"MuDst.root",
													nfiles
												);

	StVpdCalibMaker *vpdMaker = new StVpdCalibMaker();
	StBTofCalibMaker *calibMaker = new StBTofCalibMaker(  );
	calibMaker->setMuDstIn( true );

	// cout << "MUDST :: " << muDstMaker->GetFileName() << endl << endm;

	// muDstMaker->SetStatus("*",0);
 //  	muDstMaker->SetStatus("MuEvent*",1);
 //  	muDstMaker->SetStatus("PrimaryVertices*",1);
 //  	muDstMaker->SetStatus("PrimaryTrack*",1);
 //  	muDstMaker->SetStatus("GlobalTrack*",1);
 //  	// muDstMaker->SetStatus("MTD*",1);
 //  	muDstMaker->SetStatus("BTof*",1);


	// Initialize the database
	// cout << endl << "============  Data Base =========" << endl;
	dbMk = new St_db_Maker("db","MySQL:StarDb","$STAR/StarDb","StarDb");


	FemtoDstWriter *fmtWriter = new FemtoDstWriter( "MtdMc");
	fmtWriter->SetDebug(0);
	fmtWriter->setOutputFileName( "FemtoDst.root" );


	// Initialize chain
	Int_t iInit = chain->Init();
	
	// ensure that the chain initializes
	if ( iInit ) 
		chain->Fatal(iInit,"on init");

	// print the chain status
	chain->PrintInfo();
	

	//
	// Event loop
	//
	int istat = 0, i = 1;


	while ( i <= nEvents && istat != 2 ){
		//cout << endl << "============================ Event " << i << " start ============================" << endl;
	 
		// clear the chain state
		chain->Clear();

		// make the chain and get the status
		istat = chain->Make(i);

		if ( i % 500 == 0 )
			cout << "." << std::flush;

		if (istat == 2) {
			cout << "Last event processed. Status = " << istat << endl;
		} else if (istat == 3) {
			cout << "Error event processed. Status = " << istat << endl;
		}
		 
		 i++;
	}
	
	cout << endl << "============================ Event " << ( i - 1 ) << " finished ============================" << endl;

	// Chain Finish
	if (nEvents > 1) {
		chain->Finish();
	}

	delete chain;

	

}






void loadLibs(){


	if (gClassTable->GetID("TTable") < 0) {
		gSystem->Load("libStar");
		gSystem->Load("libPhysics");
	}  
	gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
	loadSharedLibraries();
	gSystem->Load("StarMagField");
	gSystem->Load("StMagF");
	gSystem->Load("StDetectorDbMaker");
	gSystem->Load("StTpcDb");
	gSystem->Load("StDaqLib");
	gSystem->Load("StDbBroker");
	gSystem->Load("StDbUtilities");
	gSystem->Load("St_db_Maker");

	gSystem->Load("StEvent");
	gSystem->Load("StEventMaker");
	gSystem->Load("StarMagField");
 
	gSystem->Load("libGeom");
	gSystem->Load("St_g2t");
	
	// Added for Run16 And beyond
	gSystem->Load("libGeom.so");
	gSystem->Load("StarRoot.so");
	gSystem->Load("St_base.so");
	gSystem->Load("StUtilities.so");
	gSystem->Load("libPhysics.so");
	gSystem->Load("StarAgmlUtil.so");
	gSystem->Load("StarAgmlLib.so");
	gSystem->Load("libStarGeometry.so");
	gSystem->Load("libGeometry.so");
	
	gSystem->Load("xgeometry");
 
	gSystem->Load("St_geant_Maker");
	
	gSystem->Load("FemtoDstFormat");
	gSystem->Load("FemtoDstWriter");

	// needed since I use the StMuTrack
	gSystem->Load("StarClassLibrary");
	gSystem->Load("StStrangeMuDstMaker");
	gSystem->Load("StMuDSTMaker");
	gSystem->Load("StBTofCalibMaker");
	gSystem->Load("StVpdCalibMaker");
	


}
