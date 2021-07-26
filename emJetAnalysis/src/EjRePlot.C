// Filename: EjRePlot.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Jul 24 13:31:34 2021 (-0400)
// URL: jlab.org/~latif

void TGraphMultiErrorsEx2(TGraphErrors *gr1, TGraphErrors *gr2);

void EjRePlot(TString fileName)
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
    
    for(Int_t i = 0; i < kEnergyBins; ++i)
    {
	for(Int_t j = 0; j < kPhotonBins; ++j)
	{
	    TString name = Form("bEbin%i_PhotonBin%i", i, j);
	    bGrPhy[i][j] = (TGraphErrors*)file->Get(name);
	    name = Form("yEbin%i_PhotonBin%i", i, j);
	    yGrPhy[i][j] = (TGraphErrors*)file->Get(name);

	    
	    name = Form("Sys_bEbin%i_PhotonBin%i", i, j);
	    bGrPhy_sys[i][j] = (TGraphErrors*)file->Get(name);
	    name = Form("Sys_yEbin%i_PhotonBin%i", i, j);
	    yGrPhy_sys[i][j] = (TGraphErrors*)file->Get(name);
	}
    }
    file->Close();

    Int_t canvasCount = 1;
    TCanvas *c1 = new TCanvas("EMjet_A_N_fms", "EM Jet A_{N}");
    c1->Divide(kEnergyBins -1, kPhotonBins -1);
    for(Int_t i = 0; i < kPhotonBins - 1; ++i)
    {
	for(Int_t j = 1; j < kEnergyBins; ++j)
	{
	    bGrPhy_sys[j][i]->SetFillColor(2);
	    bGrPhy_sys[j][i]->SetFillStyle(3001);
	    yGrPhy_sys[j][i]->SetFillColor(2);
	    yGrPhy_sys[j][i]->SetFillStyle(3001);

	    bGrPhy[j][i]->SetMaximum(0.05);
	    bGrPhy[j][i]->SetMinimum(-0.05);
	    bGrPhy_sys[j][i]->SetMaximum(0.05);
	    bGrPhy_sys[j][i]->SetMinimum(-0.05);

	    yGrPhy[j][i]->SetMaximum(0.05);
	    yGrPhy[j][i]->SetMinimum(-0.05);
	    yGrPhy_sys[j][i]->SetMaximum(0.05);
	    yGrPhy_sys[j][i]->SetMinimum(-0.05);

	    
	    c1->cd(canvasCount);

	    bGrPhy_sys[j][i]->Draw();
	    bGrPhy[j][i]->Draw("same");
	    	    
	    bGrPhy_sys[j][i]->SetDrawOption("a2");	    
	    bGrPhy[j][i]->SetDrawOption("p same");

	    yGrPhy_sys[j][i]->Draw("same 2");
	    yGrPhy[j][i]->Draw("same p");
	    	    
	    TLine* L1Temp = new TLine(1.5, 0, 9.5, 0);
	    L1Temp->Draw("same");
	    ++canvasCount;
	}
    }

}

