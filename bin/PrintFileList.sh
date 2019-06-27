#!/bin/bash
# uses fileCatalog to get list of MuDSTs
#
# Note: use -keys to control what columns you want
#       use -cond which is a comma-delimited list of key(operator)value
#       operator can be: = for 'equal to', ~ for 'contains', et al
#
# output is tee'd to $outputfile.*

##------------------------------------------------------------------

echo "---------------------------------------------------------------------"
echo "You need to provide the follwoing fields. Examples are:"
echo "production: P16id"
echo "trgsetupname: production_pp200trans_2015"
echo "storage: local"
echo "file name pattern: st_physics_16"
echo "Max files (enter 0 for all): 10"
echo "---------------------------------------------------------------------"
echo -e "\n"
echo "Enter Production:"
read production_

echo "Enter trigger setup:"
read trgsetupname_

echo "Enter Storage type:"
read storage_

echo "Enter file name pattern:"
read filename_

echo "Enter max files:"
read maxfiles_
echo "---------------------------------------------------------------------"

##------------------------------------------------------------------

# querey the fileCatalog
#   vars for awk:      $1   $2      $3   $4   $5   $6       $7   $8     $9
get_file_list.pl -keys path,filename -cond production=$production_,trgsetupname~$trgsetupname_,filename~$filename_,filetype=daq_reco_mudst,storage=$storage_ -limit $maxfiles_ -delim '/'




