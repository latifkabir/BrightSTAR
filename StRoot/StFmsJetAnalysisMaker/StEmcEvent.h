#ifndef ST_EMC_EVENT_HH
#define ST_EMC_EVENT_HH

#include "StObject.h"
#include <vector>

#ifndef ST_NO_NAMESPACES
using std::vector;
#endif


//EMC event class for 
//-Yuxi Pan 04/03/2013


class StEmcEvent : public StObject {

public:
        StEmcEvent() {}
        ~StEmcEvent() {}

        void Clear( const char* opt = "" );
        void Print( const char* opt = "" ) const;

        Int_t mNhits;
        
	vector<float> mVhitAdc;          //tower adc
	vector<float> mVhitE;           //tower energy
        vector<float> mVhitEt;          //tower Et
        vector<float> mVhitEta;         //tower eta
        vector<float> mVhitPhi;         //tower phi
	vector<float> mVhitPedestal;    //tower pedestals mean
	vector<float> mVhitPedrms;      //tower pedestals rms
	vector<int>   mVhitStatus;      //tower status
	vector<int>   mVhitDetId;       //detector id 9: BEMC, 13: EEMC
	vector<int>   mVhitId;          //tower Id

private:
        ClassDef(StEmcEvent,1)
};
#endif
