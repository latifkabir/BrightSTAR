#!/bin/bash



starsim -w 0 -b pythia.kumac NEVENTS=${nevents} RUN=${run} CYCLE=${cycle} FILE=${basename}.fzd

root4star -q -b runBfcFms.C\(${nevents},\"${fzdfile}\"\)

root4star -q -b RunFmsJetMakerSim.C\(-1, \"${basename}.MuDst.root\", \"${basename}.pythia.root\", \"fmsJet_${basename}.root\")
