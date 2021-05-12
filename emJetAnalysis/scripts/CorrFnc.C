// Filename: CorrFnc.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon May 10 10:36:58 2021 (-0400)
// URL: jlab.org/~latif


Double_t CorrFncEng()
{
    Double_t p0 = -8.482;
    Double_t p1 = 4.069;
    Double_t p2 = -0.3063;
    Double_t p3 = 0.01401;
    Double_t p4 = -0.0003243;
    Double_t p5 = 3.729*pow(10, -6);
    Double_t p6 = -1.697*pow(10, -8);
 

    TF1 *fnc = new TF1("fnc", "pol6", 0, 100);
    fnc->SetParameter(0, p0);
    fnc->SetParameter(1, p1);
    fnc->SetParameter(2, p2);
    fnc->SetParameter(3, p3);
    fnc->SetParameter(4, p4);
    fnc->SetParameter(5, p5);
    fnc->SetParameter(6, p6);

    fnc->Draw();
}

Double_t CorrFncPt()
{
    Double_t p0 = 1.486;
    Double_t p1 = 0.07337;
    Double_t p2 = 0.215;
    Double_t p3 = -0.01525;

    TF1 *fnc = new TF1("fnc", "pol3", 0, 20);
    fnc->SetParameter(0, p0);
    fnc->SetParameter(1, p1);
    fnc->SetParameter(2, p2);
    fnc->SetParameter(3, p3);

    fnc->Draw();
    
}


void CorrFnc()
{
    //cout << CorrFncPt() <<endl;
    cout << CorrFncEng() <<endl;
 
}
