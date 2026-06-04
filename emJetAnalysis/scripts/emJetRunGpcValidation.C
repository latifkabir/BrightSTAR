// Filename: emJetRunGpcValidation.C
// Description: 
// Author: Latif Kabir < latiful.kabir@ucr.edu >
// Created: Thu May 14 00:11:49 2026 (-0400)
// URL: latifkabir.github.io


Step 1A:
EjRunEmJetTreeMaker("database/exFiles.list")

EjRunEmJetTreeMaker("root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P15ik/2015/068/16068023/st_fms_16068023_raw_3000001.MuDst.root")

Step 1B:

Step 2A:
EjAnalysisTreeQa("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass5/fms/nanoDst/NanoJetTree_EjRunEmJetTreeMaker_16066033.root", "EjAnaTreeQaFms.root", "fms")

Step 2B:
EjAnalysisTreeQa("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass4/eemcTow/NanoJetTree_EjRunEmJetTreeMaker_16064077.root", "EjAnaTreeQaEEmc.root", "eemc")

Step 3A:
EjCreateBinnedHistMerged(gFirstRun, "/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass5/fms/nanoDst/NanoJetTree_EjRunEmJetTreeMaker_", "fms", gFirstRun, gFirstRun)

Step 3B:
EjCreateBinnedHistMerged(gFirstRun, "/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass4/eemcTow/NanoJetTree_EjRunEmJetTreeMaker_", "eemc", gFirstRun, gFirstRun)

Step 3C:
EjCreateBinnedHistExtended(gFirstRun, "/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass5/fms/nanoDst/NanoJetTree_EjRunEmJetTreeMaker_", "fms", gFirstRun, gFirstRun)

Step 3D:
EjCreateBinnedHistIntegratedEEmc(gFirstRun, "/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass4/eemcTow/NanoJetTree_EjRunEmJetTreeMaker_", "eemc", gFirstRun, gFirstRun)

Step 4A:
EjCalculateAN("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass5/BinnedHist_Fms_Merged.root",  "R15FmsEmJetAN_Merged.root", "fms")


Step 4B:
EjCalculateAN("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass4/EEmcJetPass4_BinnedHist.root", "R15EEmcEmJetAN_Merged.root", "eemc")

Step 4C:
EjCalculateANextended("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass5/BinnedHist_ext_fms_2_eq_less.root",  "R15FmsEmJetAnExt_2ph_eq_less.root", "fms")

Step 4D:
EjCalculateANintegratedEEmc("/gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet/run15/pass4/EjCreateBinnedHistIntegratedEEmc.root", "eemcANintegrated.root", "eemc")


Step 5A:
EjReplotAn("R15FmsEmJetAN_Merged.root", "fms", 1)

Step 5B:
EjReplotAn("R15EEmcEmJetAN_Merged.root", "eemc", 1)

Step 5C:
EjRePlotAnExt("R15FmsEmJetAnExt_2ph_eq_less.root")

Step 5D:
EjRePlotAnExt("eemcANintegrated.root")
