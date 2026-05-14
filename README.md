BrightSTAR
============

Analysis framework for the STAR data at RHIC. The main features include:

- Many new modules (Maker classes) for different types of analyses
- New scheduler workflow based on `HTCondor` which is itself integrated in `root`
- Configurable using the `config` file for each user
- Built-in help or instructions 

Quick Start
--------------

- Download or clone this repositiory on STAR RCF computer:


- From the top level directory do:
```
star config
```
This will create some directories required.

- Compile the library:
```
starver SL20a
source setup.[c]sh
make
```
Choose either `setup.sh` or `setup.csh` depending your terminal preference.

- Start root and type `help()`
```
root -l
[0] help()
```

- (Optional) To take full advantage of the framework, update the configuration file `config/config.cfg` accordingly.

Selective Compilation
----------------------

- Using the command `make` will compile everything. It is possoble to compile only the selected libraries that you need.
```
make lib
make cons
make macro
make emj
make sim
```
The above commands will compile `libBrStar`, `StRoot`, `runMacros`, `emJetAnalysis`, `starSim` directories respectively. Next section explains the purpose of these directories.
Depending on which part of analysis you are interested in, you can just do `make lib`, `make macros` and `make emj`.

EM-Jet Analysis Using FMS and EEMC
---------------------------------

- Follow the instructions from here: `emJetAnalysis/README.md`.

Directory Structure
--------------------

- `libBrStar`: Main Bright STAR classes. Functionality added on top on `StRoot`. Class name starts with `TSt`.
- `StRoot`: StRoot classes. Based on StMaker design. Used to generate user defined pico/nano DST.
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
├── bin
├── condor
├── config
├── database
├── dst 
├── emJetAnalysis
├── include
├── jobOutput
├── jobResults
├── jobs
├── libBrStar
├── Makefile
├── README.md
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
