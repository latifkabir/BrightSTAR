void jobMacro(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    JobPrototype(fileList, outName);
    //RunEEmcNanoDstMaker(fileList, outName);
}
