#include <iostream>
#include <fstream>
#include "RootInclude.h"
#include "StJetMaker/towers/StjTowerEnergyCutEEmcSmd.h"
#include "StJetMaker/towers/StjTowerEnergyCutFMS.h"
#include "StJetMaker/StAnaPars.h"
#include "StRootInclude.h"
#include "StJetMaker/StJetMaker2015.h"
#include "BrightStInclude.h"
#include "BrJetMaker/TStNanoJetTreeMaker.h"

using namespace std;

void EjRunEmJetTreeMakerEEmcSmd(TString inFile, TString outFile, TString det, Bool_t isMC)
{
    TStopwatch sw;
    sw.Start();

    TStar::gConfig->Print();
    //Bool_t isMC;
    //isMC = kFALSE;
    //isMC = kTRUE;

    det = "eemc"; //<------------------ Only for cron job. Should be commented in all other cases

    if(gROOT->IsBatch())
	inFile = TStScheduler::CopyInputFiles(inFile);
        
    if(!(det == "fms" || det == "eemc"))
    {
	cout << "Invalid detector name" <<endl;
	return;
    }
    
    TString Jetfile = (TString)"jets_" + outFile;
    TString Uefile = (TString)"ueoc_" + outFile;
    TString Skimfile = (TString)"skim_" + outFile;

    //-------------------------------------------
    if(!TStar::gBrDebug)
    {
	gMessMgr->SetLimit("I", 0);
	gMessMgr->SetLimit("Q", 0);
	gMessMgr->SetLimit("W", 0);
    }
    //-------------------------------------------

    StChain* chain = new StChain;
    StMuDstMaker* muDstMaker = new StMuDstMaker(0, 0, "", inFile, "", 1000);
    StMuDbReader* muDstDb = StMuDbReader::instance();
    
    Int_t runNumber;    
    if(!isMC)
	runNumber = TStRunList::GetRunFromFileName((string)inFile);
    else
	runNumber = 16066000;
    if(runNumber < 1)
    {
    	cout << "Unable to get run number" <<endl;
    	return;
    }

    const Int_t kMaxTriggers = 9;
    Int_t trigIds[kMaxTriggers];
    TString triggerNames_fms[kMaxTriggers] = {"FMS-JP0", "FMS-JP1", "FMS-JP2", "FMS-sm-bs1", "FMS-sm-bs2", "FMS-sm-bs3", "FMS-lg-bs1", "FMS-lg-bs2", "FMS-lg-bs3"};
    TString triggerNames_eemc[kMaxTriggers] = {"EHT0", "JP1", "JP2", "EHT0*EJP1*L2Egamma", "JP2*L2JetHigh", "BHT1*VPDMB-30", "BHT0*BBCMB", "BHT1*BBCMB", "BHT2*BBCMB"};

    //Consider FMS or EEMC EM Jets only. Note, bulk of the jets come from mid-rapidity.
    //Unless this cut is applied, file size will be large and no need to make separate nano Dst.
    Double_t etaMax = 4.5;
    Double_t etaMin = 0.8;
    
    if(!isMC)
    {
	StTriggerFilterMaker* filterMaker = new StTriggerFilterMaker;
	filterMaker->printTriggerId();
	if(det == "fms")
	{
	    for(Int_t i = 0; i < kMaxTriggers; ++i)
	    {
		trigIds[i] = TStTrigDef::GetTrigId(runNumber, triggerNames_fms[i]);
		filterMaker->addTrigger(trigIds[i]);
	    }
	    filterMaker->addVetoTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-LED"));
	}
	else if(det == "eemc")
	{
	    for(Int_t i = 0; i < kMaxTriggers; ++i)
	    {
		trigIds[i] = TStTrigDef::GetTrigId(runNumber, triggerNames_eemc[i]);
		filterMaker->addTrigger(trigIds[i]);
	    }
	}
    }        
    St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");
    starDb->SetAttr("blacklist", "ist");
    starDb->SetAttr("blacklist", "mtd");
    starDb->SetAttr("blacklist", "pmd");
    starDb->SetAttr("blacklist", "pp2pp");
    starDb->SetAttr("blacklist", "pxl");
    starDb->SetAttr("blacklist", "ssd");
    starDb->SetAttr("blacklist", "svt");
    starDb->SetDebug();

    StEEmcDbMaker* eemcDb = new StEEmcDbMaker;
    StSpinDbMaker* spinDb = new StSpinDbMaker;
    StFmsDbMaker*  fmsDb  = new StFmsDbMaker("fmsDb"); 
    fmsDb->SetAttr("fmsGainCorr","fmsGainCorr-BNL-C");
    Bool_t isHotCh[4][571] = {0};
    Int_t nHotCh = 0;
    TStFmsHotChDB *fmsHotChDb = new TStFmsHotChDB();
    fmsHotChDb->GetHotChList(runNumber, isHotCh);
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

    
    StEmcADCtoEMaker* adc = new StEmcADCtoEMaker;
    adc->saveAllStEvent(true);
    StFmsHitMaker*   fmshitMk = new StFmsHitMaker();
    StFmsPointMaker* fmsptMk  = new StFmsPointMaker();

    //Note that the default is already: fmsptMk->SetReadMuDst(0); fmshitMk->SetReadMuDst(0); fmsptMk->setMergeSmallToLarge(1);
    
    if(isMC)
	fmshitMk->SetReadMuDst(1);                //for simu set to 1
    //-------------------------------------------

    //-------------------------------- START OF EEMC SMD MAKERS ---------------------------------------------
    StEEmcDbMaker       *eemcDbMaker      = 0;
    StEEmcA2EMaker      *a2EMakerPtr      = 0;
    StEEmcEnergyMaker_t *energyMakerPtr   = 0;
    StEEmcTreeMaker_t   *treeMakerPtr     = 0;
    StMcEEmcTreeMaker_t *mcTreeMakerPtr   = 0;
    StSpinInfoMaker_t   *spinInfoMakerPtr = 0;

    // just to make sure StEvent is found for the trig simu
    StMuDst2StEventMaker *muDst2StEvent = new StMuDst2StEventMaker();

    //
    // Initialize EEMC database
    //
    eemcDbMaker = new StEEmcDbMaker("eemcDb");

    // note: moving the slow simu to here doesn't seem to make any difference


    //~~ PART 1 ~~~
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
	//spinDb = new StSpinDbMaker("spinDb");
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
    TString outputFileName = (TString)"eemcTreeP1_" + outFile;
    treeMakerPtr = new StEEmcTreeMaker_t( "EEmcTreeMkr" );
    treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::WRITE,  outputFileName );
    treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::IGNORE, "" );
    treeMakerPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::IGNORE, "" );
    treeMakerPtr->setMaxNumEvents( muDstMaker->chain()->GetEntries() );
    treeMakerPtr->setEEmcEnergyMkr( energyMakerPtr );
    treeMakerPtr->doSpinInfoIO( !isMC );
    treeMakerPtr->doEvtHddrIO( 1 );
    treeMakerPtr->setHTTPthres( 0, 0 );
    //treeMakerPtr->setNumTowerThres(-1); //This will cause writing event header for every event. Will takes lots of unnecessary space.


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

    //-------------------------------- END OF EEMC SMD MAKERS ---------------------------------------------

    //----------------- Jet Finder Configuration -------------------------
    StJetSkimEventMaker* skimEventMaker = new StJetSkimEventMaker("StJetSkimEventMaker", muDstMaker, Skimfile);

    StJetMaker2015* jetmaker = new StJetMaker2015("StJetMaker2015");
    jetmaker->setJetFile(Jetfile);
    jetmaker->setJetFileUe(Uefile);
    TString bbcSlewingData = TStar::gConfig->GetStarHome() + "/database/bbc_slewing_run15_pp200.dat"; 
    jetmaker->ReadBbcSlewing(bbcSlewingData.Data()); //CKim

    StAnaPars* anapars12 = new StAnaPars;
    anapars12->useTpc  = true;
    anapars12->useBemc = false;  //Do not use BEMC, otherwise it will leak to EEMC -- Latif
    anapars12->useEemc = false;
    if(det == "fms")
	anapars12->useFms  = true;
    else if(det == "eemc")
	anapars12->useEemcSmd = true;
    anapars12->useEmJetMode = true; //Exclude TPC tracks
    //anapars12->useFmsHit = true; //CKim
    anapars12->addTpcCut(new StjTrackCutFlag(0));
    anapars12->addTpcCut(new StjTrackCutNHits(12));
    anapars12->addTpcCut(new StjTrackCutPossibleHitRatio(0.51));
    anapars12->addTpcCut(new StjTrackCutDca(3));
    anapars12->addTpcCut(new StjTrackCutPt(0.2,200));
    anapars12->addTpcCut(new StjTrackCutEta(-2.5,2.5));
    anapars12->addTpcCut(new StjTrackCutLastPoint(125));

    //BEMC cuts
    anapars12->addBemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars12->addBemcCut(new StjTowerEnergyCutAdc(4,3)); //ADC-ped>4 AND ADC-ped>3*RMS
    anapars12->addBemcCut(new StjTowerEnergyCutEt(0.2));

    //EEMC cuts
    // anapars12->addEemcCut(new StjTowerEnergyCutBemcStatus(1));
    // anapars12->addEemcCut(new StjTowerEnergyCutAdc(4,3)); //ADC-ped>4 AND ADC-ped>3*RMS
    // anapars12->addEemcCut(new StjTowerEnergyCutEt(0.2));

    // EEMC SMD Cut
    anapars12->addEemcSmdCut(new StjTowerEnergyCutEEmcSmd(2.0, 200)); //!!!!!!!!!! <------------ To be updated 
    
    //FMS cuts, CKim
    //anapars12->addFmsCut(new StjTowerEnergyCutFMS(0.2, 200)); //min, max //Latif: changed to 0.2, it was set to 3 by Chong
    //anapars12->addFmsCut(new StjTowerEnergyCutFMS(0.5, 200)); //!!!!!!!!!! For Comparing With Zhanwen's Result Only !!!!!!
    anapars12->addFmsCut(new StjTowerEnergyCutFMS(2.0, 200)); //!!!!!!!!!! Updated based on Simulation !!!!!!
    //* 3 GeV cut was determined by RUN15 calibration condition: Zgg < 0.7 + pairE > 20 GeV

    //Jet cuts
    anapars12->addJetCut(new StProtoJetCutPt(0.01,200));
    //anapars12->addJetCut(new StProtoJetCutEta(-1,5)); //CKim, extend to FMS acceptance
    anapars12->addJetCut(new StProtoJetCutEta(0.8, 5)); //For FMS and EEMC EM-jet analysis

    //Set anti-kt R=0.7 parameters
    StFastJetPars* AntiKtR070Pars = new StFastJetPars;
    AntiKtR070Pars->setJetAlgorithm(StFastJetPars::antikt_algorithm);
    AntiKtR070Pars->setRparam(0.7);
    AntiKtR070Pars->setRecombinationScheme(StFastJetPars::E_scheme);
    AntiKtR070Pars->setStrategy(StFastJetPars::Best);
    AntiKtR070Pars->setPtMin(2);

    jetmaker->addBranch("AntiKtR070NHits12",anapars12,AntiKtR070Pars);
    StOffAxisConesPars *off070 = new StOffAxisConesPars(0.7);
    jetmaker->addUeBranch("OffAxisConesR070", off070);

    TStNanoJetTreeMaker *nanoMaker = new TStNanoJetTreeMaker(jetmaker, skimEventMaker, "NanoJetTreeMaker");
    nanoMaker->SetTrigIds(trigIds);
    nanoMaker->SetOutFileName((TString)"NanoJetTree_" + outFile);
    nanoMaker->SetEtaMax(etaMax);
    nanoMaker->SetEtaMin(etaMin);
    
    chain->Init();
    chain->EventLoop();
    chain->Finish();
    delete chain;

    sw.Stop();
    sw.Print();

    if(gROOT->IsBatch() && inFile.Contains("/tmp/"))
	TStScheduler::DeleteTempFiles(inFile);

    cout << "-----------> Deleting Original jet finder files !!! <--------------------" <<endl;
    gROOT->ProcessLine(".! rm jets_*.root ueoc_*root skim_*.root eemcTree*.root");
    
    std::cout <<"Done!" <<endl;
    
    return;
}//Main

