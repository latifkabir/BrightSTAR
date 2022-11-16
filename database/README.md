# How To Create Database Files

## Run List/File List Database (JSON):
- Use `bin/GenerateRunListDB.sh` to generate your RunList DB in JSON format for the given run period and trigger setup.

## Run List (txt):
- This could be text dump from JSON file or a QAed runlist in a text format. To dump from JSON dat5abase use `TStRunList::GetRunList->Print("all")`. The runlist in txt file is used to merge job files by run number.

- Alternatively, you can get full runlist from the runLog browser by selecting run period and trigger setup.

- The distinction of different runlists (fms, phys etc) within same run period comes from different data stream and the fact that physics trigger of any perticular stream (say fms) might not be evevated to physics yet, though other stream are already evevated to physics. For example the first day of data taking for FMS stream is always later than that for physics stream.

## Trigger Database (JSON):
- You can just use `TStTrigDef::MakeTrigIdDB()` with appropriate DB port number. For a list of port numbers for various runs see: `https://drupal.star.bnl.gov/STAR/comp/db/onlinedb/online-server-port-map`. 

- Alternatively, check `binbin/TriggerIdMaker.sh` script to get trigger ids in a text file.

- The easiest approach is to add all relevant trigger ids from the production page: https://www.star.bnl.gov/public/comp/prod/DataSummary.html. Click on the production you are using and then click on 
TriggerId for the specific data stream. It has all variations mentioned in one place.

## Fill Number Database:
- Use the shell script `bin/getFillNumber.csh` to generate txt DB and use this function to generate JSON DB from text DB: void TStFillNoDB::GenerateFillDB(TString inFile)

## Polarization By Fill:

- Get the polarization by fill number for any run period from the RHIC Spin Group wiki page: `https://wiki.bnl.gov/rhicspin/Results_(Polarimetry)`. Now save it as text format and modify so that it can be read easuly from your script.


## FMS Hot Channel List:

- Use `RunFmsHotChQaMaker` to generate FMS energy distribution for every file using job submission. Use `BHT1*VPDMB-30` trigger for this. Use emJetAnalysis/scripts/cronJob.C to submit jobs for entire dataset.

- Use the macro `diffAnalysis/scripts/assorted/MergeFmsHotChQaFiles.C` to merge output files by fill number.

- Use `AnFmsHotChFinder` along with the generated FMS energy didtribution per fill to generate hot and highly bit shifted channel list for every fill.



## QAed Run List

- Run 15 FMS runlist is based on QAed runlist from Chong / Akio (see under resources directory)

- Run 17 FMS runlist can be based on the one made by Chris and can be found here: https://www.star.bnl.gov/protected/spin/dilks/trgmon17/500/trigger.html
  Based on the above good runlist reference, the FMS trigger was elevated to physics from day 74 (this is only for FMS stream). All runs prior to day 74 have been excluded in the FMS runlist json file.
