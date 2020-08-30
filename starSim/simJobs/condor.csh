#!/bin/csh

setenv STARSIM_CYCLE $1
setenv STARSIM_ENTRIES $2    
@ STARSIM_CYCLE = $STARSIM_CYCLE + 1000

#-------------------------------------------- Part 1------------------------------------------------
#source /star/u/kabir/GIT/BrightSTAR/setup.csh
source ${GROUP_DIR}/.starver SL20a
setenv LHAPDF_DATA_PATH lhapdf
setenv STARHOME "/star/u/kabir/GIT/BrightSTAR"
setenv LD_LIBRARY_PATH "${STARHOME}/lib:${STARHOME}/.sl73_gcc485/lib:${LD_LIBRARY_PATH}"
    	
root4star -l -q -b "/star/u/kabir/GIT/BrightSTAR/starSim/simJobs/jobMacroP1.C($STARSIM_CYCLE, $STARSIM_ENTRIES)"

#-------------------------------------------- Part 2------------------------------------------------
# Part 2 is already combined in Part1. No need to call here    
#root4star -l -q -b "/star/u/kabir/GIT/BrightSTAR/starSim/simJobs/jobMacroP2.C($STARSIM_CYCLE, $STARSIM_ENTRIES)"

    
#-------------------------------------------- Part 3------------------------------------------------
#source /star/kabir/GIT/fmsJetSim/setup.csh

source ${GROUP_DIR}/.starver SL19a

setenv STARHOME "/star/u/kabir/GIT/fmsJetSim"
setenv LD_LIBRARY_PATH "${STARHOME}/lib:${STARHOME}/.sl73_gcc485/lib:${LD_LIBRARY_PATH}"
    
root4star -l -q -b "/star/u/kabir/GIT/BrightSTAR/starSim/simJobs/jobMacroP3.C($STARSIM_CYCLE, $STARSIM_ENTRIES)"


    
