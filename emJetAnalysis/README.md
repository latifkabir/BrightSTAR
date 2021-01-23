EM-jet A_N Analysis
================================

- The relevant directory for EM-jet analysis is: BrightSTAR/emJetAnalysis

- My analysis has three steps. Step-1 uses MuDst to generate NanoJetTree. Step-2 uses NanoJetTree from step-1 to gegerate hitogram in root file. Step-3 uses histograms in root file from Step-2 to calculate A_N and plot them.


- Step-1: Generate DST files (EM-jet Trees)
The corresponding script is: emJetAnalysis/src/EjRunEmJetTreeMaker.cxx
It relies on two makers named StJetMaker2015 and TStNanoJetTreeMaker under StRoot to produce Nano jet trees.


- Step-2: Generate Binned Histograms Required for A_N calculation
The corresponding script is: emJetAnalysis/src/EjCreateBinnedHistExtended.cxx
The name `extended` indicates that I have extended my original analysis to Zhanwen's to reproduce his result.


- Step-3: Calculate A_N and plot them
The corresponding script is: emJetAnalysis/src/EjCalculateANextended.cxx


- Nano Jet Tree file location: /gpfs/mnt/gpfs01/star/pwg/kabir/dst/emJet

- I have a different way of running root ananlysis than most other people at STAR. In case you want to run it, the instruction can be found at the top level (of BrightSTAR) README  file titled "Quick Start".


