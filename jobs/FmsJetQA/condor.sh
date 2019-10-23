#!/bin/bash
stardev
source /star/u/kabir/GIT/BrightSTAR/setup.sh
root4star -l -q -b "/star/u/kabir/GIT/BrightSTAR/jobs/FmsJetQA/jobMacro.C(\"$1\", \"$2\")"
