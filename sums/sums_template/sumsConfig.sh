#!/bin/bash

# Filename: SubmitJob.sh
# Description: 
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Sat Jun 22 18:03:20 2019 (-0400)
# URL: jlab.org/~latif


#----------------------------------- Configure here -------------------------------
#RUN15 pp200trans (16066033 - 16093018), physics
# export FILEN='~st_physics'
# export FILET='daq_reco_mudst'
# export PROD='P16id'
# export TRIG='production_pp200trans_2015'

#RUN15 pp200trans (16066033 - 16093018), fms
export FILEN='~st_fms_16'
export FILET='daq_reco_mudst'
export PROD='P15ik'
export TRIG='production_pp200trans_2015||production_fms_pp200trans_2015'
#--------------------------------------------------------------------------

export MAXFILES=all
#export MAXFILES=1

#--------------------------------------------------------------------------
export BASEOUTDIR=${STARHOME}/jobResults            # .root files directory. Relative to current directory
export JOBOUTDIR=${STARHOME}/jobOutput       #stderr, stdout, report etc directory. Relative to current directory
export SOURCE=${STARHOME}/.sl73_gcc485
export ROOTSETUP=${STARHOME}/rootlogon.C
export MACRO=${STARHOME}/jobs/$JOBNAME/jobMacro.C
export SETUPFILE=${STARHOME}/setup.csh
export LIBBRIGHT=${STARHOME}/lib
export CONFIG=${STARHOME}/config
export DATABASE=${STARHOME}/database
#---------------------------------------------------------------------------
