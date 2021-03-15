
FMS EM Jet Simulation
===================================


The Workflow:
---------------

- **Step-1:** Generate events using your desired event generators including either pythia 6 or pythia 8. STAR framework has an array of generators to choose from. The output is a `.fzd` file.

- **Step-2:** Use the generated events to get detector hits using the `bfc.C` script. The output is a MuDst root file.

- **Step-3:** Run reconstruction (say Jet reconstruction)on the generated MuDst file. The output is a root file. You will likely apply the trigger before this step.

- **Step-4:** Analyze the reconstructed dataset for QA distributions.

- **Step-5:** Add trigger filter to simulate triggering

- **Step-6:** Produce embedded samples by mixing min biased real data with simulated data. Analyze embeded data for data and simulation comparison.

- **Step-7:** Perform unfolding using Singular Value Decomposition (SVD). Can use RooUnfolding to facilitate the task.

- **Step-8:** Analyzed embeded sample (that is a good representative of real data) to calculate various systematic effects.




How to Run Examples
--------------

1. Generate Pythia Events:
```
starsim -w 0 -b pythia.kumac NEVENTS=$1 CYCLE=$2 FILENAME=$3
```

Replace $1 to $3 by appropriate armuments.


2. Convert to MuDst:
```
root4star
[0] .L runBfcFms.C
[1] runBfcFms(-1, "pythiaOut.fzd")
```

3. Run Jet Reconstruction:
```
root4star
[0] .L RunFmsJetMakerSim.C
[1] RunFmsJetMakerSim(-1, "pythiaOut.MuDst.root", "pythiaOut.pythia.root")
```


StRoot Modules for FMS Simulation
-----------------------------------

**Required Modules:**

- `StRoot/StarGenerator:` The C++ wwrapper for the Pythia 6 kumac files. Checked out from github. Local copy kept to allow local customization.

- `StRoot/StSpinPool/StFmsSimulatorMaker`: The main FMS simulation maker. 
Already included in the release (so no local copy). This is automatically loaded in tge BFC if fmsSim is used in the chain option.
There is an alternative version called `StFmsFastSimMaker`, but that will require to include this maker to BCF separately. Just stick to `StFmsSimulatorMaker`.

- `StRoot/StMCFilter/StFmsJetFilter.cxx`: For pyhtia level event filtering.
Avaiable in the release. Checked out from github star-cvs repo. Local version kept to customize it based on the analysis. 

- `StRoot/StSpinPool/StFmsFilterMaker`: For BFC level event filtering.
Available on the github repo STAR-CVS [here](https://github.com/star-bnl/star-cvs/tree/master/StRoot/StSpinPool/StFmsFilterMaker), but no longer included in the current release. Checked out from github from same same commit as meentioned for `StFmsTriggerMaker`.

- `StRoot/StSpinPool/StFmsTriggerMaker`: For Jet reconstruction level trigger (FMS-JP1, JP2 etc) simulation.
This module is available from early tag. It (`StFmsTriggerMaker`) is still availale under CVS or github [here](https://github.com/star-bnl/star-cvs/tree/master/StRoot/StSpinPool/StFmsTriggerMaker). But it is not included in the release and hence also missing in the doxygen class references. It has been downloaded from star-cvs github commit `7e89155618e7363f6b2987e3a1d43536b64fe6e8`, download date Sep 1, 2020.


The Synchronization Issue: Pythia vs MuDst/Jet Tree
------------------------------------------------

Because we use event filter, only events passing the filter will end up in the MuDst file and jet files. However, the pythia branch is still unfiltered. This will cause synchronizaton issue if you want to compare generated vs reconstructed quantities. A separate script has been written for the synchronization purpose. In the jet trees, the particle branch will not be filtered and will definitely have event-by-event synchronization issue.


Improvements compared to Run 11 analysis effort
-------------------------------------------------

- starsim bug has been fixed (the problem reported by Minghui)
- The FMS trigger simulator has been fixed to give correct phi distibution
- The FMS event filter bug has been fixed to use six phi bins instead of eight. Also one phi bin was implemented incorrectly. E sum has been replaced by E_T sum.
- Previous analysis probably had the synchronization issue between Pythia and MuDst/Jet trees.






