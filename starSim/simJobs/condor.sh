#!/bin/bash

source setup.sh
mv .sl73_gcc485_20a .sl73_gcc485
mv lib20a lib
mv rootlogon.C_20a rootlogon.C    

echo "FmsSimRunStarsimAndBfc(0, 2, 1)" | root4star -l -b

rm -rf .sl73_gcc485  lib rootlogon.C 
mv .sl73_gcc485_19a .sl73_gcc485
mv lib19a lib
mv rootlogon.C_19a rootlogon.C    

bstarver SL19a
ls -ltra
echo "RunFmsJetFinderPro(0, 2)" | root4star -l -b
