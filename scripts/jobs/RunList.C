// Filename: runList.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Nov 18 01:06:09 2019 (-0500)
// URL: jlab.org/~latif

void RunList()
{
    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList();
    //runList->Print("all");
    Int_t totRuns = runList->GetN();

    //Get runs in sequence
    Int_t run = runList->Next();   
}
