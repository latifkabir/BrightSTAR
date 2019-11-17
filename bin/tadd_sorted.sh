#!/bin/bash

## hadd does not add root files in sorted order when the digits are not of the same size. The following solution is a work-around for this.
## This is an example only
## This is a nasty and hard way of merging root files in sorted order using hadd of the same run.
## This is error prone and must check the output from hadd to ensure it did it properly.
## ls -v might not return in sorted form on other system. It has been tested for RCAS machine.
## Do not use this approach for any serious analysis
## Here, `tac` reverses the previous output and awk '{print $9}' shows only the 9th column.

hadd output_file.root `ls -v jets_RunJetFinder_16072006_*.root | tac | awk '{print $9}'`
