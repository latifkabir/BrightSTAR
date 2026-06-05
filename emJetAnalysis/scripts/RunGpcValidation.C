// Filename: RunGpcValidation.C
// Description: 
// Author: Latif Kabir < latiful.kabir@ucr.edu >
// Created: Thu May 14 00:11:49 2026 (-0400)
// URL: latifkabir.github.io

void RunGpcValidation(TString _step)
{
    
    cout << "Running analysis step:" << _step << endl;
    if (_step == "1a")
    {
	//Step 1a:
	//EjRunEmJetTreeMaker("database/exFiles.list");
	EjRunEmJetTreeMaker("root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P15ik/2015/068/16068023/st_fms_16068023_raw_3000001.MuDst.root", "jets.root", "fms");
    }
    else if (_step == "1b")
    {
	//Step 1b:
	EjRunEmJetTreeMaker("root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/068/16068023/st_physics_16068023_raw_0000004.MuDst.root", "jets.root", "eemc");	
    }
    else if (_step == "2a")
    {
	//Step 2a:
	EjAnalysisTreeQa("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass5/fms/nanoDst/NanoJetTree_EjRunEmJetTreeMaker_16066033.root", "EjAnaTreeQaFms.root", "fms");
    }
    else if (_step == "2b")
    {    
	//Step 2b:
	EjAnalysisTreeQa("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass4/eemcTow/NanoJetTree_EjRunEmJetTreeMaker_16064077.root", "EjAnaTreeQaEEmc.root", "eemc");
    }
    else if (_step == "3a")
    {    
	//Step 3a:
	EjCreateBinnedHistMerged(gFirstRun, "/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass5/fms/nanoDst/NanoJetTree_EjRunEmJetTreeMaker_", "fms", gFirstRun, gFirstRun);
    }
    else if (_step == "3b")
    {
	//Step 3b:
	EjCreateBinnedHistMerged(gFirstRun, "/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass4/eemcTow/NanoJetTree_EjRunEmJetTreeMaker_", "eemc", gFirstRun, gFirstRun);
    }
    else if (_step == "3c")
    {    
	//Step 3c:
	EjCreateBinnedHistExtended(gFirstRun, "/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass5/fms/nanoDst/NanoJetTree_EjRunEmJetTreeMaker_", "fms", gFirstRun, gFirstRun);
    }
    else if (_step == "3d")
    {    
	//Step 3d:
	EjCreateBinnedHistIntegratedEEmc(gFirstRun, "/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass4/eemcTow/NanoJetTree_EjRunEmJetTreeMaker_", "eemc", gFirstRun, gFirstRun);
    }
    else if (_step == "4a")
    {    
	//Step 4a:
	EjCalculateAN("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass5/BinnedHist_Fms_Merged.root",  "R15FmsEmJetAN_Merged.root", "fms");
    }
    else if (_step == "4b")
    {    
	//Step 4b:
	EjCalculateAN("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass4/EEmcJetPass4_BinnedHist.root", "R15EEmcEmJetAN_Merged.root", "eemc");
    }
    else if (_step == "4c")
    {    
	//Step 4c:
	EjCalculateANextended("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass5/BinnedHist_ext_fms_2_eq_less.root",  "R15FmsEmJetAnExt_2ph_eq_less.root", "fms");
    }
    else if (_step == "4d")
    {    
	//Step 4d:
	EjCalculateANintegratedEEmc("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass4/EjCreateBinnedHistIntegratedEEmc.root", "eemcANintegrated.root", "eemc");
    }
    else if (_step == "5a")
    {    
	//Step 5a:
	EjReplotAn("R15FmsEmJetAN_Merged.root", "fms", 1);
    }
    else if (_step == "5b")
    {    
	//Step 5b:
	EjReplotAn("R15EEmcEmJetAN_Merged.root", "eemc", 1);
    }
    else if (_step == "5c")
    {    
	//Step 5c:
	EjRePlotAnExt("R15FmsEmJetAnExt_2ph_eq_less.root");
    }
    else if (_step == "5d")
    {    
	//Step 5d:
	EjRePlotAnExt("eemcANintegrated.root");
    }
    else
    {
	cout << "Invalid step" << endl;
    }
}
