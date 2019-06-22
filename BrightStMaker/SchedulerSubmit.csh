#!/bin/csh
#
source /afs/rhic/rhstar/group/star_login.csh

set MACRO   = $1
set RUNLIST = $2

set BASEDIR = `pwd`
set OUTDIR = jobResults
set OUTNAME = 'ex_scheduler'
set SOURCE  = ${BASEDIR}/.sl73_gcc485
set ROOTSETUP  =  ${BASEDIR}/rootlogon.C

if (-d ./jobOutput/out_$OUTNAME) then
	echo "WARNING! A folder with same name exist: delete old one and proceed? [y/n]"
	switch ($<)

		case y:
		echo "Deleting old folders..."
		rm -r ./jobOutput/out_$OUTNAME
		breaksw

		default:
		echo "Abort job submission..."
		exit 0
	endsw
endif

foreach RUN (`cat $RUNLIST`)

	mkdir -p ./jobOutput/log_$OUTNAME/$RUN
	mkdir -p ./jobOutput/out_$OUTNAME
	mkdir -p ./jobOutput/report

	echo
	echo "Submitting job for run" $RUN

	#RUN11 pp500 (12079026 - 12098031), physics
	#set FILEN = '~st_fms_12'
	#set FILET = 'daq_reco_mudst'
	#set PROD  = 'P11id'
	#set TRIG  = 'pp500_production_2011||pp500_production_2011_noeemc||pp500_production_2011_long'

	#RUN15 pp200trans (16066033 - 16093018), physics
	set FILEN = '~st_physics'
	set FILET = 'daq_reco_mudst'
	set PROD  = 'P16id'
	set TRIG  = 'production_pp200trans_2015'

	#RUN15 pp200trans (16066033 - 16093018), fms
	#set FILEN = '~st_fms_16'
	#set FILET = 'daq_reco_mudst'
	#set PROD  = 'P15ik'
	#set TRIG  = 'production_pp200trans_2015||production_fms_pp200trans_2015'

	#RUN15 pp200long2 (16093061 - 16117019), fms
	#set FILEN = '~st_fms_16'
	#set FILET = 'daq_reco_mudst'
	#set PROD  = 'P15ik'
	#set TRIG  = 'production_pp200long2_2015'

	#RUN15 pAu200 (16125035 - 16159025), fms
	#set FILEN = '~st_fms_16'
	#set FILET = 'daq_reco_mudst'
	#set PROD  = 'P15il'
	#set TRIG  = 'production_pAu200_2015||production_pAu200_fms_2015'

	#RUN15 pAl200 (16160016 - 16169041), fms
	#set FILEN = '~st_fms_16'
	#set FILET = 'daq_reco_mudst'
	#set PROD  = 'P16id'
	#set TRIG  = 'production_pAl200_2015||production_pAl200_fms_2015'

	star-submit-template \
	 -template Scheduler_template.xml \
	 -entities BASEDIR=$BASEDIR,MACRO=$MACRO,OUTDIR=$OUTDIR,OUTNAME=$OUTNAME,RUN=$RUN,SOURCE=$SOURCE,ROOTSETUP=$ROOTSETUP,FILEN=$FILEN,FILET=$FILET,PROD=$PROD,TRIG=$TRIG

	echo
	sleep 1
end
