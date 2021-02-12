// Filename: RunNanoDstMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 13 17:25:43 2019 (-0500)
// URL: jlab.org/~latif

#include "StRootInclude.h"
#include "RootInclude.h"
#include "BrightStInclude.h"
#include "BrNanoDstMaker/TStRpFilterMaker.h"
#include "BrNanoDstMaker/TStNanoDstMaker.h"

#include <vector>
#include <iostream>
using namespace std;

void RunNanoDstMaker(TString fileList, TString outFile, Bool_t showMsg, vector <Int_t> *evtTrig,  vector <Int_t> *vetoTrig, Int_t maxEvents)
{
    TH1D* hEvtCount = new TH1D("hEvtCount", "Event counts", 20, 0, 20);

    if(!showMsg)
    {
	gMessMgr->SetLimit("I", 0);   //Disable StInfo messages including Skipped event message
	gMessMgr->SetLimit("Q", 0);   //Disable StQAInfo messages (includes event processing status)
    }
    StChain *chain		= new StChain;
    StMuDstMaker *muDstMaker	= new StMuDstMaker(0, 0, "", fileList, "", 1000);
    St_db_Maker *starDb		= new St_db_Maker("db","$HOME/StarDb","MySQL:StarDb","$STAR/StarDb");

    //-------- Keep these commented out for Cold QCD Nano Dst files -----------
    // starDb->SetAttr("blacklist", "emc");
    // starDb->SetAttr("blacklist", "eemc");
    // starDb->SetAttr("blacklist", "pp2pp");
    // starDb->SetAttr("blacklist", "tof");
    // starDb->SetAttr("blacklist", "tpc");
    
    starDb->SetAttr("blacklist", "ftpc");
    starDb->SetAttr("blacklist", "ist");
    starDb->SetAttr("blacklist", "mtd");
    starDb->SetAttr("blacklist", "pmd");
    starDb->SetAttr("blacklist", "pxl");
    starDb->SetAttr("blacklist", "ssd");
    starDb->SetAttr("blacklist", "svt");
    starDb->SetAttr("blacklist", "fgt");
    
    StSpinDbMaker* spindb	= new StSpinDbMaker("spinDb");
    StFmsDbMaker* fmsDb		= new StFmsDbMaker("fmsDb");
    
    //======================================================== Trigger Filter ==============================================
    if(evtTrig || vetoTrig)
    {    
	//TStRpFilterMaker* filterMaker = new TStRpFilterMaker("TStRpFilterMaker"); // Filter/Skip Events if no RP or FMS BS/JP Trigger
	StTriggerFilterMaker* filterMaker = new StTriggerFilterMaker;
	if(evtTrig)
	{
	    for(Int_t i = 0; i < evtTrig->size(); ++i)
		filterMaker->addTrigger(evtTrig->at(i));
	}
	if(vetoTrig)
	{
	    for(Int_t i = 0; i < vetoTrig->size(); ++i)
		filterMaker->addVetoTrigger(vetoTrig->at(i));
	}
	//filterMaker->SetHist1d(hEvtCount);
    }

    Int_t nEvents = muDstMaker->chain()->GetEntries();
    //======================================================== EMC ==============================================
    if(TStar::gConfig->EnableEmc())
    {
	// Maker to apply calibration
	StEmcADCtoEMaker *adc_to_e = new StEmcADCtoEMaker();
	adc_to_e->setPrint(kFALSE);
	// Makers for cluster finding
	StPreEclMaker *pre_ecl = new StPreEclMaker();
	pre_ecl->setPrint(kFALSE);
	StEpcMaker *epc = new StEpcMaker();
	epc->setPrint(kFALSE);
	epc->setFillHisto(kTRUE);

	TStEmcTrackMatchingMaker *trackMatchingMkr = new TStEmcTrackMatchingMaker();
    }
    
    //======================================================== FMS ==============================================
    if(TStar::gConfig->EnableFms())
    {
	//Enable new FMS calibration and mask hot/bad channels
	fmsDb->SetAttr("fmsGainCorr","fmsGainCorr-BNL-C");
	Bool_t isHotCh[4][571] = {0};
	TStFmsHotChDB *fmsHotChDb = new TStFmsHotChDB();
	Int_t runNumber = TStRunList::GetRunFromFileName((string)fileList);
	fmsHotChDb->GetHotChList(runNumber, isHotCh);
	cout << "The following FMS cells are masked:" <<endl;
	for(int i = 0; i < 4; ++i)
	{
	    for(int j = 0; j < 571; ++j)
		if(isHotCh[i][j])
		    cout << "det "<< (i + 1)<< " ch "<< (j+1) << " hot/bad status:"<< isHotCh[i][j] <<endl;
	}
	fmsDb->maskChannels(isHotCh);
	
	StEventMaker* eventMk = new StEventMaker();
	StFmsHitMaker*   fmsHitMk   = new StFmsHitMaker();
	StFmsPointMaker* fmsPointMk = new StFmsPointMaker();

	// mudst reading
	// if 0, get info from StTriggerData from StTriggerDataMaker/StEvent/MuDst
	// and apply new DB; cluster finding/fitting is redone
	// if 1, cluster finding/fitting is NOT redone (i.e., mudst result is read
	// out); however, some recalculations (like point positions) from updated DB values
	// are done
	fmsPointMk->SetReadMuDst(0); // (0)
	fmsHitMk->SetReadMuDst(0);   // (0)

	// shower shape
	// 0='use lednev params', 1='use zhanwen params', 2='use yuxi params'
	// fmsPointMk->setShowerShapeWithAngle(1); // (1)
  
	// cluster merging over large/small boundary
	// if 1, merges large and small cell clusters (experimental!; default 1 in StFmsPointMaker)
	fmsPointMk->setMergeSmallToLarge(1); // (1)

	// vertex correction
	// if 0, no vertex correction; if 1, use MuDst BBC vertex based on run11 calibration (needs update?)
	fmsPointMk->setVertexZ(1); // (1)

	//======================================================== FPS ==============================================
	// FPS
	StFmsFpsMaker * fpsMk = new StFmsFpsMaker();
	fpsMk->setReadMuDST(1);      // (1) // (reverse 0<->1 convention as FmsHitMaker and FmsPointMaker)
    }
    //====================================================== EEMC =============================================
    if(TStar::gConfig->EnableEEmc())
    {
	cout << "\033[1;31m=============== It only keeps events for which there is a valid EEMC event/response ====================================\033[0m"<<endl;
	Bool_t isMC = kFALSE;
	StEEmcDbMaker       *eemcDbMaker;
	StEEmcA2EMaker      *a2EMakerPtr;
	StEEmcEnergyMaker_t *energyMakerPtr;
	StEEmcTreeMaker_t   *treeMakerPtr;
	StSpinInfoMaker_t   *spinInfoMakerPtr;
	// just to make sure StEvent is found for the trig simu
	StMuDst2StEventMaker *muDst2StEvent = new StMuDst2StEventMaker();

	// Initialize EEMC database
	eemcDbMaker = new StEEmcDbMaker("eemcDb");
	
	//~~ PART 1 ~~~
	// Energy to ADC maker
	a2EMakerPtr = new StEEmcA2EMaker("EEmcA2EMaker");
	a2EMakerPtr->database("eemcDb");          // sets db connection
	a2EMakerPtr->source("MuDst",1);           // sets mudst as input
	a2EMakerPtr->threshold(3.0,0);            // tower threshold
	a2EMakerPtr->threshold(3.0,1);            // pre1 threshold 
	a2EMakerPtr->threshold(3.0,2);            // pre2 threshold
	a2EMakerPtr->threshold(3.0,3);            // post threshold
	a2EMakerPtr->threshold(3.0,4);            // smdu threshold
	a2EMakerPtr->threshold(3.0,5);            // smdv threshold

	//
	// Now start things particular to the StEEmcTreeMaker
	//

	//
	// create spin info maker
	spinInfoMakerPtr = new StSpinInfoMaker_t( "SpinInfoMaker" );

	// Energy Structure Maker
	energyMakerPtr = new StEEmcEnergyMaker_t( "energyMkr", "EEmcA2EMaker" );
	energyMakerPtr->setStripThres( isMC ? 0.0 : 0.001 );
	energyMakerPtr->setTowerThres( isMC ? 0.0 : 1.0 );

	// I'm told one must use slow simulator to get pedestals correct for L2,
	// but I'll put it after the A2Emaker and Energy maker, so it doesn't mess them up

	//
	// Tree Maker
	//
	TString outputFileName = (TString)"eemcTreeP1_" + outFile;
	treeMakerPtr = new StEEmcTreeMaker_t( "EEmcTreeMaker" );
	treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::WRITE,  outputFileName );
	treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::IGNORE, "" );
	treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::IGNORE, "" );
	treeMakerPtr->setMaxNumEvents( nEvents );
	treeMakerPtr->setEEmcEnergyMkr( energyMakerPtr );
	treeMakerPtr->doSpinInfoIO( !isMC );
	treeMakerPtr->doEvtHddrIO( 1 );
	treeMakerPtr->setHTTPthres( 0, 0 );
 
	if( !isMC )
	    treeMakerPtr->setSpinInfoMkr( spinInfoMakerPtr );

	//~~~ PART 2 and 3 ~~
	TString str1 = (TString)"eemcTreeP2_" + outFile;
	TString str2 = (TString)"eemcTreeP3_" + outFile;
	const Char_t *eemcTreePart2FileName = str1.Data();
	const Char_t *eemcTreePart3FileName = str2.Data();

	std::cout << "***** Instanciating all the classes *****" << endl;

	//
	// some variables that tend to be made global
	//
	StEEmcTowerClusterFinder_t              *towerClusterFinderPtr = 0;
	StEEmcStripClusterFinderTSIU_t          *stripClusterFinderPtr = 0;
	StEEmcPointFinderIU_t                   *pointFinderPtr        = 0;
	StEEmcEnergyApportionerIU_t             *energyApportionerPtr  = 0;
	StEEmcHitMakerSimple_t                  *hitMakerPtr           = 0;
	StEEmcTreeMaker_t                       *treeReaderPtr         = 0;
	StEEmcTreeMaker_t                       *treeWriterPtr         = 0;
   

	//
	// TREE MAKER FOR READING
	//
	treeReaderPtr = new StEEmcTreeMaker_t( "EEmcTreeReader" );
	//treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::READ,   eemcTreePart1FileName );
	treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::IGNORE, "" );
	treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::IGNORE, "" );
	treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::IGNORE, "" );
	treeReaderPtr->doSpinInfoIO( !isMC );
	treeReaderPtr->doEvtHddrIO( 1 );
	treeReaderPtr->setMaxNumEvents( muDstMaker->chain()->GetEntries() );
	treeReaderPtr->setEEmcTreeReader( treeMakerPtr );

	//
	// CREATE ALL THE FINDERS AND THE HIT MAKER
	//

	// tower cluster finder
	//    towerClusterFinderPtr = new StEEmcTowerClusterFinderMinesweeper_t();
	//    towerClusterFinderPtr->setSeedEnergyThreshold( 2.0 );

	// strip cluster finder
	stripClusterFinderPtr = new StEEmcStripClusterFinderTSIU_t();

	// parameter set d

	stripClusterFinderPtr->setNumSmoothIters( 10 );
	stripClusterFinderPtr->setNumStripsPerSide( 3 );
	stripClusterFinderPtr->setMinStripsPerCluster( 5 );
	stripClusterFinderPtr->setSeedAbsThres( 0.002 );
	stripClusterFinderPtr->setSeedRelThres( 0.0 );
	stripClusterFinderPtr->setMinEnergyPerCluster( 0.003 );

	// point finder
	pointFinderPtr = new StEEmcPointFinderIU_t();

	// energy apportioner
	energyApportionerPtr = new StEEmcEnergyApportionerIU_t();
	energyApportionerPtr->setCheckTowerBits(0);

	// Hit maker
	hitMakerPtr = new StEEmcHitMakerSimple_t ( "hitMaker",
						   "EEmcTreeReader",
						   towerClusterFinderPtr,
						   stripClusterFinderPtr,
						   pointFinderPtr,
						   energyApportionerPtr
	    );
	hitMakerPtr->doClusterTowers( 0 );
	hitMakerPtr->doClusterPreShower1( 0 );
	hitMakerPtr->doClusterPreShower2( 0 );
	hitMakerPtr->doClusterPostShower( 0 );
	hitMakerPtr->doClusterSMDStrips( 1 );

	//
	// Extra things if MC
	//

	// Associate hits with tracks, if it is MC data
	//    if( isMC )
	//       mcHitMakerPtr = new StMcEEmcHitMakerStrips_t( "mcHitMaker", "responseTreeReader", "hitMaker" );

	//
	// TREE MAKER FOR WRITING
	//
	treeWriterPtr = new StEEmcTreeMaker_t( "EEmcTreeWriter" );
	treeWriterPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::IGNORE, "" );
	treeWriterPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::WRITE,  eemcTreePart2FileName );
	treeWriterPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::WRITE,  eemcTreePart3FileName );
	treeWriterPtr->doSpinInfoIO( 0 );
	treeWriterPtr->doEvtHddrIO( 0 );
	treeWriterPtr->doMakePairs( 1 );
	treeWriterPtr->setEEmcTreeReader( treeReaderPtr );
	treeWriterPtr->setEEmcHitMkr( hitMakerPtr );	
    }
    //======================================================== Nano DST Maker ==============================================
    TStNanoDstMaker *nanoDstMaker = new TStNanoDstMaker("TStNanoDstMaker");
    //nanoDstMaker->SetTree(tree);
    if(evtTrig)
	nanoDstMaker->SetTrigIDs(*evtTrig);
    nanoDstMaker->SetOutFileName(outFile);
    nanoDstMaker->SetMuDstMaker(muDstMaker);
    nanoDstMaker->SetBeamMomentum(100.0);           //----> To be moved to configuration file
    
    if(TStar::gConfig->EnableEvent())
	nanoDstMaker->EnableEvent(true);
    if(TStar::gConfig->EnableEmc())
	nanoDstMaker->EnableEmc(true);
    if(TStar::gConfig->EnableFms())
	nanoDstMaker->EnableFms(true);
    if(TStar::gConfig->EnableRps())
    {
	nanoDstMaker->EnableRps(true);
	nanoDstMaker->UseRpsAfterburner(true);
    }
    if(TStar::gConfig->EnableTpc())
	nanoDstMaker->EnableTpc(true);
    if(TStar::gConfig->EnableEEmc())
    {
	nanoDstMaker->EnableEEmc(true);
	nanoDstMaker->SyncOnEEmc(true); //true only if using EEMC detector alone
    }
	
    cout << "----------->Total Events to be processed: "<< nEvents <<" <----------------"<<endl;
    
    chain->Init();
    if(maxEvents != -1)
	chain->EventLoop(maxEvents);  // Run specified number of events
    else
	chain->EventLoop(nEvents);  // Run all events
    chain->Finish();

    cout << "Status:SUCCESS!!" <<endl;

    //delete chain;
}
