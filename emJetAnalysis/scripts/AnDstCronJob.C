// Filename: AnDstCronJob.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Feb 29 23:54:14 2020 (-0500)
// URL: jlab.org/~latif

void AnDstCronJob()
{
    TStScheduler *sc = new TStScheduler();
    sc->SetJobThreshold(500); // Number of threshold jobs that will trigger new submission
    sc->SetInterval(10);      // Wait time in minutes
    sc->SetRunIncrement(20);  // Number of runs to be incremented

    //sc->CronJob("AnRunNanoDstMaker", gFirstRun, gFirstRun); // For Test
    //sc->CronJob("AnRunNanoDstMaker", 16093018, 16093018);
    
    sc->CronJob("EjRunJetTreeMaker", 16080020, 16081004);
    
    //sc->CronJob("AnRunNanoDstMaker"); // For full dataset
}
