/*
  NOTE: The option to run this script on MC data has been completely renoved.
  If you need that option, check the original script  under STAR CVS. -- Latif Kabir

 
  * Created May 2012, by S. Gliske
  *
  * Macro to write "Part 1" of the EEmcTree, by reading in a MuDst
  * and applying the calibration.
  *
  * Note: much of this script is based on Alice's runABClusterMaker.C 
  * and Weihong's runEEmcPi0.C
  * 
  *   This script has been modified for data only -- Latif Kabir
  * 
  */

#include "StRootInclude.h"

//
// the main routine
//
void RunEEmcTreeMakerPart1(const Char_t *inputFileName,
			    const Char_t *outputFileName,
			    Int_t displayFreq,
			    Bool_t isMC)
{    
    //
    // some variables that others tend to make global
    //
    StChain             *analysisChain    = 0;
    StMuDstMaker        *muDstMaker       = 0;
    St_db_Maker         *starDatabase     = 0;
    StEEmcDbMaker       *eemcDbMaker      = 0;
    StSpinDbMaker       *spinDb           = 0;
    StEEmcA2EMaker      *a2EMakerPtr      = 0;
    StEEmcEnergyMaker_t *energyMakerPtr   = 0;
    StEEmcTreeMaker_t   *treeMakerPtr     = 0;
    StMcEEmcTreeMaker_t *mcTreeMakerPtr   = 0;
    StSpinInfoMaker_t   *spinInfoMakerPtr = 0;

    Long_t neventsIn = -1; 
    Long_t neventsOut = -1;
    
    //
    // Create the analysis chain
    //
    analysisChain = new StChain("eemcAnalysisChain");

    //
    // MuDst maker for reading input
    //
    Int_t nfiles = 10000;
    muDstMaker = new StMuDstMaker( 0, 0, "", inputFileName, "MuDst", nfiles );

    //1.<--------------
    // just to make sure StEvent is found for the trig simu
    StMuDst2StEventMaker *muDst2StEvent = new StMuDst2StEventMaker();


    //2.<-----------------------
    //
    // Connect to the STAR databse
    //
    starDatabase = new St_db_Maker("StarDb","MySQL:StarDb","MySQL:StarDb","$STAR/StarDb");
    starDatabase->SetAttr("blacklist", "fgt");
    starDatabase->SetAttr("blacklist", "svt");
    starDatabase->SetAttr("blacklist", "tpc");
    starDatabase->SetAttr("blacklist", "ftpc");

    Bool_t noMCdateTime = true;
    if( isMC && noMCdateTime )
	starDatabase->SetDateTime(20060512,43500);

    //3.<-----------------------
    //
    // Initialize EEMC database
    //
    eemcDbMaker = new StEEmcDbMaker("eemcDb");

    // this is the interface to the STAR logger
    // SwitchOn("D") turns on debugging
    // SwitchOff("D") turns off debuggin
    // "I" is info and "W" is warn
    gMessMgr->SwitchOff("D");
    gMessMgr->SwitchOff("I");
    //gMessMgr->SetLevel(2);

    // note: moving the slow simu to here doesn't seem to make any difference

    //4.<-----------------------
    //
    // Trigger counter
    //
    //StTrigCounter* trigCounter = new StTrigCounter( "trigCounter", 117001 );

    //5.<-----------------------
    //
    // Energy to ADC maker
    //
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
    // create spin DB maker and info maker
    //
    if( !isMC ){
	spinDb = new StSpinDbMaker("spinDb");
	spinInfoMakerPtr = new StSpinInfoMaker_t( "SpinInfoMaker" );
    };

    // Energy Structure Maker
    energyMakerPtr = new StEEmcEnergyMaker_t( "energyMkr", "EEmcA2EMaker" );
    energyMakerPtr->setStripThres( isMC ? 0.0 : 0.001 );
    energyMakerPtr->setTowerThres( isMC ? 0.0 : 1.0 );

    // I'm told one must use slow simulator to get pedestals correct for L2,
    // but I'll put it after the A2Emaker and Energy maker, so it doesn't mess them up

    // Note: this will actually add the pedestal to the adc value, so
    // it seems this is the wrong thing to do if one already has the
    // pedestals in the adc's in the MuDst.
    if( 0 && isMC==1 )
    {
	StEEmcSlowMaker *slowSim = new StEEmcSlowMaker("slowSim");

	// note: changing the sampling fraction does not effect the
	// L0 trigger decision.  Also, it is probably better to use the value
	// in the slow simulator, so that it matches data. (S. Gliske,
	// Sept 2012)

	// This 1.3 comes from the iron/air MC, where the sampling
	// fraction was believed to be near 4%, but slow/fast simulator
	// used 5%.

	// Now that the slow/fast simulator uses 0.048, it is a 1.25 scaling factor

	// slowSim->setSamplingFraction(0.0384); // effectively scale all Tower energies with a factor of 1.3 (added by: Ilya Selyuzhenkov; April 11, 2008)

	slowSim->setAddPed(true);
	slowSim->setSmearPed(true);
    };

    //
    // Tree Maker
    //
    treeMakerPtr = new StEEmcTreeMaker_t( "EEmcTreeMkr" );
    treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::WRITE,  outputFileName );
    treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::IGNORE, "" );
    treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::IGNORE, "" );
    treeMakerPtr->setMaxNumEvents( neventsIn );
    treeMakerPtr->setEEmcEnergyMkr( energyMakerPtr );
    treeMakerPtr->doSpinInfoIO( !isMC );
    treeMakerPtr->doEvtHddrIO( 1 );
    treeMakerPtr->setHTTPthres( 0, 0 );
 


    if( !isMC )
	treeMakerPtr->setSpinInfoMkr( spinInfoMakerPtr );


    // debugging info
    std::cout << "***** Done instanciating all the classes *****" << endl;
    analysisChain->ls(3);

    //
    // Initialize all makers
    //

    std::cout << "***** Initializing all makers in the analysis chain *****" << std::endl;

    analysisChain->Init();

    std::cout << "***** Initialization done *****" << std::endl;

    //
    // Finally ready to loop over the events 
    //

    // If neventsIn/Out is negative, reset to a large value
    // for an Int_t
    if( neventsIn < 0 )
	neventsIn = 1<<30-1;
    if( neventsOut < 0 )
	neventsOut = 1<<30-1;

    Int_t ierr  = kStOK;  // err flag
    Long_t nevents = 1;    // cumulative number of events in
    for( ; nevents <= neventsIn && treeMakerPtr->getNumPart1EventsWritten() < neventsOut && !ierr; ++nevents ){
	// clear
	analysisChain->Clear();

	// make
	ierr = analysisChain->Make();

	// Print every so many events
	if( (nevents+1) % displayFreq == 1 ){
	    //trigCounter->printStatus();
	    std::cout << "***** finished event number " << nevents << ", " << treeMakerPtr->getNumPart1EventsWritten() << " *****" << std::endl;
	};

	if( ierr ){
	    std::cout << "***** ERROR FLAG " << ierr << " on event number " << nevents << " *****" << endl;
	};
    };
 
    //---------------------------------------------------------------


    //
    // Calls the ::Finish() method on all makers
    //
    analysisChain->Finish(); 

    //
    // Delete the chain
    //
    // analysisChain->Delete();
    
    
    return;
};
