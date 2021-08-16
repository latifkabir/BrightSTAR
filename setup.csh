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

    #set scriptdir=`readlink -f  $0`
    #set STARHOME=`dirname $scriptdir`
endif

if(! $?SUMS_USER) then
    setenv STARHOME "$STARHOME"
else	
    setenv STARHOME "$PWD"  #For SUMS jobs
endif
    
setenv LD_LIBRARY_PATH ".sl73_gcc485/lib:lib:${STARHOME}/lib:${STARHOME}/.sl73_gcc485/lib:${LD_LIBRARY_PATH}"

