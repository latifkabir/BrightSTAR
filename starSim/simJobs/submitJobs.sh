#!/bin/bash

echo "Updating libraries .... ..."
cp -ur /star/u/kabir/GIT/BrightSTAR/.sl73_gcc485 .sl73_gcc485_20a
cp -ur /star/u/kabir/GIT/BrightSTAR/lib lib20a
cp -ur /star/u/kabir/GIT/fmsJetSim/.sl73_gcc485 .sl73_gcc485_19a
cp -ur /star/u/kabir/GIT/fmsJetSim/lib lib19a

echo "Submitting jobs ... ..."
condor_submit condor.job

