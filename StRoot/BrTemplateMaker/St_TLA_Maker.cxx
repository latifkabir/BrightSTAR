
#include "St_TLA_Maker.h"
#include "TDataSetIter.h"
#include "StDAQMaker/StDAQReader.h"


ClassImp(St_TLA_Maker)

//_____________________________________________________________________________
/// TLA constructor
/*!
  const char *name -  the name of this constructor
  The first comment lines after the opening bracket
  ({) of a member function are considered as a member function description 
  See <A HREF="http://root.cern.ch/root/Documentation.html"> ROOT HTML documentation </A>

 */
St_TLA_Maker::St_TLA_Maker(const char *name):StMaker(name)
{
  //
}


//_____________________________________________________________________________
/// This is TLA destructor
/*!
  The first comment lines after the opening bracket
  ({) of a member function are considered as a member function description 
  
  The first comment lines after the opening bracket
  ({) of a member function are considered as a member function description 
  see: <A HREF="http://root.cern.ch/root/Documentation.html"> ROOT HTML documentation </A> 

 */
St_TLA_Maker::~St_TLA_Maker()
{
  //
}


//_____________________________________________________________________________
/// Init - is a first method the top level StChain calls to initialize all its makers 
Int_t St_TLA_Maker::Init()
{
  // Create tables
  // Create Histograms    
   return StMaker::Init();
}


//_____________________________________________________________________________
/// Make - this method is called in loop for each event
Int_t St_TLA_Maker::Make()
{
  //
  //  PrintInfo();
  //
  int nArray; unsigned char *Array;

 TObjectSet *os = (TObjectSet*)GetDataSet("StDAQReader");
 assert(os);
 StDAQReader *dr = (StDAQReader*)os->GetObject();
 StTPCReader *tr = dr->getTPCReader();
 int sec=0;
 for (sec=1;sec<=12;sec++)
 {
  int padrow;
   for (padrow=1;padrow<20; padrow++)
   {
     int maxpad = tr->getMaxPad(padrow);
     int pad;
     for (pad=1;pad<=maxpad; pad++)
     {     
       int ans =  tr->getPedestals(sec,padrow,pad,nArray,Array);
       if (ans<=0) continue;
       if (!Array) continue;
       { LOG_INFO << Form(" %d %d %d %d\n",sec,padrow,pad,nArray) << endm; }

     }
   }
 }

 return kStOK;
}
