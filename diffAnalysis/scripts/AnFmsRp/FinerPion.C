// Filename: FinerPion.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Mar 19 17:46:42 2020 (-0400)
// URL: jlab.org/~latif

void FinerPion()
{
   //For pid def see: https://www.star.bnl.gov/webdata/dox/html/StFmsPoint_8h_source.html
    //T->Draw("fmsPointPair.mM>>hist(200, 0, 1.0)", "fmsPointPair.mE > 0 && fmsPointPair.mZgg < 0.8 && Iteration$==0 && (fmsPointPair.mFpsPid1 > 9 && fmsPointPair.mFpsPid1 < 17) && (fmsPointPair.mFpsPid2 > 9 && fmsPointPair.mFpsPid2 < 17)");
    //T->Draw("sqrt(fmsPointPair.mY1*fmsPointPair.mY1 + fmsPointPair.mX1*fmsPointPair.mX1)");


    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList();
    Int_t maxRuns = runList->GetN();
    Int_t run = 0;
    Int_t nRunsDone = 0;
    TString fileName;
    TString filePrefix = "/star/u/kabir/GIT/BrightSTAR/dst/R15FmsTrigNanoDst/AnRunNanoDstMaker_";

    TH2D *h2Merged = new TH2D("FmsPointXY_Merged", "FMS point Y vs X",  50, -100, 100, 50, -100, 100);
    TH2D *h2temp;
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
	h2temp = (TH2D*)file->Get("FmsPointXY");
	h2Merged->Add(h2temp);

       
	++nRunsDone;
	file->Close();
	delete file;
	
    }

    TCanvas *c1 = new TCanvas();
    h2Merged->Draw("colz");

    TCanvas *c2 = new TCanvas();
    h2Merged->Draw("lego");

    TFile * mergedFile = new TFile("FmsPointXYmerged.root", "recreate");
    h2Merged->Write();
    
}
