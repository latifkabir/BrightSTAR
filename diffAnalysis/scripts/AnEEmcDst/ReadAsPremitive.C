// Filename: ReadAsPremitive.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Mar 15 05:38:01 2020 (-0400)
// URL: jlab.org/~latif

/*
This is an example that object members should NOT be read using primitive data type biffer.
It does not work this way. You must create object buffer. see the other script.

 */


void ReadAsPremitive()
{
    TFile *f = new TFile("merged.root");

    TTree *T = (TTree*)f->Get("T");
    TTree *event = (TTree*)f->Get("event");

    Int_t evtId;
    Int_t evtNum;
    
    T->SetBranchAddress("mEventId", &evtId);  //This will not work
    event->SetBranchAddress("mEventNumber", &evtNum); //This will not work

    for(int i = 0; i < 10; ++i)
    {
	T->GetEntry(i);
	cout<<"From Dst evtId: "<< evtId <<" From event evtNum: "<<evtNum<<endl;	    
    }
    
}
