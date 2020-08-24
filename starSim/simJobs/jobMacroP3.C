void jobMacroP3(Int_t cycle, Int_t nEntries)
{
    cycle += 500;
    gROOT->Macro("/star/u/kabir/GIT/fmsJetSim/rootlogon.C");
    TString inMuDstFile = Form("FmsSim_Run15_%i_evt%i.MuDst.root", cycle, nEntries);
    TString outJetFile = Form("FmsJet_Run15_%i_evt%i.root", cycle, nEntries);
    if(gSystem->AccessPathName(inMuDstFile))
    {
	cout << "No MuDSt Created" <<endl;
	return;
    }
    RunFmsJetFinderPro(inMuDstFile, outJetFile);
    gROOT->ProcessLine(".! rm *.geant.root");
}
