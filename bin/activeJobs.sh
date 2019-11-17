#!/bin/bash


#Number of active jobs
condor_q -global | grep $USER | grep "jobs"  | wc -l
