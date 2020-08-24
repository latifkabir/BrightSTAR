//
// Run bfc.C with FMS fast simulator in the chain to import FPD/FMS hits
// from GEANT to StEvent to MuDst.
//
class StBFChain;
StBFChain* chain = 0;
void runBfcFms(int nevents = -1,
	       const char* fzfile = "inPythiaFile.fzd",        
	       //const char* chainopt = "ry2011b agml usexgeom MakeEvent ITTF NoSsdIt NoSvtIt Idst BAna l0 l3 Tree logger Sti  fmsSim   IdTruth big fzin clearmem sdt20110417.193427"  //Zhanwen
	       const char* chainopt = "DbV20150923 y2015 MakeEvent ITTF NoSsdIt NoSvtIt Idst BAna l0 Tree logger Sti VFPPVnoCTB beamLine tpcDB TpcHitMover TpxClu fmsDb fmsSim fmspoint  tags emcY2 EEfs evout -dstout IdTruth geantout big fzin MiniMcMk clearmem  sdt20150417.193427" //Mriganka
    ) 
{  
  gROOT->LoadMacro("bfc.C");
  bfc(-1,chainopt,fzfile);
  
  gSystem->Load("StJetSkimEvent");
  gSystem->Load("StBfcTriggerFilterMaker");
  // gSystem->Load("StFmsFastSimulatorMaker");
  // StFmsFastSimulatorMaker *fmsFastSim = new StFmsFastSimulatorMaker; 
  
  StPythiaEventMaker* pythia = new StPythiaEventMaker;
  TString pyfile = gSystem->BaseName(fzfile);
  pyfile.ReplaceAll(".fzd",".pythia.root");
  pythia->SetPythiaFile(pyfile);
  //chain->AddAfter("geant",pythia);
  
  chain->Init();

  if(nevents == -1)
      chain->EventLoop();
  else
      chain->EventLoop(nevents);
}
