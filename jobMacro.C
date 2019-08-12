void jobMacro(TString fileList, TString outName)
{
	gROOT->Macro("rootlogon.C");
	RunEEmcTreeMakerPart2and3(fileList, outName);
}
