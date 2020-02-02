
#ifdef __CINT__

#pragma link C++ function AnRunFmsRpTreeMaker;                   // Run Fms + Rp Tree Maker
#pragma link C++ class TAnFmsRpTreeReader;                       // Fms + Rp Tree Reader
#pragma link C++ function AnFmsRpQA;                             // Fms + Rp Correlation and event selection QA with multiple tracks allowed
#pragma link C++ function AnFmsRpCorr;                           // Fms + Rp Correlation with only 1 good RP track per event allowed
#pragma link C++ function AnFmsRpCorrMergedFull;                 // Fms + Rp Correlation with only 1 good RP track per event allowed, uses fully merged single root file.

#pragma link C++ function AnRunEEmcRpTreeMakerPart1;             // Rp + EEMC Part1 Tree Maker
#pragma link C++ class TAnEEmcRpTreeReader;                      // EEMC + Rp Tree Reader
#pragma link C++ function AnEEmcRpCorr;                          // EEMC + Rp Correlation with only 1 good RP track per event allowed
#pragma link C++ function AnEEmcRpCorrMergedFull;                // EEMC + Rp Correlation with only 1 good RP track per event allowed, uses fully merged single root file.
#pragma link C++ function AnResubmitJobs;                        // Resubmit failed jobs

#pragma link C++ function AnRunAnTreeMaker;                      // Run AnTree Maker
#pragma link C++ function AnDstQA;                               // DST QA Plots
#pragma link C++ function AnFmsRpPionSource;                     // For Rp-stream, plot pions based on category/source

#endif
