#!/bin/bash
#
#source /afs/rhic/rhstar/group/star_login.csh

export MACRO=$1
export RUNLIST=$2

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

export BASEDIR=`pwd`
export OUTDIR=jobResults
export OUTNAME='ex_scheduler'
export SOURCE=${BASEDIR}/.sl73_gcc485
export ROOTSETUP=${BASEDIR}/rootlogon.C

if [ -d ./jobOutput/out_$OUTNAME ]
then
    echo "WARNING! A folder with same name exist: delete old one and proceed? [y/n]"
    read OPTION
    case $OPTION in
        y)
	    echo "Deleting old folders..."
	    rm -r ./jobOutput/out_$OUTNAME
	    ;;
	*)
	    echo "Abort job submission..."
	    exit 0
    esac
fi

for RUN in `cat $RUNLIST`
do
    mkdir -p ./jobOutput/log_$OUTNAME/$RUN
    mkdir -p ./jobOutput/out_$OUTNAME
    mkdir -p ./jobOutput/report

    echo
    echo "Submitting job for run" $RUN

    star-submit-template \
	-template Scheduler_template.xml \
	-entities BASEDIR=$BASEDIR,MACRO=$MACRO,OUTDIR=$OUTDIR,OUTNAME=$OUTNAME,RUN=$RUN,SOURCE=$SOURCE,ROOTSETUP=$ROOTSETUP,FILEN=$FILEN,FILET=$FILET,PROD=$PROD,TRIG=$TRIG

    echo
    sleep 1
done

