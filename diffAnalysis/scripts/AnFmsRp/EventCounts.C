// Filename: EventCounts.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Mar  5 14:16:16 2020 (-0500)
// URL: jlab.org/~latif


void EventCounts()
{
        //------- Change Here Function Name ------------
    TString funcName = "AnFmsRpCorr";
    TString jobName = funcName;
    //--------------------------------------
    
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList();
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    TString fileName;
    Int_t runCount = 0;
    TGraph *gr = new TGraph();
    
    //------- Change Here the path and file prefix ---------------
    TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/R15FmsTrigNanoDst/AnRunNanoDstMaker_";
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
	TFile *file = new TFile(fileName);
	TTree *t = (TTree*) file->Get("T");
	if(!t)
	{
	    cout << "No TTree in the file" <<endl;
	    continue;
	}
	cout << "Number of Entries:"<< t->GetEntries() <<endl;

	gr->SetPoint(runCount, runCount+100, t->GetEntries());
	++runCount;
	file->Close();
    }

    gr->Draw();
}
