// Filename: CronJob.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Feb 29 23:54:14 2020 (-0500)
// URL: jlab.org/~latif

void CronJob()
{
    TStScheduler *sc = new TStScheduler();

    //Optimal setup for FMS stream
    sc->SetJobThreshold(1000); // Number of threshold jobs that will trigger new submission
    sc->SetInterval(10);      // Wait time in minutes
    sc->SetRunIncrement(80);  // Number of runs to be incremented
    sc->SetMaxFilesPerJob(1); //Maximum number of files per job
    
    //sc->CronJob("AnRunNanoDstMaker", gFirstRun, gFirstRun); // For Test
    //sc->CronJob("AnRunNanoDstMaker", 16093018, 16093018);

    
    //sc->CronJob("RunFmsHotChQaMaker");
    
    //sc->CronJob("EjRunEmJetTreeMaker", 16079016, 16081024); 
    //sc->CronJob("EjRunEmJetTreeMaker", 16079013, gLastRun); // For partial dataset
    //sc->CronJob("RunFmsHotChQaMaker", 16071023, gLastRun); // For partial dataset

    sc->CronJob("EjRunEmJetTreeMakerEEmcSmd", gFirstRun, gLastRun -1);
    
    //sc->CronJob("EjRunEmJetTreeMaker"); // For full dataset    
    //sc->CronJob("RunFmsHotChQaMaker"); // For full dataset    
}
