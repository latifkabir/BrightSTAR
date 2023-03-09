// Filename: CronJob.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Feb 29 23:54:14 2020 (-0500)
// URL: jlab.org/~latif

void CronJob()
{
    TStScheduler *sc = new TStScheduler();

    //Optimal setup for FMS stream
    sc->SetJobThreshold(1000); // Run 17, initial
    //sc->SetJobThreshold(1000); // Number of threshold jobs that will trigger new submission
    //sc->SetJobThreshold(1500); // Number of threshold jobs that will trigger new submission
    sc->SetInterval(20);      // Wait time in minutes
    // sc->SetRunIncrement(40);  // Number of runs to be incremented (physics stream)
    //sc->SetRunIncrement(30);  // Number of runs to be incremented (Run 17 Fms stream, initial)
    sc->SetRunIncrement(100);  // Number of runs to be incremented (Fms stream)
    //sc->SetRunIncrement(160);  // aggresive Number of runs to be incremented
    // sc->SetMaxFilesPerJob(5); //Maximum number of files per job (physics stream)
    //sc->SetMaxFilesPerJob(2); //Maximum number of files per job (Fms stream)
    sc->SetMaxFilesPerJob(5); //Maximum number of files per job (ssdmb stream)	
    //sc->CronJob("AnRunNanoDstMaker", gFirstRun, gFirstRun); // For Test
    //sc->CronJob("AnRunNanoDstMaker", 16093018, 16093018);

    
    //sc->CronJob("RunFmsHotChQaMaker");
    
    //sc->CronJob("EjRunEmJetTreeMaker", 16080028, gLastRun); 
    //sc->CronJob("EjRunEmJetTreeMaker", 16079013, gLastRun); // For partial dataset
    //sc->CronJob("RunFmsHotChQaMaker", 16071023, gLastRun); // For partial dataset

    //sc->CronJob("EjRunEmJetTreeMakerEEmcSmd", 16064083, gLastRun);
    
    sc->CronJob("EjRunEmJetTreeMaker"); // For full dataset    
    // sc->CronJob("EjRunEmJetTreeMaker", 18071019, 18071019); // For full dataset    
    //sc->CronJob("EjRunEmJetTreeMaker", -1, -1, "_zn_"); // For full dataset    
    //sc->CronJob("RunFmsHotChQaMaker"); // For full dataset

    //sc->CronJob("EjRunEmJetTreeMaker", 18084054, 18087062); // Run 17 dataset
}
