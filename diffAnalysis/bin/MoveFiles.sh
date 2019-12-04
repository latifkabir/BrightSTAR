#!/bin/bash

a=1

while [ $a -lt 83 ]
do
    echo "Moving files from: AnRunEEmcRpTreeMakerPart1$a/*"
    #mv AnRunEEmcRpTreeMakerPart1$a/* R15EEmcRpTree/.
    #rm -r AnRunEEmcRpTreeMakerPart1$a
    let a=$a+1
done
