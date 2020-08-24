// macro to instantiate the Geant3 from within
// STAR  C++  framework and get the starsim prompt
// To use it do
//  root4star starsim.C

class St_geant_Maker;
St_geant_Maker *geant_maker = 0;

class StarGenEvent;
StarGenEvent   *event       = 0;

class StarPrimaryMaker;
StarPrimaryMaker *primary = 0;

class StarKinematics;
StarKinematics *kinematics = 0;

class StarPythia6;
StarPythia6 *pythia6 = 0;

// ----------------------------------------------------------------------------
void geometry( TString tag, Bool_t agml=true )
{
  TString cmd = "DETP GEOM "; cmd += tag;
  if ( !geant_maker ) geant_maker = (St_geant_Maker *)chain->GetMaker("geant");
  geant_maker -> LoadGeometry(cmd);
  //  if ( agml ) command("gexec $STAR_LIB/libxgeometry.so");
}
// ----------------------------------------------------------------------------
void command( TString cmd )
{
    if ( !geant_maker ) geant_maker = (St_geant_Maker *)chain->GetMaker("geant");
    geant_maker -> Do( cmd );
}
// ----------------------------------------------------------------------------
void trig( Int_t n=1 )
{
  for ( Int_t i=0; i<n; i++ ) {
    chain->Clear();
    chain->Make();
    
  }
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void Pythia6( TString mode="pp:w", Int_t tune=370 )
{
  //  gSystem->Load( "libStarGeneratorPoolPythia6_4_23.so" );
  printf("Loading PDFLIB\n");  
  gSystem->Load("/star/u/dolv719/run13/embeddingV2/libLHAPDF.so");
  printf("Done loading PDFLIB\n");  
  gSystem->Load( "libPythia6_4_28.so");

  //  gSystem->Load( "StarPythia6.so"   );

  pythia6 = new StarPythia6("pythia6");
  if ( mode=="pp:W" ){
    pythia6->SetFrame("CMS", 200.0 );
    pythia6->SetBlue("proton");
    pythia6->SetYell("proton");
    if ( tune ) pythia6->PyTune( tune );
    // Setup pythia process
    PySubs_t &pysubs = pythia6->pysubs();
    pysubs.msel = 12;
    pysubs.ckin(3)=4.0;

  }else if ( mode == "pp:qcd" ){
    pythia6->SetFrame("CMS", 200.0 );
    pythia6->SetBlue("proton");
    pythia6->SetYell("proton");
    if ( tune ) {
	pythia6->PyTune( tune );
	PyPars_t &pypars = pythia6->pypars();
	printf("PARP(90) was %f, replacing it with 0.213",pypars.parp(90));
	pypars.parp(90)=0.213;
    }
    // Setup pythia process                                                                                                       
    PySubs_t &pysubs = pythia6->pysubs();
    pysubs.msel = 1;
    pysubs.ckin(3)=1.0;
    pysubs.ckin(13)=1.0;
  }else if ( mode == "pp:minbias" ){
    pythia6->SetFrame("CMS", 200.0 );
    pythia6->SetBlue("proton");
    pythia6->SetYell("proton");
    if ( tune ) pythia6->PyTune( tune );
    PySubs_t &pysubs = pythia6->pysubs();
    pysubs.msel = 2;
  }else if ( mode == "ep" ){
    Double_t pblue[]={0.,0.,30.0};
    Double_t pyell[]={0.,0.,-320.0};
    pythia6->SetFrame("3MOM", pblue, pyell );
    pythia6->SetBlue("e-");
    pythia6->SetYell("proton");
    if ( tune ) pythia6->PyTune( tune );
  }
  
  primary->AddGenerator(pythia6);
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void pythia6FmsFastSim( Int_t nevents=10, Int_t rngSeed=1234, int RunGeant=0, int flat=0)
{ 

  gROOT->ProcessLine(".L bfc.C");
  if(RunGeant){
      TString simple = "y2015 geant gstar agml usexgeom ";
      bfc(0, simple );
      gSystem->Load( "libVMC.so");
      gSystem->Load( "StarGeneratorUtil.so" );
      gSystem->Load( "StarGeneratorEvent.so" );
      gSystem->Load( "StarGeneratorBase.so" );
      gSystem->Load( "libMathMore.so"   );  
      gSystem->Load( "xgeometry.so"     );
  }else{
      TString simple = "tables nodefault";
      bfc(0, simple );
      gSystem->Load( "libVMC.so");
      gSystem->Load( "St_g2t.so" );
      gSystem->Load( "St_geant_Maker.so" ); 
      gSystem->Load( "StarGeneratorUtil.so" );
      gSystem->Load( "StarGeneratorEvent.so" );
      gSystem->Load( "StarGeneratorBase.so" );
      gSystem->Load( "libMathMore.so"   );  
  }
  gROOT->Macro("load.C");

  // Setup RNG seed and map all ROOT TRandom here
  StarRandom::seed( rngSeed );
  StarRandom::capture();
  
  //
  // Create the primary event generator and insert it
  // before the geant maker
  //
  //  StarPrimaryMaker *
  primary = new StarPrimaryMaker();
  {
      primary -> SetFileName(Form("pythia6.%d.starsim.root",rngSeed));
      if(RunGeant) {chain -> AddBefore( "geant", primary );}
  }

  //
  // Setup an event generator
  //
  if(flat==0){
      Pythia6( "pp:qcd" );
  }else{
      gSystem->Load( "libKinematics.so");
      kinematics = new StarKinematics();    
      primary->AddGenerator(kinematics);      
  }   
  //
  // Setup cuts on which particles get passed to geant for
  //   simulation.   
  //
  // If ptmax < ptmin indicates an infinite ptmax.
  // ptmin will always be the low pT cutoff.
  //
  //                    ptmin  ptmax
  primary->SetPtRange  (0.0,  -1.0);         // GeV
  //
  // If etamax < etamin, there is no cut in eta.
  // otherwise, particles outside of the specified range are cut.
  //
  //                    etamin etamax
  //  primary->SetEtaRange ( -3.0, +3.0 );
  //
  //  phirange will be mapped into 0 to 2 pi internally.
  //
  //                    phimin phimax
  primary->SetPhiRange ( 0., TMath::TwoPi() );
    
  // 
  // Setup a realistic z-vertex distribution:
  //   x = 0 gauss width = 1mm
  //   y = 0 gauss width = 1mm
  //   z = 0 gauss width = 30cm
  // 
  primary->SetVertex( 0., 0., 0. );
  primary->SetSigma( 0.1, 0.1, 0.0 );

  //
  // Initialize primary event generator and all sub makers
  //
  primary -> Init();  
  PyPars_t &pypars = pythia6->pypars();
  printf("PARP(90) = %f",pypars.parp(90));

  /*
  int kf,kc;
  //PyDat3_t &pydat3 = pythia6->pydat3();
  kf=111; kc=pythia6->PyComp(kf); cout << "pi0 kf="<<kf<<" kc="<<kc<<endl; pythia6->pydat3().mdcy(kc,1)=1;
  kf=221; kc=pythia6->PyComp(kf); cout << "eta kf="<<kf<<" kc="<<kc<<endl; pythia6->pydat3().mdcy(kc,1)=1;
  */

  //
  // Setup geometry and set starsim to use agusread for input
  //
  if(RunGeant){
      geometry("y2015a");
      command("gkine -4 0");
      command("gfile o pythia6.starsim.fzd");
  }

  // gSystem->Load("StFmsFastSimMaker.so");
  // StFmsFastSimMaker* fmssim = new StFmsFastSimMaker;
  // fmssim->setPrint(0);
  // fmssim->Init();

  // gSystem->Load("StFmsDiPi0.so");
  // StFmsDiPi0* dipi0 = new StFmsDiPi0;
  // dipi0->setFileName(Form("pythia6.%d.dipi0.root",rngSeed));
  // dipi0->setPythia();
  // dipi0->Init();

  //
  // Trigger on nevents
  //
  //trig( nevents );
  StarGenEvent* event = primary->event();
  for ( Int_t i=0; i<nevents; i++ ) {
      chain->Clear();
      if(flat==1) {
	  //kinematics->Kine(1, "mu+", 2.0, 2.01, 3.00, 3.01, 0.0, 3.141592654*2.0);
      }
      chain->Make();
  }
  if(RunGeant) command("call agexit");  // Make sure that STARSIM exits properly
}
// ----------------------------------------------------------------------------

