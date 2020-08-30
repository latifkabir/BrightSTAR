// Filename: OverviewJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Aug 25 02:58:05 2020 (-0400)
// URL: jlab.org/~latif

void OverviewJobs()
{
    TStFillNoDB db;
    vector <int> fills = db.GetAllFillNos();
    int run;
    
    for(int k = 0; k < fills.size(); ++k)
    {
	run = db.GetRunsWithFill(fills[k])[0];
	TStScheduler::SubmitJob("RunEmJetTreeMaker", run, 1);
    }    
}
