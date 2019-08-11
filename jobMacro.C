void jobMacro(TString fileList, TString outName)
{
	gROOT->Macro("rootlogon.C");
	FmsQA(fileList, outName);
}
