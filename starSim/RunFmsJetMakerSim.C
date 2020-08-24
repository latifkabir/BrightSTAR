#include "TChain.h"
#include "TSystem.h"
#include "TFile.h"
#include <iostream>

class StMuDstMaker;
StMuDstMaker* maker;

void RunFmsJetMakerSim(Int_t nEvents = -1, const char* inFile = "pythiaOut.MuDst.root", const char* pythiaFile = "pythiaOut.pythia.root", const char* outFile = "fmsJet.root")
{
    //	gROOT->Macro("load.C");
    // gROOT->Macro("loadMuDst.C");
    // gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    // loadSharedLibraries();
    // gROOT->Macro("LoadLogger.C");
    // gSystem->Load("StTpcDb");
    // gSystem->Load("StDetectorDbMaker");
    // gSystem->Load("StDbUtilities");
    // gSystem->Load("StDaqLib");
    // gSystem->Load("StEmcRawMaker");
    // gSystem->Load("StEmcADCtoEMaker");
    // gSystem->Load("StPreEclMaker");
    // gSystem->Load("StEpcMaker");
    // gSystem->Load("StDbBroker");
    // gSystem->Load("St_db_Maker");
    // gSystem->Load("StEEmcUtil");
    // gSystem->Load("StEEmcDbMaker");
    // gSystem->Load("StSpinDbMaker");
    // gSystem->Load("StEmcTriggerMaker");
    // gSystem->Load("StTriggerUtilities");
    // gSystem->Load("StMCAsymMaker");
    // gSystem->Load("StRandomSelector");

    // //after SL6 upgrade
    // gSystem->Load("libfastjet.so");
    // gSystem->Load("libsiscone.so");
    // gSystem->Load("libsiscone_spherical.so");
    // gSystem->Load("libfastjetplugins.so");

    // gSystem->Load("StJetFinder");
    // gSystem->Load("StJetSkimEvent");
    // gSystem->Load("StJets");
    // gSystem->Load("StJetEvent");
    // //gSystem->Load(".sl73_gcc485/obj/StRoot/StJetMaker/StJetMaker.so");


    // // Load your shared libraries here
    // gSystem->Load("StFmsUtil");
    // gSystem->Load("StFmsDbMaker");
    // //gSystem->Load("StTriggerDataMaker");
    // //	gSystem->Load("StFmsJetMaker");
    // gSystem->Load("libMinuit.so");

    // gSystem->Load("StTriggerFilterMaker");
    // gSystem->Load("StFmsHitMaker");
    // gSystem->Load("StFmsPointMaker");
    // //gSystem->Load("StFmsAnalysisMaker");
    // //	gSystem->Load("StFmsQAHistoMaker");

    // std::cout<<"load done"<<endl;
    StChain* chain = new StChain("StChain");
    chain->SetDEBUG(0);
    StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",inFile,".",1000000,"MuDst");
    StMuDbReader* muDB = StMuDbReader::instance();
    //	gMessMgr->SwitchOff("D");
    //	gMessMgr->SwitchOff("I");	

    //	St_db_Maker *dbMk = new St_db_Maker("db","MySQL:StarDb","$STAR/StarDb");
    //	StSpinDbMaker *spDb=new StSpinDbMaker("spinDb"); 
    //	dbMk->SetDEBUG(0);
    //	dbMk->SetDateTime(20110601, 0);

    St_db_Maker* dbMk = new St_db_Maker("StarDb","MySQL:StarDb");
    dbMk->SetDateTime(20110417,193427);
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
    fmsdb->readGainFromText(1);
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

    // TString skimfile = "skim.root";
    // StJetSkimEventMaker* skimEventMaker = new StJetSkimEventMaker("StJetSkimEventMaker",muDstMaker,skimfile);
    //StFmsJetMaker* jetmakerFMS = new StFmsJetMaker();
    // StJetMaker2009* jetmaker = new StJetMaker2009("StJetMaker2009");
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
    jetmaker->addBranch("AntiKtR070Particle",anaparsParticle,AntiKtR070Pars);
    jetmaker->addBranch("AntiKtR070ParticleAll",anaparsParticleAll,AntiKtR070Pars);
	
    //move all this before jet finder 

    //StFmsAnalysisMaker* fms = new StFmsAnalysisMaker("StFmsAnalysisMaker",jetmakerFMS, jetmaker,pythiaFile);
    //	fms->EnableEdepCor = true;// have to do it here for jet finding;
    //fms->SetOutputFile(outFile);

    // const Int_t kMaxTriggers = 9;
    // Int_t trigIds[kMaxTriggers];
    // TStNanoJetTreeMaker *nanoMaker = new TStNanoJetTreeMaker(jetmaker, skimEventMaker, "NanoJetTreeMaker");
    // nanoMaker->SetTrigIds(trigIds);
    
    cout<<" start chain    "<<endl;
    chain->Init();
    if(nEvents == -1)
	chain->EventLoop();
    else
	chain->EventLoop(nEvents);
    chain->Finish();
    delete chain;

}

