// Filename: AnFmsPhotonAngDistForAllFills.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Jul 18 15:09:39 2020 (-0400)
// URL: jlab.org/~latif

#include "iostream"
#include <map>
#include <vector>
#include "TStFillNoDB.h"
#include "TStScheduler.h"

using namespace std;

void AnFmsPhotonAngDistJobs()
{
    TStFillNoDB fDb;
    map <int, vector <int> > db = fDb.GetFillNoDB();
    map <int, vector <int> >::iterator it;
    Int_t nJobs = 0;
    for(it = db.begin(); it!= db.end(); ++it)
    {
	cout << "Fill: "<< it->first <<endl;
	cout << "Submitting the first run (first file) from the fill ...." <<endl;
	cout << "Run: "<<it->second[0]<<"    " <<endl;
	TString outName = "FmsPhotonAngDist_Fill_" + to_string(it->first) + ".root";
	TString jobName = "FmsPhotonAngDist_Fill_" + to_string(it->first);
	TStScheduler::SubmitJob("AnFmsPhotonAngDist", it->second[0], 1, outName, jobName);

	++nJobs;
	if(nJobs > 100)
	{
	    cout << "Breaking the jobs as a precaution ... ..." <<endl;
	    break;
	}
    }    
}
