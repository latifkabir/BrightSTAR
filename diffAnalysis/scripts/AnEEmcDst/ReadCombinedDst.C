// Filename: ReadCombinedDst.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Mar 15 05:03:02 2020 (-0400)
// URL: jlab.org/~latif


#include"TFile.h"
#include"TTree.h"
#include <iostream>

using namespace std;

/*
Must load all necessary library / dictionaries

 */

void ReadCombinedDst()
{
    TFile *f = new TFile("merged.root");

    TTree *T = (TTree*)f->Get("T");
    TTree *event = (TTree*)f->Get("event");

    TStEventData *evtId = new TStEventData();  //Accessing individual leaf (with primitive data type buffer) without object buffer would not work
    StEvtHddr *evtHddr = new StEvtHddr();
    
    //Int_t evtId;
    Int_t evtNum;
    T->SetBranchAddress("event", &evtId);
    event->SetBranchAddress("evtHddr", &evtHddr);

    for(int i = 0; i < 10; ++i)
    {
	T->GetEntry(i);
	// event->GetEntry(i);  //This call in not necessary, since event is a friend of T. Calling does not hurt as well.
	cout<<"From Dst evtId: "<< evtId->mEventId <<" From event evtNum: "<<evtHddr->GetEventNumber()<<endl;	    
    }   
}



