// Filename: CronJob.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Feb 29 23:54:14 2020 (-0500)
// URL: jlab.org/~latif

void CronJob()
{
    TStScheduler *sc = new TStScheduler();
    sc->SetJobThreshold(500); // Number of threshold jobs that will trigger new submission
    sc->SetInterval(20);      // Wait time in minutes
    sc->SetRunIncrement(40);  // Number of runs to be incremented
    sc->SetMaxFilesPerJob(1); //Maximum number of files per job
    
    //sc->CronJob("AnRunNanoDstMaker", gFirstRun, gFirstRun); // For Test
    //sc->CronJob("AnRunNanoDstMaker", 16093018, 16093018);

    
    //sc->CronJob("RunFmsHotChQaMaker");
    
     sc->CronJob("RunEmJetTreeMaker", 16080020, 16080033); 
    //sc->CronJob("RunEmJetTreeMaker", 16080049, gLastRun); // For partial dataset
    //sc->CronJob("RunFmsHotChQaMaker", 16071023, gLastRun); // For partial dataset

    
    //sc->CronJob("RunEmJetTreeMaker"); // For full dataset    
    //sc->CronJob("RunFmsHotChQaMaker"); // For full dataset    
}
