#!/bin/bash
# runs condor_q in a loop which does not exit until no jobs
# remain in the condor queue

while [ 1 ]
do
  n=$(condor_q $(whoami) | tail -n1 | awk '{print $1}')
  if [ $n -gt 0 ]
  then
    echo "$n condor jobs remain..."
    sleep 10
  else
    echo "condor done"
    break
  fi
done
