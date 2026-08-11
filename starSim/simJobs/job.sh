#!/bin/bash

let CYCLE=5000+$1

source setup.sh
export LHAPDF_DATA_PATH=./lhapdf/
echo "FmsSimRunStarsimAndBfc($CYCLE, $2)" | root4star -l -b
echo "RunFmsJetFinderPro($CYCLE, $2)" | root4star -l -b
