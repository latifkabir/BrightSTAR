#!/bin/bash

inlist="goodruns.dat" ##Input file should not have any additional line at the end
outfile="trigid.dat"
dbserver="db04.star.bnl.gov"
#dbserver="dbbak.starp.bnl.gov"
dbport=3414

> $outfile
while read line; do
  run=$(echo $line | awk '{print $1}')
  echo $run
  mysql --skip-column-names --host $dbserver --port $dbport Conditions_rts -e\
  "SELECT idx_rn,idx_trigger,name FROM triggers WHERE idx_rn=${run}"\
  | grep -E 'FMS' \
  | grep -v FMS_RP \
  | sed 's/\t/ /g' \
  >> $outfile
done < $inlist

