//Now in order to read microDST use the DoMicroDst.
class StChain;
  StChain *chain=0;


int RunEEmcMakerGeomEx(
	  char* file    = "st_physics_16066033_raw_0000006.MuDst.root",
	  Int_t nFiles  = 1, 
	  char* inDir   = "root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/066/16066033/",
	  int nEve=10)
{ 


  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();
  cout << " loading done " << endl;

//   gSystem->Load("StDbLib");
//   gSystem->Load("StDbBroker");
//   gSystem->Load("St_db_Maker");

// // Load my maker
//   assert(gSystem->Load("StEEmcUtil")==0);
//   //assert(gSystem->Load("StEEmcPoolmuDst")==0 );
//   assert(gSystem->Load("StEEmcDbMaker")==0);
//   assert(gSystem->Load("DemoMaker")==0);


// create chain    
  chain = new StChain("StChain"); 
  
// Now we add Makers to the chain...   
  maker = new StMuDstMaker(0,0,inDir,file,"MuDst.root",nFiles);
  StMuDbReader* db = StMuDbReader::instance();

  
  // instantiate your maker here 
  /* #1 */ St_db_Maker *dbMk = new St_db_Maker("StarDb", "MySQL:StarDb");
  /* #2 */ StEEmcDbMaker  *myMk=new StEEmcDbMaker("eemcDb");  
  
  // request DB for sectors you need (dafault:1-12)
  // myMk->setSectors(5,8);

  // to overwritte the time stamp 
  // reverse order of the above makers: first #2, then #1
  // activate the line below
  //myMk->setTimeStampDay(20030514);  // format: yyyymmdd

  // change DB-server name (if needed)
  // myMk->setDBname("TestScheme/eemc");
 
  // request alternative flavor of DB table (if needed)
  // myMk->setPreferedFlavor("set430","eemcPMTcal");

  TStEEmcMakerGeomEx *m = new TStEEmcMakerGeomEx("jasEE","MuDst");
  
  chain->Init();
  chain->ls(3);

  int eventCounter=0;
  int stat=0;

  //---------------------------------------------------
  while ( stat==0 ) {// loop over events
    if(eventCounter>=nEve) break;
    chain->Clear();
    stat = chain->Make();
    printf(" event# %d done\n", eventCounter++);
    
  }
  
  //  chain->Finish();  
}
