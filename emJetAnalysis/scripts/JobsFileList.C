// Filename: JobsFileList.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Sep 10 06:34:22 2020 (-0400)
// URL: jlab.org/~latif


void JobsFileList()
{

    TString inFile = "/star/u/kabir/GIT/BrightSTAR/resources/failedJobsFiles.list";
    TString fileName;
    ifstream inFileList(inFile); //Use absolute path. No ~ for home directory.
    if(!inFileList)
    {
	cout << "Unable to read run number from file list" <<endl;
	return -1;
    }
    TString prefix;
    TString outName;
       
    while(getline(inFileList, fileName))
    {
	prefix = (TString)"NanoJetTree_RunEmJetTreeMaker_" + TStRunList:::GetRunFromFileName(fileName) + "_" + TStRunList:::GetFileNoFromFileName(fileName);
	outName = prefix + ".root";

	TStScheduler::SubmitJob("RunEmJetTreeMaker", fileName, outName, prefix);
    }
    inFileList.close();
    
}
