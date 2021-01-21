void jobMacro(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon.C");
    //CepNanoDstMaker(fileList, outName);
    //JobPrototype(fileList, outName);
    RunEEmcTreeMakerPart1(fileList, outName);
}
