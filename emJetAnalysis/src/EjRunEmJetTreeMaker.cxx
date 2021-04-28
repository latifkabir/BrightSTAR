#include <iostream>
#include <fstream>
#include "RootInclude.h"
#include "StJetMaker/towers/StjTowerEnergyCutFMS.h"
#include "StJetMaker/towers/StjTowerEnergyCutEnergy.h"
#include "StJetMaker/StAnaPars.h"
#include "StRootInclude.h"
#include "StJetMaker/StEmVertexMaker.h"
#include "StJetMaker/StJetMaker2015.h"
#include "BrightStInclude.h"
#include "BrJetMaker/TStNanoJetTreeMaker.h"
#include "EjAna.h"

using namespace std;

void EjRunEmJetTreeMaker(TString inFile, TString outFile, TString det, Bool_t isMC)
{
    TStopwatch sw;
    sw.Start();

    TStar::gConfig->Print();
    EjAna::Print();
    
    if(gROOT->IsBatch())
    {
	inFile = TStScheduler::CopyInputFiles(inFile);
	det = EjAna::kDefaultDet;
    }
        
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
    // // --> Do not use trigger simulator's result for FMS data or EM jet, no implementation for FMS trigger
    StTriggerSimuMaker* simuTrig = new StTriggerSimuMaker;
    simuTrig->useOnlineDB();
    simuTrig->setMC(isMC);

    //simuTrig->useBbc();
    simuTrig->useBemc();
    simuTrig->useEemc();
    simuTrig->bemc->setConfig(StBemcTriggerSimu::kOnline);

    StEmVertexMaker *emVertexMkr = new StEmVertexMaker("StEmVertexMaker");
    TString bbcSlewingData = TStar::gConfig->GetStarHome() + "/database/bbc_slewing_run15_pp200.dat"; 
    emVertexMkr->ReadBbcSlewing(bbcSlewingData.Data()); //CKim
    
    StJetSkimEventMaker* skimEventMaker = new StJetSkimEventMaker("StJetSkimEventMaker", muDstMaker, Skimfile);

    StJetMaker2015* jetmaker = new StJetMaker2015("StJetMaker2015");
    jetmaker->setJetFile(Jetfile);
    jetmaker->setJetFileUe(Uefile);
 
    StAnaPars* anapars12 = new StAnaPars;
    anapars12->useTpc  = true;
    anapars12->useBemc = false;
    if(det == "eemc")	    
	anapars12->useEemc = true;
    else if(det == "fms")
	anapars12->useFms  = true;
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
    anapars12->addEemcCut(new StjTowerEnergyCutBemcStatus(1));
    anapars12->addEemcCut(new StjTowerEnergyCutAdc(4,3)); //ADC-ped>4 AND ADC-ped>3*RMS
    anapars12->addEemcCut(new StjTowerEnergyCutEt(EjAna::kEEmcEtMin));  

    //FMS cuts,
    anapars12->addFmsCut(new StjTowerEnergyCutFMS(EjAna::kFmsEngMin, 200)); 
  

    //Jet cuts
    anapars12->addJetCut(new StProtoJetCutPt(0.01,200));
    anapars12->addJetCut(new StProtoJetCutEta(-1,5)); //CKim, extend to FMS acceptance

    //Set anti-kt R=0.7 parameters
    StFastJetPars* AntiKtR070Pars = new StFastJetPars;

    StFastJetAreaPars *JetAreaPars = new StFastJetAreaPars;
    JetAreaPars->setGhostMaxRap(5.0);	//Needed to extend to forward rapidity
    JetAreaPars->setGhostArea(0.04);    //0.04 was set for mid-rapidity. Find an optimal value for FMS / EEMC cell size
    
    AntiKtR070Pars->setJetAlgorithm(StFastJetPars::antikt_algorithm);
    AntiKtR070Pars->setRparam(EjAna::kRMax);
    AntiKtR070Pars->setRecombinationScheme(StFastJetPars::E_scheme);
    AntiKtR070Pars->setStrategy(StFastJetPars::Best);
    AntiKtR070Pars->setPtMin(EjAna::kPtMin);
    AntiKtR070Pars->setJetArea(JetAreaPars);

    jetmaker->addBranch("AntiKtR070NHits12",anapars12,AntiKtR070Pars);
    StOffAxisConesPars *off070 = new StOffAxisConesPars(EjAna::kRMax);
    jetmaker->addUeBranch("OffAxisConesR070", off070);

    TStNanoJetTreeMaker *nanoMaker = new TStNanoJetTreeMaker("NanoJetTreeMaker");
    nanoMaker->SetTrigIds(trigIds);
    nanoMaker->SetOutFileName((TString)"NanoJetTree_" + outFile);
    nanoMaker->SetBranchName("AntiKtR070NHits12");
    nanoMaker->SetEtaMax(etaMax);
    nanoMaker->SetEtaMin(etaMin);

    Int_t nEvents = muDstMaker->chain()->GetEntries();
    cout << "------------> Number of entries to be processed: "<< nEvents <<endl;

    chain->Init();
    chain->EventLoop(0, nEvents);
    chain->Finish();
    delete chain;

    sw.Stop();
    sw.Print();

    if(gROOT->IsBatch() && inFile.Contains("/tmp/"))
	TStScheduler::DeleteTempFiles(inFile);

    //cout << "-----------> Deleting Original jet finder files !!! <--------------------" <<endl;
    //gROOT->ProcessLine(".! rm jets_*.root ueoc_*root skim_*.root");
    
    std::cout <<"Done!" <<endl;
    
    return;
}//Main

