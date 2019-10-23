#ifndef ST_FMS_EventHeader_HH
#define ST_FMS_EventHeader_HH

#include "StObject.h"
#include <vector>

#ifndef ST_NO_NAMESPACES
using std::vector;
#endif

#include "TLorentzVector.h"

//event header class
//-Yuxi Pan 04/09/2013


class StFmsEventHeader : public StObject {

public:
        StFmsEventHeader() {}
        ~StFmsEventHeader() {}

        void Clear( const char* opt = "" );
        void Print( const char* opt = "" ) const;

        UInt_t mFillnum;                  //fill number
        UInt_t mRnum;                     //run number
        UInt_t mIevt;                     //event number
	int mSpinbit ;
        Int_t mBunchId;                   //bunch crossing id 7 bit
        Int_t mBbcCoincidenceRate;        //BBC coincidence rate

        vector<int> mTriggerIds;          //event trigger id
	
	TVector3 mVertex;
	unsigned short mBbcTimeBin;


private:
        ClassDef(StFmsEventHeader,1)
};
#endif
