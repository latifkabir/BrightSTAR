// Filename: CronJob.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Feb 29 23:54:14 2020 (-0500)
// URL: jlab.org/~latif

void CronJob2()
{
    TStScheduler *sc = new TStScheduler();

    //Optimal setup for FMS stream
    sc->SetJobThreshold(1500); // Run 17, initial
    //sc->SetJobThreshold(1000); // Number of threshold jobs that will trigger new submission
    //sc->SetJobThreshold(1500); // Number of threshold jobs that will trigger new submission
    sc->SetInterval(20);      // Wait time in minutes
    //sc->SetRunIncrement(20);  // Number of runs to be incremented (physics stream)
    sc->SetRunIncrement(30);  // Number of runs to be incremented (Run 17 Fms stream, initial)
    //sc->SetRunIncrement(100);  // Number of runs to be incremented (Fms stream)
    //sc->SetRunIncrement(160);  // aggresive Number of runs to be incremented
    //sc->SetMaxFilesPerJob(5); //Maximum number of files per job (physics stream)
    sc->SetMaxFilesPerJob(1); //Maximum number of files per job (Fms stream)
    
    //sc->CronJob("AnRunNanoDstMaker", gFirstRun, gFirstRun); // For Test
    //sc->CronJob("AnRunNanoDstMaker", 16093018, 16093018);

    
    //sc->CronJob("RunFmsHotChQaMaker");
    
    //sc->CronJob("EjRunEmJetTreeMaker", 16080028, gLastRun); 
    //sc->CronJob("EjRunEmJetTreeMaker", 16079013, gLastRun); // For partial dataset
    //sc->CronJob("RunFmsHotChQaMaker", 16071023, gLastRun); // For partial dataset

    //sc->CronJob("EjRunEmJetTreeMakerEEmcSmd", 16064083, gLastRun);
    
    //sc->CronJob("EjRunEmJetTreeMaker"); // For full dataset    
    //sc->CronJob("EjRunEmJetTreeMaker", -1, -1, "_zn_"); // For full dataset    
    //sc->CronJob("RunFmsHotChQaMaker"); // For full dataset

    cout << "------------------> Cron Jobs for Group 4 <--------------------" <<endl;
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin4.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18087063, 18091049, "Gr4"); 

    cout << "------------------> Cron Jobs for Group 5 <--------------------" <<endl;
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin5.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18091050, 18094031, "Gr5"); 

    cout << "------------------> Cron Jobs for Group 6 <--------------------" <<endl;
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin6.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18094032 , 18098014, "Gr6");

    cout << "------------------> Cron Jobs for Group 7 <--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin7.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18098015 , 18102012, "Gr7");

    cout << "------------------> Cron Jobs for Group 8 <--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin8.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18102013 , 18105044, "Gr8");

    cout << "------------------> Cron Jobs for Group 9 <--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin9.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18107102 , 18114012, "Gr9");

    cout << "------------------> Cron Jobs for Group 10<--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin10.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18114013 , 18122030, "Gr10");

    cout << "------------------> Cron Jobs for Group 11 <--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin11.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18122031 , 18128022, "Gr11");

    cout << "------------------> Cron Jobs for Group 12 <--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin12.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18128023 , 18132012, "Gr12");

    cout << "------------------> Cron Jobs for Group 13 <--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin13.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18132013 , 18136010, "Gr13"); 

    cout << "------------------> Cron Jobs for Group 14 <--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin14.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");    
    sc->CronJob("EjRunEmJetTreeMaker", 18136011 , 18139061, "Gr14"); 

    cout << "------------------> Cron Jobs for Group 15 <--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin15.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18139062 , 18142026, "Gr15");

    cout << "------------------> Cron Jobs for Group 16 <--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin16.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18142027 , 18147011, "Gr16");

    cout << "------------------> Cron Jobs for Group 17 <--------------------" <<endl;    
    gROOT->ProcessLine(".! cp /star/u/kabir/GIT/BrightSTAR/database/run17FmsGC/FmsGainCorr_fin17.txt /star/u/kabir/GIT/BrightSTAR/FmsGainCorr.txt");
    sc->CronJob("EjRunEmJetTreeMaker", 18147012 , 18149069, "Gr17"); 
}

