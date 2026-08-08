!#/bin/bash

singularity exec -e -B /direct -B /star -B /afs -B /gpfs -B /sdcc/lustre02 /cvmfs/star.sdcc.bnl.gov/containers/rhic_sl7.sif /bin/csh -c "starver SL20a && /bin/bash -c 'source /star/u/kabir/GIT/EmJet-GPC-BrightSTAR/setup.sh && echo "'EjAnalysisTreeQa\(\"/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass4/eemcTow/NanoJetTree_EjRunEmJetTreeMaker_16066033.root\",\"EjAnalysisTreeQaR15Pass4EEmc_16066033.root\",\"eemc\"\)'" | root4star -l -b /star/u/kabir/GIT/EmJet-GPC-BrightSTAR/rootlogon.C'"

