#!/bin/bash

# Filename: SubmitJob.sh
# Description: 
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Sat Jun 22 18:03:20 2019 (-0400)
# URL: jlab.org/~latif

if [ $# -ne 4 ]
then
    echo "Syntax: $0  <Function Name> <RunList>  <OutName>  <JobName>"
    exit
fi

if [ -z "$STARHOME" ]
then
    echo "STARHOME must be set. Make sure to source setup.[c]sh"
    exit
fi    

export FUNCTION=$1
export RUNLIST=$2
export OUTNAME=$3
export JOBNAME=$4
export SUMSDIR=$STARHOME/sums

source $SUMSDIR/sumsConfig.sh

mkdir -p $STARHOME/jobs/$JOBNAME
mkdir -p $STARHOME/jobResults/$JOBNAME

echo "Generating Job Macro ..."
echo "void jobMacro(TString fileList, TString outName)" > $STARHOME/jobs/$JOBNAME/jobMacro.C	
echo "{" >> $STARHOME/jobs/$JOBNAME/jobMacro.C	
echo "    gROOT->Macro(\"$STARHOME/rootlogon.C\");" >> $STARHOME/jobs/$JOBNAME/jobMacro.C
echo "    $FUNCTION(fileList, outName);" >> $STARHOME/jobs/$JOBNAME/jobMacro.C  
echo "}" >> $STARHOME/jobs/$JOBNAME/jobMacro.C


for RUN in `cat $RUNLIST`
do
    export OUTDIR=$BASEOUTDIR/$JOBNAME/$RUN
    mkdir -p $OUTDIR
    mkdir -p $JOBOUTDIR/log_$OUTNAME/$RUN
    echo "results from run $RUN will be saved in $OUTDIR"
    echo "stdout and stderr from run $RUN will be saved in $JOBOUTDIR/log_$OUTNAME/$RUN"
    echo
    echo "Submitting job for run" $RUN

    star-submit-template \
    	-template $STARHOME/sums/scheduler_template.xml \
    	-entities STARHOME=$STARHOME,MACRO=$MACRO,OUTDIR=$OUTDIR,JOBOUTDIR=$JOBOUTDIR,OUTNAME=$OUTNAME,RUN=$RUN,SOURCE=$SOURCE,ROOTSETUP=$ROOTSETUP,FILEN=$FILEN,FILET=$FILET,PROD=$PROD,TRIG=$TRIG,SETUPFILE=$SETUPFILE,LIBBRIGHT=$LIBBRIGHT,LIBANA=$LIBANA,LIBMACROS=$LIBMACROS 
    
    echo
    sleep 1
done

