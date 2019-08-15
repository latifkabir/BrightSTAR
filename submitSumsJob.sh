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

export FUNCTION=$1
export RUNLIST=$2
export OUTNAME=$3

source $PWD/sumsConfig.sh

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

