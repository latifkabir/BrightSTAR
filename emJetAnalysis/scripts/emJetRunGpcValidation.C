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
EjCreateBinnedHistExtended()

Step 3D:
EjCreateBinnedHistIntegratedEEmc()

Step 4A:
EjCalculateAN()

Step 4B:

Step 5A:
EjReplotAn()

Step 5B:


