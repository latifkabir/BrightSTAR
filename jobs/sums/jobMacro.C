void jobMacro(TString fileList, TString outName)
{
    gROOT->Macro("/star/u/kabir/GIT/BrightSTAR/rootlogon.C");
    resources/temp/temp.list(fileList, outName);
}
