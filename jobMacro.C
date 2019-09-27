void jobMacro(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    RunPidTreeMaker(fileList, outName);
}
