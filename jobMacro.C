void jobMacro(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    RunFmsTreeMaker(fileList, outName);
}
