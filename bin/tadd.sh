#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Syntax: ./$0 <runList>  <FilePrefix> <optional: use root-6 version = r6>"
    exit
fi

if [ "$3" == "r6" ]
then
    source  ~/TOOLS/root6_18/bin/thisroot.sh
fi

RunList=$1
FilePrefix=$2
InputDir=$STARHOME/jobResults
OutDir=$STARHOME/results/root/${FilePrefix}
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
	echo -e "\n ============== Now Merging Run Number: ${RunNumber} ======================\n"
	hadd $outfile $infile

	echo -e "\n-----> Moving the merged file ... ..."
	if [ -f ${OutDir}/${FilePrefix}_${RunNumber}.root ]
	then
	    echo -e "\n--------->Same output file already exist. Saving with time stamp ...\n"
	    mv $outfile ${OutDir}/${FilePrefix}_${RunNumber}_ver$TIME.root
	else
	    mv $outfile ${OutDir}/${FilePrefix}_${RunNumber}.root
	fi
    fi
done < $RunList


##If you do not want to merge Trees, use option -T
#hadd -T out_${RunNumber}.root jobOut_${RunNumber}_*.root

echo "To delete individual job output root files enter y:"
read option

if [ "$option" == "y" ]
then
    while read -r RunNumber
    do
	if ! [ "${RunNumber}" == "" ]
	then
	    echo "Deleting the directory ... ...: ${InputDir}/${RunNumber}"
	    rm -r ${InputDir}/${RunNumber}
	fi
    done < $RunList
fi