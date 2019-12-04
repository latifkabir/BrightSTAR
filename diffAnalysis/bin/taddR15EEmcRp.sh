#!/bin/bash

if [ $# -lt 3 ] || [ $# -gt 4 ]
then
    echo "Syntax: ./$0 <runList>  <FolderName> <FilePrefix> <optional: use root-6 version = r6>"
    exit
fi

if [ "$4" == "r6" ]
then
    source  ~/TOOLS/root6_18/bin/thisroot.sh
fi

RunList=$1
FolderName=$2
FilePrefix=$3
InputDir=$STARHOME/jobResults/${FolderName}
OutDir=$STARHOME/dst/${FolderName}
TIME=`date "+%Y%m%d%I"`

mkdir -p $OutDir

if ! [ -d $InputDir ] || ! [ -d $OutDir ]
then
    echo "Input or output directory NOT found"
    exit
fi

cp $RunList $OutDir

while read -r RunNumber
do
    if ! [ "${RunNumber}" == "" ]
    then
	infile=${InputDir}/${RunNumber}/${FilePrefix}_${RunNumber}_*.root
	outfile=${InputDir}/${RunNumber}/${FilePrefix}_${RunNumber}.root
	
	infile2=${InputDir}/${RunNumber}/RpTree_${FilePrefix}_${RunNumber}_*.root
	outfile2=${InputDir}/${RunNumber}/RpTree_${RunNumber}.root
	echo -e "\n ============== Now Merging Run Number: ${RunNumber} ======================\n"
	hadd $outfile $infile
	hadd $outfile2 $infile2

	echo -e "\n-----> Moving the merged file ... ..."
	if [ -f ${OutDir}/${FilePrefix}_${RunNumber}.root ]
	then
	    echo -e "\n--------->Same output file already exist. Saving with time stamp ...\n"
	    mv $outfile ${OutDir}/${FilePrefix}_${RunNumber}_ver$TIME.root
	    mv $outfile2 ${OutDir}/RpTree_${RunNumber}_ver$TIME.root
	else
	    mv $outfile ${OutDir}/${FilePrefix}_${RunNumber}.root
	    mv $outfile2 ${OutDir}/RpTree_${RunNumber}.root
	fi
    fi
done < $RunList

