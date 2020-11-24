
#ifdef __CINT__

#pragma link C++ function AnFmsPos2ChLookupTable;                // Make 2D Hist as FMS Position to channel lookup table
#pragma link C++ function AnFmsGetChFromPos;                     // Get FMS ch and det id from position
#pragma link C++ function AnFmsPrintChIdOnHist;                  // Print det/ch id on given 2d histogram
#pragma link C++ function AnFmsHotChFinder;                      // Fms Hot and Bad Channels Finder
#pragma link C++ function AnFmsCellActivity;                     // Fms Hot and Bad Channels on 2d plot for manual iteration
#pragma link C++ function AnHotChQaSaveAsPdf;                    // Fms Hot and Bad Channels on 2d plot for manual iteration
#pragma link C++ function AnFmsPhotonAngDist;                    // Fms Hot and Bad Channels Finder QA
#pragma link C++ function AnFmsPhotonAngDistJobs;                // Fms Hot and Bad Channels Finder QA Jobs

#pragma link C++ function AnRunFmsRpTreeMaker;                   // Run Fms + Rp Tree Maker
#pragma link C++ class TAnFmsRpTreeReader;                       // Fms + Rp Tree Reader
#pragma link C++ function AnFmsRpQA;                             // Fms + Rp Correlation and event selection QA with multiple tracks allowed
#pragma link C++ function AnFmsRpCorrUpb;                        // Fms + Rp Correlation with only 1 good RP track per event allowed (Unpolarized proton breaks)
#pragma link C++ function AnFmsRpCorrPpb;                        // Fms + Rp Correlation with only 1 good RP track per event allowed (Polarized proton breaks)
#pragma link C++ function AnFmsRpCorrMergedFull;                 // Fms + Rp Correlation with only 1 good RP track per event allowed, uses fully merged single root file.

#pragma link C++ function AnRunEEmcRpTreeMakerPart1;             // Rp + EEMC Part1 Tree Maker
#pragma link C++ class TAnEEmcRpTreeReader;                      // EEMC + Rp Tree Reader
#pragma link C++ function AnEEmcRpCorr;                          // EEMC + Rp Correlation with only 1 good RP track per event allowed
#pragma link C++ function AnEEmcRpCorrUpb;                       // EEMC + Rp Correlation with only 1 good RP track per event allowed, unpolarized proton breaks
#pragma link C++ function AnEEmcRpCorrMergedFullPpb;             // EEMC + Rp Correlation with only 1 good RP track per event allowed, uses fully merged single root file.

#pragma link C++ function AnRunAnTreeMaker;                      // Run AnTree Maker
#pragma link C++ function AnDstQA;                               // DST QA Plots
#pragma link C++ function AnFmsRpPionSource;                     // For Rp-stream, plot pions based on category/source
#pragma link C++ function AnRunNanoDstMaker;                     // Nano DST maker for AN Analysis

#pragma link C++ function AnRunDiffJetTreeMaker;                 // Diffractive Jet Tree Maker

#endif
