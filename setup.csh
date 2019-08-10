#!/bin/csh

# Filename: setup.csh
# Description: 
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Sat Aug 10 18:35:07 2019 (-0400)
# URL: jlab.org/~latif


set called=($_)
if ("$called" != "") then
   set scriptdir=$called[2]
   set STARHOME=`dirname $scriptdir`
   set STARHOME=`c\d $STARHOME && pwd`    # ensure absolute path
else
    set scriptdir=$1
    set STARHOME=$scriptdir
endif

setenv STARHOME "$STARHOME"
setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${STARHOME}/libBrStar/lib:${STARHOME}/analysis/lib:${STARHOME}/.sl73_gcc485/lib"
