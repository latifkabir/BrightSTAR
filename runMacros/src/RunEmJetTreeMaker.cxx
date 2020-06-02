#include <iostream>
#include "RootInclude.h"
#include "StJetMaker/towers/StjTowerEnergyCutFMS.h"
#include "StJetMaker/StAnaPars.h"
#include "StRootInclude.h"
#include "StJetMaker/StJetMaker2015.h"
#include "BrightStInclude.h"

using namespace std;

void RunEmJetTreeMaker(TString inFile, TString outFile)
{
    TStopwatch sw;
    sw.Start();

    TStar::gConfig->Print();
    
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

    StTriggerFilterMaker* filterMaker = new StTriggerFilterMaker;
    filterMaker->printTriggerId();

    Int_t runNumber = TStRunList::GetRunFromFileName((string)inFile);
    if(runNumber < 1)
    {
    	cout << "Unable to get run number" <<endl;
    	return;
    }

    //For EEMC EM Jet using physics stream
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "EHT0"));
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "JP1"));
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "JP2"));
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "EHT0*EJP1*L2Egamma"));
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "BHT1*VPDMB-30"));
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "VPDMB-5-ssd"));
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "BHT0*BBCMB"));
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "BHT1*BBCMB"));
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "BHT2*BBCMB"));
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "BHT1*VPDMB-30-nobsmd"));
    filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "JP2*L2JetHigh"));

    //For FMS EM Jet using FMS stream
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber, "FMS-JP0"));
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-JP1"));
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-JP2"));
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-DiJP"));
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-sm-bs1"));
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-sm-bs2"));
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-sm-bs3"));
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-lg-bs1"));
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-lg-bs2"));
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-lg-bs3"));
    // filterMaker->addTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-DiBS"));
    
    // filterMaker->addVetoTrigger(TStTrigDef::GetTrigId(runNumber,"FMS-LED"));
    
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
    TStFmsHotChDB *fmsHotChDb = new TStFmsHotChDB();
    fmsHotChDb->GetHotChList(runNumber, isHotCh);
    cout << "The following FMS cells are masked:" <<endl;
    for(int i = 0; i < 4; ++i)
    {
	for(int j = 0; j < 571; ++j)
	    if(isHotCh[i][j])
		cout << "det "<< (i + 1)<< " ch "<< (j+1) << " hot/bad status:"<< isHotCh[i][j] <<endl;
    }
    fmsDb->maskChannels(isHotCh);

    
    StEmcADCtoEMaker* adc = new StEmcADCtoEMaker;
    adc->saveAllStEvent(true);
    StFmsHitMaker*   fmshitMk = new StFmsHitMaker();
    StFmsPointMaker* fmsptMk  = new StFmsPointMaker();

    //-------------------------------------------

    StJetSkimEventMaker* skimEventMaker = new StJetSkimEventMaker("StJetSkimEventMaker", muDstMaker, Skimfile);

    StJetMaker2015* jetmaker = new StJetMaker2015();
    jetmaker->setJetFile(Jetfile);
    jetmaker->setJetFileUe(Uefile);
    jetmaker->ReadBbcSlewing("/star/u/kabir/GIT/BrightSTAR/database/bbc_slewing_run15_pp200.dat"); //CKim

    StAnaPars* anapars12 = new StAnaPars;
    anapars12->useTpc  = true;
    anapars12->useBemc = true;
    anapars12->useEemc = true;
    anapars12->useFms  = true;
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
    anapars12->addEemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars12->addEemcCut(new StjTowerEnergyCutAdc(4,3)); //ADC-ped>4 AND ADC-ped>3*RMS
    anapars12->addEemcCut(new StjTowerEnergyCutEt(0.2));

    //FMS cuts, CKim
    anapars12->addFmsCut(new StjTowerEnergyCutFMS(0.2, 200)); //min, max //Latif: changed to 0.2, it was set to 3 by Chong
    //* 3 GeV cut was determined by RUN15 calibration condition: Zgg < 0.7 + pairE > 20 GeV

    //Jet cuts
    anapars12->addJetCut(new StProtoJetCutPt(0.01,200));
    anapars12->addJetCut(new StProtoJetCutEta(-1,5)); //CKim, extend to FMS acceptance

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

    chain->Init();
    chain->EventLoop();
    chain->Finish();
    delete chain;

    sw.Stop();
    sw.Print();
    std::cout <<"Done!" <<endl;

    return;
}//Main

