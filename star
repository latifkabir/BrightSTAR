#!/bin/bash

# Filename: star
# Description: Unified Interface for all STAR related software.
# Author: Latif Kabir < kabir@bnl.gov >
# Created: Apr 30 02:15:54 2019 (-0500)
# URL: jlab.org/~latif

WelcomeMessage()
{
    echo "          ---------------------------------------------------------------------------------"
    echo "          |										  |"
    echo "          |                      Welcome to Bright STAR Software Manager		  |"
    echo -e "          |                    For a list of options, type \"star -h\" \t                  |"
    echo "          |		           Report issues to: latiful.kabir@ucr.edu		  |"
    echo "          ---------------------------------------------------------------------------------"    
}

Instruction()
{
    echo "                   "		
    echo "          Syntax: star <OPTION>"
    echo "                   "			
    echo "          ------------------------------------- List of available options --------------------------------"
    echo "          config             : Create required directories"
    echo "          set-mode           : Set analysis mode: 15fms | 15phy | 17fms | 17phy"
    echo "          clean              : Delete schedular files"
    echo "          cancel-job         : Cancel all jobs submitted"
    echo "          file-list          : Print file list"
    echo "          hpss-get           : Restore file from HPSS (tape)"
    echo "          job | status       : Check farm job status from current node"    
    echo "          jobs               : Check farm job status report from all nodes"
    echo "          submit             : Submit job using SLUMS"
    echo "          sout               : Open job std-out for corresponding root output"
    echo "          serr               : Open job err out for corresponding root output"
    echo "          hold-reason        : Print job hold reason"
    echo "          touch              : Change access time of all important dst files to current time"

    echo "          ------------------------------------------------------------------------------------------------"
    echo "                    "    
}
    
#-------------------------------------------------------------------------------------------------------------------------
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#--------------------------------------------------------------------------------------------------------------------------

WelcomeMessage

if [ $# -lt 1 ]
then
    echo "                   "
    echo "          Syntax: star <OPTION>"
    echo "                    "
    exit
fi

if ! [ -n "$BSTAR_DIR" ]   
then
    BSTAR_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    if [ -f $BSTAR_DIR/setup.sh ]
    then
	source $BSTAR_DIR/setup.sh
    else
	echo "Script to setup environment is NOT found."
	exit
    fi	
fi

OPTION=$1

case $OPTION in
    -h | -help | help | --help)
	Instruction
	exit
	;;
    config)
	if ! [ -d "jobOutput" ]
	then
	    mkdir -p /gpfs/mnt/gpfs01/star/pwg/$USER/jobOutput
	    ln -s /gpfs/mnt/gpfs01/star/pwg/$USER/jobOutput .
	    echo "Done creating the directory jobOutput"
	fi
	if ! [ -d "jobResults" ]
	then
	    mkdir -p /gpfs/mnt/gpfs01/star/pwg/$USER/jobResults
	    ln -s /gpfs/mnt/gpfs01/star/pwg/$USER/jobResults .
	    echo "Done creating the directory jobResults"
	fi
	if ! [ -d "dst" ]
	then
	    mkdir -p /gpfs/mnt/gpfs01/star/pwg/$USER/dst
	    ln -s /gpfs/mnt/gpfs01/star/pwg/$USER/dst .
	    echo "Done creating the directory dst"
	fi
	;;
    cancel-job)
	condor_rm $USER
	;;
    file-list)
	echo -e "
                Syntax: get_file_list.pl -keys 'path,filename' -cond 'storage=XX,filetype=XX,filename~XX, production=XX,trgsetupname=XX' -limit NN -distinct -delim '/' \n
	        Example-1: get_file_list.pl -keys 'path,filename' -cond 'storage=hpss,filetype=daq_reco_muDst,filename~st_physics,production=P11id,trgsetupname=AuAu19_production' -limit 10 -distinct -delim '/' \n
	        Example-2: get_file_list.pl -keys path,filename -cond storage=local,trgsetupname=production_pp200trans_2015,filetype=daq_reco_mudst,filename~st_fms_16 -delim '/' \n
	        Example-3: get_file_list.pl -keys 'fdid,storage,site,node,path,filename,events' -cond 'trgsetupname=AuAu19_production, filetype=daq_reco_MuDst, filename~st_physics, storage!=hpss, runnumber[]12112034-12112042' -limit 60 -delim '/' \n
                Run 17: get_file_list.pl -keys 'path,filename' -cond 'filetype=daq_reco_MuDst,trgsetupname=pp500_production_2017,filename~st_fms,storage!=hpss' -limit 10 -delim '/'  \n
	        PicoDst: get_file_list.pl -keys path,filename -cond production=P19ib,runnumber[]19130078-19131031,filetype=daq_reco_PicoDst,trgsetupname=27GeV_production_2018,tpx=1,sname2=st_physics,sanity=1,storage!=HPSS -limit 100"
	    ;;	
    hpss-get)
	echo -e "
                 hpss_user.pl -r <output_dir> -f <file_list> 
                 where <file_list> is just a list of files.\n
                 hpss_user.pl <input_file_name with full path> <output_file_name with full path> \n
                 hpss_user.pl -f <input_file_list> 
                 where each line is space separated in and out file names. \n
                 hpss_user.pl -h for details
"
	;;
    job-status | status | job)
	# echo "I=idle, R= running, C=complete, H=held"
	# condor_q
	echo "Name                                        Machine                  RunningJobs   IdleJobs   HeldJobs"
	condor_status -submitters | grep $USER 
	;;
    jobs)
	condor_q -global | grep $USER
	;;
    submit)
	if [ $# -ne 2 ]
	then
	    echo "Syntax: $0 <sumbit> <job-description-file>"
	    exit
	fi
	echo "Submitting the job: $2 ... ..."
	star-submit $2
	;;
    clean)
	echo "Deleting files ..."
	rm -r jobOutput/* 2>/dev/null
	rm  *.dataset 2>/dev/null
	rm  *.session.xml 2>/dev/null
	rm -r sched*.package 2>/dev/null
	rm sched*.package.zip 2>/dev/null
	echo "Done!"
	;;
    sout)
	fileName=$(echo $2 | sed "s/root/out/")
	cat $STARHOME/jobOutput/$fileName
	;;
    serr)
	fileName=$(echo $2 | sed "s/root/err/")
	cat $STARHOME/jobOutput/$fileName
	;;
    hold-reason)
	condor_q -hold -af HoldReason
	;;
    set-mode)
	if [ $# -ne 2 ]
	then
	    echo "Syntax: $0 <set-mode> <15fms | 15phy | 17fms | 17phy>"
	    exit
	fi

	if [ "$2" != "15fms" ] && [ "$2" != "15phy" ] && [ "$2" != "17fms" ] && [ "$2" != "17phy" ]
	then
	    echo "Syntax: $0 <set-mode> <15fms | 15phy | 17fms | 17phy>"
	    echo "You entered: $2"
	    exit
	fi
	cp $BSTAR_DIR/config/configRun$2.cfg $BSTAR_DIR/config/config.cfg
	if [ $? == 0 ]
	then
	   echo "Successfully set the analysis mode to: $2"
	else
	    echo "Unable to set the analysis mode to: $2"
	fi
	;;
    touch)
	find /star/u/kabir/GIT/BrightSTAR/dst/ -print -exec touch -a {} +
	;;
    *)
	echo "                                       "	   
	echo "              Invalid option or to be implemented        "
esac  
