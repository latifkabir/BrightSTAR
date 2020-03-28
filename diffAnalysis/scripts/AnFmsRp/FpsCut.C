// Filename: FpsCut.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Mar 25 14:24:45 2020 (-0400)
// URL: jlab.org/~latif


void FpsCut()
{

    TChain *ch = new TChain("T");
    ch->Add("/star/u/kabir/GIT/BrightSTAR/dst/R15FmsTrigNanoDst/AnRunNanoDstMaker_160930*");
    
   //For pid def see: https://www.star.bnl.gov/webdata/dox/html/StFmsPoint_8h_source.html
    T->DrawNormalized("fmsPointPair.mM>>hist(200, 0, 1.0)", "fmsPointPair.mE > 0 && fmsPointPair.mZgg < 0.8 && Iteration$==0");
    T->DrawNormalized("fmsPointPair.mM>>hist(200, 0, 1.0)", "fmsPointPair.mE > 0 && fmsPointPair.mZgg < 0.8 && Iteration$==0 && (fmsPointPair.mFpsPid1 > 9 && fmsPointPair.mFpsPid1 < 17) && (fmsPointPair.mFpsPid2 > 9 && fmsPointPair.mFpsPid2 < 17)", "same");


    
    //T->Draw("sqrt(fmsPointPair.mY1*fmsPointPair.mY1 + fmsPointPair.mX1*fmsPointPair.mX1)");

    
}
