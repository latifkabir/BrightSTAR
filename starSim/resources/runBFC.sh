#Old FMS example to produce in TTree format
# root4star -q -b bfc.C\(5,\""DbV20120923 tpcRS y2012 MakeEvent ITTF NoSsdIt NoSvtIt Idst BAna l0 Tree logger Sti VFPPVnoCTB beamLine tpcDB TpcHitMover TpxClu fmsDb fmsSim bbcSim btofsim tags emcY2 EEfs evout -dstout IdTruth geantout big fzin MiniMcMk clearmem sdt20120417.193427"\",\"test.fzd\"\)

#The following will produce picoDst output (from Oleg's W alalysis)
# root4star -q -b bfc.C\(5,\""DbV20200225 pp2017a StiCA btof mtd mtdCalib PicoVtxDefault PicoCovMtxWrite fmsDat fmsPoint fpsDat BEmcChkStat OSpaceZ2 OGridLeak3D -evout -hitfilt"\",\"test.fzd\"\)

#The following one is an example from STAR drupal instruction to run on kinemactics events
#root4star -q -b bfc.C\(10, \""ry2012a AgML USExgeom fzin TpcFastSim sti ittf cmudst geantout "\", \"test.fzd\"\)


#The following one is for Run 15 FMS EM Jet Simulation to produced 
root4star bfcFms.C
