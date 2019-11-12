#include "StMuRpsCollection2.h"

#if defined RUN_ON_MUDST
#include "StMuDSTMaker/COMMON/StMuRpsCollection.h"
#elif defined RUN_ON_KRAKOW_PICODST
#include "../StMuRpsCollection.h"
#endif

ClassImp(StMuRpsCollection2)

StMuRpsCollection2::StMuRpsCollection2(){}

StMuRpsCollection2::StMuRpsCollection2(const StMuRpsCollection *rps){
  mSiliconBunch = rps->siliconBunch();
  for(int i=0;i<mNumberOfRomanPot;i++){
    mNumberPlanesWithClusters[i] = rps->numberOfPlanesWithClusters(i);
    mStatusRomanPot[i] = rps->status(i);
    mADC[i][0] = rps->adc(i,0);
    mADC[i][1] = rps->adc(i,1);
    mTAC[i][0] = rps->tac(i,0);
    mTAC[i][1] = rps->tac(i,1);
    for(int j=0;j<mNumberOfPlanes;j++){
      mOffsetPlane[i][j]  = rps->offsetPlane(i,j);
      mzPlane[i][j]  = rps->zPlane(i,j);
      mAnglePlane[i][j]  = rps->anglePlane(i,j);
      mOrientationPlane[i][j]  = rps->orientationPlane(i,j);
      mStatusPlane[i][j]  = rps->statusPlane(i,j);
      mNumberOfClusters[i][j]  = rps->numberOfClusters(i,j);
      for(int k=0;k<mNumberOfClusters[i][j];k++){
	mPositionCluster[i][j].push_back(rps->positionCluster(i,j,k));
	mPositionRMSCluster[i][j].push_back(rps->positionRMSCluster(i,j,k)); // such method not implemented in StMuRpsCollection !!!
	mLengthCluster[i][j].push_back(rps->lengthCluster(i,j,k));
	mEnergyCluster[i][j].push_back(rps->energyCluster(i,j,k));
	mXYCluster[i][j].push_back(rps->xyCluster(i,j,k));
	mQualityCluster[i][j].push_back(rps->qualityCluster(i,j,k));
      }
    }
  }
}

StMuRpsCollection2::~StMuRpsCollection2(){
  for(unsigned int i=0; i<mTracks.size(); ++i) if(mTracks[i]) delete mTracks[i];
  for(unsigned int i=0; i<mTrackPoints.size(); ++i) if(mTrackPoints[i]) delete mTrackPoints[i];
}

void StMuRpsCollection2::addTrackPoint(StMuRpsTrackPoint *trackPoint){
  if(trackPoint) mTrackPoints.push_back(trackPoint);
}

void StMuRpsCollection2::addTrack(StMuRpsTrack *track){
  if(track) mTracks.push_back(track);
}