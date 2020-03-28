// Filename: FinerPion.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Mar 19 17:46:42 2020 (-0400)
// URL: jlab.org/~latif

void FinerPion()
{

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
	if(h2temp)
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

void MaskHotCh()
{
    TFile *f = new TFile("FmsPointXYmerged.root");
    TH2D *hist = (TH2D*)f->Get("FmsPointXY_Merged");

    //hist->Draw("colz");


    for(Int_t i = 1; i <= hist->GetNbinsX(); ++i)
    {
	for (Int_t j = 1; j <= hist->GetNbinsY(); ++j)
	{
	    //cout<<"Bin "<<i<<","<<j<<" : "<<hist->GetBinContent(i, j) <<endl;

	    if(hist->GetBinContent(i, j) > 1.5e6)
		hist->SetBinContent(i, j, 0);
	}
    }
    
    cout << "Minimum: "<< hist->GetBinContent(hist->GetMinimumBin())<<endl;
    cout << "Maximum: "<< hist->GetBinContent(hist->GetMaximumBin())<<endl;


    cout<<"Bin content from position:" << hist->GetBinContent(hist->FindBin(-40, -65)) <<endl;
    
    TCanvas *c1 = new TCanvas();
    hist->Draw("colz");

    TCanvas *c2 = new TCanvas();
    hist->Draw("lego");
}
