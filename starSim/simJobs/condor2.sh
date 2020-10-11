#!/bin/bash

let CYCLE=1000+$1

source setup.sh
echo "FmsSimRunStarsimAndBfc($CYCLE, $2)" | root4star -l -b >> /dev/null
echo "RunFmsJetFinderPro($CYCLE, $2)" | root4star -l -b >> /dev/null
