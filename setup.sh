#!/bin/bash

# Filename: setup.sh
# Description: 
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Sat Aug 10 18:39:21 2019 (-0400)
# URL: jlab.org/~latif


STARHOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export STARHOME

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$STARHOME:$STARHOME/libBrStar/lib:$STARHOME/analysis/lib:$STARHOME/runMacros/lib:$STARHOME/diffAnalysis/lib:$STARHOME/emJetAnalysis/lib:$STARHOME/cepAnalysis/lib:$STARHOME/starSim/lib:$STARHOME/.sl73_gcc485/lib:$STARHOME/starSim/.sl73_gcc485/lib
export PATH=$PATH:$STARHOME:$STARHOME/bin
export STARDEV=/afs/rhic.bnl.gov/star/packages/DEV/

