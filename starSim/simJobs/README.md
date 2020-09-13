FMS Jet Simulation With Trigger Simulator
------------------------------------------------

1. Ensure that you compile after last change.

2. If you need to change FMS filtering threshold, do that a 3rd argument for `FmsSimRunStarsimAndBfc()`.

3. If you need to start from a different base cycle, add that to the script `condor.csh` by adding the base cycle with `CYCLE`.

4. Note that the for the jobs, all resources are copied to execution host (including lhapdf) and libraries and rootlogon.C are accessed from the execution directory host.

5. Submit jobs as:

```
condor_submit condor.job
```

