// Filename: LinkDef.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Jul  1 13:52:59 2019 (-0400)
// URL: jlab.org/~latif

#ifdef __CINT__

#pragma link C++ function RunRpDistMaker;                        // Read RP QA Maker
#pragma link C++ function RunDeltaPhiMaker;                      // Read  data (example/Demo) 
#pragma link C++ function RunEEmcDistMaker;                      // Read EEMC QA Maker
#pragma link C++ function RunEEmcMakerEx1;                       // Read EEMC data
#pragma link C++ function RunEEmcEvtDispMaker;                   // Read EEMC event display with ADC and enegry event by event for Tower, SMD, Pre-1, 2 and Post 
#pragma link C++ function RunEEmcEvtDispEngMaker;                // Read EEMC event display with enegry event by event for Tower, SMD, Pre-1, 2 and Post 
//#pragma link C++ function RunEEmcMakerGeomEx;                     // Read EEMC data

#endif
