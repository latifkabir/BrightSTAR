void jobMacro(TString fileList, TString outName)
{
	gROOT->Macro("/star/u/kabir/GIT/BrightSTAR/rootlogon.C");
	AnDstQA(gFirstRun, gFirstRun)(fileList, outName);
}
