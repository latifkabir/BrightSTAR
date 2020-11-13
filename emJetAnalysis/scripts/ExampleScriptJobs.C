// Filename: ExampleScriptJobs.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Nov  5 12:10:59 2020 (-0500)
// URL: jlab.org/~latif

void ExampleScriptJobs()
{

    Int_t fillNo[] = {
	18753,
	18754,
	18758,
	18761,
	18763,
	18764,
	18765,
	18766,
	18772,
	18776,
	18777,
	18778,
	18779,
	18780,
	18787,
	18788,
	18789,
	18790,
	18792,
	18793,
	18794,
	18795,
	18796,
	18797,
	18798,
	18800,
	18801,
	18803,
	18804,
	18805,
	18807,
	18815,
	18816,
	18818,
	18819,
	18820,
	18827,
	18828,
	18829,
	18837,
	18838,
	18843,
	18846,
	18847,
	18848,
	18849,
	18850,
	18852,
	18853,
	18854,
	18855,
	18856,
	18857
    };

    TString filePrefix = "/star/u/zwzhu/01zwzhu/Run15/byfill/ipi0jetcor_stfms_Fill";
    Float_t ebinlows[] = {18, 23, 28, 33, 38, 43, 48, 53, 58, 65, 75, 120};
    Float_t ebinhighs[] = {23, 28, 33, 38, 43, 48, 53, 58, 65, 75, 90, 150};

    Int_t nFills = 51;
    Int_t nBins = 12;

    vector <string> jobList;
    
    for(Int_t i = 0; i < nFills; ++i)
    {
	for(Int_t j = 0; j < nBins; ++j)
	{
	    TString fileName = filePrefix;
	    fileName += fillNo[i];
	    fileName += ".root";

	    // TString txtName = Form("/star/u/kabir/pwg/zhanwen_bug_test/results/Run15/jetAn2/jetAnJProf_Fill%d_mass%.2fto%.2fGeV_E%.1fto%.1fGeV.txt",fillNo[i], 1.00, 1.00, ebinlows[j], ebinhighs[j]);
	    // if(!gSystem->AccessPathName(txtName))
	    // 	continue;
	    
	    TString job = ".x /star/u/kabir/GIT/BrightSTAR/emJetAnalysis/scripts/crjetAn.C(";
	    job += ebinlows[j];
	    job += ",";
	    job += ebinhighs[j];
	    job +=  ", 1, 1,";
	    job += fillNo[i];
	    job += ", \"";
	    job += fileName; 
	    job += "\")"; 
	    cout << job <<endl;
	    jobList.push_back(string(job));
	}
    }

    TStScheduler::SubmitJob(jobList, "zhanwenCompare");
	
}
