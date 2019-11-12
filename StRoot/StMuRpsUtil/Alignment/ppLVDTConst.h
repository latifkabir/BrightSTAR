/* $Id: ppLVDTConst.h,v 1.26 2015/10/26 22:56:41 bogdan Exp $ */
//
/* these are constants to calculate actual position of SI
   detectors for each RP using LVDT raw data :
   SI_POS_0[si_plane] = LVDT_OFFSET[si_plane]+LVDT_SCALE[si_plane]*LVDT_DATA[rp];
   - LVDT_DATA[] above is LVDT readings, must be obtained from STAR-DB 
   - NOTE! units are milimeters
   - si_plane = 4*SEQ_NUMBER(0-7) + CHAIN(0-3)
 */ 
#ifndef PP_LVDT_CONST_H 
#define PP_LVDT_CONST_H
//
// define default set
#ifndef LVDT_TILTED
#define LVDT_NOTILT
#endif
//
#ifdef LVDT_NOTILT
#ifdef LVDT_INIT
/* here are initial values for LVDT --> Strip0 conversion
   - hypothetical tilts of RP sttions is ignored
*/
const char *LVDT_REVISION = "LVDTConst Version: 0.1 ";
const double LVDT_OFFSET[32] = { 
  4.863,-36.878,  4.860,-36.878, -3.933, 41.350, -3.934,  41.353,
  5.259,-13.585,  5.263,-13.582, -4.721, 63.269, -4.722,  63.266,
  4.763, 41.467,  4.764, 41.474, -4.584,-37.898, -4.586, -37.893,
  4.065, 62.952,  4.072, 62.958, -4.823,-16.167, -4.825, -16.166
};
const double LVDT_SCALE[32] = { 
  0.999,  0.000,  0.999,  0.000,  0.999,  0.000,  0.999,  0.000,
  0.997,  0.000,  0.997,  0.000,  0.997,  0.000,  0.997,  0.000,
  0.993,  0.000,  0.993,  0.000,  1.002,  0.000,  1.002,  0.000,
  1.004,  0.000,  1.004,  0.000,  1.002,  0.000,  1.002,  0.000
};
#else
/* Here are corected (aligned)  "nontilted" LVDT constants
   - nonlinear/erratic dependence of Ypos corrections on LVDT
     for all EAST RP stations approximate with average 
   - xMPT required be the same in all branches
   - no requirments on yMPT
   - individual planes in alignment
   - position dependent corrections applied 
*/
const char *LVDT_REVISION = "LVDTConst Version: 1.1";
const double LVDT_OFFSET[32] = { 
   4.991, -36.620,   4.945, -36.610,  -4.147,  41.738,  -4.103,  41.722,
   5.114, -14.433,   5.197, -14.490,  -4.439,  64.106,  -4.679,  63.878,
   4.708,  42.106,   4.764,  41.758,  -5.443, -37.980,  -5.365, -38.274,
   4.065,  63.583,   4.072,  63.513,  -3.665, -16.674,  -3.714, -16.881,
};

const double LVDT_SCALE[32] = { 
   0.999,  -0.010,   0.999,  -0.011,   0.999,   0.000,   0.999,   0.000,
   0.997,   0.000,   0.997,   0.000,   0.997,   0.000,   0.997,   0.000,
   0.993,   0.000,   0.993,   0.000,   0.966,   0.000,   0.965,   0.000,
   1.004,   0.000,   1.004,   0.000,   1.050,   0.000,   1.049,   0.000,
};
#endif
#endif
//
#ifdef LVDT_TILTED
#ifdef LVDT_INIT
// here are initial values as in Lukasz presentation: 
// https://drupal.star.bnl.gov/STAR/system/files/fulek_April_29_2015.pdf
const char *LVDT_REVISION = "LVDTConst Version: 0.0 ";
const double LVDT_OFFSET[32] = { 
  4.5520,-36.2900,  4.5490,-36.2900, -4.1640, 41.7790, -4.1650,  41.782,
  6.2860,-15.7740,  6.2900,-15.7710, -5.1050, 63.9790, -5.1060,  63.976,
  8.0000, 47.9920,  8.0010, 47.9990, -3.4010,-35.3500, -3.4030, -35.345,
  3.9880, 62.8090,  3.9940, 62.8150, -4.9710,-16.4460, -4.9730, -16.445
};
const double LVDT_SCALE[32] = { 
  0.9989,  0.0000,  0.9989,  0.0000,  0.9991,  0.0000,  0.9991,  0.0000,
  0.9969,  0.0051,  0.9969,  0.0051,  0.9974,  0.0000,  0.9974,  0.0000,
  0.9933, -0.0142,  0.9933, -0.0142,  1.0016,  0.0058,  1.0016,  0.0058,
  1.0040,  0.0000,  1.0040,  0.0000,  1.0019,  0.0000,  1.0019,  0.0000
};
#else
#ifdef LVDT_V10 
// xMPT required to be the same in all branches
// no requirments on yMPT
// offsets nonzero intercept position {x0,y0}={~1.2,~0.9} mm
const char *LVDT_REVISION = "LVDTConst Version: 1.0 ";
const double LVDT_OFFSET[32] = { 
  5.4614,-36.7738,  5.4584,-36.7738, -4.4676, 41.5957, -4.4686,  41.599,
  5.5056,-14.6380,  5.5096,-14.6350, -4.7711, 63.6775, -4.7721,  63.674,
  6.2094, 44.2864,  6.2104, 44.2934, -3.9615,-36.2429, -3.9635, -36.238,
  5.6527, 65.6694,  5.6587, 65.6754, -4.4285,-14.9431, -4.4305, -14.942,
};
const double LVDT_SCALE[32] = { 
  0.9989,  0.0000,  0.9989,  0.0000,  0.9991,  0.0000,  0.9991,  0.0000,
  0.9969,  0.0051,  0.9969,  0.0051,  0.9974,  0.0000,  0.9974,  0.0000,
  0.9933, -0.0142,  0.9933, -0.0142,  1.0016,  0.0058,  1.0016,  0.0058,
  1.0040,  0.0000,  1.0040,  0.0000,  1.0019,  0.0000,  1.0019,  0.0000
};
#else
#ifdef LVDT_V20
// xMPT same in all branches
// no requirments on yMPT
// individual planes in alignment
// position dependent corrections applied 
const char *LVDT_REVISION = "LVDTConst Version: 2.0";
const double LVDT_OFFSET[32] = { 
  5.4561,-36.8055,  5.4104,-36.7974, -4.4512, 41.6271, -4.4076, 41.6106,
  5.4395,-14.6417,  5.5217,-14.7002, -4.6579, 63.7561, -4.8961, 63.5281,
  6.2214, 44.4315,  6.2917, 44.0845, -5.0226,-36.1377, -4.9259,-36.4143,
  5.6669, 65.6778,  5.6349, 65.6091, -3.1667,-14.8623, -3.1441,-15.0686,
};

const double LVDT_SCALE[32] = { 
  0.9989,  0.0000,  0.9989,  0.0000,  0.9991,  0.0000,  0.9991,  0.0000,
  0.9969,  0.0051,  0.9969,  0.0051,  0.9974,  0.0000,  0.9974,  0.0000,
  0.9933, -0.0142,  0.9933, -0.0142,  0.9658,  0.0058,  0.9655,  0.0058,
  1.0040,  0.0000,  1.0040,  0.0000,  1.0499,  0.0000,  1.0499,  0.0000,
};
#else
// xMPT same in all branches
// no requirments on yMPT
// individual planes in alignment
// position dependent corrections applied 
// W1UX disabled
const char *LVDT_REVISION = "LVDTConst Version: 3.0";
const double LVDT_OFFSET[32] = { 
  5.4563,-37.9128,  5.4105,-37.9051, -4.4511, 41.7790, -4.4077, 41.7820,
  5.4398,-15.7740,  5.5215,-15.7710, -4.6588, 63.8988, -4.8970, 63.6778,
  6.2218, 42.2297,  6.2915, 41.8690, -4.7808,-36.8949, -4.6842,-37.1721,
  5.6660, 63.5386,  5.6344, 63.4724, -3.3954,-15.4391, -3.3733,-15.6504,
};

const double LVDT_SCALE[32] = { 
  0.9989,  0.0000,  0.9989,  0.0000,  0.9991,  0.0000,  0.9991,  0.0000,
  0.9969,  0.0051,  0.9969,  0.0051,  0.9974, -0.0124,  0.9974, -0.0122,
  0.9933, -0.0285,  0.9933, -0.0280,  0.9658,  0.0229,  0.9655,  0.0229,
  1.0040, -0.0220,  1.0040, -0.0222,  1.0498,  0.0225,  1.0498,  0.0223,
};
#endif
#endif
#endif
#endif


#endif
