
#ifdef __CINT__

#pragma link C++ class TStar+;                   // Collection of static fields and functions
#pragma link C++ class TStConfig+;               // Star configuration
#pragma link C++ class TStRun+;                  // Star configuration
#pragma link C++ class TStRunList+;              // Star configuration
#pragma link C++ function help;                  // Helper function
#pragma link C++ function history;               // Print Command History
//Batch Farm Job
#pragma link C++ function SubmitJob;             // Submit job using star template scheduler
#pragma link C++ function JobStatus;             // Check Job Status
#pragma link C++ function SubmitJob;             // Sumit Condor Job
#pragma link C++ function SubmitSumsJob;         // Sumit Job using SUMS

#endif
