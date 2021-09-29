// Filename: AnalysisTreeQaJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Jan 15 18:47:02 2020 (-0500)
// URL: jlab.org/~latif

// Make sure to change 3 places nummbered below

void AnalysisTreeQaJobs(Int_t firstRun = -1, Int_t lastRun = -1)
{
    vector <string> jobList;
    //------- 1.Change Here Function Name ------------
    TString funcName = "EjAnalysisTreeQa";
    TString jobName = "EjAnalysisTreeQaR17Pass0Fms";
    //--------------------------------------
    
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList(firstRun, lastRun);
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    TString fileName;
    TString outName;
    //------- 2.Change Here the path and file prefix ---------------
    TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/emJet/run17/pass0/Run17FmsEmJetGr1_10/NanoJetTree_EjRunEmJetTreeMaker_";
    //TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/emJet/pass2/eemc/NanoJetTree_EjRunEmJetTreeMakerEEmcSmd_";
    //TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/emJet/pass1/eemc/R15EmJetNanoTree/NanoJetTree_RunEmJetTreeMaker_"; //<--------- This is PASS 1 !!
    //----------
    
    cout << "Total number of files to be processed: "<< maxRuns <<endl;

    //------------ Loop over runs --------------------------
    for (Int_t run_i = 0; run_i < maxRuns; ++run_i)
    {
	run = runList->GetEntry(run_i);
	fileName = filePrefix;
	fileName += run;
	fileName += ".root";
	outName = jobName;
	outName += "_";
	outName += run;
	outName += ".root";
	
	if(gSystem->AccessPathName(fileName))
	{
	    cout << "No DST file for run number "<< run <<" ... SKIPPED."<<endl;
	    continue;
	}
	cout << "Processing run number: "<< run <<endl;

	//-------------- 4.Change Here to fit Function's argument ---------------
	TString argList = "(";
	argList += "\"";
	argList += fileName;
	argList += "\"";
	argList += ",\"";
	argList += outName;
	argList += "\"";
	argList += ",\"fms\"";
	argList += ")";	
	//---------------------------------------------------------------------
	
	cout << (funcName + argList)<<endl;
	jobList.push_back(string(funcName + argList));
	//break;
    }
    TStScheduler::SubmitJob(jobList, jobName);
}
