void jobMacroP1(Int_t cycle, Int_t nEvents)
{
    gROOT->Macro("/star/u/kabir/GIT/BrightSTAR/rootlogon.C");
    FmsSimRunStarsimAndBfc(cycle, nEvents, 1);
}
