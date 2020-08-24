
// #include <iostream>
// #include "RootInclude.h"
// #include "StRootInclude.h"

// #include "StJetMaker/StJetMaker2015.h"

void RunJetMaker(Int_t nEvents = 5, const char* inFile = "pythiaOut200GeV_Pt_2_80.MuDst.root", const char* pythiaFile = "pythiaOut200GeV_Pt_2_80.pythia.root", const char* geantfile = "pythiaOut200GeV_Pt_2_80.geant.root", const char* outFile = "test.root")
{

    gSystem->Load("StTriggerUtilities");
    gSystem->Load("StFmsTriggerMaker");
    
    StChain* chain = new StChain("StChain");
    chain->SetDEBUG(0);
    StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",inFile,".",1000000,"MuDst");
    StMuDbReader* muDB = StMuDbReader::instance();
    //	gMessMgr->SwitchOff("D");
    //	gMessMgr->SwitchOff("I");	

    St_db_Maker* dbMk = new St_db_Maker("StarDb","MySQL:StarDb");
    dbMk->SetDateTime(20150417,193427);
    dbMk->SetAttr("blacklist", "tpc");
    dbMk->SetAttr("blacklist", "svt");
    dbMk->SetAttr("blacklist", "ssd");
    dbMk->SetAttr("blacklist", "ist");
    dbMk->SetAttr("blacklist", "pxl");
    dbMk->SetAttr("blacklist", "pp2pp");
    dbMk->SetAttr("blacklist", "ftpc");
    dbMk->SetAttr("blacklist", "emc");
    dbMk->SetAttr("blacklist", "eemc");
    dbMk->SetAttr("blacklist", "mtd");
    dbMk->SetAttr("blacklist", "pmd");
    dbMk->SetAttr("blacklist", "tof");

    dbMk->SetFlavor("ofl");


    StFmsDbMaker* fmsdb = new StFmsDbMaker("fmsDb");
    // fmsdb->readGainFromText(1);
    fmsdb->forceUniformGainCorrection(1.0);
    //	  fmsdb->setDebug(1);

    // Endcap database
    StEEmcDbMaker* eemcDb = new StEEmcDbMaker;
    // Barrel ADC to energy maker
    StEmcADCtoEMaker* adc = new StEmcADCtoEMaker;
    adc->saveAllStEvent(true);

    StFmsHitMaker* fmshitMk = new StFmsHitMaker();
    //	    fmshitMk->SetDEBUG();
    //	    fmshitMk->SetTimeDepCorr(1);		  //default 0
    fmshitMk->SetReadMuDst(1);                //for simu set to 1
    //	    fmshitMk->SetCorrectAdcOffByOne(0);           //default 1

    StFmsPointMaker* fmsptMk = new StFmsPointMaker();
    //	fmsptMk->setMergeSmallToLarge();
    //	fmsptMk->setCategorizationAlgo();

    fmsptMk->setTry1PhotonFit();
    fmsptMk->setScaleShowerShape();

    cout<<"point done"<<endl;

    Float_t jetPtMin = 1.5;
    //jetmaker->setJetFile(jetfile);

    // Set anti-kt R=0.7 parameters
    StFastJetPars* AntiKtR070Pars = new StFastJetPars;
    AntiKtR070Pars->setJetAlgorithm(StFastJetPars::antikt_algorithm);
    AntiKtR070Pars->setRparam(0.7);
    AntiKtR070Pars->setRecombinationScheme(StFastJetPars::E_scheme);
    AntiKtR070Pars->setStrategy(StFastJetPars::Best);
    AntiKtR070Pars->setPtMin(jetPtMin);
    // new
    StAnaPars* anapars12 = new StAnaPars;
    anapars12->useTpc  = true;
    anapars12->useBemc = false;
    anapars12->useEemc = false;
    anapars12->addTpcCut(new StjTrackCutFlag(0));
    anapars12->addTpcCut(new StjTrackCutNHits(12));
    anapars12->addTpcCut(new StjTrackCutPossibleHitRatio(0.51));
    anapars12->addTpcCut(new StjTrackCutDca(3));
    anapars12->addTpcCut(new StjTrackCutDcaPtDependent);
    anapars12->addTpcCut(new StjTrackCutTdcaPtDependent);
    anapars12->addTpcCut(new StjTrackCutChi2(0,4));
    anapars12->addTpcCut(new StjTrackCutPt(0.2,200));
    anapars12->addTpcCut(new StjTrackCutEta(-2.5,2.5));
    anapars12->addTpcCut(new StjTrackCutLastPoint(125));


    anapars12->addBemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars12->addBemcCut(new StjTowerEnergyCutAdc(4,3)); // ADC-ped>4 AND ADC-ped>3*RMS
    anapars12->addBemcCut(new StjTowerEnergyCutEt(0.2));
    anapars12->addEemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars12->addEemcCut(new StjTowerEnergyCutAdc(4,3)); // ADC-ped>4 AND ADC-ped>3*RMS
    anapars12->addEemcCut(new StjTowerEnergyCutEt(0.2));
    anapars12->addJetCut(new StProtoJetCutPt(.01,200));
    anapars12->addJetCut(new StProtoJetCutEta(0,5));

    //for pjet
    St_pythia_Maker* pythia = new St_pythia_Maker;
    pythia->SetFile(pythiaFile);
    // Pythia4pMaker
    StPythiaFourPMaker* pythia4pMaker = new StPythiaFourPMaker;
    // Instantiate the JetMaker

    //
    // GEANT reader
   
    StIOMaker* ioMaker = new StIOMaker;
    ioMaker->SetFile(geantfile);
    ioMaker->SetIOMode("r");
    ioMaker->SetBranch("*",0,"0");             // Deactivate all branches  
    ioMaker->SetBranch("geantBranch",0,"r");   // Activate geant branch    

    
    StTriggerSimuMaker* simuTrig = new StTriggerSimuMaker;
    simuTrig->setMC(1); // Must be before individual detectors, to be passed    
    //   simuTrig->useOnlineDB();
    simuTrig->InitRun(11098017);
    simuTrig->useOfflineDB();


    // FMS trigger simulator     
    // StFmsTriggerMaker* fmstrig = new StFmsTriggerMaker;
    // fmstrig->useMuDst();
    //fmstrig->InitRun();

    
    StJetMaker2015* jetmaker = new StJetMaker2015;
    jetmaker->setJetFile(outFile);
    jetmaker->setJetFileUe("ue.root");

    // Set analysis cuts for particle jets branch
    StAnaPars* anaparsParticle = new StAnaPars;
    anaparsParticle->useMonteCarlo = true;
    anaparsParticle->addMcCut(new StjMCParticleCutStatus(1)); // final state particles
    //anaparsParticle->addMcCut(new StjMCParticleCutEM(111)); // photon only
    anaparsParticle->addJetCut(new StProtoJetCutPt(1.5,200));
    anaparsParticle->addJetCut(new StProtoJetCutEta(2,5));

    StAnaPars* anaparsParticleAll = new StAnaPars;
    anaparsParticleAll->useMonteCarlo = true;
    anaparsParticleAll->addMcCut(new StjMCParticleCutStatus(1)); // final state particles
    anaparsParticleAll->addJetCut(new StProtoJetCutPt(1.5,200));
    anaparsParticleAll->addJetCut(new StProtoJetCutEta(2,5));

    // Set analysis cuts for parton jets branch
    StAnaPars* anaparsParton = new StAnaPars;
    anaparsParton->useMonteCarlo = true;
    // MC cuts
    anaparsParton->addMcCut(new StjMCParticleCutParton);
    // Jet cuts
    anaparsParton->addJetCut(new StProtoJetCutPt(3,200));
    anaparsParton->addJetCut(new StProtoJetCutEta(-100,100));

	
    //jetmakerFMS->addBranch("AntiKtR070FMS",anapars12,AntiKtR070Pars);
    jetmaker->addBranch("AntiKtR070NHits12",anaparsParticle,AntiKtR070Pars);
    // jetmaker->addBranch("AntiKtR070ParticleAll",anaparsParticleAll,AntiKtR070Pars);
	
    
    cout<<" start chain    "<<endl;
    chain->Init();
    if(nEvents == -1)
	chain->EventLoop();
    else
	chain->EventLoop(nEvents);
    chain->Finish();
    delete chain;

}

