// Filename: GenericJobSubmission.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Jan 15 18:47:02 2020 (-0500)
// URL: jlab.org/~latif

void GenericJobSubmission()
{
    //------- Only Change Here ------------
    TString jobName = "AnFmsRpCorr";
    TString funcName = "AnFmsRpCorr";
    //--------------------------------------
    
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList();
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    TString fileName;
    TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/R15RpStream/AnRunAnTreeMaker_";
    
    cout << "Total number of files to be processed: "<< maxRuns <<endl;

    //------------ Loop over runs --------------------------
    for (Int_t run_i = 0; run_i < maxRuns; ++run_i)
    {
	run = runList->GetEntry(run_i);
	fileName = filePrefix;
	fileName += run;
	fileName += ".root";

	if(gSystem->AccessPathName(fileName))
	{
	    cout << "No DST file for run number "<< run <<" ... SKIPPED."<<endl;
	    continue;
	}
	cout << "Processing run number: "<< run <<endl;
	cout << "Percent completed: "<< (Int_t)(nRunsDone*100.0 / maxRuns) << "%"<<endl;

	TString argList;
	TString currentJobName;
	
	currentJobName = jobName + "_";
	currentJobName += run_i;

	argList = "(";
	argList += run;
	argList += ",";
	argList +=run;
	argList += ",\"" + currentJobName + ".root\")";

	cout << (funcName + argList)<<endl;

	TStScheduler::SubmitGenericJob(funcName + argList, currentJobName);
	//break;
    }

}
