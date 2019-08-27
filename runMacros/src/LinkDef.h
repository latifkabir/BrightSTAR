// Filename: LinkDef.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Jul  1 13:52:59 2019 (-0400)
// URL: jlab.org/~latif

#ifdef __CINT__

#pragma link C++ function RunDeltaPhiMaker;                      // Read Track Info data (example/Demo)


#pragma link C++ function RunRpQAMaker;                          // Read RP QA Maker


#pragma link C++ function RunEEmcMakerEx1;                       // Read EEMC data (example/Demo)
#pragma link C++ function RunEEmcQAMaker;                        // Read EEMC QA Maker
#pragma link C++ function RunEEmcEvtDispMaker;                   // Read EEMC event display with ADC and enegry event by event for Tower, SMD, Pre-1, 2 and Post 
#pragma link C++ function RunEEmcEvtDispEngMaker;                // Read EEMC event display with enegry event by event for Tower, SMD, Pre-1, 2 and Post 
#pragma link C++ function RunEEmcMakerGeomEx;                    // Read EEMC geometry info example
#pragma link C++ function RunEEmcIUPi0;                          // Pi0 reconsntruction in EEMC  using IU algorithm
#pragma link C++ function RunEEmcTreeMakerPart1;                 // EEMC reconstruction using TSIU algorithm. This is step no. 1 out of 3 of the process. This is the algorithm followed for the analysis. To be run with local StRoot copy.
#pragma link C++ function RunEEmcTreeMakerPart2and3;             // EEMC reconstruction using TSIU algorithm. This is step no. 2 and 3 out of 3 of the process. This is the algorithm followed for the analysis.


#pragma link C++ function RunFmsTreeMaker;                       // FMS reconstruction 

#pragma link C++ function RunEmcTreeMaker;                       // Emc gamma, pi0 reconstruction 

#endif
