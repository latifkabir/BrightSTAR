// Produces both Zilong's off axis cones and
// Grant's regions underlying event trees
// Original script name: RunJetFinder2013UePro
void RunJetFinder(TString mudstfile = "",
		  const char* jetfile = "st_physics_new.jets.root",
		  const char* skimfile = "st_physics_new.skim.root",
		  const char* conesfile = "st_physics_new.ueCones.root",
		  const char* regionsfile = "st_physics_new.ueRegions.root"
    )
{
    mudstfile = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/072/16072047/st_physics_16072047_raw_1000005.MuDst.root";
    int nevents = 1e+8;
    printf("mudstfile: %s\n",mudstfile);
    printf("jetfile: %s\n",jetfile);
    printf("skimfile: %s\n",skimfile);
    printf("conesfile: %s\n",conesfile);
    printf("regionsfile: %s\n",regionsfile);

    gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    loadSharedLibraries();

    //gROOT->Macro("loadMuDst.C");
    //gROOT->Macro("LoadLogger.C");
    gSystem->Load("StDetectorDbMaker");
    gSystem->Load("StTpcDb");
    gSystem->Load("StDbUtilities");
    gSystem->Load("StMcEvent");
    gSystem->Load("StMcEventMaker");
    gSystem->Load("StDaqLib");
    gSystem->Load("StEmcRawMaker");
    gSystem->Load("StEmcADCtoEMaker");
    gSystem->Load("StEpcMaker");
    gSystem->Load("StEmcSimulatorMaker");
    gSystem->Load("StDbBroker");
    gSystem->Load("St_db_Maker");
    gSystem->Load("StEEmcUtil");
    gSystem->Load("StEEmcDbMaker");
    gSystem->Load("StSpinDbMaker");
    gSystem->Load("StEmcTriggerMaker");
    gSystem->Load("StTriggerUtilities");
    gSystem->Load("StMCAsymMaker");
    gSystem->Load("StRandomSelector");
    gSystem->Load("libfastjet.so");
    gSystem->Load("libsiscone.so");
    gSystem->Load("libsiscone_spherical.so");
    gSystem->Load("libfastjetplugins.so");
    gSystem->Load("StJetFinder");
    gSystem->Load("StJetSkimEvent");
    gSystem->Load("StJets");
    gSystem->Load("StJetEvent");
    gSystem->Load("StUeEvent");
    gSystem->Load("StJetMaker");
    gSystem->Load("StTriggerFilterMaker");

    StChain* chain = new StChain; 

    // MuDst reader
    StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",mudstfile,"",100000,"MuDst");

    // MuDst DB
    StMuDbReader* muDstDb = StMuDbReader::instance();

    // Trigger filter
    StTriggerFilterMaker* filterMaker = new StTriggerFilterMaker;

    // run15 pp200
    //BBCMB 
    filterMaker->addTrigger(490003);
    filterMaker->addTrigger(480501);
    //filterMaker->addTrigger(430015);
    //VPDMB
    /* (Run 13 Values)
       filterMaker->addTrigger(430001);
       filterMaker->addTrigger(430011);
       filterMaker->addTrigger(430021);
       filterMaker->addTrigger(430031);
    */
    //VPDMB-novtx (Run 15)
    filterMaker->addTrigger(490004);
    //VPDMB-30 (Run 15)
    filterMaker->addTrigger(490904);
    /**JP0
       filterMaker->addTrigger(430401);
       filterMaker->addTrigger(430411);
       filterMaker->addTrigger(430421);
       filterMaker->addTrigger(430431);
       //JP0DiJet
       filterMaker->addTrigger(430407);
       filterMaker->addTrigger(430417);
       filterMaker->addTrigger(430427);
       filterMaker->addTrigger(430437);
    **/
    //JP1 (All updated to Run15 values)
    filterMaker->addTrigger(490404);
    filterMaker->addTrigger(470404); //Trigger is for Runs outside of runlist I am using
    /**JP1DiJet
       filterMaker->addTrigger(430406);
       filterMaker->addTrigger(430416);
       filterMaker->addTrigger(430426);
       filterMaker->addTrigger(430436);
    **/
    //JP2
    filterMaker->addTrigger(490401);
    filterMaker->addTrigger(470401); //Trigger is for Runs outside of runlist I am using
    //JP2-bsmd
    filterMaker->addTrigger(470402); //Trigger is for Runs outside of runlist I am using
    filterMaker->addTrigger(490402);
    //AJP
    filterMaker->addTrigger(470403); //Trigger is for Runs outside of runlist I am using
    filterMaker->addTrigger(490403);
    /* What follows are triggers for Run 13 that I am not using
    //BAJP
    filterMaker->addTrigger(430424);
    filterMaker->addTrigger(430444);
    //EEMCDiJet
    filterMaker->addTrigger(430408);
    filterMaker->addTrigger(430418);
    */
    // star database
    St_db_Maker* starDb = new St_db_Maker("StarDb","MySQL:StarDb");

    // Endcap database
    StEEmcDbMaker* eemcDb = new StEEmcDbMaker;

    // Spin database
    StSpinDbMaker* spinDb = new StSpinDbMaker;

    // Barrel ADC to energy maker
    StEmcADCtoEMaker* adc = new StEmcADCtoEMaker;
    adc->saveAllStEvent(true);

    // Trigger simulator
    StTriggerSimuMaker* simuTrig = new StTriggerSimuMaker;
    simuTrig->useOnlineDB(); // for trigger definitions and thresholds
    simuTrig->setMC(false); // Must be before individual detectors, to be passed
    simuTrig->useBbc();
    simuTrig->useBemc();
    simuTrig->useEemc();
    simuTrig->bemc->setConfig(StBemcTriggerSimu::kOnline);

    // Skim event maker
    StJetSkimEventMaker* skimEventMaker = new StJetSkimEventMaker("StJetSkimEventMaker",muDstMaker,skimfile);

    // 2012 jet maker for UE cones method
    StJetMaker2012* jetmaker = new StJetMaker2012;
    jetmaker->setJetFile(jetfile);
    jetmaker->setJetFileUe(conesfile);

    // 2009 jet maker for UE regions method
    StUEMaker2009* uemaker = new StUEMaker2009;
    uemaker->setUeFile(regionsfile);

    /***************  Set analysis cuts for 12-point branch ***************/
    StAnaPars* anapars12 = new StAnaPars;
    anapars12->useTpc  = true;
    anapars12->useBemc = true;
    anapars12->useEemc = true;
    //anapars12->useEemc = false;

    // The classes available for correcting tower energy for tracks are:
    // 1. StjTowerEnergyCorrectionForTracksMip
    // 2. StjTowerEnergyCorrectionForTracksFraction
    // 3. StjTowerEnergyCorrectionForTracksNull (default: no correction)
    anapars12->setTowerEnergyCorrection(new StjTowerEnergyCorrectionForTracksFraction(1.00));
    anapars12->addTpcCut(new StjTrackCutFlag(0));
    anapars12->addTpcCut(new StjTrackCutNHits(12));
    anapars12->addTpcCut(new StjTrackCutPossibleHitRatio(0.51));
    anapars12->addTpcCut(new StjTrackCutDca(3));
    anapars12->addTpcCut(new StjTrackCutTdcaPtDependent);
    //anapars12->addTpcCut(new StjTrackCutChi2(0,4));
    anapars12->addTpcCut(new StjTrackCutPt(0.2,200));
    anapars12->addTpcCut(new StjTrackCutEta(-2.5,2.5));
    anapars12->addTpcCut(new StjTrackCutLastPoint(125));

    // BEMC cuts
    anapars12->addBemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars12->addBemcCut(new StjTowerEnergyCutAdc(4,3));	// ADC-ped>4 AND ADC-ped>3*RMS
    anapars12->addBemcCut(new StjTowerEnergyCutEt(0.2));

    // EEMC cuts
    anapars12->addEemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars12->addEemcCut(new StjTowerEnergyCutAdc(4,3));	// ADC-ped>4 AND ADC-ped>3*RMS
    anapars12->addEemcCut(new StjTowerEnergyCutEt(0.2));

    // Jet cuts
    anapars12->addJetCut(new StProtoJetCutPt(5,200));
    anapars12->addJetCut(new StProtoJetCutEta(-100,100));

    //---------Region Criteria---------
    StAnaPars *anapars12_toward = new StAnaPars(*anapars12); // Toward Region for Tracks and Towers
    anapars12_toward->setTrackRegion(new StjTrackRegion(60.0,-60.0,1.0));
    anapars12_toward->setTowerRegion(new StjTowerRegion(60.0,-60.0,1.0));

    StAnaPars *anapars12_away = new StAnaPars(*anapars12); // Away Region for Tracks and Towers
    anapars12_away->setTrackRegion(new StjTrackRegion(120.0,-120.0,1.0));
    anapars12_away->setTowerRegion(new StjTowerRegion(120.0,-120.0,1.0));

    StAnaPars *anapars12_transPlus = new StAnaPars(*anapars12); // Trans Plus for Tracks and Towers
    anapars12_transPlus->setTrackRegion(new StjTrackRegion(120.0,60.0,1.0));
    anapars12_transPlus->setTowerRegion(new StjTowerRegion(120.0,60.0,1.0));

    StAnaPars *anapars12_transMinus = new StAnaPars(*anapars12); // Trans Minus for Tracks and Towers
    anapars12_transMinus->setTrackRegion(new StjTrackRegion(-60.0,-120.0,1.0));
    anapars12_transMinus->setTowerRegion(new StjTowerRegion(-60.0,-120.0,1.0));
    //---------------------------------

    //=======Settings for 5-point Branch===============
    StAnaPars *anapars5 = new StAnaPars;
    anapars5->useTpc  = true;
    anapars5->useBemc = true;
    anapars5->useEemc = true;
    anapars5->useFms = true;
	
    // The classes available for correcting tower energy for tracks are:
    // 1. StjTowerEnergyCorrectionForTracksMip
    // 2. StjTowerEnergyCorrectionForTracksFraction
    // 3. StjTowerEnergyCorrectionForTracksNull (default: no correction)
    anapars5->setTowerEnergyCorrection(new StjTowerEnergyCorrectionForTracksFraction(1.00));

    // TPC cuts
    anapars5->addTpcCut(new StjTrackCutFlag(0));
    anapars5->addTpcCut(new StjTrackCutNHits(5));
    anapars5->addTpcCut(new StjTrackCutPossibleHitRatio(0.51));
    anapars5->addTpcCut(new StjTrackCutDca(3));
    anapars5->addTpcCut(new StjTrackCutTdcaPtDependent);
    //anapars5->addTpcCut(new StjTrackCutChi2(0,4));
    anapars5->addTpcCut(new StjTrackCutPt(0.2,200));
    anapars5->addTpcCut(new StjTrackCutEta(-2.5,2.5));

    // BEMC cuts
    anapars5->addBemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars5->addBemcCut(new StjTowerEnergyCutAdc(4,3)); // ADC-ped>4 AND ADC-ped>3*RMS
    anapars5->addBemcCut(new StjTowerEnergyCutEt(0.2));

    // EEMC cuts
    anapars5->addEemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars5->addEemcCut(new StjTowerEnergyCutAdc(4,3)); // ADC-ped>4 AND ADC-ped>3*RMS
    anapars5->addEemcCut(new StjTowerEnergyCutEt(0.2));

    // Jet cuts
    anapars5->addJetCut(new StProtoJetCutPt(5,200));
    anapars5->addJetCut(new StProtoJetCutEta(-100,100));

    /*
    //---------Region Criteria 5 point Branch---------
    StAnaPars *anapars5_toward = new StAnaPars(*anapars5); // Toward Region for Tracks and Towers
    anapars5_toward->setTrackRegion(new StjTrackRegion(60.0,-60.0,1.0));
    anapars5_toward->setTowerRegion(new StjTowerRegion(60.0,-60.0,1.0));

    StAnaPars *anapars5_away = new StAnaPars(*anapars5); // Away Region for Tracks and Towers
    anapars5_away->setTrackRegion(new StjTrackRegion(120.0,-120.0,1.0));
    anapars5_away->setTowerRegion(new StjTowerRegion(120.0,-120.0,1.0));

    StAnaPars *anapars5_transPlus = new StAnaPars(*anapars5); // Trans Plus for Tracks and Towers
    anapars5_transPlus->setTrackRegion(new StjTrackRegion(120.0,60.0,1.0));
    anapars5_transPlus->setTowerRegion(new StjTowerRegion(120.0,60.0,1.0));

    StAnaPars *anapars5_transMinus = new StAnaPars(*anapars5); // Trans Minus for Tracks and Towers
    anapars5_transMinus->setTrackRegion(new StjTrackRegion(-60.0,-120.0,1.0));
    anapars5_transMinus->setTowerRegion(new StjTowerRegion(-60.0,-120.0,1.0));
    //---------------------------------
    */
	
    //Jet Area 
    StFastJetAreaPars *JetAreaPars = new StFastJetAreaPars;
    JetAreaPars->setGhostArea(0.04);

    // Set anti-kt R=0.5 parameters
    StFastJetPars* AntiKtR050Pars = new StFastJetPars;
    AntiKtR050Pars->setJetAlgorithm(StFastJetPars::antikt_algorithm);
    AntiKtR050Pars->setRparam(0.5);
    AntiKtR050Pars->setRecombinationScheme(StFastJetPars::E_scheme);
    AntiKtR050Pars->setStrategy(StFastJetPars::Best);
    AntiKtR050Pars->setPtMin(5.0);
    AntiKtR050Pars->setJetArea(JetAreaPars);

    // Set anti-kt R=0.6 parameters
    StFastJetPars* AntiKtR060Pars = new StFastJetPars;
    AntiKtR060Pars->setJetAlgorithm(StFastJetPars::antikt_algorithm);
    AntiKtR060Pars->setRparam(0.6);
    AntiKtR060Pars->setRecombinationScheme(StFastJetPars::E_scheme);
    AntiKtR060Pars->setStrategy(StFastJetPars::Best);
    AntiKtR060Pars->setPtMin(5.0);
    AntiKtR060Pars->setJetArea(JetAreaPars);


    // Set anti-kt R=0.7 parameters
    StFastJetPars* AntiKtR070Pars = new StFastJetPars;
    AntiKtR070Pars->setJetAlgorithm(StFastJetPars::antikt_algorithm);
    AntiKtR070Pars->setRparam(0.7);
    AntiKtR070Pars->setRecombinationScheme(StFastJetPars::E_scheme);
    AntiKtR070Pars->setStrategy(StFastJetPars::Best);
    AntiKtR070Pars->setPtMin(5.0);
    AntiKtR070Pars->setJetArea(JetAreaPars);


    jetmaker->addBranch("AntiKtR050NHits12",anapars12,AntiKtR050Pars); 
    jetmaker->addBranch("AntiKtR060NHits12",anapars12,AntiKtR060Pars);
    //jetmaker->addBranch("AntiKtR050NHits5",anapars5,AntiKtR050Pars); 
    //jetmaker->addBranch("AntiKtR060NHits5",anapars5,AntiKtR060Pars);
    jetmaker->addBranch("AntiKtR070NHits12",anapars12,AntiKtR070Pars);
    StOffAxisConesPars *off050 = new StOffAxisConesPars(0.5);
    StOffAxisConesPars *off060 = new StOffAxisConesPars(0.6);
    StOffAxisConesPars *off070 = new StOffAxisConesPars(0.7);
    jetmaker->addUeBranch("OffAxisConesR050", off050);
    jetmaker->addUeBranch("OffAxisConesR060", off060);
    jetmaker->addUeBranch("OffAxisConesR070", off070);
    uemaker->addBranch("toward",anapars12_toward,"AntiKtR060NHits12");
    uemaker->addBranch("away",anapars12_away,"AntiKtR060NHits12");
    uemaker->addBranch("transP",anapars12_transPlus,"AntiKtR060NHits12");
    uemaker->addBranch("transM",anapars12_transMinus,"AntiKtR060NHits12");
    uemaker->addBranch("toward",anapars12_toward,"AntiKtR070NHits12");
    uemaker->addBranch("away",anapars12_away,"AntiKtR070NHits12");
    uemaker->addBranch("transP",anapars12_transPlus,"AntiKtR070NHits12");
    uemaker->addBranch("transM",anapars12_transMinus,"AntiKtR070NHits12");
    //uemaker->addBranch("toward",anapars5_toward,"AntiKtR060NHits5");
    //uemaker->addBranch("away",anapars5_away,"AntiKtR060NHits5");
    //uemaker->addBranch("transP",anapars5_transPlus,"AntiKtR060NHits5");
    //uemaker->addBranch("transM",anapars5_transMinus,"AntiKtR060NHits5");

    // Run
    chain->Init();
    chain->EventLoop(nevents);
}

