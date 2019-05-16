// Filename: ExAnalysis2.C
// Description: 
// Author: Latif Kabir < latif@jlab.org >
// Created: Thu May 16 15:07:14 2019 (-0400)
// URL: jlab.org/~latif

void ExAnalysis2()
{
    gROOT->ProcessLine(".L ExAna2Class.cpp"); // In practice, you will be compiling the classes using Makefile and load it. here we used just to demonstrate.
    ExAna2Class t;
    t.GetEntry(12); // Fill t data members with entry number 12
    t.Show();       // Show values of entry 12
    t.Show(16);     // Read and show values of entry 16
    //t.Loop();     // Loop on all entries

    cout<<"Total number of primary tracks:" << t.PrimaryTracks_ <<endl;
    for (Int_t i = 0; i < t.PrimaryTracks_; ++i)
    {
	cout<<"Primary track number:"<< (i+1) <<endl;
	cout<<"Track pt:"<< t.PrimaryTracks_mPt[i]<<endl;	
    }
}
