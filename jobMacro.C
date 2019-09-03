void jobMacro(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    RunEmcTreeMaker(fileList, outName);
}
