void jobMacro(TString fileList, TString outName)
{
    gROOT->Macro("rootlogon_prod.C");
    CepNanoDstMaker(fileList, outName);
}
