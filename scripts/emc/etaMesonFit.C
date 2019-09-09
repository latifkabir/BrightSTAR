// Filename: roofit_argus_bkg.C
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Tue Oct  9 00:14:29 2018 (-0400)
// URL: jlab.org/~latif

#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"

using namespace RooFit ; 
void etaMesonFit()
{
    // --- Get Data ----------
    TString fileName = "/star/u/kabir/GIT/BrightSTAR/results/root/3.Pi0_In_Cal/Emc_Pi0/eta_meson.root";
    TFile *file = new TFile(fileName);
    TH1D *eta = (TH1D*)file->Get("eta");

    // --- Observable ---
    RooRealVar mes("mes","M_{#pi^{0}} (GeV)",0.0,1.2) ;
    //Data to be fitted
    RooDataHist *data = new  RooDataHist("data", "Two photon invariant mass", mes, eta);
    
    // --- Parameters ---
    RooRealVar sigmean("sigmean","#pi^{0} mass", 0.135, 0.0, 1.2) ;
    RooRealVar sigwidth("sigwidth","#pi^{0} width", 0.04, 0.0, 0.5) ;

    // --- Build Gaussian or BW PDF ---
    // RooGaussian signal("signal","signal PDF", mes, sigmean, sigwidth) ;
    RooBreitWigner signal("signal","signal PDF", mes, sigmean, sigwidth);
    //RooVoigtian signal("signal","signal PDF",mes, sigmean, sigwidth, sigwidth) ;
    
    // --- Build background PDF part1 ---
    // The names must be diffent in constructor
    RooRealVar par0("param1", "param0", 0.0, -100000., 100000.);
    RooRealVar par1("param2", "param1", 1.0, -100000., 100000.);
    RooRealVar par2("param3", "param2", 1.0, -100000., 100000.);
    RooRealVar par3("param4", "param3", 1.0, -100000., 100000.);
    RooRealVar par4("param4", "param4", 1.0, -100000., 100000.);
    RooArgList parmList;
    parmList.add(par0);
    parmList.add(par1);
    parmList.add(par2);
    parmList.add(par3);
    parmList.add(par4);

    RooPolynomial background1("background1", "Polynomial PDF", mes, parmList); //Note: In RooFit const term is set automatically from normalization for polynomial.

    //----------- Eta Resonance as background 2----------------
    RooRealVar sigmean_e("sigmean_e","#pi^{0} mass", 0.547862, 0.0, 1.2) ;
    RooRealVar sigwidth_e("sigwidth_e","#pi^{0} width", 0.04, 0.0, 0.5);

    // --- Build Gaussian PDF ---
    RooGaussian background2("signal_e","signal PDF", mes, sigmean_e, sigwidth_e);
    
    //RooExponential background2("background2", "expo PDF", mes, par0);

    // --- Construct total background PDF ---
    RooRealVar b1("b1", "b1 coeffi", 1.0, -1000000.0, 1000000.0);
    RooRealVar b2("b2", "b2 coeffi", 1.0, -1000000.0, 1000000.0);
    
    //Convoluted (combination of two background functions) background
    // background = b1 x poly bkg1 + b2 x expo bkg2
    RooAddPdf background("tot_bkg","expo_bkg+poly_bkg", RooArgList(background1, background2), RooArgList(b1, b2));

    // ------ Construct signal + background PDF -----
    //nsig or nbnk initial value and range must cover (be similar for initial value) total number of entries unter the distribution. Otherwise the fit fill NOT work.
    //You might need to keep adjusting these values to make sure fit works for all cases.
    RooRealVar nsig("nsig", "signal coeffi", 100000.0, 0.0, 1000000.0);
    RooRealVar nbkg("nbkg", "background coeffi", 100000.0, 0.0, 1000000.0);
    // model signal = nsig x signal + bkg x background.  nsig gives background subtracted integral or taol number of particles.
    RooAddPdf model("model","sig+bkg", RooArgList(signal, background), RooArgList(nsig, nbkg));

    // --- Perform extended ML fit of composite PDF to toy data ---
    //RooFitResult *rf = model.fitTo(*data, Save(), Extended()); // Minimization by Log Likelyhood approach
    RooFitResult *rf = model.chi2FitTo(*data, Save(), Extended()); // Minimization by Chi square approach. The option Save() will enable returning fit function
    //model.fitTo(*data, "et") ;

    Double_t integral = nsig.getVal();
    Double_t error = nsig.getError();

    cout << "\n---------------------------------------------------------------------------------------------" <<endl;
    cout << "Integrated value (no. of entries) under BW: "<< integral << " +/- " << error <<endl;
    cout << "-----------------------------------------------------------------------------------------------" <<endl;
    
    // --- Plot toy data and composite PDF overlaid ---
    RooPlot* mesframe = mes.frame();
    data->plotOn(mesframe, Name("data"));
    model.plotOn(mesframe, Name("model"), LineColor(kBlue));
    model.plotOn(mesframe,Components(background1),LineStyle(kDashed)) ;
    model.plotOn(mesframe, Components(signal), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(mesframe, Components(background2), LineStyle(kDashed), LineColor(kRed));

    Double_t chiSq = mesframe->chiSquare("model", "data", 12); // 12 is number of fit parameters
    cout << "The chi squre: "<<chiSq<<endl;

    TPaveLabel *pt = new TPaveLabel(0.7, 0.6, 0.9,0.65, Form("#chi^{2} = %f", chiSq), "brNDC");
    mesframe->addObject(pt);    

    TPaveLabel *pi0 = new TPaveLabel(0.7, 0.3, 0.9,0.35, Form("M_{#pi^{0}} = %0.4f +/- %0.4f", sigmean.getVal(), sigmean.getError()), "brNDC");
    mesframe->addObject(pi0);  
    TPaveLabel *eta_meson = new TPaveLabel(0.5, 0.5, 0.9, 0.35, Form("M_{#eta} = %0.4f +/- %0.4f", sigmean_e.getVal(), sigmean_e.getError()), "brNDC");
    mesframe->addObject(eta_meson);  
    
    
    TCanvas* c = new TCanvas();
    mesframe->Draw();           
}
