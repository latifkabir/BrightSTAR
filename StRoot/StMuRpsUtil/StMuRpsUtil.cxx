#include "StMuRpsUtil.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream> 

#include <TVector3.h>
#include <TProcessID.h>
#include <TROOT.h>

#include "Alignment/pp2ppGeom.h"
#include "StMuRpsUtil/StMuRpsCollection2.h"

#if defined RUN_ON_MUDST
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#elif defined RUN_ON_KRAKOW_PICODST
#include "../particle_event.h"
#endif


ClassImp(StMuRpsUtil)

StMuRpsUtil::StMuRpsUtil(
  #if defined RUN_ON_MUDST
  StMuDstMaker* const evt
  #elif defined RUN_ON_KRAKOW_PICODST
  particle_event** const evt
  #endif
): mEvt(evt), mCurrentRun(0), mAlignmentPtr(nullptr), mNewRpsCollection(nullptr), mNewRpsCollectionCreated(false){
  
  mCalledByProcessMethod = false;
  
  mBeamMomentum[kEAST] = 100.14;
  mBeamMomentum[kWEST] = 100.14;
  
  mXYZ_IP[kX] = 0.0;
  mXYZ_IP[kY] = 0.0;
  mXYZ_IP[kZ] = 0.0;
  mThetaXY_tilt[kX] = 0.0;
  mThetaXY_tilt[kY] = 0.0;
  mDistanceFromIPtoDX[kEAST] = 9.8;
  mDistanceFromIPtoDX[kWEST] = 9.8;
  mLDX[kEAST] = 3.7;
  mLDX[kWEST] = 3.7;
  mBendingAngle[kEAST] = 0.018832292;
  mBendingAngle[kWEST] = 0.018826657;
  
  loadAlignment();
  loadHotStripList();
  
  setCorrectPmtSlewing();
  setRemoveHotStrips();
}


StMuRpsUtil::~StMuRpsUtil(){
  if(mAlignmentPtr) delete mAlignmentPtr;
  clear();
}


void StMuRpsUtil::init(){
}


void StMuRpsUtil::clear(){
  if(mNewRpsCollectionCreated){
    delete mNewRpsCollection;
    mNewRpsCollectionCreated = false;
  }
  mTrackPointsMap.clear();
  TProcessID::SetObjectCount(mObjectNumber);
}


StMuRpsCollection2* StMuRpsUtil::process(){
  #if defined RUN_ON_MUDST
  updateRunNumber( mEvt->muDst()->event()->runId() );
  setStMuRpsCollectionPointer( mEvt->muDst()->RpsCollection() );
  #elif defined RUN_ON_KRAKOW_PICODST
  updateRunNumber( (*mEvt)->getRunNum() );
  setStMuRpsCollectionPointer( &((*mEvt)->mRpsCollection) );
  #endif
  
  mObjectNumber = TProcessID::GetObjectCount();
  mNewRpsCollection = new StMuRpsCollection2(mRpsColl);
  mNewRpsCollectionCreated = true;
  mCalledByProcessMethod = true;
  
  // loop over track-points
  for(int i=0; i<mRpsColl->numberOfTrackPoints(); ++i){
    const StMuRpsTrackPoint *tpOld = mRpsColl->trackPoint(i);
    StMuRpsTrackPoint *tpNew = new StMuRpsTrackPoint();
    trackPointInfo tpInfo;
    TRACKPOINT_EVALUATION rating = recalcTrackPoint( tpOld, tpNew );
    if( rating==rpsSuccess ){
      mNewRpsCollection->addTrackPoint(tpNew);
      tpInfo.mTrackPoint = tpNew;
    } else{
      delete tpNew;
      tpInfo.mTrackPoint = nullptr;
    }
    tpInfo.mProcessed = true;
    mTrackPointsMap[tpOld] = tpInfo;
  }
  
  // loop over tracks
  for(int i=0; i<mRpsColl->numberOfTracks(); ++i){
    const StMuRpsTrack *tOld = mRpsColl->track(i);
    StMuRpsTrack *tNew = new StMuRpsTrack();
    if( recalcTrack( tOld, tNew ) ){
      /*if( warunki na primary_track ) */ mNewRpsCollection->addTrack(tNew);
    } else{
      delete tNew;
    }
  }
  
  mCalledByProcessMethod = false;
  
  return mNewRpsCollection;
}


StMuRpsUtil::TRACKPOINT_EVALUATION StMuRpsUtil::recalcTrackPoint(const StMuRpsTrackPoint* oldTrackPoint, StMuRpsTrackPoint* newTrackPoint/*, StMuRpsCollection* const mRpsColl*/) const{
  if(!oldTrackPoint || !newTrackPoint){
    std::cerr << "Error in StMuRpsUtil::recalcTrackPoint(): at least one of pointer arguments is NULL." << std::cout;
    return rpsError;
  }
  
  int rpId = oldTrackPoint->rpId();
  double xyz[3] = {0.0, 0.0, 0.0};
  unsigned int nClusters[kCoordinates] = {0, 0};
  
  for(int i=0; i<oldTrackPoint->mNumberOfPlanesInRp; ++i){
    int coordinate = (i==0||i==2)?kY:kX;
    //if() cluster not hot then proceed // to be added
    newTrackPoint->setClusterId(oldTrackPoint->clusterId(i), i);
    if(oldTrackPoint->clusterId(i)>-1){
      ++nClusters[coordinate];
      xyz[coordinate] += mAlignmentPtr->GetStrip0(rpId,i) + kOrientations[rpId][i]*mRpsColl->positionCluster(rpId,i,oldTrackPoint->clusterId(i));
      xyz[kZ] += mRpsColl->zPlane(rpId,i);
    }
  }
  
  if(nClusters[kX]==0 || nClusters[kY]==0) return rpsFake;
  TVector3 newPosition(xyz[kX]/nClusters[kX], xyz[kY]/nClusters[kY], xyz[kZ]/(nClusters[kX]+nClusters[kY]));
  newTrackPoint->setPosition(newPosition);
  newTrackPoint->setRpId(oldTrackPoint->rpId());
  newTrackPoint->setQuality( ((nClusters[kX]+nClusters[kY])==oldTrackPoint->planesUsed()) ? oldTrackPoint->quality() : StMuRpsTrackPoint::rpsNormal );
  //if() using slewing correction then correct for slewing // to be added
  for(int i=0; i<oldTrackPoint->mNumberOfPmtsInRp; ++i) newTrackPoint->setTime(oldTrackPoint->time(i), i);

  // checking if the same track-point has already been created only if called by Process() method
  if(mCalledByProcessMethod){
    for(std::map<const StMuRpsTrackPoint*,trackPointInfo>::const_iterator it=mTrackPointsMap.begin(); it!=mTrackPointsMap.end(); ++it){
      if(it->second.mProcessed && it->second.mTrackPoint){
	if( newTrackPoint->rpId() == it->second.mTrackPoint->rpId() ){
	  bool fullCopy = true;
	  for(int i=0; i<newTrackPoint->mNumberOfPlanesInRp; ++i){
	    if(newTrackPoint->clusterId(i) != it->second.mTrackPoint->clusterId(i)){
	      fullCopy = false;
	      break;
	    }
	  }
	  if(fullCopy) return rpsExists;
	}
      }
    }
  }
  
  return rpsSuccess;
}


bool StMuRpsUtil::recalcTrack(const StMuRpsTrack* oldTrack, StMuRpsTrack* newTrack/*, StMuRpsCollection* const mRpsColl*/){
  if(!oldTrack || !newTrack){
    std::cerr << "Error in StMuRpsUtil::recalcTrack(): at least one of pointer arguments is NULL." << std::cout;
    return false;
  }
  
  bool hasTrackPoints[kStationsPerBranch] = {false, false};
  StMuRpsTrackPoint *tpNew[kStationsPerBranch] = {nullptr, nullptr};
  
  // recalculating track-points (if already done - use recalculated)
  for(int i=0; i<kStationsPerBranch; ++i){
    const StMuRpsTrackPoint *tpOld = oldTrack->trackPoint(i);
    if( tpOld==nullptr ) continue;
    if( mTrackPointsMap[tpOld].mProcessed == true ){
      if( mTrackPointsMap[tpOld].mTrackPoint != nullptr ) hasTrackPoints[i] = true;
    } else{
      tpNew[i] = new StMuRpsTrackPoint();
      trackPointInfo tpInfo;
      TRACKPOINT_EVALUATION rating = recalcTrackPoint( tpOld, tpNew[i] );
      if( rating==rpsSuccess ){
	tpInfo.mTrackPoint = tpNew[i];
	hasTrackPoints[i] = true;
      } else{
	delete tpNew[i];
	tpInfo.mTrackPoint = nullptr;
      }
      tpInfo.mProcessed = true;
      mTrackPointsMap[tpOld] = tpInfo;
    }
    tpNew[i] = mTrackPointsMap[tpOld].mTrackPoint;
  }
  
  int branch = oldTrack->branch();
  unsigned int side = ( branch < kBranches/2 ? kEAST : kWEST );
  int sign = (side == kEAST ? -1 : 1 );
  
  // if track-points reconstructed in both stations in branch
  if( hasTrackPoints[kRP1] && hasTrackPoints[kRP2] ){

    newTrack->setBranch( branch ); // setting ID of branch
    newTrack->setType( StMuRpsTrack::rpsGlobal ); // setting the type of the track
    newTrack->setTrackPoint( tpNew[kRP1], kRP1 ); // setting constituent track-points
    newTrack->setTrackPoint( tpNew[kRP2], kRP2 ); // setting constituent track-points

    // below calculating momentum vector
    double localThetaX = newTrack->thetaRp( StMuRpsTrack::rpsAngleThetaX ) - sign*mThetaXY_tilt[kX]; // REMINDER: sensitive to changes in StMuRpsTrack::thetaRp() !
    double localThetaY = newTrack->thetaRp( StMuRpsTrack::rpsAngleThetaY ) - sign*mThetaXY_tilt[kY]; // REMINDER: sensitive to changes in StMuRpsTrack::thetaRp() !
    double x_BCS =  tpNew[kRP1]->x() - mXYZ_IP[kX] - std::sin(mThetaXY_tilt[kX])*( tpNew[kRP1]->z() - mXYZ_IP[kZ] ); // x_RP1 in beam coordinate system
    double d2 = std::abs( tpNew[kRP1]->z() ) - mLDX[side] - mDistanceFromIPtoDX[side]; // distance from DX magnet exit to first RP station
    double thetaX_IP = ( x_BCS - (d2 + 0.5*mLDX[side])*localThetaX ) / ( mDistanceFromIPtoDX[side]  + 0.5*mLDX[side] );
    double xi = 1. / ( 1 + (mBendingAngle[side]*(mDistanceFromIPtoDX[side] + 0.5*mLDX[side])) / ( localThetaX*std::abs( tpNew[kRP1]->z() ) - x_BCS ) );
    double momentumValue = mBeamMomentum[side] * (1.-xi);

    TVector3 momentumVector( 0, 0, sign*momentumValue );
    momentumVector.RotateX( -sign*localThetaY );
    momentumVector.RotateY( sign*thetaX_IP );
    newTrack->setP( momentumVector ); // setting the momentum vector

    return true;
  }
  // if track-point reconstructed only in one station in branch
  else if( hasTrackPoints[kRP1] || hasTrackPoints[kRP2] ){

    int station = hasTrackPoints[kRP1]==true ? kRP1 : kRP2; // checking ID of station where track-point was found
    newTrack->setBranch( branch ); // setting ID of branch
    newTrack->setType( StMuRpsTrack::rpsLocal ); // setting the type of the track
    newTrack->setTrackPoint( tpNew[station], station ); // setting constituent track-point

    // below calculating momentum vector (assuming no momentum loss == elastic track)
    double x_BCS = tpNew[station]->x() - mXYZ_IP[kX] - std::sin(mThetaXY_tilt[kX])*( tpNew[station]->z() - mXYZ_IP[kZ] ); // x_RP in beam coordinate system
    double y_BCS = tpNew[station]->y() - mXYZ_IP[kY] - std::sin(mThetaXY_tilt[kY])*( tpNew[station]->z() - mXYZ_IP[kZ] ); // y_RP in beam coordinate system
    double localThetaX = x_BCS / std::abs( tpNew[station]->z() );
    double localThetaY = y_BCS / std::abs( tpNew[station]->z() );
    double momentumValue = mBeamMomentum[side];

    TVector3 momentumVector( 0, 0, sign*momentumValue );
    momentumVector.RotateX( -sign*localThetaY );
    momentumVector.RotateY( sign*localThetaX );
    newTrack->setP( momentumVector ); // setting the momentum vector

    return true;
  }
  else return false;
}

void StMuRpsUtil::updateVertex(double newX, double newY, double newZ){
  mXYZ_IP[kX] = newX;
  mXYZ_IP[kY] = newY;
  mXYZ_IP[kZ] = newZ;
}


void StMuRpsUtil::updateRunNumber(int runNum){
  if(mCurrentRun!=runNum){
    mCurrentRun = runNum;
    mAlignmentPtr->GetData(mCurrentRun);
  }
}


void StMuRpsUtil::loadAlignment(){
    mAlignmentPtr = new pp2ppGeom("StRoot/StMuRpsUtil/Alignment/pp2ppRPpositions","StRoot/StMuRpsUtil/Alignment/pp2ppOffsets.bin.v1.1.x"); //Changed the location <-- Latif
}


void StMuRpsUtil::loadHotStripList(){
  std::ifstream fp;
  std::string filePath("StRoot/StMuRpsUtil/HotStrips/hotStrips.list");
  fp.open(filePath.c_str());

  if(!fp.good()){
    std::cerr << "Error in StMuRpsUtil::loadHotStripList(): couldn't open file " << filePath << std::endl;
    return;
  }
  else{
    int siPlaneCounter = 0;
    char buffer[200];
    while(fp.getline(buffer, 200)){
      if(buffer[0]=='>') continue;
      else if(buffer[0]=='#'){
	++siPlaneCounter;
	continue;
      }
      else{
	int rpId = (siPlaneCounter-1)/4;
	int planeId = siPlaneCounter - 4*rpId - 1;
	std::string thisLine(buffer);
	int stripId;
	std::istringstream(thisLine) >> stripId;
	mHotStrip[rpId][planeId].push_back( stripId );
      }
    }
    fp.close();
    
    if(siPlaneCounter!=32) std::cout << "Warning in StMuRpsUtil::loadHotStripList(): file with hot strip IDs seem to contain too few Si planes defined - check is recommended." << std::endl;
  }
}


const int StMuRpsUtil::kOrientations[StMuRpsUtil::kRomanPots][StMuRpsUtil::kPlanesPerCoordinate*StMuRpsUtil::kCoordinates] = 
{{1,1,1,1}, {-1,-1,-1,-1}, {1,1,1,1}, {-1,-1,-1,-1}, {1,-1,1,-1}, {-1,1,-1,1}, {1,-1,1,-1}, {-1,1,-1,1}};
const int StMuRpsUtil::kPlanes[StMuRpsUtil::kCoordinates][StMuRpsUtil::kPlanesPerCoordinate] =
				 {{1, 3},  // kX (vertical strips)
				  {0, 2}}; // kY (horizontal strips)
const int StMuRpsUtil::kRpInBranch[StMuRpsUtil::kBranches][StMuRpsUtil::kStationsPerBranch] =
				{{0, 2}, {1, 3},
				 {4, 6}, {5, 7}};
