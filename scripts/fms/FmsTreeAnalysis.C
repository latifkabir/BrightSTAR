// Filename: FmsTreeAnalysis.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Sep  2 14:37:35 2019 (-0400)
// URL: jlab.org/~latif

void FmsTreeAnalysis()
{

    TFile *file = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/Pi0_In_All_ECal/Fms_Pi0/FmsTree_16080002.root");
    TTree *t = (TTree*)file->Get("T");
    //t->Print();

    t->Draw("mass>>hist(200, 0, 1.2)", "zgg < 0.7 && pi0E > 20 && pi0Pt >5");
    //t->Draw("pi0Y:pi0X>>hist(200, -100, 100, 100, -100, 100)", "zgg < 0.7 && pi0E > 20", "colz");
    
}
