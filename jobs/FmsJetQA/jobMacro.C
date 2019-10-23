void jobMacro(TString fileList, TString outName)
{
	gROOT->Macro("/star/u/kabir/GIT/BrightSTAR/rootlogon.C");
	RunJetFinder(fileList, outName);
}
