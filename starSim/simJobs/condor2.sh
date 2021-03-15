#!/bin/bash

let CYCLE=0+$1

source setup.sh

# echo "FmsSimRunStarsimAndBfc($CYCLE, $2)" | root4star -l -b >> /dev/null
# echo "RunFmsJetFinderPro($CYCLE, $2)" | root4star -l -b >> /dev/null


echo "EEmcSimRunStarsimAndBfc($CYCLE, $2)" | root4star -l -b >> /dev/null
echo "RunEEmcJetFinderPro($CYCLE, $2)" | root4star -l -b >> /dev/null
