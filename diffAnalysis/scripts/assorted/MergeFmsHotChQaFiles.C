// Filename: MergeFmsHotChQaFiles.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Jun  2 12:54:51 2020 (-0400)
// URL: jlab.org/~latif

void MergeFmsHotChQaFiles()
{
    TString filePath = "~/GIT/BrightSTAR/dst/fmsHotChQaRedone/RunFmsHotChQaMaker_";
    TString fileName;
    
    TStFillNoDB db;
    vector <int> fills = db.GetAllFillNos();
    vector <int> runs;
    cout << "Total Fills: "<<fills.size() <<endl;

    for(Int_t i = 0; i < fills.size(); ++i)
    {
	runs.clear();
	runs = db.GetRunsWithFill(fills[i]);
	TString inFiles;
	TString outFile;
	outFile = "~/GIT/BrightSTAR/dst/fmsHotChQaRedone/R15FmsHotChQa_Fill_";
	outFile += fills[i];
	outFile += ".root";
	
	for(Int_t j = 0; j < runs.size(); ++j)
	{
	    //cout<< runs[j] <<endl;
	    fileName = filePath;
	    fileName += runs[j];
	    fileName += ".root";
	    
	    inFiles += "  ";

	    if(gSystem->AccessPathName(fileName))
		inFiles += fileName;
	}
	
	cout << outFile << "\t"<<inFiles <<endl;
	gROOT->ProcessLine(".! hadd " + outFile + " " + inFiles);
    }
    
}
