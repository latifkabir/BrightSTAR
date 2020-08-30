void jobMacroP2(Int_t cycle, Int_t nEvents)
{
    gROOT->Macro("/star/u/kabir/GIT/BrightSTAR/rootlogon.C");
    FmsSimRunBfc(cycle, nEvents);
}
