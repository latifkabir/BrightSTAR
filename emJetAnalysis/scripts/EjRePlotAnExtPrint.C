// Filename: EjRePlotAnExtPrint.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Mar 19 15:43:35 2025 (-0400)
// URL: jlab.org/~latif



void EjRePlotAnExtPrint()
{
    TFile *f1And2Ph = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass5/R15FmsEmJetAnExt_2ph_eq_less.root");
    TFile *f4OrMorePh = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass5/R15Fms_EmJetAN_ext_3Ph_GrTh3Ph.root");

    // if(gSystem->AccessPathName(f1And2Ph) || gSystem->AccessPathName(f4OrMorePh))
    // {
    // 	cout << "Could not localte input file" << endl;
    // 	return;
    // }
    
    //TFile *file = new TFile(fileName);
    
    TGraphErrors* gr1And2Ph = (TGraphErrors*)f1And2Ph->Get("An2Photons"); //1, 2 photon case was saved as 2 photon. Do not be confused about name
    gr1And2Ph->SetName("An1And2Photons");
    TGraphErrors* gr3Ph = (TGraphErrors*)f4OrMorePh->Get("An2Photons");   // 3-photon was saved with name 2-photon
    gr3Ph->SetName("An3Photons");
    TGraphErrors* gr4OrMorePh = (TGraphErrors*)f4OrMorePh->Get("AnNphotons");

    gr1And2Ph->SetMarkerColor(kRed);
    gr3Ph->SetMarkerColor(kBlack);
    gr4OrMorePh->SetMarkerColor(kBlue);

    gr1And2Ph->SetMarkerSize(3);
    gr3Ph->SetMarkerSize(3);
    gr4OrMorePh->SetMarkerSize(3);
    
    gr1And2Ph->SetLineColor(kRed);
    gr3Ph->SetLineColor(kBlack);
    gr4OrMorePh->SetLineColor(kBlue);

    //Copy Graphs for systematic error bars
    TGraphErrors *gr1And2Ph_sys = (TGraphErrors*)gr1And2Ph->Clone("An1And2Photon_sys");
    TGraphErrors *gr3Ph_sys = (TGraphErrors*)gr3Ph->Clone("An3Photons_sys");
    TGraphErrors *gr4OrMorePh_sys = (TGraphErrors*)gr4OrMorePh->Clone("AnNphotons_sys");

    gr1And2Ph_sys->SetFillStyle(3001);
    gr3Ph_sys->SetFillStyle(3001);
    gr4OrMorePh_sys->SetFillStyle(3001);
    
    gr1And2Ph_sys->SetFillColor(kRed);
    gr3Ph_sys->SetFillColor(kBlack);
    gr4OrMorePh_sys->SetFillColor(kBlue);
    
    Double_t x, y;
    cout << "---------- Number of photons inside EM-jet: 1 or 2 -------------" << endl;
    for(Int_t i = 0; i < gr1And2Ph_sys->GetN(); ++i)
    {
	gr1And2Ph_sys->GetPoint(i, x, y);
	gr1And2Ph_sys->SetPointError(i, x*0.03, y*0.05); // 3% sys error for x and 5% for y sys error

	cout << i << "\t" << x << "\t" << y << "\t" << gr1And2Ph->GetErrorX(i) << "\t" << gr1And2Ph_sys->GetErrorX(i) << "\t\t" << gr1And2Ph->GetErrorY(i) << "\t" << gr1And2Ph_sys->GetErrorY(i) << endl;
    }

    cout << "---------- Number of photons inside EM-jet: 3 -------------" << endl;
    for(Int_t i = 0; i < gr3Ph_sys->GetN(); ++i)
    {
	gr3Ph_sys->GetPoint(i, x, y);
	gr3Ph_sys->SetPointError(i, x*0.03, y*0.07);

	cout << i << "\t" << x << "\t" << y << "\t" << gr3Ph->GetErrorX(i) << "\t" << gr3Ph_sys->GetErrorX(i) << "\t\t" << gr3Ph->GetErrorY(i) << "\t" << gr3Ph_sys->GetErrorY(i) << endl;
    }

    cout << "---------- Number of photons inside EM-jet: 4 or more -------------" << endl;
    for(Int_t i = 0; i < gr4OrMorePh_sys->GetN(); ++i)
    {
	gr4OrMorePh_sys->GetPoint(i, x, y);
	gr4OrMorePh_sys->SetPointError(i, x*0.03, y*0.05);

	cout << i << "\t" << x << "\t" << y << "\t" << gr4OrMorePh->GetErrorX(i) << "\t" << gr4OrMorePh_sys->GetErrorX(i) << "\t\t" << gr4OrMorePh->GetErrorY(i) << "\t" << gr4OrMorePh_sys->GetErrorY(i) << endl;
    }
    
    gr1And2Ph_sys->Draw();
    gr1And2Ph->Draw("same");

    // gr1And2Ph->Draw("AP"); // Fow without Sys only
    
    gr3Ph_sys->Draw("same");
    gr3Ph->Draw("same");

    gr4OrMorePh_sys->Draw("same");
    gr4OrMorePh->Draw("same");

    gr1And2Ph_sys->SetDrawOption("a2");
    gr1And2Ph->SetDrawOption("p same");

    gr3Ph_sys->SetDrawOption("2");
    gr3Ph->SetDrawOption("p same");

    gr4OrMorePh_sys->SetDrawOption("2");
    gr4OrMorePh->SetDrawOption("p same");    
}
