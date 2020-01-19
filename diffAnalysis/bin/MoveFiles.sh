#!/bin/bash

a=1

while [ $a -lt 35 ]
do
    echo "Moving files from: AnRunAnTreeMaker$a/*"
    mv AnRunAnTreeMaker$a/* R15RpStream/.
    rm -r AnRunAnTreeMaker$a
    let a=$a+1
done
