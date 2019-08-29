
#ifdef __CINT__

//RP
#pragma link C++ function RpReadDemo;                     // Read RP data
#pragma link C++ function RpQA;                           // Various RP distributions

//EEMC
#pragma link C++ function EEmcReadDemo;                   // Read EEMC data
#pragma link C++ function EEmcReadDemo2;                  // Read EEMC data
#pragma link C++ function EEmcQA;                         // Various EEMC distributions
#pragma link C++ function EEmcEvent;                      // Various EEMC distributions

//FMS
#pragma link C++ function FmsQA;                          // FMS QA for cell status
#pragma link C++ function FmsQAusingMaker;                // FMS QA for cell status using maker
#pragma link C++ function FmsCellStatus;                  // FMS cell status list and plot
#pragma link C++ function FmsPi0InvMass1;                 // FMS Pi0 Invariant mass directly from MuDST files.
#pragma link C++ function FmsPi0MakerEx;                  // FMS Pi0 Invariant mass directly from MuDST files using StFmsPointPair.

#endif


/*
--------  List of yet to be compiled scripts -----------
EmcPi0Maker     : Pi0 from EMC points
EmcReadEnergy   : Example to get energy from EMC ADCs
EmcReadPoint    : Point reconstruction for EMC data

*/
