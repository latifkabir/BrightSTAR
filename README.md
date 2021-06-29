BrightSTAR
============

Analysis framework (with brightness) for the STAR data at RHIC. The main features include:

- Many new modules (Maker classes) for different types of analyses
- New scheduler workflow based on `HTCondor` which is itself integrated in `root`
- Configurable using the `config` file for each user
- Built-in help or instructions 

Quick Start
--------------

- Download or clone this repositiory on STAR RCF computer:

```
git clone https://github.com/latifkabir/BrightSTAR.git
```

- From the top level directory do:
```
star config
```
This will create some directories required.

- Compile the library:
```
source setup.[c]sh
make
```
Choose either `setup.sh` or `setup.csh` depending your terminal preference.

- Start root and type `help()`
```
root -l
[0] help()
```

- To take full advantage of the framework, update the configuration file `config/config.cfg` accordingly.


Directory Structure
--------------------

- `libBrStar`: Main Bright STAR classes. Functionality added on top on `StRoot`. Class name starts with `TSt`.
- `StRoot`: StRoot classes. Based on StMaker design. Used to generate user defined pico DST.
- `analysis`: Prototype analysis example for each detector. Detector specific scripts start with the acronym of the detector.
- `diffAnalysis`: Main analysis scripts for the physics analysis. A_N specific scripts starts with `An`. 
- `runMacros`: Compilable macros to run the Makers from StRoot.
- `emJetAnalysis`: EM Jet Ananlysis with FMS and EEMC
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

The BrightSTAR framework provides multiple approaches for batch farm job submission.

**Condor**
- Run build-in scheduler from root prompt as:
```
TStScheduler::SubmitJob(TString functionName, Int_t firstRun, Int_t lastRunOrNfiles)
```
Use this approach for generating DST from MuDst.

- Submit generic job for any function or script:
```
TStScheduler::SubmitJob(vector<string> jobList, TString jobName)
```
Where `jobList` is function list in `FunctionName(arg1, ...)` format or script list in `.x Script.C(arg1, ...)` format. 

For more options type `help()`.

- Run condor scheduler from terminal:

Modify `condor.job` and `jobMacro.C` inside the directory `condor/` according to your need and submit job as:
```
condor_submit condor.job
```
This approach is convenient for simulation jobs for example.

**Sums**
- Submit job using SUMS (template) from root prompt:

```
TStScheduler::SubmitSumsJob(TString function, TString runList, TString outName)
```
For more options type `help()`

- Submit job using SUMS (template) from terminal:
  - Go to `sums/sums_template` directory.
  - Modify `jobMacro.C` and `sumsConfig.sh` accordingly and submit the job as:

```
./submitSumsJob.sh <function name>  <runList>  <outName>. 
```

- Submit jobs using SUMS (non template approach):
  - From the top level directory, update the function name inside jobMacro.C file, modify sumsJobs.xml to update dataset.
  - Submit jobs: `star-submit sumsJobs.xml`


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
├── studentsAna
├── sums
```





