//
// Run bfc.C with FMS fast simulator in the chain to import FPD/FMS hits
// from GEANT to StEvent to MuDst.
//
class StBFChain;
StBFChain* chain = 0;
void bfcFms(int nevents = 1000, int file = 1,
	    const char* fzfile = "pythiaOut.fzd",
	    const char* chainopt = "DbV20150923 y2015a MakeEvent ITTF NoSsdIt NoSvtIt Idst BAna l0 Tree logger Sti VFPPVnoCTB beamLine tpcDB TpcHitMover TpxClu fmsDb fmsSim fmspoint  tags emcY2 EEfs evout -dstout IdTruth geantout big fzin MiniMcMk clearmem  sdt20150417.193427")
    //	    const char* chainopt = "DbV20150923  y2015a MakeEvent ITTF NoSsdIt NoSvtIt Idst BAna l0 Tree logger Sti VFPPVnoCTB beamLine   fmsDb fmsSim  tags emcY2 EEfs evout -dstout IdTruth geantout big fzin MiniMcMk clearmem sdt20150417.193427")
{
    // char fzfile1[200];
    // sprintf(fzfile1,"test.fzd");
    // fzfile = &fzfile1[0];
  
    gROOT->LoadMacro("bfc.C");
    bfc(-1,chainopt,fzfile);
  
    gSystem->Load("StFmsFastSimulatorMaker");
    StFmsFastSimulatorMaker *fmsFastSim = new StFmsFastSimulatorMaker; 
 
    // fmsFastSim->Hist(); // value ADC
    // fmsFastSim->Write(); // value ADC
  
    /* gSystem->Load("St_db_Maker");
       gSystem->Load("StFmsDbMaker");
       St_db_Maker* dbMk = new St_db_Maker("db","MySQL:StarDb","$STAR/StarDb"); 
       dbMk->SetDEBUG(0);
       // dbMk->SetDateTime(20110923,0);
 
       //StFmsDbMaker* fmsdb = new StFmsDbMaker();  
       //fmsdb->setDebug(1); 
       //   fmsdb->readGainFromText(1);
       // fmsdb->readGainFromText(1);
       //fmsdb->forceUniformGainCorrection(1.0);
       //fmsdb->setDebug(0);
  
    
       //fmsFastSim->setFmsZS(3); // value ADC

       /*
       gSystem->Load("StJetSkimEvent");
       gSystem->Load("StBfcTriggerFilterMaker");
       StPythiaEventMaker* pythia = new StPythiaEventMaker;
       TString pyfile = gSystem->BaseName(fzfile);
       pyfile.ReplaceAll(".fzd",".pythia.root");
       pythia->SetPythiaFile(pyfile);
       chain->AddAfter("geant",pythia);
  


    */
    /*
      gSystem->Load("StTriggerUtilities");
      gSystem->Load("StTriggerFilterMaker");
      gSystem->Load("StFmsTriggerMaker");
  
  
      StTriggerSimuMaker* simuTrig = new StTriggerSimuMaker;
      simuTrig->setMC(0); 
      StFmsTriggerMaker* fmstrig = new StFmsTriggerMaker;
      fmstrig->useMuDst();
    */
    // StFmsTriggerFilterMaker *trigFms = new StFmsTriggerFilterMaker;

    // chain->AddAfter("MakeEvent"fmsFastSim);
    chain->AddAfter("Sti",fmsFastSim);

    chain->Init();
    chain->EventLoop(nevents);

}
