#ifndef PP_TYPES_H
#define PP_TYPES_H
/*
$Id: ppTypes.h,v 1.11 2016/01/25 16:43:25 bogdan Exp $
*/

#include <stdlib.h>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>


typedef struct pedestal_t{
  float avr;
  float rms;
} PEDEDESTAL_T;

typedef struct hit_t {
  short  chan;
  float   ADC;                                 // in chan RMS units
  bool operator<(const hit_t& rhs) const       // operator LESS defined for std::sort
    {
        return chan < rhs.chan;
    }
} HIT_T;

typedef struct cluster_t {
  int   status;             // -1/0/N selfmatched/unmatched/matched with cluster of ordinal number N-1 
  int   len;
  float pos;                // local in meters
  float sigma;
  float E;                  // in ADC unit
  float weight;             // energy E in rms/chnl units
  bool operator<(const cluster_t& rhs) const   
    {
        return len > rhs.len;
    }
} CLUSTER_T;

typedef struct point_t {
  float dist;    // matching distance = ( X_1 - X_2)
  float pos;     // position = (X_1 + X_2)/2. 
  int par1;      // parent clusters ordinal numbers
  int par2;    
} POINT_T;

typedef struct rp_pair_t{
  int RPE;
  int RPW;
} RP_PAIR_T;

typedef struct roman_pot {
  short ID;
  bool  status;
  float pmt_adc[2];
  float pmt_tac[2];
  std::vector < hit_t > hits[4];
  std::vector < cluster_t > clusters[4];
  std::vector < point_t > X;
  std::vector < point_t > Y;
  bool operator<(const roman_pot& rhs) const
    {
        return ID < rhs.ID;
    }
} ROMAN_POT;

typedef struct bbc_data_t {
  int Sum_West;
  int Sum_East;
  int Sum_LargeTile_West;
  int Sum_LargeTile_East;
  int EarliestTAC_West;
  int EarliestTAC_East;
  int EarliestTAC_Large_West;
  int EarliestTAC_Large_East;
  int TimeDifference;
  int TimeDifferenceLarge;
  int west_adc[24];
  int west_tac[24];
  int east_adc[24];
  int east_tac[24];

} BBC_DATA;


#endif
