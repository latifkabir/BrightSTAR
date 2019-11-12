/*$Id: ppUtil.h,v 1.22 2016/01/27 08:14:33 bogdan Exp $ */
#ifndef PP_UTIL_H
#define PP_UTIL_H
//
#include <stdlib.h>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include "ppTypes.h"

using namespace std;

//

#ifdef __CINT__
// cint does not understand bitwise OR for bitset class
bitset<8> operator|( const bitset<8>& lhs, const bitset<8>& rhs){
  bitset<8> ret;
  ret |= lhs;
  return ret |= rhs;
}
#endif 

const int kMAXSEQ = 8;                      // number of RPs 
const int kMAXCHAIN = 4;                    // number of Si planes per RP
const int kMAXSVX[4] = { 4, 6, 4, 6 };      // number of SVX chips for plane A,B,C,D ( Y,X,Y,X coordinate )
const int kMAXSVX_CHAN = 128;               // number of channels for one SVX chip
const int kTOTSVXNUM = 160;                 // number of all SVXes =  
// Mappings to deal with the trigger data
/*
   if "i" is the RP number (0-7) and ipmt=0/1 pmt number 
   adc = trgd->pp2ppADC((StBeamDirection)ew[i], vh[i], udio[i], ipmt )
   tac = trgd->pp2ppTAC((StBeamDirection)ew[i], vh[i], udio[i], ipmt )
*/
//
// stuff specific for Run 2009
// 
#ifdef RUN_2009
const double PMT_TAC_MIN = 100.; 
const double PMT_TAC_MAX = 1700.;
const double PMT_ADC_MIN = 5.; 
const std::string RP_NAME[kMAXSEQ] = { "EHI", "EHO", "EVU", "EVD", "WHI", "WHO", "WVD", "WVU" };
const int Plane_Direction[kMAXSEQ*kMAXCHAIN] = {-1,1,-1,1,  1,-1,1,-1,  1,1,1,1, -1,-1,-1,-1, -1,-1,-1,-1,  1,1,1,1,  -1,1,-1,1,  1,-1,1,-1 };
const short ew[kMAXSEQ] = { 0, 0, 0, 0, 1, 1, 1, 1 } ; // East = 0, West = 1
const short vh[kMAXSEQ] = { 1, 1, 0, 0, 1, 1, 0, 0 } ; // Vertical = 0, Horizontal = 1
const short ud[kMAXSEQ] = { 1, 0, 0, 1, 1, 0, 1, 0 } ; // Up=0, Down=1; Outer=0, Inner=1

const bitset<8> EHI( string("00000001") ); 
const bitset<8> EHO( string("00000010") ); 
const bitset<8> EVU( string("00000100") ); 
const bitset<8> EVD( string("00001000") ); 
const bitset<8> WHI( string("00010000") ); 
const bitset<8> WHO( string("00100000") ); 
const bitset<8> WVD( string("01000000") ); 
const bitset<8> WVU( string("10000000") );
 
const int NGOOD_TRGS = 12;
const int NTRG_CLEAN = 8;
const bitset<8> ET_LIST[] = { (EHI|WHO), (EHO|WHI), (EVU|WVD), (EVD|WVU),
                              (EVU|EHI|WHO|WVD), (EVD|EHO|WHI|WVU),
                              (EVD|EHI|WHO|WVU), (EVU|EHO|WHI|WVD),
                              (EVU|EHI|WHO), (EVD|EHO|WVU),
                              (EHI|WHO|WVD), (EVD|WHI|WVU),
                              (EHI|WVD), (EHO|WVU), (EVU|WHO), (EVD|WHI) 
 };
const std::string ET_NAME[] = { "EHI_WHO", "EHO_WHI", "EVU_WVD", "EVD_WVU", 
                           "EIU_WOD", "EOD_WIU", "EID_WOU", "EOU_WID",
                           "EAU_WHO", "EAD_WVU",
                           "EHI_WAD", "EVD_WAU",
                           "EHI_WVD", "EHO_WVU", "EVU_WHO", "EVD_WHI" 
 };

enum { kEHI, kEHO, kEVU, kEVD, kWHI, kWHO, kWVD, kWVU };
enum { kEHI_WHO, kEHO_WHI, kEVU_WVD, kEVD_WVU, kEIU_WOD, kEOD_WIU,
       kEID_WOU, kEOU_WID, kEAU_WHO, kEAD_WVU, kEHI_WAD, kEVD_WAU,
       kEHI_WVD, kEHO_WVU, kEVU_WHO, kEVD_WHI
 };

 
#else
//
// stuff for current Run 2015
// 
enum {  kE1U, kE1D, kE2U, kE2D, kW1U, kW1D, kW2U, kW2D };
const double PMT_TAC_MIN = 200.; 
const double PMT_TAC_MAX = 2200.;
const double PMT_ADC_MIN = 120.; 


const short ew[kMAXSEQ] = { 0, 0, 0, 0, 1, 1, 1, 1 } ; // East = 0, West = 1
const short vh[kMAXSEQ] = { 1, 1, 0, 0, 1, 1, 0, 0 } ; // Vertical = 0, Horizontal = 1
const short ud[kMAXSEQ] = { 1, 0, 0, 1, 1, 0, 0, 1 } ; // Up=0, Down=1; Outer=0, Inner=1

const std::string    RP_NAME[kMAXSEQ] = {  "E1U", "E1D", "E2U", "E2D", "W1U", "W1D", "W2U", "W2D" };
const std::string PlaneName[kMAXSEQ*kMAXCHAIN] = { "E1U0", "E1U1", "E1U2", "E1U3", "E1D0", "E1D1", "E1D2", "E1D3",
					      "E2U0", "E2U1", "E2U2", "E2U3", "E2D0", "E2D1", "E2D2", "E2D3",  
					      "W1U0", "W1U1", "W1U2", "W1U3", "W1D0", "W1D1", "W1D2", "W1D3",  
					      "W2U0", "W2U1", "W2U2", "W2U3", "W2D0", "W2D1", "W2D2", "W2D3"};  
const std::string RP_PAIR_NAME[4]  = { "E1U_W1D", "E2U_W2D", "E1D_W1U", "E2D_W2U" };
const rp_pair_t RP_PAIRS[4]        = { {kE1U, kW1D}, {kE2U, kW2D}, {kE1D, kW1U}, {kE2D, kW2U} };

const bitset<8> E1U( string("00000001") ); 
const bitset<8> E1D( string("00000010") ); 
const bitset<8> E2U( string("00000100") ); 
const bitset<8> E2D( string("00001000") ); 
const bitset<8> W1U( string("00010000") ); 
const bitset<8> W1D( string("00100000") ); 
const bitset<8> W2U( string("01000000") ); 
const bitset<8> W2D( string("10000000") );
 
const int NGOOD_TRGS = 14;
const int NTRG_CLEAN = 6;
const bitset<8> ET_LIST[] = { (E1U|W1D), (E1D|W1U), (E2U|W2D), (E2D|W2U),
                              (E2U|E1U|W1D|W2D), (E2D|E1D|W1U|W2U),
                              (E2U|E1U|W1D), (E2U|E1U|W2D), (E2D|E1D|W1U), (E2D|E1D|W2U),
                              (E1U|W1D|W2D), (E2U|W1D|W2D), (E1D|W1U|W2U), (E2D|W1U|W2U),
                              (E1U|W2D), (E1D|W2U), (E2U|W1D), (E2D|W1U) 
 };
const std::string ET_NAME[] = { "E1U_W1D", "E1D_W1U", "E2U_W2D", "E2D_W2U", "EAU_WAD", "EAD_WAU",
                           "EAU_W1D", "EAU_W2D", "EAD_W1U", "EAD_W2U",
                           "E1U_WAD", "E2U_WAD", "E1D_WAU", "E2D_WAU",
                           "E1U_W2D", "E1D_W2U", "E2U_W1D", "E2D_W1U" 
 };
 enum { kE1U_W1D, kE1D_W1U, kE2U_W2D, kE2D_W2U, kEAU_WAD, kEAD_WAU,
        kEAU_W1D, kEAU_W2D, kEAD_W1U, kEAD_W2U,
        kE1U_WAD, kE2U_WAD, kE1D_WAU, kE2D_WAU,
        kE1U_W2D, kE1D_W2U, kE2U_W1D, kE2D_W1U
 };

 
#endif

const int NTRG_TYPES = (sizeof( ET_LIST )/sizeof( ET_LIST[0])); 
//
//+ 

inline bool   PmtInRange ( const int tac, const int adc ){ if( tac > PMT_TAC_MIN && adc > PMT_ADC_MIN ) return true; return false; }
inline int    isETOK(const bitset<8> trig ){ for(int i=0; i<NTRG_TYPES; i++ ) {if ( trig == ET_LIST[i] ) return i;} return -1;}

#endif
