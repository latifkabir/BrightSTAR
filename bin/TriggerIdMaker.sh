#!/bin/bash

inlist="goodruns.dat" ##Input file should not have any additional line at the end
outfile="trigid.dat"
dbserver="db04.star.bnl.gov"
#dbserver="dbbak.starp.bnl.gov"
dbport=3414


while read line; do
  run=$(echo $line | awk '{print $1}')
  echo $run
  mysql --skip-column-names --host $dbserver --port $dbport Conditions_rts -e\
  "SELECT idx_rn,offlineBit,name FROM triggers WHERE idx_rn=${run}"
done < $inlist

