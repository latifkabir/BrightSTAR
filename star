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
    echo "          |		           Report issues to: latif@jlab.org			  |"
    echo "          ---------------------------------------------------------------------------------"    
}

Instruction()
{
    echo "                   "		
    echo "          Syntax: star <OPTION>"
    echo "                   "			
    echo "          ------------------------------------- List of available options --------------------------------"
    echo "          clean              : Move intermediate schedular files"
    echo "          cancel-job         : Cancel all jobs submitted"
    echo "          file-list          : Print file list"
    echo "          hpss-get           : Restore file from HPSS (tape)"
    echo "          job-status         : Check farm job status"    
    echo "          job-report         : Check farm job status report"
    echo "          submit             : Submit job using SLUMS"
    echo "          status             : Check farm job status"


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
    cancel-job)
	condor_rm $USER
	;;
    file-list)
	echo -e "
                Syntax: get_file_list.pl -keys 'path,filename' -cond 'storage=XX,filetype=XX,filename~XX, production=XX,trgsetupname=XX' -limit NN -distinct -delim '/' \n
	        Example-1: get_file_list.pl -keys 'path,filename' -cond 'storage=hpss,filetype=daq_reco_muDst,filename~st_physics,production=P11id,trgsetupname=AuAu19_production' -limit 10 -distinct -delim '/' \n
	        Example-2: get_file_list.pl -keys path,filename -cond storage=local,trgsetupname=production_pp200trans_2015,filetype=daq_reco_mudst,filename~st_fms_16 -delim '/' \n
	        Example-3: get_file_list.pl -keys 'fdid,storage,site,node,path,filename,events' -cond 'trgsetupname=AuAu19_production, filetype=daq_reco_MuDst, filename~st_physics, storage!=hpss' -limit 60 -delim '/' 
                Run 17: get_file_list.pl -keys 'path,filename' -cond 'filetype=daq_reco_MuDst,trgsetupname=pp500_production_2017,filename~st_fms,storage!=hpss' -limit 10 -delim '/' "
	
	    ;;	
    hpss-get)
	echo -e "
                 hpss_user.pl -r <output_dir> -f <file_list> 
                 where <file_list> is just a list of files.\n
                 hpss_user.pl <input_file_name> <output_file_name> \n
                 hpss_user.pl -f <input_file_list> 
                 where each line is space separated in and out file names. \n
                 hpss_user.pl -h for details
"
	;;
    job-status | status)
	# echo "I=idle, R= running, C=complete, H=held"
	# condor_q
	echo "Name                                        Machine                  RunningJobs   IdleJobs   HeldJobs"
	condor_status -submitters | grep $USER 
	;;
    job-report)
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
	echo "Now moving intermediate files ...."
	mv sched*.package.zip jobDir/
	mv sched*.package jobDir/
	mv sched*.dataset jobDir/
	mv *.session.xml jobDir/
	echo "Deleting log files ..."
	rm -r jobOutput/*
	;;
    *)
	echo "                                       "	   
	echo "              Invalid option or to be implemented        "	   
esac  
