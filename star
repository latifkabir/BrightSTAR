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
    echo "          |                      Welcome to Bright STAR Software Manager			  |"
    echo -e "          |                    For a list of options type \"star -h\" \t                  |"
    echo "          |		           Report issues to: latif@jlab.org			  |"
    echo "          ---------------------------------------------------------------------------------"    
}

Instruction()
{
    echo "                   "		
    echo "          Syntax: star <OPTION>"
    echo "                   "			
    echo "          ------------------------------------- List of available options --------------------------------"
    echo "          file-list          : Print File List"
    echo "          hpss-get           : Restore file from HPSS (tape)"

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
    if [ -f $BSTAR_DIR/bin/setup.sh ]
    then
	source $BSTAR_DIR/bin/setup.sh
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
    file-list)
	echo -e "
                Syntax: get_file_list.pl -keys 'path,filename' -cond 'storage=XX,filetype=XX,filename~XX, production=XX,trgsetupname=XX' -limit NN -distinct -delim '/' \n
	        Example-1: get_file_list.pl -keys 'path,filename' -cond 'storage=hpss,filetype=daq_reco_muDst,filename~st_physics,production=P11id,trgsetupname=AuAu19_production' -limit 10 -distinct -delim '/' \n
	        Example-2: get_file_list.pl -keys path,filename -cond storage=local,trgsetupname=production_pp200trans_2015,filetype=daq_reco_mudst,filename~st_fms_16"
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
    *)
	echo "                                       "	   
	echo "              Invalid option or to be implemented        "	   
esac  
