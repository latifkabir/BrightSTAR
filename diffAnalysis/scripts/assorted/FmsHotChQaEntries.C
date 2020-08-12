// Filename: FmsHotChQaEntries.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Aug  9 23:17:44 2020 (-0400)
// URL: jlab.org/~latif

void FmsHotChQaEntries()
{
    TStFillNoDB *fillDb = new TStFillNoDB();
    vector <int> fills = fillDb->GetAllFillNos();

    cout << "Total fills: "<< fills.size() <<endl;
    TString fileName;
    TH1D* hist;
    for(int i = 0; i < fills.size(); ++i)
    {
	fileName = Form("/star/u/kabir/GIT/BrightSTAR/dst/R15FmsHotChQa/R15FmsHotChQa_Fill_%i.root", fills[i]);
	if(gSystem->AccessPathName(fileName))
	{
	    cout << "Input file not found" <<endl;
	    continue;
	}
	TFile *file = new TFile(fileName);
	hist = (TH1D*) file->Get("engDist_10_180");
	cout << "Fill No: "<< fills[i] << " Entries: "<< hist->GetEntries() <<endl;
    }   
}
