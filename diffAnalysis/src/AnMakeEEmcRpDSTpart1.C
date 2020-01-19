// Filename: AnMakeEEmcRpDSTPart1.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Dec  4 00:08:00 2019 (-0500)
// URL: jlab.org/~latif


//Workflow:
//Make sure config file is set to physics for RunList DD, trigger and stream-path
//Check that the run increment is right (use 10 for physics stream) in the cron job

//Once jobs are completed, use AnRemoveBadFiles.C to remove Zombie files.
//Use the shell script under jobResults/R15EEmcRptree to merge trees
//The RunList can be created with command "ls -1 >> runList.txt" (remove last entry which is the list itself)


void AnMakeEEmcRpDSTpart1()
{
    TStScheduler::CronJob("AnRunEEmcRpTreeMakerPart1");
}


