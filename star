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
    echo "          get-file          : Print File List"
    echo "          jcache-status     : Get status for pending jcache jobs" 

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
    get-file)
	if [ "-h" == $2 ]
	then
	    echo "Syntax: star get-file <storage> <file_pattern> <trigger> <limit>"
	    echo -e "Alternate Usage Example: get_file_list.pl -keys 'path,filename' -cond 'storage=hpss,filetype=daq_reco_muDst,filename~st_physics,production=P11id,trgsetupname=AuAu19_production' -limit 10 -distinct -delim '/'"
	fi

	if [ $# -eq 3 ]
	then
	    echo "3 args"
#	else
	fi
	;;
    jcache-status)
	jcache pendingRequest -u `whoami`
	;;
    *)
	echo "                                       "	   
	echo "              Invalid option or to be implemented        "	   
esac  
