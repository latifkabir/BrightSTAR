#!/bin/bash

# Filename: SubmitJob.sh
# Description: 
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Sat Jun 22 18:03:20 2019 (-0400)
# URL: jlab.org/~latif


#----------------------------------- Configure here -------------------------------
#RUN15 pp200trans (16066033 - 16093018), physics
export FILEN='~st_physics'
export FILET='daq_reco_mudst'
export PROD='P16id'
export TRIG='production_pp200trans_2015'

#RUN15 pp200trans (16066033 - 16093018), fms
# export FILEN='~st_fms_16'
# export FILET='daq_reco_mudst'
# export PROD='P15ik'
# export TRIG='production_pp200trans_2015||production_fms_pp200trans_2015'
#--------------------------------------------------------------------------


#--------------------------------------------------------------------------
export BASEDIR=`pwd`
export BASEOUTDIR=jobResults     # .root files directory. Relative to current directory
export JOBOUTDIR=jobOutput       #stderr, stdout, report etc directory. Relative to current directory
export SOURCE=${BASEDIR}/.sl73_gcc485
export ROOTSETUP=${BASEDIR}/rootlogon.C
export MACRO=${BASEDIR}/jobMacro.C
export SETUPFILE=${BASEDIR}/setup.csh
export LIBBRIGHT=${BASEDIR}/libBrStar/lib/libBrStar.so
export LIBANA=${BASEDIR}/analysis/lib/libStAnalysis.so
export LIBMACROS=${BASEDIR}/runMacros/lib/libRunMacros.so
#---------------------------------------------------------------------------
