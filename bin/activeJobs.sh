#!/bin/bash


#Number of active jobs
#condor_q -global | grep $USER | grep "exec"  | wc -l > /star/u/kabir/GIT/BrightSTAR/resources/temp/ActiveJobs.txt
condor_q -global | grep $USER | grep "condor"  | wc -l > /star/u/kabir/GIT/BrightSTAR/resources/temp/ActiveJobs.txt
