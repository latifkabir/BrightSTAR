#!/bin/bash

singularity exec -e -B /direct -B /star -B /afs -B /gpfs -B /sdcc/lustre02 /cvmfs/star.sdcc.bnl.gov/containers/rhic_sl7.sif /bin/csh -c "starver SL20a && /star/u/kabir/GIT/EmJet-GPC-BrightSTAR/starSim/simJobs/job.sh $1 $2"
