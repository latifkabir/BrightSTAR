// macro to instantiate the Geant3 from within
// STAR  C++  framework and get the starsim prompt
// To use it do
//  root4star starsim_pythia6..C

class St_geant_Maker;
St_geant_Maker *geant_maker = 0;

class StarGenEvent;
StarGenEvent   *event       = 0;

class StarPrimaryMaker;
StarPrimaryMaker *_primary = 0;

class StarPythia6;
StarPythia6* pythia6;

class StarFilterMaker;
StarFilterMaker *filter = 0;

//
// This is for testing only and should NOT be used in a production (i.e. batch 
// system) run.  It will spam the heck out of our AFS system, and bring the
// wrath of Jerome down upon you.  We will install data files in appropriate
// path, and you should update when available.
//
TString LHAPDF_DATA_PATH="./current/";

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
void Pythia6() {
  
  //  gSystem->Load( "libStarGeneratorPoolPythia6_4_23.so" );
  if ( LHAPDF_DATA_PATH.Contains("afs") ) {
     cout << "WARNING: LHAPDF_DATA_PATH points to an afs volume" << endl << endl;
     cout << "         You are advised to copy the PDF files you need into a local" << endl;
     cout << "         directory and set the LHAPDF_DATA_PATH to point to it."      << endl;
  }

  gSystem->Setenv("LHAPDF_DATA_PATH", LHAPDF_DATA_PATH.Data() );

  gSystem->Load( "/opt/star/$STAR_HOST_SYS/lib/libLHAPDF.so");
  gSystem->Load( "libPythia6_4_28.so");

  //  gSystem->Load( "StarPythia6.so"   );

  pythia6 = new StarPythia6("pythia6");
  pythia6 -> SetFrame( "CMS", 500.0 ); 
  pythia6 -> SetBlue("proton");
  pythia6 -> SetYell("proton");
  pythia6 -> PyTune( 370 );


  // if ( mode == "pp:200:minbias" )  {
  //   //pythia6->SetFrame("CMS", 510.0 );
  //   pythia6->SetFrame("CMS", 200.0 );
  //   pythia6->SetBlue("proton");
  //   pythia6->SetYell("proton");
  //   if ( tune ) pythia6->PyTune( tune );
  //   //PyPars_t &pypars = pythia6->pypars();
  //   //int& mstp5 = pypars.mstp(5); mstp5 = 370;
  //   //float& parp90 = pypars.parp(90); parp90 = 0.213;
  // }
  // if ( mode == "ep" )  {
  //   Double_t pblue[]={0.,0.,30.0};
  //   Double_t pyell[]={0.,0.,-320.0};
  //   pythia6->SetFrame("3MOM", pblue, pyell );
  //   pythia6->SetBlue("e-");
  //   pythia6->SetYell("proton");
  //   if ( tune ) pythia6->PyTune( tune );
  // }

  // if ( mode == "pp:500:minbias" )  {
  //   //pythia6->SetFrame("CMS", 510.0 );
  //   pythia6->SetFrame("CMS", 200.0 );
  //   pythia6->SetBlue("proton");
  //   pythia6->SetYell("proton");
  //   if ( tune ) pythia6->PyTune( tune );
  //   //PyPars_t &pypars = pythia6->pypars();
  //   //int& mstp5 = pypars.mstp(5); mstp5 = 370;
  //   //float& parp90 = pypars.parp(90); parp90 = 0.213;
  // }
   
  _primary->AddGenerator(pythia6);
}
//__________________________________________________________________________________
void setRngSeed(){
  
  TString sid = gSystem->Getenv("JOBID");  sid.Resize(8);  
  TString six = gSystem->Getenv("JOBINDEX");

  unsigned long long id = gROOT->ProcessLine(Form( "0x%s", sid.Data() ) );
  unsigned int id1 = (0xffff&id);
  unsigned int id2 = gROOT->ProcessLine( six.Data() );

  StarRandom::seed( _seed = id1 + id2 );
  StarRandom::capture();

}
//__________________________________________________________________________________
void simJetForward( int nevents = 1, int rcf=18010 ) //  Int_t nevents=10, Int_t rngSeed=0 , Int_t eventnameid=0)
{ 

  gROOT->ProcessLine(".L bfc.C");  {
    TString simple = "y2011b geant gstar agml usexgeom -db -detdb";
    bfc(0, simple );
  
  gSystem->Load( "libVMC.so");
  gSystem->Load( "StarGeneratorUtil.so" );
  gSystem->Load( "StarGeneratorEvent.so" );
  gSystem->Load( "StarGeneratorBase.so" );
  gSystem->Load( "StarGeneratorFilt.so" );

  gSystem->Load( "libMathMore.so"   );  
  gSystem->Load( "xgeometry.so"     );

  }

  setRngSeed(); // random number seed using SUMS jobid
  //
  // Create the primary event generator and insert it
  // before the geant maker
  //
  //  StarPrimaryMaker *
  TString filebasename = "";
  _primary = new StarPrimaryMaker();  

  TString RCF="rcf"+rcf;
  // JOB index from SUMS
  TString six = gSystem->Getenv("JOBINDEX");
  int job = six.Atoi();
  TString NEV=Form("%ievts",nevents);
  filebasename = RCF + "_" + six + "_" + NEV;    
  //  _primary -> SetFileName( Form("%s.genevts.root", filebasename.Data()) );
  _primary -> SetFileName( Form( "rcf%i_%i_%ievts.genevent.root", rcf, job, nevents ) );

  //_primary->SetFileName("test.root");
  chain -> AddBefore( "geant", _primary );
  

  //
  // Setup an event generator
  //
  Pythia6( );
  

  // Setup FMS sum filter
  filter = new StarFmsSumFilter(); {
  ((StarFmsSumFilter*)filter)->SetCut(30);
  _primary -> AddFilter( filter );
  _primary->SetAttr("FilterKeepAll",     int(1));
  }

  //
  // Setup cuts on which particles get passed to geant for
  //   simulation.  
  //
  // If ptmax < ptmin indicates an infinite ptmax.
  // ptmin will always be the low pT cutoff.
  //
  //                    ptmin  ptmax
  _primary->SetPtRange  (0.0,  -1.0);         // GeV
  //
  // If etamax < etamin, there is no cut in eta.
  // otherwise, particles outside of the specified range are cut.
  //
  //                    etamin etamax
  _primary->SetEtaRange ( -2.0, +6.0 );
  //
  //  phirange will be mapped into 0 to 2 pi internally.
  //
  //                    phimin phimax
  _primary->SetPhiRange ( 0., TMath::TwoPi() );    
  // 
  // Setup a realistic z-vertex distribution:
  //   x = 0 gauss width = 1mm
  //   y = 0 gauss width = 1mm
  //   z = 0 gauss width = 30cm
  //     gvertex 0.320 -0.086 -0.339
  //     gspread 0.061  0.002  48.0
  _primary->SetVertex( 0.320, -0.086, -0.339 );
  _primary->SetSigma ( 0.061,  0.002,  48.00 );
  //
  // Initialize _primary event generator and all sub makers
  //
  _primary -> Init();
  //
  // Setup geometry and set starsim to use agusread for input
  //
  geometry("y2011b");
  command("gkine -4 0");

  cout << Form( "gfile o rcf%i_%i_%ievts.fzd", rcf, job, nevents ) << endl;
  command ( Form( "gfile o rcf%i_%i_%ievts.fzd", rcf, job, nevents ) ); 

  //
  // Trigger on nevents
  //


  // Improved tune for forward...
  PyPars_t &pypars = pythia6->pypars();
  pypars.parp(90) = 0.213;

  // Make everything decay in pythia (loses parent history in geant record)
  PyDat3_t &pydat3 = pythia6->pydat3();
  pydat3.mdcy(102,1)=1; // PI0 111
  pydat3.mdcy(106,1)=1; // PI+ 211
  pydat3.mdcy(109,1)=1; // ETA 221
  pydat3.mdcy(116,1)=1; // K+ 321
  pydat3.mdcy(112,1)=1; // K_SHORT 310
  pydat3.mdcy(105,1)=1; // K_LONG 130
  pydat3.mdcy(164,1)=1; // LAMBDA0 3122
  pydat3.mdcy(167,1)=1; // SIGMA0 3212
  pydat3.mdcy(162,1)=1; // SIGMA- 3112
  pydat3.mdcy(169,1)=1; // SIGMA+ 3222
  pydat3.mdcy(172,1)=1; // Xi- 3312
  pydat3.mdcy(174,1)=1; // Xi0 3322
  pydat3.mdcy(176,1)=1; // OMEGA- 3334

  ///.... note-- setup so all decay ....


  trig( nevents );

  command("call agexit");  // Make sure that STARSIM exits properly
}
// ----------------------------------------------------------------------------

