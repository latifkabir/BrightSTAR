#!/bin/bash
# uses fileCatalog to get list of MuDSTs
#
# Note: use -keys to control what columns you want
#       use -cond which is a comma-delimited list of key(operator)value
#       operator can be: = for 'equal to', ~ for 'contains', et al
#
# output is tee'd to $outputfile.*

outputfile=CATALOG15.dat

# querey the fileCatalog
#   vars for awk:      $1   $2      $3   $4   $5   $6       $7   $8     $9
get_file_list.pl -keys fdid,storage,site,node,path,filename,size,events,runnumber -cond production=P15ik,trgsetupname~production_pp200trans_2015,filename~st_fms,filetype=daq_reco_mudst,storage=hpss -limit 0 2>&1 | tee ${outputfile}.info
get_file_list.pl -keys fdid,storage,site,node,path,filename,size,events,runnumber -cond production=P15ik,trgsetupname~production_fms_pp200trans_2015,filename~st_fms,filetype=daq_reco_mudst,storage=hpss -limit 0 2>&1 | tee -a ${outputfile}.info

echo ""
echo "---------------------------------------------------------"
echo ""

# print simpler table for jobCheck.py to read
# columns: [runnumber] [filename] [size (in bytes)] [#events]
cat ${outputfile}.info | sed 's/::/ /g' | awk '{print $9" "$6" "$7" "$8}' | sort -n | uniq | tee $outputfile
