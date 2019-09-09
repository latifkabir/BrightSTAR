// Filename: EmcTreeAnalysis.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Aug 29 21:24:36 2019 (-0400)
// URL: jlab.org/~latif

void EmcTreeAnalysis()
{
    const Int_t size = 32;
    Int_t runList[size] = {16072006, 16072007, 16072008, 16072009, 16072010, 16072012, 16072013, 16072014, 16072021, 16072022, 16072023, 16072024, 16072025, 16072026, 16072033, 16072034, 16072035, 16072036, 16072038, 16072039, 16072040, 16072041, 16072042, 16072043, 16072046, 16072047, 16072057, 16072058, 16072059, 16072060, 16072061, 16072062};
        
    TString fPath = "/star/u/kabir/GIT/BrightSTAR/results/root/RunEmcTreeMaker/RunEmcTreeMaker_";
    
    TChain *chain = new TChain("T");
    cout << "Total files to be added:"<< size <<endl;
    for(Int_t i = 0; i < size; ++i)
    {
	TString fileName = fPath + Form("%i", runList[i]) + ".root/T";
	chain->Add(fileName);
    }
    
    cout << "Total events: " << chain->GetEntries() <<endl;
    //chain->Print();
    //chain->MakeClass("EmcTreeReader");

    //chain->Draw("point");
    // chain->Draw("pi0");
    
    //Trigger
    //chain->Draw("trig_MB", "");

    // chain->Draw("0 >> hist0(5, 0, 5)", "trig_MB == 1 ", "");
    // chain->Draw("1 >> hist1(5, 0, 5)", "trig_HT1 == 1", "");
    // chain->Draw("2 >> hist2(5, 0, 5)", "trig_HT2 == 1", "");

    // hist0->SetName("MB");
    // hist1->SetName("HT1");
    // hist2->SetName("HT2");

    // hist2->Draw();
    // hist1->Draw("sames");
    // hist0->Draw("sames");

    //Photon
    //chain->Draw("point_nTracks>>hist(100, 0, 1500)");
    //chain->Draw("point_E>>hist(500, 0, 10)", "point_nTracks == 0");

    //chain->Draw("pi0_nTracks1>>hist(200, 0, 50)");
    
    //Pi0
    //chain->Draw("pi0_M>>hist(200, 0, 1.2)", "pi0_zgg < 0.7 && pi0_nTracks1 == 0 && pi0_nTracks2 == 0");
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_zgg < 0.7 && pi0_nTracks1 == 0 && pi0_nTracks2 == 0 && trig_MB == 1");
    // chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_zgg < 0.7 && pi0_nTracks1 == 0 && pi0_nTracks2 == 0 && trig_HT1 == 1");
    // chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_zgg < 0.7 && pi0_nTracks1 == 0 && pi0_nTracks2 == 0 && trig_HT2 == 1");

    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_zgg < 0.7 && pi0_nTracks1 == 0 && pi0_nTracks2 == 0 && pi0_Px > 1 && pi0_Pt < 2");

    // TFile *file = new TFile("eta_meson.root", "RECREATE");
    // chain->Draw("pi0_M>>hist(120, 0.0, 1.2)", "pi0_nTracks1 == 0 && pi0_nTracks2 == 0 && pi0_E > 7 && pi0_zgg < 0.7", "");
    // hist->SetName("eta");
    // hist->Write();

    //chain->Draw("pi0_nTracks1:pi0_nTracks2", "", "colz");
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "");
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_zgg < 0.7 && pi0_nTracks1 == 0 && pi0_nTracks2 == 0 && pi0_theta < 0.2"); // Best    
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_zgg < 0.7");
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_nTracks1 == 0 && pi0_nTracks2 == 0 && pi0_theta < 0.5");
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_theta < 0.5");
    //chain->Draw("pi0_zgg");    
    //chain->Draw("pi0_zgg", "pi0_theta < 0.5");    
    //chain->Draw("pi0_dgg", "pi0_nTracks1 == 0 && pi0_nTracks2 == 0");
    //chain->Draw("pi0_M>>hist(120, 0.0, 1.2)", "pi0_nTracks1 == 0 && pi0_nTracks2 == 0 && pi0_dgg< 100");

    //chain->Draw("pi0_theta", "pi0_nTracks1 == 0 && pi0_nTracks2 == 0");
    //chain->Draw("pi0_theta>>hist(200, 0, 3.14)", "pi0_nTracks1 == 0 && pi0_nTracks2 == 0");
    //chain->Draw("pi0_M:pi0_theta>>hist(400, 0, 1.0, 400, 0.0, 1.2)", "pi0_nTracks1 == 0 && pi0_nTracks2 == 0", "colz");
    
    // const Int_t pi0 = 1000; 
    // Double_t px;
    // Double_t py;
    // Double_t pz;
    // Double_t E;

    //chain->SetBranchAddress("pi0_px", &px);
    
}
