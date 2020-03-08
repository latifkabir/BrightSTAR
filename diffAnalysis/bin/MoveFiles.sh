#!/bin/bash

if [ $# -ne 3 ]
then
    echo "Syntax: $0 <Input directory name without index>  <Output directory name>  <Max Index>"
    exit
fi

inDir=$1
outDir=$2
maxIndex=$3

mkdir -p $outDir

if [ ! -d $outDir ]
then  
  echo "Output directory does NOT exist"
  exit
fi  

a=4
let maxIndex=$maxIndex+1

while [ $a -lt $maxIndex ]
do
    echo "Moving files from: $inDir$a/*"
    mv $inDir$a/* $outDir/.
    if [ $? -eq 0 ]
    then
	rm -r $inDir$a
    else
	echo "Moving was unsuccessful."
    fi
    let a=$a+1
done
