#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Syntax: ./$0 <runNumber>"
    exit
fi

RunNumber=$1

hadd out_${RunNumber}.root jobOut_${RunNumber}_*.root

##If you do not wante to merger Trees, use option -T
#hadd -T out_${RunNumber}.root jobOut_${RunNumber}_*.root
