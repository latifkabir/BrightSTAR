//Now in order to read microDST use the DoMicroDst.

#include "StRootInclude.h"
#include "BrEEmcMaker/TStEEmcMakerGeomEx.h"

int RunEEmcMakerGeomEx( const Char_t* file, Int_t nFiles, const Char_t* inDir, int nEve)
{
    StChain *chain=0;
    // create chain    
    chain = new StChain("StChain"); 
  
    // Now we add Makers to the chain...   
    StMuDstMaker *maker = new StMuDstMaker(0,0,inDir,file,"MuDst.root",nFiles);
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
    m->SetOutFile("EEmcMakerGeomEx.root");
  
    chain->Init();
    chain->ls(3);

    int eventCounter=0;
    int stat=0;

    //---------------------------------------------------
    while ( stat==0 )
    {// loop over events
	if(eventCounter>=nEve) break;
	chain->Clear();
	stat = chain->Make();
	printf(" event# %d done\n", eventCounter++);    
    }
  
    //  chain->Finish();  
}
