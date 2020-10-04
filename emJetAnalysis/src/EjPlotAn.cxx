// Filename: EjPlotAn.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat Oct  3 01:20:38 2020 (-0400)
// URL: jlab.org/~latif

#include "RootInclude.h"
#include "Hists.h"

void EjPlotAn()
{
    gStyle->SetOptStat(0);
    TGraphErrors* graphs[4][3];

    for (int i = 0; i < 4; i++)
    {
	for (int j = 0; j < 3; j++)
	{
	    graphs[i][j] = new TGraphErrors();
	    //graphs[i][j]->SetPoint(1, );
	}
    }

    
    //...  prepare the graphs
    TCanvas* c3 = new TCanvas("casym","Asymmetries",1000,600);
    float varMins[4] = { 0.00, 0.1, -0.3, 0.3};
    float varMaxs[4] = { 0.35, 0.9, 0.7, 2.3};
    const char* xTitles[4] = { "x_{N}","x_{#pi}","x_{F}","q_{T} [GeV]" };
    const char* yTitles[3] = {
	"A_{T}^{sin(2#phi_{CS} #minus #phi_{S})}",
	"A_{T}^{sin(2#phi_{CS} + #phi_{S})}",
	"A_{T}^{sin(#phi_{S})}" };

    PanelPlot* asymPlot = new PanelPlot(c3,4,3,1,"asym",xTitles,yTitles);

    for (int i = 0; i < 4; i++)
    {
	for (int j = 0; j < 3; j++)
	{
	    asymPlot->GetPlot(i,j)->SetXRange( varMins[i], varMaxs[i]);
	    asymPlot->GetPlot(i,j)->SetYRange( -1, 1);
	    asymPlot->GetPlot(i,j)->Add( graphs[i][j], Plot::Point | Plot::Erry);
	}
    }
    asymPlot->Draw();     
}
