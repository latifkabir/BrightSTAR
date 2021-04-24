// Filename: StEmVertexMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "StEmVertexMaker.h"
#include "StEvent/StEvent.h"
#include "StEvent/StTriggerData.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StBTofHeader.h"

ClassImp(StEmVertexMaker)

//_____________________________________________________________________________ 
StEmVertexMaker::StEmVertexMaker(const char *name):StMaker(name)
{

}

//_____________________________________________________________________________ 
StEmVertexMaker::~StEmVertexMaker()
{

}


//_____________________________________________________________________________ 
Int_t StEmVertexMaker::Init()
{

    return kStOK;
}


//_____________________________________________________________________________
Int_t StEmVertexMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");

    if(!mMuDst)
    {
	LOG_ERROR << "StEmVertexMaker::Make - No MuDst found" <<endm;
	return kStFatal;
    }

    //Priority: VPD -> BBC w/ slewing correction -> BBC w/o slewing correction
    Double_t zVtx = 0.;
    Double_t vpdZ = -999.;
    Double_t bbcZ = -999.;

    //Check VPD zVtx
    if (mMuDst->btofHeader()) vpdZ = mMuDst->btofHeader()->vpdVz();

    //Assign zVtx
    if (vpdZ != -999.) zVtx = vpdZ; //Use vpdZ and be done with it, if it's valid
    else
    {
	//No vpdZ or found vpdZ is NOT valid: go to BBC
	if (mReadBbcSlewing == true) //BBCz w/ slewing
	{
	    const StTriggerData* triggerData = mMuDst->event()->triggerData();
	    bbcZ = GetBbcZCorr(triggerData);
	}
	else
	{
	    const Double_t bbcTdiff = mMuDst->event()->triggerData()->bbcTimeDifference();
	    if (fabs(bbcTdiff) > 1.e-6) bbcZ = -0.3 * (bbcTdiff - 4096);
	}
	if (bbcZ != -999.) zVtx = bbcZ;
    }

    mEmVertexZ = zVtx;
    
    return kStOK;
}

//_____________________________________________________________________________
Int_t StEmVertexMaker::Finish()
{
    return kStOK;
}

//_____________________________________________________________________________
Int_t StEmVertexMaker::ReadBbcSlewing(const char* filename_bbc) //Written by Oleg Eyser
{
    mReadBbcSlewing = true;

    //reading parameters for BBC slewing correction
    char s[100];
    int iew, ipmt;
    float ca, cb, cc;

    FILE *pFile = fopen( filename_bbc, "read" );
    fgets( s, 100, pFile );
    for( int ew=0; ew<2; ew++ )
	for( int p=0; p<16; p++ )
	{
	    fscanf( pFile, " %d %d %f %f %f \n", &iew, &ipmt, &ca, &cb, &cc);
	    if ( ew==iew && p+1==ipmt )
	    {
		mBbcSlew[ew][p][0] = ca;
		mBbcSlew[ew][p][1] = cb;
		mBbcSlew[ew][p][2] = cc;
	    }
	    else return kError;
	}
    fclose( pFile );

    cout << "\nBBC slewing: z(A+B/[C+adc])" << endl;
    for( int ew=0; ew<2; ew++ )
    {
        if( ew==0 ) cout << " East" << endl;
        if( ew==1 ) cout << " West" << endl;
        for( int p=0; p<16; p++ )
        {
            cout << Form("PMT%2d - %7.2f %7.2f %7.2f ",
			 p+1, mBbcSlew[ew][p][0], mBbcSlew[ew][p][1], mBbcSlew[ew][p][2]) << endl;
        }
    }
    cout <<endl;

    return kStOK;
}//ReadBbcSlewing

//_____________________________________________________________________________
Double_t StEmVertexMaker::GetBbcZCorr(const StTriggerData* triggerData) //Written by Oleg Eyser, Modified by CKim
{
    //DO NOT use muDST->event()->bbcTriggerDetector() -- obsolete!!!
    Double_t bbcZ     = -999.;
    Double_t bbcTdiff = -999.;
    UShort_t tdc1east, tdc1west;
    UShort_t pmt1east, pmt1west;
    UShort_t adc1east, adc1west;
    unsigned int tdcMatchEast = 0;
    unsigned int tdcMatchWest = 0;
    bbcTdiff = (Double_t)triggerData->bbcTimeDifference();
    tdc1east = triggerData->bbcEarliestTDC(east);
    tdc1west = triggerData->bbcEarliestTDC(west);

    //Compare TDC values to find earliest PMT (east/west)
    for ( int i=1; i<=16; i++ )
    {
        if ( tdc1east==triggerData->bbcTDC(east, i) )
        {
            adc1east = triggerData->bbcADC(east, i);
            pmt1east = i-1;
            ++tdcMatchEast;
        }
        if ( tdc1west==triggerData->bbcTDC(west, i) )
        {
            adc1west = triggerData->bbcADC(west, i);
            pmt1west = i-1;
            ++tdcMatchWest;
        }
    }

    //BBC slewing correction (east/west)
    if ( tdcMatchEast==1 && tdcMatchWest==1 )
    {
        Double_t zEast = -0.3 * (bbcTdiff
				- mBbcSlew[0][pmt1east][0] - mBbcSlew[0][pmt1east][1]/(mBbcSlew[0][pmt1east][2] + adc1east)
	    );
        Double_t zWest = -0.3 * (bbcTdiff
				- mBbcSlew[1][pmt1west][0] - mBbcSlew[1][pmt1west][1]/(mBbcSlew[1][pmt1west][2] + adc1west)
	    );
        bbcZ = (zEast + zWest)/2.0;
    }

    return bbcZ;
}//GetBbcZCorr
