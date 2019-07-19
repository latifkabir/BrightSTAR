// Filename: StRootInclude.h
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sun Jun  2 02:49:12 2019 (-0400)
// URL: jlab.org/~latif

#ifndef ST_ROOT_INCLUDE
#define ST_ROOT_INCLUDE

//MuDST
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuEmcCollection.h"
#include "StMuDSTMaker/COMMON/StMuDbReader.h"

//StChain
#include "StChain/StChain.h"

//Database
#include "St_db_Maker/St_db_Maker.h"

//RP
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"

//EEMC
#include "StEEmcDbMaker/StEEmcDbMaker.h"
#include "StEEmcUtil/database/StEEmcDb.h"
#include "StEEmcUtil/database/EEmcDbItem.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"
#include "StEEmcUtil/StEEmcSmd/StEEmcSmdGeom.h"
#include "StEmcUtil/database/StBemcTables.h"

// BEMC
#include "StDaqLib/EMC/StEmcDecoder.h"
#include "StEmcUtil/database/StBemcTables.h"
#include "StEmcRawMaker/defines.h"

//FMS
#include "StMuDSTMaker/COMMON/StMuFmsHit.h"
#include "StMuDSTMaker/COMMON/StMuFmsCluster.h"
#include "StMuDSTMaker/COMMON/StMuFmsPoint.h"
#include "StMuDSTMaker/COMMON/StMuFmsCollection.h"
#include "StMuDSTMaker/COMMON/StMuFmsUtil.h"
#include "StMuDSTMaker/COMMON/StMuFmsInfo.h"
#include "StFmsDbMaker/StFmsDbMaker.h"

//Trigger
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"
#include "StTriggerId.h"
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"

#endif
