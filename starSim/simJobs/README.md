FMS Jet Simulation With Trigger Simulator
------------------------------------------------

0. The Pythia event generation, Geant3 (starsim) and bfc parts are done with SL20a. The jet reconstruction with trigger simulator is done with SL19a. 

1. Ensure that BrightSTAR is compiled with SL20a and fmsjetSim is compiled with SL19a if you made any change.

2. If you need to change FMS filtering threshold, do that from script `jobMacroP1.C` by adding a 3rd argument for `FmsSimRunStarsimAndBfc()`.

3. If you need to start from a different base cycle, add that to the script `condor.csh` by adding the base cycle with `STARSIM_CYCLE`.

4. Note that the for the jobs, no resources are copied to execution host except (except lhapdf) and libraries and rootlogon.C are accessed from the submitting host.

5. Submit jobs as:

```
condor_submit condor.job
```

