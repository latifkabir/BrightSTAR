// Filename: EmcPi0.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Thu Aug 29 21:24:36 2019 (-0400)
// URL: jlab.org/~latif

void EmcPi0()
{
    TString fPath = "/star/u/kabir/GIT/BrightSTAR/results/root/3.Pi0_In_Cal/Emc_Pi0/";

    TChain *chain = new TChain("T");
    chain->Add(fPath + "EmcTree_16072047.root/T");
    chain->Add(fPath + "EmcTree_16072057.root/T");
    chain->Add(fPath + "EmcTree_16072058.root/T");
    //chain->Print();

    //chain->Draw("point_nTracks");
    //chain->Draw("pi0_nTracks1:pi0_nTracks2", "", "colz");
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "");
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_zgg < 0.7 && pi0_nTracks1 == 0 && pi0_nTracks2 == 0 && pi0_theta < 0.2"); // Best
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_zgg < 0.7 && pi0_nTracks1 == 0 && pi0_nTracks2 == 0");                    // 2nd best
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_zgg < 0.7");
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_nTracks1 == 0 && pi0_nTracks2 == 0 && pi0_theta < 0.5");
    //chain->Draw("pi0_M>>hist(120, 0, 1.2)", "pi0_theta < 0.5");
    //chain->Draw("pi0_zgg");    
    chain->Draw("pi0_zgg", "pi0_theta < 0.5");    
    //chain->Draw("pi0_zgg", "pi0_nTracks1 == 0 && pi0_nTracks2 == 0");    
}
