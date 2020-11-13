// Filename: SubmitMissingRuns.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Jul 28 13:57:10 2020 (-0400)
// URL: jlab.org/~latif

void SubmitMissingRuns()
{
    TStRunList *rList = new TStRunList();
    TEntryList *missingList = rList->GetMissingRunList("/star/u/kabir/GIT/BrightSTAR/dst/emJet/pass1/eemc/R15EmJetNanoTree/NanoJetTree_RunEmJetTreeMaker_");
    TStScheduler::SubmitJob(1, "EjRunEmJetTreeMaker", missingList);    
}
