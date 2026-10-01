#!/bin/bash

let CYCLE=5000+$1

source setup.sh
export LHAPDF_DATA_PATH=./lhapdf/
echo "EEmcSimRunStarsimAndBfc($CYCLE, $2)" | root4star -l -b
echo "RunEEmcJetFinderPro($CYCLE, $2)" | root4star -l -b
