#!/bin/csh

## Put these in your .login and .cshrc file
# setenv USE_NFS4 1
# setenv GROUP_DIR /star/nfs4/AFS/star/group

## Then from new terminal, run this command first
singularity exec -e -B /direct -B /star -B /afs -B /gpfs -B /sdcc/lustre02 /cvmfs/star.sdcc.bnl.gov/containers/rhic_sl7.sif csh

## Manually set STAR library version and source all BrightSTAR libraries
# starver SL20a
# source setup.csh
