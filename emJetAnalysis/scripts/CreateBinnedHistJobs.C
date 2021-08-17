// Filename: CreateBinnedHistJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Jan 15 18:47:02 2020 (-0500)
// URL: jlab.org/~latif

// Make sure to change 3 places nummbered below

void CreateBinnedHistJobs(Int_t firstRun = -1, Int_t lastRun = -1)
{
    vector <string> jobList;
    //------- 1.Change Here Function Name ------------
    //TString funcName = "EjCreateBinnedHist";
    TString funcName = "EjCreateBinnedHistExtended";
    //TString jobName = "EjCreateBinnedHisR17G1_3Pass0";
    TString jobName = "R15EEmcEjCreateBinnedHistExtPass4_AllPh";
    //--------------------------------------
    
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList(firstRun, lastRun);
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    TString fileName;

    //------- 2.Change Here the path and file prefix ---------------
    //TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/EmJetWOmasking_znFms/NanoJetTree_EjRunEmJetTreeMaker_";
    //TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/emJet/pass2/eemc/NanoJetTree_EjRunEmJetTreeMakerEEmcSmd_";
    TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass4/eemcTow/NanoJetTree_EjRunEmJetTreeMaker_";
    //TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass5/fms/nanoDst/NanoJetTree_EjRunEmJetTreeMaker_";

    //TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/scratch/EmJetEEmcTowPass4Merged/NanoJetTree_EjRunEmJetTreeMaker_";
    //TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/EmJetWOmaskingUe_znFms/NanoJetTree_EjRunEmJetTreeMaker_";
    //TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/emJet/run17/pass0/Run17FmsEmJetGr1_3/NanoJetTree_EjRunEmJetTreeMaker_";
    //----------
    
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

	//-------------- 4.Change Here to fit Function's argument ---------------
	TString argList = "(";
	argList += run;
	argList += ",\"";
	argList += filePrefix;
	//argList += "\",\"fms\","; //<------ Change detector
	argList += "\",\"eemc\","; //<------ Change detector
	argList += run;
	argList += ",";
	argList += run;
	argList += ")"; //<--------------- Update Here, min number of photon 
	//argList += ", 3)"; //<--------------- Update Here, min number of photon 
	
	//---------------------------------------------------------------------
	
	cout << (funcName + argList)<<endl;
	jobList.push_back(string(funcName + argList));
    }
    TStScheduler::SubmitJob(jobList, jobName);
}
