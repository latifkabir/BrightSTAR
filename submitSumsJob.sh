#!/bin/bash

# Filename: SubmitJob.sh
# Description: 
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Sat Jun 22 18:03:20 2019 (-0400)
# URL: jlab.org/~latif

if [ $# -ne 3 ]
then
    echo "Syntax: $0  <Function Name> <RunList>  <OutName>"
    exit
fi

#------------------------------------------------------------
export FUNCTION=$1
export RUNLIST=$2
export OUTNAME=$3

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
#----------------------------------------------------------

#----------------------------------- Configure here -------------------------------
#RUN15 pp200trans (16066033 - 16093018), physics
export FILEN='~st_physics'
export FILET='daq_reco_mudst'
export PROD='P16id'
export TRIG='production_pp200trans_2015'

#RUN15 pp200trans (16066033 - 16093018), physics
# export FILEN='~st_physics'
# export FILET='daq_reco_mudst'
# export PROD='P16id'
# export TRIG='production_pp200trans_2015'

#RUN15 pp200trans (16066033 - 16093018), fms
#export FILEN='~st_fms_16'
#export FILET='daq_reco_mudst'
#export PROD ='P15ik'
#export TRIG ='production_pp200trans_2015||production_fms_pp200trans_2015'
#--------------------------------------------------------------------------

echo "Generating Job Macro ..."
echo "void jobMacro(TString fileList, TString outName)" > jobMacro.C	
echo "{" >> jobMacro.C	
echo "    gROOT->Macro(\"rootlogon.C\");" >> jobMacro.C
echo "    $FUNCTION(fileList, outName);" >> jobMacro.C  
echo "}" >> jobMacro.C

#----------------------------------------------------------------------
if [ -d $JOBOUTDIR/out_$OUTNAME ]
then
    echo "WARNING! A folder with same name exist: delete old one and proceed? [y/n]"
    read OPTION
    case $OPTION in
        y)
	    echo "Deleting old folders..."
	    rm -r $JOBOUTDIR/out_$OUTNAME
	    ;;
	*)
	    echo "Abort job submission..."
	    exit 0
    esac
fi

for RUN in `cat $RUNLIST`
do
    export OUTDIR=$BASEOUTDIR/$RUN
    mkdir -p $OUTDIR
    mkdir -p $JOBOUTDIR/log_$OUTNAME/$RUN
    echo "results from run $RUN will be saved in $OUTDIR"
    echo "stdout and stderr from run $RUN will be saved in $JOBOUTDIR/log_$OUTNAME/$RUN"
    echo
    echo "Submitting job for run" $RUN

    star-submit-template \
    	-template scheduler_template.xml \
    	-entities BASEDIR=$BASEDIR,MACRO=$MACRO,OUTDIR=$OUTDIR,JOBOUTDIR=$JOBOUTDIR,OUTNAME=$OUTNAME,RUN=$RUN,SOURCE=$SOURCE,ROOTSETUP=$ROOTSETUP,FILEN=$FILEN,FILET=$FILET,PROD=$PROD,TRIG=$TRIG,SETUPFILE=$SETUPFILE,LIBBRIGHT=$LIBBRIGHT,LIBANA=$LIBANA,LIBMACROS=$LIBMACROS 
    
    echo
    sleep 1
done

