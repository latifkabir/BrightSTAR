Bright STAR
============

Analysis framework (with brightness) for the STAR data at the RHIC.

Directory structure
--------------------

- `libBrStar`: Main Bright STAR classes. Functionality added on top on `StRoot`. Class name starts with `TSt`.
- `StRoot`: StMaker scripts for batch farm job submission. Used to generate user defined pico DST.
- `analysis`: Main analysis scripts for the physics analysis. A_N specific scripts starts with `An`. Detector specific scripts start with the acronym of the detector.
- `runMacros`: Compilable macros to run the Makers from StRoot.

- Current directory structure:
```
├── analysis
│   ├── lib
│   ├── Makefile
│   └── src
├── bin
│   ├── GenerateDB.sh
│   ├── MakeCatalog.sh
│   ├── nadd.sh
│   └── PrintFileList.sh
├── config
│   └── config.cfg
├── jobConfig
│   ├── Scheduler_template.xml
│   └── SubDeltaPhiMaker.xml
├── jobDir -> /star/u/kabir/pwg/jobDir
├── jobOutput -> /star/u/kabir/pwg/jobOutput
├── jobResults -> /star/u/kabir/pwg/jobResults
├── libBrStar
│   ├── lib
│   ├── Makefile
│   └── src
├── libLoader
├── Makefile
├── README.md
├── resources
│   ├── fileList.list
│   ├── inFiles_hpss.list
│   ├── jobRunList
│   ├── Maps
│   ├── RunList
│   └── temp
├── results -> /star/u/kabir/pwg/results
├── rootlogon.C
├── schedTemplateExp.xml
├── Scheduler_template.xml
├── setup.sh
├── star
├── StRoot
│   ├── BrAnMaker
│   ├── BrEEmcMaker
│   ├── BrExampleMaker
│   ├── BrFmsMaker
│   ├── BrRpMaker
│   ├── macros
│   ├── Makefile
│   └── StEEmcPool
├── SubmitJob.csh
└── SubmitJob.sh
```


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
Modify `jobMacro.C` accordingly and submit the job as:

```
./submitSumsJob.sh <function name>  <runList>  <outName> 
```

**Check Job Status**
From root prompt do:
```
JobStatus()
```

