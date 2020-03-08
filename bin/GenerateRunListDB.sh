#!/bin/bash
# uses fileCatalog to get list of MuDSTs
#
# Note: use -keys to control what columns you want
#       use -cond which is a comma-delimited list of key(operator)value
#       operator can be: = for 'equal to', ~ for 'contains', et al
#
# output is tee'd to $outputfile.*

##----------------------- pp 2015 trans physics stream-------------------------------------------
# outputfile_=Run15PhysRunList.json
# tempoutput_=temp.dat
# production_=P16id
# trgsetupname_=production_pp200trans_2015
# storage_=local
# filename_=st_physics
##------------------------------------------------------------------

##------------------------- pp 2015 trans fms stream -----------------------------------------
outputfile_=Run15FmsRunList.json
tempoutput_=temp.dat
production_=P15ik
trgsetupname_=production_pp200trans_2015
storage_=local
filename_=st_fms
##------------------------------------------------------------------

##----------------------- pp 2015 trans RP stream-------------------------------------------
# outputfile_=Run15RpRunList.json
# tempoutput_=temp.dat
# production_=P15ik
# trgsetupname_=production_pp200trans_2015
# storage_=local
# filename_=st_rp
##------------------------------------------------------------------


# querey the fileCatalog
#   vars for awk:      $1   $2      $3   $4   $5   $6       $7   $8     $9
get_file_list.pl -keys fdid,storage,site,node,path,filename,size,events,runnumber -cond production=$production_,trgsetupname~$trgsetupname_,filename~$filename_,filetype=daq_reco_mudst,storage=$storage_ -limit 0 -distinct 2>&1 | tee ${tempoutput_}

echo ""
echo "---------------------------------------------------------"
echo ""

# columns: [runnumber] [filename] [size (in bytes)] [#events]
echo "[" | tee ${outputfile_}
cat ${tempoutput_} | sed 's/::/ /g' | awk '{print "{\"run\":" $9", \"data\": {\"file\": \""$6"\", \"size\": "$7", \"events\": "$8 "}}," }' | sort -n | uniq | tee -a ${outputfile_}
echo -e "{\"run\":-1, \"data\": {\"file\": \"\", \"size\": -1, \"events\": -1}}]" | tee -a ${outputfile_}
#python -m json.tool ${tempoutput_}.json > $outputfile_
rm ${tempoutput_}


