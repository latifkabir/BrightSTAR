#!/bin/csh

# Filename: setup.csh
# Description: 
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Sat Aug 10 18:35:07 2019 (-0400)
# URL: latifkabir.github.io

starver SL20a    

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

# For condor jobs only     
# setenv STARHOME "/star/u/kabir/GIT/EmJet-GPC-BrightSTAR"    

    setenv LD_LIBRARY_PATH ".sl73_x8664_gcc485/lib:lib:${STARHOME}/lib:${STARHOME}/.sl73_x8664_gcc485/lib:${LD_LIBRARY_PATH}"

# Add path for fastjet after AL9 migration 
setenv LD_LIBRARY_PATH "/star/nfs4/AFS/star/opt/star/sl73_x8664_gcc485/lib:${LD_LIBRARY_PATH}"



    
    
