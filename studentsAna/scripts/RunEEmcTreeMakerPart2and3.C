/* 
 * Created May 2012, by S. Gliske
 *
 * Macro to read in "Part 1" of the EEmcTree and write out "Part 2"
 * and "Part 3" using the TSIU algorithm.
 * 
 */

void LoadLibs();
//
// the main routine
//
void RunEEmcTreeMakerPart2and3( const Char_t *eemcTreePart1FileName,
				const Char_t *outName,
                                Int_t neventsIn,
                                Bool_t isMC,
                                Int_t displayFreq)
{

    LoadLibs();
    
    TString str1 = TString("eemcTreeP2_") + outName;
    TString str2 = TString("eemcTreeP3_") + outName;
    const Char_t *eemcTreePart2FileName = str1.Data();
    const Char_t *eemcTreePart3FileName = str2.Data();

    std::cout << "***** Instanciating all the classes *****" << endl;

    //
    // some variables that tend to be made global
    //
    StChain                                 *analysisChain = 0;
    StEEmcTowerClusterFinder_t              *towerClusterFinderPtr = 0;
    StEEmcStripClusterFinderTSIU_t          *stripClusterFinderPtr = 0;
    StEEmcPointFinderIU_t                   *pointFinderPtr        = 0;
    StEEmcEnergyApportionerIU_t             *energyApportionerPtr  = 0;
    StEEmcHitMakerSimple_t                  *hitMakerPtr           = 0;
    StEEmcTreeMaker_t                       *treeReaderPtr         = 0;
    StEEmcTreeMaker_t                       *treeWriterPtr         = 0;
   
    //
    // CREATE THE ANALYSIS CHAIN
    //
    analysisChain = new StChain("eemcAnalysisChain");

    //
    // TREE MAKER FOR READING
    //
    treeReaderPtr = new StEEmcTreeMaker_t( "EEmcTreeReader" );
    treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::READ,   eemcTreePart1FileName );
    treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::IGNORE, "" );
    treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::IGNORE, "" );
    treeReaderPtr->doSpinInfoIO( !isMC );
    treeReaderPtr->doEvtHddrIO( 1 );
    treeReaderPtr->setMaxNumEvents( neventsIn );


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

    // debugging info
    std::cout << "***** Done instanciating all the classes *****" << endl;
    //analysisChain->ls(3);

    //
    // INITIALIZE ALL MAKERS
    //

    std::cout << "***** Initializing all makers in the analysis chain *****" << std::endl;

    analysisChain->Init();

    std::cout << "***** Initialization done *****" << std::endl;

    //
    // FINALLY READY TO LOOP OVER THE EVENTS 
    //

    Int_t ierr  = kStOK;  // err flag
    Int_t nIn = -1;
    if( neventsIn < 0 )
	neventsIn = 1<<30;

    Int_t nhits = 0;

    for( nIn = 0; nIn < neventsIn && !ierr; ++nIn ){

	// clear
	analysisChain->Clear();

	// make
	ierr = analysisChain->Make();

	// number of hits
	nhits += hitMakerPtr->getHitVecSize();

	// Print every so many events
	if( (nIn+1) % displayFreq == 0 )
	    std::cout << "***** Entries read " << nIn+1 << ", total hits = " << nhits << endl;
    };

    std::cout << "***** Entries read " << nIn+1 << ", total hits = " << nhits << endl;

    if( ierr && ierr != 2 )
	std::cout << "***** ERROR FLAG " << ierr << endl;
 
    //---------------------------------------------------------------
    //
    // Calls the ::Finish() method on all makers (done automatically)
    //
    // analysisChain->Finish(); 

    //
    // Delete the chain (done automatically)
    //
    //analysisChain->Delete();
    //delete analysisChain;

    return;
};



void LoadLibs()
{
    gSystem->Load("libMinuit.so");
    //--------- MuDST Related --------------
    gSystem->Load("libStTableUtilities");
    gSystem->Load("libTable");
    gSystem->Load("libPhysics");
    gSystem->Load("St_base");
    gSystem->Load("StChain");
    gSystem->Load("St_Tables");
    gSystem->Load("StUtilities");        // new addition 22jul99
    gSystem->Load("StTreeMaker");
    gSystem->Load("StIOMaker");
    gSystem->Load("StarClassLibrary");
    gSystem->Load("StTriggerDataMaker"); // new starting from April 2003
    gSystem->Load("StBichsel");
    gSystem->Load("StEvent");
    gSystem->Load("StEventUtilities");
    gSystem->Load("StDbLib");
    gSystem->Load("StEmcUtil");
    gSystem->Load("StTofUtil");
    gSystem->Load("StPmdUtil");
    gSystem->Load("StPreEclMaker");
    gSystem->Load("StStrangeMuDstMaker");
    gSystem->Load("StMuDSTMaker");

    //-------------Trigger --------------
    gSystem->Load("StTriggerUtilities");
    //------------- DB -----------------
    gSystem->Load("StTpcDb");
    gSystem->Load("StSpinDbMaker");
    gSystem->Load("St_db_Maker");
    gSystem->Load("StDbBroker");
    gSystem->Load("StDbUtilities");
    gSystem->Load("StDetectorDbMaker");

    //--------- StEvent ----------------
    //gSystem->Load("StEvent");
    gSystem->Load("StEventMaker");
    gSystem->Load("StDaqLib");
    
    //-------- EEMC Related ------------
    gSystem->Load("StEEmcDbMaker");
    gSystem->Load("StEEmcUtil");
    gSystem->Load("StEmcRawMaker");
    gSystem->Load("StEmcADCtoEMaker");
    gSystem->Load("StEmcTriggerMaker");
    gSystem->Load("StDbBroker");
    gSystem->Load("StEEmcPoolmuDst");
    gSystem->Load("StEEmcA2EMaker");
    //TSIU Algorithm
    gSystem->Load("StEEmcPoolEEmcTreeContainers");  //Note: Atypical shared object name
    gSystem->Load("StEEmcTreeMaker");
    //IU Algorithm
    gSystem->Load("StEEmcHitMaker");
    gSystem->Load("StEEmcClusterMaker");    
    gSystem->Load("StEEmcPointMaker");
    gSystem->Load("StEEmcIUPi0");
    
}
