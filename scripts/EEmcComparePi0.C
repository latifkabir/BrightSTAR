// Filename: EEmcComparePi0.C
// Description: Compare Pi0 recosntruction by IU vs TSIU.
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Aug 14 13:11:17 2019 (-0400)
// URL: jlab.org/~latif

void EEmcComparePi0()
{
    TString tsiuFile = "/star/u/kabir/pwg/results/root/3.Pi0_In_Cal/EEmc_Pi0_TSIU/eemcTreeP3_16072047_57-62_merged.root";
    TString iuFile = "/star/u/kabir/pwg/results/root/3.Pi0_In_Cal/EEmc_Pi0_IU/EEmc_Pi0_IU.root";

    TFile *f_ts = new TFile(tsiuFile);
    TFile *f_iu = new TFile(iuFile);
    TTree *t = (TTree*) f_ts->Get("tree");

    //TSIU photon energy
    //t->Draw("photon.E>>hist(200, 0, 50)", "");

    //TSIU photon position
    //t->Draw("photon.position.fY : photon.position.fX >>hist(500, -250, 250, 500, -250, 250)", "","colz");

    //TSIU pi0 energy
    t->Draw("pi0.E>>hist(200, 0, 100)", "");
    
    //TSIU Pi0 position
    //t->Draw("pi0.position.fY:pi0.position.fX>>hist(500, -250, 250, 500, -250, 250)", "pi0.M > 0 && pi0.M < 1.2", "colz");

    //TSIU Dgg vs Mpi
    // t->Draw("pi0.M : pi0.D >> hist(100, 0, 50, 100, 0, 0.5)", "pi0.M > 0 && pi0.M < 0.5 && pi0.D > 0 && pi0.D < 50", "colz");

    //TSIU Zgg vs Mpi
    //t->Draw("pi0.M:pi0.Z >> hist(100, 0, 1.0, 100, 0, 1.2)", "pi0.M > 0 && pi0.M < 1.2", "colz");

    //TSIU Dgg vs E_0
    //t->Draw("pi0.D : pi0.E >> hist(100, 0, 50, 100, 0, 50)", "pi0.M > 0 && pi0.M < 1.2", "colz");

}


