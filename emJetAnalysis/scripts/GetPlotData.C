// Filename: GetPlotData.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Sep 18 21:37:00 2021 (-0400)
// URL: jlab.org/~latif


void GetPlotData(TString fileName, TString beamColor)
{
    if(gSystem->AccessPathName(fileName))
    {
	cout << "Could not find the input file" <<endl;
	return;
    }
    TFile *file = new TFile(fileName);

    const Int_t kEnergyBins = 5;
    const Int_t kPhotonBins = 6;

    TGraphErrors *bGrPhy[kEnergyBins][kPhotonBins];
    TGraphErrors *yGrPhy[kEnergyBins][kPhotonBins];
    //For systematic error bars: Duplicate all graphs
    TGraphErrors *bGrPhy_sys[kEnergyBins][kPhotonBins];
    TGraphErrors *yGrPhy_sys[kEnergyBins][kPhotonBins];

    TGraphErrors *gr;
    TGraphErrors *grSys;
    Double_t x, y, xe, ye;
    
    for(Int_t i = 0; i < kEnergyBins; ++i)
    {
	cout << "------------------- Energy: " << i*20 << " - " << (i + 1)*20  << "------------------\n\n" << endl;
	
	for(Int_t j = 0; j < kPhotonBins -1; ++j)
	{
	    TString name = Form("bEbin%i_PhotonBin%i", i, j);
	    bGrPhy[i][j] = (TGraphErrors*)file->Get(name);
	    name = Form("yEbin%i_PhotonBin%i", i, j);
	    yGrPhy[i][j] = (TGraphErrors*)file->Get(name);
	    
	    name = Form("Sys_bEbin%i_PhotonBin%i", i, j);
	    bGrPhy_sys[i][j] = (TGraphErrors*)file->Get(name);
	    name = Form("Sys_yEbin%i_PhotonBin%i", i, j);
	    yGrPhy_sys[i][j] = (TGraphErrors*)file->Get(name);

	    bGrPhy[i][j]->SetTitle(""); //Remove title 
	    yGrPhy[i][j]->SetTitle("");
	    bGrPhy_sys[i][j]->SetTitle("");
	    yGrPhy_sys[i][j]->SetTitle("");

	    if(beamColor == "blue")
	    {
		gr = bGrPhy[i][j];
		grSys = bGrPhy_sys[i][j];
	    }
	    else if(beamColor == "yellow")
	    {
		gr = yGrPhy[i][j];
		grSys = yGrPhy_sys[i][j];
	    }
	    else
	    {
		cout << "Invalid beam" <<endl;
		return;
	    }
	    cout << "\nPhoton Multiplicity:"<< (j + 1) <<endl;
    
	    for(Int_t k = 0; k < gr->GetN(); ++k)
	    {
		gr->GetPoint(k, x, y);
		cout << k <<"\t"<< x <<"\t"<< y <<"\t"<< gr->GetErrorX(k) <<"\t"<< gr->GetErrorY(k) <<"\t"<< grSys->GetErrorX(k) <<"\t"<< grSys->GetErrorY(k) << endl;
	    }
	}
    }    
}
