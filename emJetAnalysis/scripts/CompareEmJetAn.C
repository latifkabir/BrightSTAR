// Filename: CompareEmJetAn.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Jan 26 02:14:51 2021 (-0500)
// URL: jlab.org/~latif

void CompareEmJetAn()
{
    TCanvas *c = new TCanvas();
    //c->Draw();
    //------------- Inclusive pion A_N global fit, taken from Zhanwen's paper code -----------
    TFile *file = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/emJet/compare/theoryCurve.root");
    TGraphErrors *gth1_200 = (TGraphErrors*)file->Get("gth1_200");
    TGraphErrors *gth1_500 = (TGraphErrors*)file->Get("gth1_500");
    TGraph *gth1_200L = (TGraph*)file->Get("gth1_200L");
    TGraph *gth1_500L = (TGraph*)file->Get("gth1_500L");
    
    gth1_200L->SetLineColor(kRed);
    gth1_200->SetFillColorAlpha(kRed-9,0.35);
    gth1_500L->SetLineColor(kBlue);
    gth1_500->SetFillColorAlpha(kBlue-7,0.35);
    gth1_200->Draw();
    
    gth1_200->Draw("3csame");
    //gth1_500->Draw("3csame");
    gth1_200L->Draw("csame");
    //gth1_500L->Draw("csame");
    TLegend *leg4=new TLegend(0.65,0.62,0.88,0.78);
    leg4->SetMargin(0.4);
    leg4->SetBorderSize(0);
    leg4->SetFillColor(0);
    leg4->SetTextSize(0.03);
    leg4->AddEntry(gth1_200,"Theory 200 GeV","f");
    //leg4->AddEntry(gth1_500,"Theory 500 GeV","f");
    leg4->Draw();

    //------- My results generated with matching Zhanwen's cuts (as much as possible) ----------------
    TFile *myAnFile = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/emJet/compare/emJetAn_compre_with_zn.root");
    TGraphErrors *myAn_allPh = (TGraphErrors*)myAnFile->Get("An3OrMorePhotons");
    TGraphErrors *myAn_2Ph = (TGraphErrors*)myAnFile->Get("An2Photons");
    TGraphErrors *znAn_allPh = (TGraphErrors*)myAnFile->Get("znGrAll");

    myAn_allPh->Draw("same");
    myAn_2Ph->Draw("same");
    znAn_allPh->Draw("same");
    
}
