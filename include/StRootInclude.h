// Filename: StRootInclude.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun  2 02:49:12 2019 (-0400)
// URL: jlab.org/~latif

#ifndef ST_ROOT_INCLUDE
#define ST_ROOT_INCLUDE

//======= Star Library =============
// Do NOT include the following headers.
// #include "StarClassLibrary/StThreeVector.hh"
// #include "StarClassLibrary/StLorentzVectorF.hh"

//============= St Event ==========
#include "StEvent/StEvent.h"
#include "StEventMaker/StEventMaker.h"
#include "StEvent/StFmsCollection.h"
#include "StEvent/StEmcCollection.h"

//==========MuDST==================
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuEmcCollection.h"
#include "StMuDSTMaker/COMMON/StMuDbReader.h"
#include "StMuDSTMaker/COMMON/StMuDst2StEventMaker.h"

//===============StChain========================
#include "StChain/StChain.h"

//===========Database================
#include "St_db_Maker/St_db_Maker.h"
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"

//============RP=====================
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"

//=============EEMC=================
#include "StEEmcDbMaker/StEEmcDbMaker.h"
#include "StEEmcUtil/database/StEEmcDb.h"
#include "StEEmcUtil/database/EEmcDbItem.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"
#include "StEEmcUtil/StEEmcSmd/StEEmcSmdGeom.h"
#include "StEmcUtil/database/StBemcTables.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomDefs.h"

//EEMC Reconstruction IU Algorithm
#include "StEEmcPool/StEEmcA2EMaker/StEEmcA2EMaker.h"
#include "StEEmcPool/StEEmcIUPi0/StEEmcIUPointMaker.h"
#include "StEEmcPool/StEEmcIUPi0/StEEmcIUClusterMaker.h"
#include "StEEmcPool/StEEmcIUPi0/StEEmcIUMixMaker.h"
#include "StEEmcPool/StEEmcIUPi0/StEEmcIUPi0Analysis.h"

//EEMC Reconstruction TSIU Algorithm
#include "StEEmcPool/StEEmcTreeMaker/StEEmcTreeMaker.h"
#include "StEEmcPool/StEEmcTreeMaker/StEEmcEnergyMaker.h"
#include "StEEmcPool/StEEmcTreeMaker/StSpinInfoMaker.h"
#include "StEEmcPool/StEEmcTreeMaker/StMcEEmcTreeMaker.h"
#include "StEEmcPool/EEmcTreeContainers/EEmc2ParticleCandidate.h"
#include "StEEmcPool/EEmcTreeContainers/EEmcParticleCandidate.h"


#include "StEEmcPool/StEEmcHitMaker/StEEmcHitMakerSimple.h"
#include "StEEmcPool/StEEmcHitMaker/StEEmcTowerClusterFinder.h"
#include "StEEmcPool/StEEmcHitMaker/StEEmcStripClusterFinder.h"
#include "StEEmcPool/StEEmcHitMaker/StEEmcStripClusterFinderTSIU.h"
#include "StEEmcPool/StEEmcHitMaker/StEEmcPointFinder.h"
#include "StEEmcPool/StEEmcHitMaker/StEEmcPointFinderIU.h"
#include "StEEmcPool/StEEmcHitMaker/StEEmcEnergyApportioner.h"
#include "StEEmcPool/StEEmcHitMaker/StEEmcEnergyApportionerIU.h"


#include "StEEmcSimulatorMaker/StEEmcSlowMaker.h"



//===========BEMC===================
#include "StDaqLib/EMC/StEmcDecoder.h"
#include "StEmcUtil/database/StBemcTables.h"
#include "StEmcRawMaker/defines.h"
#include "StEpcMaker/StEpcMaker.h"
#include "StEvent/StEmcPoint.h"
#include "StEvent/StEmcCollection.h"
#include "StEmcADCtoEMaker/StEmcADCtoEMaker.h"
#include "StPreEclMaker/StPreEclMaker.h"

//========FMS=====================
#include "StMuDSTMaker/COMMON/StMuFmsHit.h"
#include "StMuDSTMaker/COMMON/StMuFmsCluster.h"
#include "StMuDSTMaker/COMMON/StMuFmsPoint.h"
#include "StMuDSTMaker/COMMON/StMuFmsCollection.h"
#include "StMuDSTMaker/COMMON/StMuFmsUtil.h"
#include "StMuDSTMaker/COMMON/StMuFmsInfo.h"
#include "StFmsDbMaker/StFmsDbMaker.h"
#include "StEvent/StFmsPointPair.h"
#include "StEvent/StFmsCollection.h"
#include "StFmsHitMaker/StFmsHitMaker.h"
#include "StFmsPointMaker/StFmsPointMaker.h"
#include "StFmsFpsMaker/StFmsFpsMaker.h"

//=========Trigger=============
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"
#include "StTriggerId.h"
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"
#include "StTriggerUtilities/StTriggerSimuMaker.h"
#include "StTriggerUtilities/Bemc/StBemcTriggerSimu.h"

//================Jet ===========================
#include "StJetFinder/StProtoJet.h"
#include "StSpinPool/StJetEvent/StJetCandidate.h"
#include "StSpinPool/StJetEvent/StJetEvent.h"
#include "StSpinPool/StJetSkimEvent/StJetSkimEvent.h"
#include "StSpinPool/StJetSkimEvent/StJetSkimEvent.h"
#include "StSpinPool/StJetEvent/StJetVertex.h"
#include "StSpinPool/StJetEvent/StJetTrack.h"
#include "StSpinPool/StJetEvent/StJetTower.h"
#include "StSpinPool/StTriggerFilterMaker/StTriggerFilterMaker.h"
#include "StSpinPool/StJetEvent/StJetParticle.h"
#include "StJetMaker/StJetMaker2012.h"
#include "StJetMaker/StAnaPars.h"
#include "StJetMaker/towers/StjTowerEnergyCorrectionForTracksFraction.h"
#include "StJetMaker/tracks/StjTrackCutFlag.h"
#include "StJetMaker/tracks/StjTrackCutNHits.h"
#include "StJetMaker/tracks/StjTrackCutPossibleHitRatio.h"
#include "StJetMaker/tracks/StjTrackCutDca.h"
#include "StJetMaker/tracks/StjTrackCutDcaPtDependent.h"
#include "StJetMaker/tracks/StjTrackCutPt.h"
#include "StJetMaker/tracks/StjTrackCutEta.h"
#include "StJetMaker/tracks/StjTrackCutLastPoint.h"
#include "StJetMaker/tracks/StjTrackCutTdcaPtDependent.h"
#include "StJetMaker/towers/StjTowerEnergyCutBemcStatus.h"
#include "StJetMaker/towers/StjTowerEnergyCutTowerId.h"
#include "StJetMaker/towers/StjTowerEnergyCutAdc.h"
#include "StJetMaker/towers/StjTowerEnergyCutEt.h"
#include "StJetFinder/StProtoJetCutPt.h"
#include "StJetFinder/StProtoJetCutEta.h"
#include "StJetFinder/StFastJetAreaPars.h"
#include "StJetFinder/StFastJetPars.h"
#include "StJetMaker/StJetSkimEventMaker.h"
#include "StJetFinder/AbstractFourVec.h"
#include "StJetMaker/tracks/StjTrackCutChi2.h"

//Simulation
#include "StJetMaker/StPythiaFourPMaker.h"
#include "StJetMaker/St_pythia_Maker.h"
#include "StJetMaker/mcparticles/StjMCParticleCutParton.h"
#include "StJetMaker/mcparticles/StjMCParticleCutStatus.h"
#include "StSpinPool/StBfcTriggerFilterMaker/StPythiaEventMaker.h"
#include "StSpinPool/StJetSkimEvent/StPythiaEvent.h"


#endif
