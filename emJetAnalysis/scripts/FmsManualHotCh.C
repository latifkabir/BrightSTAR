// Filename: FmsHotManualHotCh.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Feb  8 12:56:02 2021 (-0500)
// URL: jlab.org/~latif

void FmsManualHotCh()
{

    ifstream inFile("/star/u/kabir/pwg/zhanwen_ana_code_new/psn0751/Code2/Gen/Run15_new/gainCorr/FmsGainCorr9.txt");

    Int_t dummy;
    Int_t det;
    Int_t ch;
    Double_t gainCorr;
    Int_t count = 0;
    while(!inFile.eof())
    {
	inFile >> dummy >> det >> ch >> gainCorr;
	if(dummy < 2)
	    continue;

	if(gainCorr != 0)
	    continue;

	cout<< "isHotCh["<< (det - 1) <<"]["<<(ch -1)<<"] = 1; ";
	++count;
    }
    cout << "\nTotal masked: "<< count <<endl;

}
