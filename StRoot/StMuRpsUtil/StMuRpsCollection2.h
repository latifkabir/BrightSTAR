#ifndef _StMuRpsCollection2_h_
#define _StMuRpsCollection2_h_

#include "setup.h"

#include "TObject.h"
#include <vector>
#include <map>
#include "TClonesArray.h"

#if defined RUN_ON_MUDST
#include "StMuDSTMaker/COMMON/StMuRpsTrackPoint.h"
#include "StMuDSTMaker/COMMON/StMuRpsTrack.h"
#elif defined RUN_ON_KRAKOW_PICODST
#include "../StMuRpsTrackPoint.h"
#include "../StMuRpsTrack.h"
#endif

using namespace std;

class StRpsCollection;
class StMuRpsCollection;

class StMuRpsCollection2{
  
public:
  StMuRpsCollection2();
  StMuRpsCollection2(const StMuRpsCollection *);
  ~StMuRpsCollection2();
  
  void addTrackPoint(StMuRpsTrackPoint *trackPoint);
  void addTrack(StMuRpsTrack *track);
  
  //Event level info
  
  unsigned char siliconBunch() const;
  
  //Roman pot Level Info
  
  unsigned int numberOfRomanPots() const  { return mNumberOfRomanPot; }
  unsigned int numberOfPlanesWithClusters(int romanPotId) const;
  unsigned int adc(int romanPotId, unsigned int) const;
  unsigned int tac(int romanPotId, unsigned int) const;
  unsigned char status(int romanPotId) const;
  
  //Plane level Info
  
  unsigned int numberOfPlanes() const { return mNumberOfPlanes; }
  double offsetPlane(int romanPotId, int planeId) const;
  double zPlane(int romanPotId, int planeId) const;
  double anglePlane(int romanPotId, int planeId) const;
  short  orientationPlane(int romanPotId, int planeId) const;
  unsigned char statusPlane(int romanPotId, int planeId) const;
  
  //Cluster level Info
  
  int numberOfClusters(int romanPotId, int planeId) const;
  double positionCluster(int romanPotId, int planeId, int cluster) const;
  double positionRMSCluster(int romanPotId, int planeId, int cluster) const;
  short lengthCluster(int romanPotId, int planeId, int cluster) const;
  double energyCluster(int romanPotId, int planeId, int cluster) const;
  double xyCluster(int romanPotId, int planeId, int cluster) const;
  unsigned char qualityCluster(int romanPotId, int planeId, int cluster) const;
  
  // Tracks and Track Points
  StMuRpsTrackPoint* trackPoint( unsigned int i ) { if ( i < mTrackPoints.size() ) return mTrackPoints[i]; return nullptr; }
  StMuRpsTrack* track( unsigned int i ) { if ( i < mTracks.size() ) return mTracks[i]; return nullptr; }
  
  inline int numberOfTrackPoints() const { return mTrackPoints.size(); }
  inline int numberOfTracks() const { return mTracks.size(); }
  
private:
  
  enum {mNumberOfRomanPot = 8, mNumberOfPlanes = 4};
  
  unsigned char mSiliconBunch;
  
  int mNumberPlanes[mNumberOfRomanPot];
  int mNumberPlanesWithClusters[mNumberOfRomanPot];
  unsigned char mStatusRomanPot[mNumberOfRomanPot];
  int mADC[mNumberOfRomanPot][2];
  int mTAC[mNumberOfRomanPot][2];
  
  double mOffsetPlane[mNumberOfRomanPot][mNumberOfPlanes];
  double mzPlane[mNumberOfRomanPot][mNumberOfPlanes];
  double mAnglePlane[mNumberOfRomanPot][mNumberOfPlanes];
  short mOrientationPlane[mNumberOfRomanPot][mNumberOfPlanes];
  unsigned char mStatusPlane[mNumberOfRomanPot][mNumberOfPlanes];
  int mNumberOfClusters[mNumberOfRomanPot][mNumberOfPlanes];
  
  vector<double> mPositionCluster[mNumberOfRomanPot][mNumberOfPlanes];
  vector<double> mPositionRMSCluster[mNumberOfRomanPot][mNumberOfPlanes];
  vector<short> mLengthCluster[mNumberOfRomanPot][mNumberOfPlanes];
  vector<double> mEnergyCluster[mNumberOfRomanPot][mNumberOfPlanes];
  vector<double> mXYCluster[mNumberOfRomanPot][mNumberOfPlanes];
  vector<unsigned char> mQualityCluster[mNumberOfRomanPot][mNumberOfPlanes];
  
  vector<StMuRpsTrackPoint*> 	mTrackPoints;
  vector<StMuRpsTrack*> 	mTracks;
  
  
  ClassDef(StMuRpsCollection2, 1)
  
};

inline unsigned char StMuRpsCollection2::siliconBunch() const { return mSiliconBunch ; }
inline unsigned int StMuRpsCollection2::numberOfPlanesWithClusters(int romanPotId) const { return romanPotId < mNumberOfRomanPot ? mNumberPlanesWithClusters[romanPotId] : 0; }
inline unsigned char StMuRpsCollection2::status(int romanPotId) const {
  if  (romanPotId < mNumberOfRomanPot) return mStatusRomanPot[romanPotId];
  else return 0;
}

inline unsigned int StMuRpsCollection2::adc(int romanPotId, unsigned int n) const { return romanPotId < mNumberOfRomanPot && n < 2 ? mADC[romanPotId][n] : 0;}
inline unsigned int StMuRpsCollection2::tac(int romanPotId, unsigned int n) const { return romanPotId < mNumberOfRomanPot && n < 2 ? mTAC[romanPotId][n] : 0;}
inline double StMuRpsCollection2::offsetPlane(int romanPotId, int planeId) const {  return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes ? mOffsetPlane[romanPotId][planeId] : 0;}
inline double StMuRpsCollection2::zPlane(int romanPotId, int planeId) const {  return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes ? mzPlane[romanPotId][planeId] : 0;}
inline double StMuRpsCollection2::anglePlane(int romanPotId, int planeId) const {  return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes ? mAnglePlane[romanPotId][planeId] : 0;}
inline short  StMuRpsCollection2::orientationPlane(int romanPotId, int planeId) const {  return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes ? mOrientationPlane[romanPotId][planeId] : 0;}
inline unsigned char StMuRpsCollection2::statusPlane(int romanPotId, int planeId) const {
  if(romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes) return mStatusPlane[romanPotId][planeId];
  else return 0;
}
inline int StMuRpsCollection2::numberOfClusters(int romanPotId, int planeId) const { return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes ? mNumberOfClusters[romanPotId][planeId] : 0;}
inline double  StMuRpsCollection2::positionCluster(int romanPotId, int planeId, int cluster) const { return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes && cluster < mNumberOfClusters[romanPotId][planeId]? mPositionCluster[romanPotId][planeId][cluster] : 0;}
inline double  StMuRpsCollection2::positionRMSCluster(int romanPotId, int planeId, int cluster) const { return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes && cluster < mNumberOfClusters[romanPotId][planeId]? mPositionRMSCluster[romanPotId][planeId][cluster] : 0;}
inline short  StMuRpsCollection2::lengthCluster(int romanPotId, int planeId, int cluster) const { return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes && cluster < mNumberOfClusters[romanPotId][planeId]? mLengthCluster[romanPotId][planeId][cluster] : 0;}
inline double StMuRpsCollection2::energyCluster(int romanPotId, int planeId, int cluster) const { return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes && cluster < mNumberOfClusters[romanPotId][planeId]? mEnergyCluster[romanPotId][planeId][cluster] : 0;}
inline double StMuRpsCollection2::xyCluster(int romanPotId, int planeId, int cluster) const  { return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes && cluster < mNumberOfClusters[romanPotId][planeId]? mXYCluster[romanPotId][planeId][cluster] : 0;}
inline unsigned char StMuRpsCollection2::qualityCluster(int romanPotId, int planeId, int cluster) const   { return romanPotId < mNumberOfRomanPot && planeId < mNumberOfPlanes && cluster < mNumberOfClusters[romanPotId][planeId]? mQualityCluster[romanPotId][planeId][cluster] : 0;}

#endif
