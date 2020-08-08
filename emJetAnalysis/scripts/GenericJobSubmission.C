// Filename: GenericJobSubmission.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Jan 15 18:47:02 2020 (-0500)
// URL: jlab.org/~latif

// Make sure to change 3 places nummbered below

void GenericJobSubmission(Int_t firstRun = -1, Int_t lastRun = -1)
{
    //------- 1.Change Here Function Name ------------
    TString funcName = "EjMakeAnalysisTree";
    //TString funcName = "AnFmsRpCorrPpb";
    //TString funcName = "AnEEmcRpCorrUpb";
    //TString jobName = funcName;
    TString jobName = "NanoJetTree_RunEmJetTreeMaker";
    //--------------------------------------
    
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList(firstRun, lastRun);
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    TString fileName;

    //------- 2.Change Here the path and file prefix ---------------
    TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/scratch/EmJetTree_part1/";
    //----------
    
    cout << "Total number of files to be processed: "<< maxRuns <<endl;

    //------------ Loop over runs --------------------------
    for (Int_t run_i = 0; run_i < maxRuns; ++run_i)
    {
	run = runList->GetEntry(run_i);
	fileName = filePrefix;
	// fileName += run;
	// fileName += ".root";

	if(gSystem->AccessPathName(fileName))
	{
	    cout << "No DST file for run number "<< run <<" ... SKIPPED."<<endl;
	    continue;
	}
	cout << "Processing run number: "<< run <<endl;

	TString argList;
	TString currentJobName;

	TString jetfile = filePrefix + "jets_RunEmJetTreeMaker_";
	jetfile += run;
	jetfile += ".root";
	TString skimfile = filePrefix + "skim_RunEmJetTreeMaker_";
	skimfile += run;
	skimfile += ".root";

	//------------------------ 3. Change Here ---------------
	TString outName = jobName;
	outName += "_";
	//TString outName = "EmJetAnaTree_Fms_";
	outName += run;
	outName += ".root";
	
	currentJobName = jobName + "_";
	currentJobName += run_i;

	//-------------- 4.Change Here to fit Function's argument ---------------
	//Fms-Rp Corr
	argList = "(\"eemc\",-1,\"";
	argList += jetfile;
	argList += "\",\"";
	argList += skimfile;
	argList += "\",\"";
	argList += outName;
	argList += "\")";

	//EEmc-Rp Corr
	// argList = "(";
	// argList += run;
	// argList += ",";
	// argList +=run;
	// argList += ",\"" + currentJobName + ".root\"";
	// argList += ")";
	
	//---------------------------------------------------------------------
	
	cout << (funcName + argList)<<endl;

	TStScheduler::SubmitGenericJob(funcName + argList, currentJobName);
	//break;
    }
}
