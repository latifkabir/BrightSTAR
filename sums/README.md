STAR Unified Meta Scheduler (SUMS) in BrightSTAR
===================================================

- Job submission can be desigened two ways: 1) everything stays in submitting host 2) everything necessary for execution is copied to the execution host. The result and std out can also be saved in execution host and then copied back to submitting host or it could be saved in real time in submitting host. There are advantages and disadvantages in both approaches. STAR recommend coping eveerything to execution host and then copying back at the end.

- STAR template scheduler (that allows job submission based on run numbers) is integrated in `TSTScheduler`. If running from BrightSTAR, you can sumbit directly from root prompt. The files `scheduler_template.xml`, `submitSumsJob.sh` and `sumsConfig.sh` are used in `TStScheduler` t submit jobs using template star scheduler.

- The file `jobMacro.C` for template scheduler from BrightSTAR is automatically generated for each job name. 

- Inside `sums_scheduler/`, you find an example on how you can submit jobs using SUMS for a function implemented in BrightSTAR. You need to copy these files to the top level directory and them submit the job using `star-submit <xml file name>`. One point of running SUMS jobs for BrightSTAR is that $STARHOME needs to be set to $PWD from setup.csh script. Also since rootlogon.C is loaded twice, do not use assertion statement in your rootlogon.C file.

- Since library resources are copied to execution host and executed from there, so if you use `stardev` in your job xml command, make sure your library is also compiled with the same library.






