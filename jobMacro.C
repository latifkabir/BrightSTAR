void jobMacro(TString fileList, TString outName)
{
	gROOT->Macro("rootlogon.C");
	RunJetFinder(fileList, outName);
}
