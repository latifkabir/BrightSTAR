STAR Unified Meta Scheduler (SUMS) in BrightSTAR
===================================================


- STAR template scheduler (that allows job submission based on run numbers) is integrated in `TSTScheduler`. If running from BrightSTAR, you do not need to touch any file from here. The files `scheduler_template.xml`, `submitSumsJob.sh` and `sumsConfig.sh` are used in `TStScheduler` t submit jobs using template star scheduler.

- The file `jobMacro.C` for template scheduler from BrightSTAR is automatically generated for each job name. The file `jobMacro.C` here is a reference purpose only.

- Inside `example/`, you find an example on how you can submit jobs using SUMS for a function implemented in BrightSTAR. You need to copy these files to the top level directory and them submit the job using `star-submit <xml file name>`. One issue with SUMS inside BrightSTAR is that, it does not get $STARHOME properly in the batch farm. As a result locallibrary loading in rootlogon.C will fail. If you do not do local loading, it should be fine.








