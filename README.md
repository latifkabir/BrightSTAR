Bright STAR
============

Analysis framework (with brightness) for the STAR data at RHIC.

Directory structure
--------------------

- `libBrStar`: Main Bright STAR classes. Functionality added on top on `StRoot`. Class name starts with `TSt`.
- `StRoot`: StRoot classes. Based on StMaker design. Used to generate user defined pico DST.
- `analysis`: Prototype analysis example for each detector.
- `diffAnalysis`: Main analysis scripts for the physics analysis. A_N specific scripts starts with `An`. Detector specific scripts start with the acronym of the detector.
- `runMacros`: Compilable macros to run the Makers from StRoot.
- `emJetAna`: EM Jet Ananlysis with FMS and EEMC
- `starSim`: Simulation work for STAR

Bright STAR Maker
----------------------
- Maker modules are inside `StRoot/` directory and named after each detector or analysis. It start with `Br` to distinguish from `StRoot` similar modules.

- The Maker classes start with `TSt` and the corresponding root macro is inside `macros` directory. Each macro start with `Run` followed by corresponding maker class name (without `TSt`). The macros have extension `.C`.

- All required shared objects are loaded from `rootlogon.C` file on `root` startup and not from the maker macros.

- User configuration is defined in `config/config.cfg`. All batch farm job description files are inside `condor/`.

Environment Variables
--------------------------

- `STARHOME`: The top level directory on the execution host where setup.[c]sh file resides. Depending on whether command is issued locally or on batch farm nodes, `STARHOME` will point to different location. This vaiable is set by the setup.[c]sh script.
- `BRIGHTHOME`: The top level directory of the BrightStar framework on user's machine. This variable is static and set from the `config` file. 


Bright Scheduler
-------------------

**Condor**
- Run build-in scheduler from root prompt as:
```
TStScheduler::SubmitJob(TString functionName, Int_t firstRun, Int_t lastRunOrNfiles)
```
For more options type `help()`.

- Run condor scheduler from terminal:

Modify `condor.job` and `jobMacro.C` accordingly and submit job as:
```
condor_submit condor.job
```

**Sums**
- Submit job using SUMS from root prompt:

```
TStScheduler::SubmitSumsJob(TString function, TString runList, TString outName)
```
For more options type `help()`

- Submit job using SUMS from terminal:
Modify `jobMacro.C` and `sumsConfig.sh` accordingly and submit the job as:

```
./submitSumsJob.sh <function name>  <runList>  <outName>. 
```

**Check Job Status**
From root prompt do:
```
TStScheduler::JobStatus(1)
```

Directory Tree
------------------
```
.
├── analysis
├── bin
├── cepAnalysis
├── condor
├── config
├── database
├── diffAnalysis
├── dst 
├── emJetAnalysis
├── include
├── jobOutput
├── jobResults
├── jobs
├── libBrStar
├── Makefile
├── README.md
├── resources
├── results
├── rootlogon.C
├── runMacros
├── scratch 
├── scripts
├── setup.csh
├── setup.sh
├── star
├── starSim
├── StRoot
├── sums
```







