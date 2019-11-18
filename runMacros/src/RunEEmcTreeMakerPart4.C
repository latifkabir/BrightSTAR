/* 
 * Created Sept 2012, by S. Gliske
 *
 * Macro to read in "Part 1" of the EEmcTree and write out "Part 4"
 * 
 */

// forward declarations
class StChain;

// other
class StEEmcTreeMaker_t;

//
// some variables that tend to be made global
//
StChain                                 *analysisChain = 0;
StEEmcTreeMaker_t                       *treeReaderPtr         = 0;
StEEmcTreeMaker_t                       *treeWriterPtr         = 0;

//
// the main routine
//
void makeEEmcTreePart4( const Char_t *eemcTreePart1FileName = "eemcTreeP1.root",
                        const Char_t *eemcTreePart4FileName = "eemcTreeP4.root",
                        Int_t neventsIn = -1,
                        Int_t displayFreq = 100 ){

   // load the shared libraries
   std::cout << "***** Loading libraries *****" << endl;
   gROOT->Macro("LoadEEmcTreeLibs.C");

   std::cout << "***** Done loading libraries *****" << endl;
   std::cout << "***** Instanciating all the classes *****" << endl;

   //
   // CREATE THE ANALYSIS CHAIN
   //
   analysisChain = new StChain("eemcAnalysisChain");

   //
   // TREE MAKER FOR READING
   //
   treeReaderPtr = new StEEmcTreeMaker_t( "EEmcTreeReader" );
   treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::READ,   eemcTreePart1FileName );
   treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::IGNORE, "" );
   treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::IGNORE, "" );
   treeReaderPtr->setTreeStatus( StEEmcTreeMaker_t::PART_4, StEEmcTreeMaker_t::IGNORE, "" );
   treeReaderPtr->doSpinInfoIO( 0 );
   treeReaderPtr->doEvtHddrIO( 1 );
   treeReaderPtr->setMaxNumEvents( neventsIn );


   //
   // TREE MAKER FOR WRITING
   //
   treeWriterPtr = new StEEmcTreeMaker_t( "EEmcTreeWriter" );
   treeWriterPtr->setTreeStatus( StEEmcTreeMaker_t::PART_1, StEEmcTreeMaker_t::IGNORE, "" );
   treeWriterPtr->setTreeStatus( StEEmcTreeMaker_t::PART_2, StEEmcTreeMaker_t::IGNORE, "" );
   treeWriterPtr->setTreeStatus( StEEmcTreeMaker_t::PART_3, StEEmcTreeMaker_t::IGNORE, "" );
   treeWriterPtr->setTreeStatus( StEEmcTreeMaker_t::PART_4, StEEmcTreeMaker_t::WRITE,  eemcTreePart4FileName );
   treeWriterPtr->doSpinInfoIO( 0 );
   treeWriterPtr->doEvtHddrIO( 0 );
   treeWriterPtr->setHTTPthres( 2, 4 );
   treeWriterPtr->setEEmcTreeReader( treeReaderPtr );

   // debugging info
   std::cout << "***** Done instanciating all the classes *****" << endl;
   //analysisChain->ls(3);

   //
   // INITIALIZE ALL MAKERS
   //

   std::cout << "***** Initializing all makers in the analysis chain *****" << std::endl;

   analysisChain->Init();

   std::cout << "***** Initialization done *****" << std::endl;

   //
   // FINALLY READY TO LOOP OVER THE EVENTS 
   //

   Int_t ierr  = kStOK;  // err flag
   Long_t nIn = -1;
   if( neventsIn < 0 )
      neventsIn = 1<<30;

   Int_t nhits1 = 0, nhits2 = 0;

   for( nIn = 0; nIn < neventsIn && !ierr; ++nIn ){

      // clear
      analysisChain->Clear();

      // make
      ierr = analysisChain->Make();

      // Print every so many events
      if( (nIn+1) % displayFreq == 0 )
         std::cout << "***** Entries read " << nIn+1 << endl;
   };

   std::cout << "***** Entries read " << nIn+1 << endl;

   if( ierr && ierr != 2 )
      std::cout << "***** ERROR FLAG " << ierr << endl;
 
   //---------------------------------------------------------------
   //
   // Calls the ::Finish() method on all makers (done automatically)
   //
   // analysisChain->Finish(); 

   //
   // Delete the chain (done automatically)
   //
   //analysisChain->Delete();
   //delete analysisChain;

   return;
};

// $Id: makeEEmcTreePart4.C,v 1.1 2012/09/19 19:59:52 sgliske Exp $
// $Log: makeEEmcTreePart4.C,v $
// Revision 1.1  2012/09/19 19:59:52  sgliske
// added Part 4: precompute ht and tp
//
// Revision 1.1  2012/06/29 17:57:06  sgliske
// Updated and undoing changes for bug investigation
//
// Revision 1.5  2012/06/19 15:04:10  sgliske
// updates
//
// Revision 1.4  2012/06/14 05:28:24  sgliske
// Removed TArrayS and TArrayF from EEmcSmdCluster and EEmcHit, as
// they still contain dynamic arrays, and so things fail when using
// TClonesArrays.  Now both use a static array.
//
// Revision 1.3  2012/05/31 22:13:14  sgliske
// working on MC
//
// Revision 1.2  2012/05/25 04:21:57  sgliske
// updates
//
// Revision 1.1  2012/05/22 22:01:08  sgliske
// updates
//
// Revision 1.8  2011/10/24 21:49:40  sgliske
// updates, working on reading MC code
//
