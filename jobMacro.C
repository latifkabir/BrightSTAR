void jobMacro(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    TpcTrackQA(fileList, outName);
}
