#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Syntax: ./$0 <runList> <FilePrefix>"
    exit
fi


RunList=$1
FilePrefix=$2
FolderPath=/star/u/kabir/GIT/BrightSTAR/jobOutput

TIME=`date "+%Y%m%d%I"`


if ! [ -d $FolderPath ] 
then
    echo "directory NOT found"
    exit
fi


while read -r RunNumber
do
    if ! [ "${RunNumber}" == "" ]
    then
	echo "Deleting Job output files for: $FolderPath/${FilePrefix}_${RunNumber}_*"
	rm  $FolderPath/${FilePrefix}_${RunNumber}_*.log	
	rm  $FolderPath/${FilePrefix}_${RunNumber}_*.err	
	rm  $FolderPath/${FilePrefix}_${RunNumber}_*.out
    fi
done < $RunList

