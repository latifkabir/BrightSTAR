#!/bin/bash

let CYCLE=2500+$1

source setup.sh
echo "FmsSimRunStarsimAndBfc($CYCLE, $2)" | root4star -l -b
echo "RunFmsJetFinderPro($CYCLE, $2)" | root4star -l -b
