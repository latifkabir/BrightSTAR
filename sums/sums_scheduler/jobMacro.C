void jobMacro(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    //CepNanoDstMaker(fileList, outName);
    JobPrototype(fileList, outName);
}
