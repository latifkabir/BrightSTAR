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


Bright STAR Maker
----------------------
- Maker modules are inside `StRoot/` directory and named after each detector or analysis. It start with `Br` to distinguish from `StRoot` similar modules.

- The Maker classes start with `TSt` and the corresponding root macro is inside `macros` directory. Each macro start with `Run` followed by corresponding maker class name (without `TSt`). The macros have extension `.C`.

- All required shared objects are loaded from `rootlogon.C` file on `root` startup and not from the maker macros. Do not use symbolic link. Symbolic link is copied as symbolic link.

- All batch farm job description files are inside `jobConfig/` and has the file name pattern `Sub` + Maker macro name (without Run prefix).


Bright Scheduler
-------------------
**Condor**
- Run build-in scheduler from root prompt as:
```
SubmitJob(TString functionName, Int_t firstRun, Int_t lastRunOrNfiles)
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
SubmitSumsJob(TString function, TString runList, TString outName)
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
JobStatus()
```

Directory Tree
------------------
```
.
├── analysis
│   ├── lib
│   ├── Makefile
│   └── src
├── bin
│   ├── activeJobs.sh
│   ├── GenerateDB.sh
│   ├── MakeCatalog.sh
│   ├── PrintFileList.sh
│   ├── submitSumsJob.csh
│   ├── tadd.sh
│   ├── tadd_sorted.sh
│   ├── TriggerIdMaker.sh
│   └── trigid.dat
├── condor
│   └── condor.config
├── config
│   ├── config.cfg
│   ├── R15ppTransFmsTrigDef.json
│   └── R15ppTransPhysTrigDef.json
├── diffAnalysis
│   ├── lib
│   ├── Makefile
│   └── src
├── include
│   ├── BrightStInclude.h
│   ├── cppInclude.h
│   ├── RootInclude.h
│   └── StRootInclude.h
├── jobOutput -> /star/u/kabir/pwg/jobOutput
├── jobResults -> /star/u/kabir/pwg/jobResults
├── jobs
├── libBrStar
│   ├── lib
│   ├── Makefile
│   └── src
├── macros -> ../StROOT/macros
├── Makefile
├── README.md
├── resources
├── results -> /star/u/kabir/pwg/results
├── rootlogon.C
├── runMacros
│   ├── lib
│   ├── Makefile
│   └── src
├── scripts
│   ├── eemc
│   ├── emc
│   ├── fms
│   ├── jet
│   ├── jobs
│   └── pid
├── setup.csh
├── setup.sh
├── star
├── StRoot
│   ├── BrAnMaker
│   ├── BrEEmcMaker
│   ├── BrEmcMaker
│   ├── BrFemtoMaker
│   ├── BrFmsMaker
│   ├── BrPicoMaker
│   ├── BrPidMaker
│   ├── BrRpMaker
│   ├── BrTemplateMaker
│   ├── README.md
│   ├── StFmsJetAnalysisMaker
│   ├── StFmsOFileMaker
│   └── StMuRpsUtil
├── sums
    ├── scheduler_template.xml
    ├── scheduler.xml
    ├── submitSumsJob.sh
    └── sumsConfig.sh
```


To Be Done
-----------

- Emc photon reconstruction: Tpc track veto. Use center of tower for the comparison. Decide if size of twoer is enouugh or 2 x size of the tower to be used.
- Charged particle pid: for TPC dEdx, use veto on nsig of proton while identifyting pion and so on. See Zinlong's slide.
