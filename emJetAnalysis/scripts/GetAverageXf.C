// Filename: GetAverageXf.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Sep  5 02:34:24 2021 (-0400)
// URL: jlab.org/~latif


void GetAverageXf(TString fileName, TString det = "eemc")
{
    if(gSystem->AccessPathName(fileName))
    {
	cout << "File NOT found" <<endl;
	return;
    }
    TFile *f = new TFile(fileName);

    const Int_t kEnergyBins = 5;
    const Int_t nPtBins = 9;
    Double_t ptBins[] = {2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 6.0, 8.0, 10.0};
    Double_t engBins[] = {0.0, 20.0, 40.0, 60.0, 80.0, 100.0}; //For info only
    
    TH1D *hAvgXf[kEnergyBins][nPtBins];
    for(Int_t i = 0; i < kEnergyBins; ++i)
    {
	for(Int_t j = 0; j < nPtBins; ++j)
	{
	    TString name = Form("hAvgXf%i_%i", i, j);
	    hAvgXf[i][j] = (TH1D*)f->Get(name);	    
	}
    }

    Int_t maxEbin = kEnergyBins;
    Int_t minEbin = 0;

    if(det == "eemc")
    {
	minEbin = 0;
	maxEbin = 1;
    }
    else if(det == "fms")
    {
	minEbin = 1;
	maxEbin = 4;
    }
    
    for(Int_t i = minEbin; i < maxEbin; ++i)
    {
	for(Int_t j = 0; j < nPtBins; ++j)
	{
	    TString name = Form("hAvgXf%i_%i", i, j);
	    hAvgXf[i][j] = (TH1D*)f->Get(name);

	    cout << "Energy: "<< engBins[i] << " - "<< engBins[i+1] << "\t pT: " << (ptBins[j] + ptBins[j+1]) / 2.0 << "\t Avg xF: " << hAvgXf[i][j]->GetMean()<<endl;

	}
	cout << "\n -------------------------------------------------------------------------------- \n" <<endl;
    }

    f->Close();
}
