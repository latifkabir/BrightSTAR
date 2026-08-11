#!/bin/bash

# Filename: setup.sh
# Description: 
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Sat Aug 10 18:39:21 2019 (-0400)
# URL:  latifkabir.github.io


STARHOME="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
export STARHOME

export LD_LIBRARY_PATH=.sl73_x8664_gcc485/lib:lib:$STARHOME/.sl73_x8664_gcc485/lib:$STARHOME/lib:$LD_LIBRARY_PATH
# Need to add path for fastjet after AL9 migration
export LD_LIBRARY_PATH=/star/nfs4/AFS/star/opt/star/sl73_x8664_gcc485/lib:$LD_LIBRARY_PATH

export PATH=$PATH:$STARHOME:$STARHOME/bin 
export LHAPDF_DATA_PATH=$STARHOME/starSim/lhapdf/

