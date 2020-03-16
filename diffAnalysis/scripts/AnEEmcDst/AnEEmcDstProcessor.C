// Filename: AnEEmcDstProcessor.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Mar 15 03:12:00 2020 (-0400)
// URL: jlab.org/~latif

void AnEEmcDstProcessor()
{   
    TFile *file_dst = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/R15PhyTrigNanoDst_afterburner_garbage/AnRunNanoDstMaker_16085026.root");
    TFile *file_p1 = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/R15PhyTrigNanoDst_afterburner_garbage/EEmc_Part1_AnRunNanoDstMaker_16085026.root");
    TFile *file_p3 = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/R15PhyTrigNanoDst_afterburner_garbage/eemcTreeP3_EEmc_Tree_AnRunNanoDstMaker_16085026.root");

    TTree *tree_dst = (TTree*)file_dst->Get("T");
    TTree *tree_p1 = (TTree*)file_p1->Get("tree");
    TTree *tree_p3 = (TTree*)file_p3->Get("tree");

    if(tree_p1->GetEntries() != tree_p3->GetEntries() || tree_dst->GetEntries() != tree_p1->GetEntries())
    {
	cout<<"Number of entries did not match. Can not process"<<endl;
	return;
    }
    
    tree_p1->SetBranchStatus("*", 0);
    // tree_p1->SetBranchStatus("evtHddr", 1);  //THis does not get the object
    
    tree_p1->SetBranchStatus("mRunNumber", 1);
    tree_p1->SetBranchStatus("mEventNumber", 1);
    
    // tree_p1->SetBranchStatus("spinInfo", 1);
    tree_p1->SetBranchStatus("mSpin4", 1);   
    //tree_p1->SetBranchStatus("vertex", 1);
    

    TFile *new_file = new TFile("merged.root", "recreate");
    TTree *tree = tree_dst->CloneTree();
    TTree *tree_m1 = tree_p1->CloneTree();
    tree_m1->SetName("event");
    
    TTree *tree_m3 = tree_p3->CloneTree();
    //tree_m2->SetName("T");

    tree->AddFriend(tree_m1); //This will allow accessing event tree from T even after writing. Also GetEntry() call on T will also do a call to firend's GetEntry() in the vent loop
    tree->AddFriend(tree_m3); //This will allow accessing pion tree from T even after writing
    
    // tree_dst->Print();
    // tree_dst->GetListOfFriends()->ls();

    // tree_dst->Scan("mRunNumber:pi0.M");
    new_file->Write();
}
