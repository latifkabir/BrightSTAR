//
// Run bfc.C with FMS fast simulator in the chain to import FPD/FMS hits
// from GEANT to StEvent to MuDst.
//
class StBFChain;
StBFChain* chain = 0;
void bfcFms(int nevents = 10000,
	    const char* fzfile = "test500.fzd",
	    const char* chainopt = "ry2011b agml usexgeom MakeEvent McEvent vfmce Idst BAna l0 l3 Tree logger fmsSim fmspoint evout -dstout IdTruth bigbig fzin geantout clearmem sdt20110417.193427" ) 
{
  //  char fzfile1[200];
  //  sprintf(fzfile1,"test.fzd");
  //  fzfile = &fzfile1[0];
  
  gROOT->LoadMacro("bfc.C");
  bfc(-1,chainopt,fzfile);
  
  gSystem->Load("StJetSkimEvent");
  gSystem->Load("StBfcTriggerFilterMaker");
  gSystem->Load("StFmsFilterMaker");
  
  StPythiaEventMaker* pythia = new StPythiaEventMaker;
  StFmsFilterMaker* fmsFilter = new StFmsFilterMaker();
  fmsFilter->setEThres(30);
  TString pyfile = gSystem->BaseName(fzfile);
  pyfile.ReplaceAll(".fzd",".pythia.root");
  pythia->SetPythiaFile(pyfile);

  // Delay write until after filter
  chain->AddAfter("fmsSim",fmsFilter);
  //  chain->AddBefore("outputStream",fmsFilter);
  chain->AddAfter("geant",pythia);
  
  chain->Init();
  
  if ( 0 )   new TBrowser();
  else       chain->EventLoop(nevents);

    
    
}
