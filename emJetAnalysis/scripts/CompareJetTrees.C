// Filename: CompareJetTrees.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Mar  8 19:25:24 2021 (-0500)
// URL: jlab.org/~latif

void CompareJetTrees()
{    
    //gSystem->Load(".sl73_gcc485/lib/libBrJetMaker.so");
    
    TFile *file_l =  new TFile("/star/u/kabir/pwg/znTest/NanoJetTree_EjRunEmJetTreeMaker_16087001_addl_hot_ch_masking_disabled.root");
    TFile *file_z =  new TFile("/star/u/jiachen/jiachen/zwzhu/Gen/Run15_new/run16087001.root");

    TTree *t_z = (TTree*)file_z->Get("tFmsjet");
    TTree *t_l = (TTree*)file_l->Get("T");

    cout<< "Zn entries: "<< t_z->GetEntries() << endl;
    cout<< "Lk entries: "<< t_l->GetEntries() << endl;

    TText *txtL = new TText();
    txtL->SetTextColor(kRed);

    TText *txtZ = new TText();
    txtZ->SetTextColor(kBlack);
    Double_t offset = 5000;
    
    TH1D* h1LkEng* = new TH1D("h1LkEng", "Jet Energy; Jet E [GeV]", 100, 0, 100);
    TH1D* h1ZnEng* = new TH1D("h1ZnEng", "Jet Energy; Jet E [GeV]", 100, 0, 100);
    h1LkEng->SetLineColor(kRed);
    h1ZnEng->SetLineColor(kBlack);
    
    TH1D* h1LkPt* = new TH1D("h1LkPt", "Jet p_{T}; Jet p_{T} [GeV/c]", 100, 0, 50);
    TH1D* h1ZnPt* = new TH1D("h1ZnPt", "Jet p_{T}; Jet p_{T} [GeV/c]", 100, 0, 50);
    h1LkPt->SetLineColor(kRed);
    h1ZnPt->SetLineColor(kBlack);
    
    TH1D* h1LkEta* = new TH1D("h1LkEta", "Jet #Eta; Jet #Eta", 100, 1, 5);
    TH1D* h1ZnEta* = new TH1D("h1ZnEta", "Jet #Eta; Jet #Eta", 100, 1, 5);
    h1LkEta->SetLineColor(kRed);
    h1ZnEta->SetLineColor(kBlack);
    
    TH1D* h1LkPhi* = new TH1D("h1LkPhi", "Jet #phi; Jet #phi [rad]", 100, -3.2, 3.2);
    TH1D* h1ZnPhi* = new TH1D("h1ZnPhi", "Jet #phi; Jet #phi [rad]", 100, -3.2, 3.2);
    h1LkPhi->SetLineColor(kRed);
    h1ZnPhi->SetLineColor(kBlack);
    
    TH1D* h1LkNph* = new TH1D("h1LkNph", "Photon multiplicity inside EM-jet; Photon multiplicity", 20, 0, 20);
    TH1D* h1ZnNph* = new TH1D("h1ZnNph", "Photon multiplicity inside EM-jet; Photon multiplicity", 20, 0, 20);
    h1LkNph->SetLineColor(kRed);
    h1ZnNph->SetLineColor(kBlack);
    
    TH1D* h1LkNjet* = new TH1D("h1LkNjet", "Number of Jets; Number of jets", 5, 0, 5);
    TH1D* h1ZnNjet* = new TH1D("h1ZnNjet", "Number of Jets; Number of jets", 5, 0, 5);
    h1LkNjet->SetLineColor(kRed);
    h1ZnNjet->SetLineColor(kBlack);
    
    TCanvas *c1 = new TCanvas();
    t_l->Draw("mJets.mE>>h1LkEng");
    t_z->Draw("fjE>>h1ZnEng", "", "sames");
    txtL->DrawText(h1LkEng->GetMean(1), h1LkEng->GetMaximum() / 2.0, "Latif");
    txtZ->DrawText(h1LkEng->GetMean(1), h1LkEng->GetMaximum() / 2.0 - offset, "Zhanwen");
    
    TCanvas *c2 = new TCanvas();
    t_l->Draw("mJets.mPt>>h1LkPt");
    t_z->Draw("fjpT>>h1ZnPt", "", "sames");
    txtL->DrawText(h1LkPt->GetMean(1), h1LkPt->GetMaximum() / 2.0, "Latif");
    txtZ->DrawText(h1LkPt->GetMean(1), h1LkPt->GetMaximum() / 2.0 - offset, "Zhanwen");

    TCanvas *c3 = new TCanvas();
    t_l->Draw("mJets.mEta >> h1LkEta");
    t_z->Draw("fjeta >> h1ZnEta", "", "sames");
    txtL->DrawText(h1LkEta->GetMean(1), h1LkEta->GetMaximum() / 2.0, "Latif");
    txtZ->DrawText(h1LkEta->GetMean(1), h1LkEta->GetMaximum() / 2.0 - offset, "Zhanwen");
    
    TCanvas *c4 = new TCanvas();
    t_l->Draw("mJets.mPhi >> h1LkPhi");
    t_z->Draw("fjphi >> h1ZnPhi", "", "sames");
    txtL->DrawText(h1LkPhi->GetMean(1), h1LkPhi->GetMaximum() / 2.0, "Latif");
    txtZ->DrawText(h1LkPhi->GetMean(1), h1LkPhi->GetMaximum() / 2.0 - offset, "Zhanwen");
    
    TCanvas *c5 = new TCanvas();
    t_l->Draw("mJets@.GetEntries() >> h1LkNjet");
    t_z->Draw("fnjet >> h1ZnNjet", "", "sames");
    txtL->DrawText(h1LkNjet->GetMean(1), h1LkNjet->GetMaximum() / 2.0, "Latif");
    txtZ->DrawText(h1LkNjet->GetMean(1), h1LkNjet->GetMaximum() / 2.0 - offset, "Zhanwen");
    
    //------
    /*
    TStJetEvent *jetEvent = 0;
    TStJetCandidate *jet;    
    t_l->SetBranchAddress("jetEvents", &jetEvent);

    TH1D *h1nPhotons = new TH1D("h1nPhotonsTow", "number of photons in EM jets from tower counts; Number of Photons", 20, 0, 20);

    for(Int_t evt = 0; evt < t_l->GetEntries(); ++evt)
    {
	t_l->GetEntry(evt);

	for(Int_t j = 0; j <  jetEvent->GetNumberOfJets(); ++j)
	{
	    jet = jetEvent->GetJet(j);
	    h1nPhotons->Fill(jet->GetNumberOfTowers());
	}
    }
    */
    //------
    //Alternative to the above block of code is just to use:
    //t_l->Draw("mJets.mNphotons");
    
    TCanvas *c6 = new TCanvas();
    //h1nPhotons->Draw();
    t_l->Draw("mJets.mNphotons >> h1LkNph");
    t_z->Draw("fjetntow >> h1ZnNph", "", "sames");
    txtL->DrawText(h1LkNph->GetMean(1), h1LkNph->GetMaximum() / 2.0, "Latif");
    txtZ->DrawText(h1LkNph->GetMean(1), h1LkNph->GetMaximum() / 2.0 - offset, "Zhanwen");    
}
