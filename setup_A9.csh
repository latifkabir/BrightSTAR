#!/bin/csh

setenv USE_NFS4 1
setenv GROUP_DIR /star/nfs4/AFS/star/group

singularity exec -e -B /direct -B /star -B /afs -B /gpfs -B /sdcc/lustre02 /cvmfs/star.sdcc.bnl.gov/containers/rhic_sl7.sif csh
