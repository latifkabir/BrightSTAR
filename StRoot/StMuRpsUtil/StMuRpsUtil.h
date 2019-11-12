#ifndef _StMuRpsUtil_h_
#define _StMuRpsUtil_h_

#include "setup.h"
#include <TObject.h>
#include <vector>
#include <map>

class pp2ppGeom;
class StMuRpsTrackPoint;
class StMuRpsTrack;
class StMuRpsCollection;
class StMuRpsCollection2;
struct trackPointInfo;
#if defined RUN_ON_MUDST
class StMuDstMaker;
#elif defined RUN_ON_KRAKOW_PICODST
class particle_event;
#endif

class StMuRpsUtil : public TObject{
  public:
    // variables
    #if defined RUN_ON_MUDST
    StMuRpsUtil(StMuDstMaker* const);
    #elif defined RUN_ON_KRAKOW_PICODST
    StMuRpsUtil(particle_event** const);
    #endif
    ~StMuRpsUtil();
    
    //constants and enumerations
    enum RP_STATIONS_PER_BRANCH { kRP1=0, kRP2=1, kStationsPerBranch=2 };
    enum SILICON_PLANES_PER_COORDINATE { kFirst=0, kSecond=1 };
    enum COORDINATES { kX=0, kY=1, kCoordinates=2, kZ = kCoordinates };
    enum SIDES { kEAST=0, kWEST=1, kSides=2};
    enum TRACKPOINT_EVALUATION { rpsError, rpsFake, rpsExists, rpsSuccess };
    static const int kRomanPots = 8; // number of RPs
    static const int kBranches = 4; // number of branches in RP system (EU, ED, WU and WD)
    static const int kPlanesPerCoordinate = kRomanPots/2; // number of Si planes for one spatial coordinate
    
    // methods
    void init();
    void clear();
    StMuRpsCollection2* process();
    TRACKPOINT_EVALUATION recalcTrackPoint(const StMuRpsTrackPoint* oldTrackPoint, StMuRpsTrackPoint* newTrackPoint/*, StMuRpsCollection* const rpsColl*/) const;
    bool recalcTrack(const StMuRpsTrack* oldTrack, StMuRpsTrack* newTrack/*, StMuRpsCollection* const mRpsColl*/);
    void setCorrectPmtSlewing(bool = true);
    void setRemoveHotStrips(bool = true);
    void updateRunNumber(int);
    void setStMuRpsCollectionPointer(StMuRpsCollection* const);
    void updateVertex(double, double, double);

  private:
    // variables
    #if defined RUN_ON_MUDST
    StMuDstMaker* mEvt;
    #elif defined RUN_ON_KRAKOW_PICODST
    particle_event** mEvt;
    #endif 
    int mCurrentRun;
    int mObjectNumber;
    bool mOption_correctForPmtSlewing;
    bool mOption_removeHotStrips;
    bool mCalledByProcessMethod;
    
    double mBeamMomentum[kSides];
//     double mSkew_param[kRomanPots][2][4] ; // 4 parameters for each PMT and there are 2 PMT's for each of the 8 RP
    double mXYZ_IP[3]; /* collision coordinates at the IP; 0=X, 1=Y, 2=Z */
    double mThetaXY_tilt[kSides]; /* tilt angles of the beam at collision; 0=X, 1=Y */
    double mDistanceFromIPtoDX[kSides]; /* distance from the IP to the DX magnet in the East and West; 0=E, 1=W */
    double mLDX[kSides];     /* length of DX in the East and West; 0=E, 1=W */
    double mBendingAngle[kSides];     /* DX bending angles in the East and West; 0=E, 1=W */
//     double mConversion_TAC_time ;	/* converting the TAC tick to time (second) */
    
    StMuRpsCollection* mRpsColl;
    pp2ppGeom *mAlignmentPtr;
    StMuRpsCollection2 *mNewRpsCollection;
    bool mNewRpsCollectionCreated;
    std::vector<int> mHotStrip[kRomanPots][kCoordinates*kPlanesPerCoordinate];
    std::map<const StMuRpsTrackPoint*,trackPointInfo> mTrackPointsMap;
    
    //constants
    static const int kOrientations[kRomanPots][kCoordinates*kPlanesPerCoordinate];
    static const int kPlanes[kCoordinates][kPlanesPerCoordinate];
    static const int kRpInBranch[kBranches][kStationsPerBranch];
    
    // methods
    void loadAlignment();
    void loadHotStripList();

  ClassDef(StMuRpsUtil, 1)
};

inline void StMuRpsUtil::setCorrectPmtSlewing(bool opt){ mOption_correctForPmtSlewing = opt; };
inline void StMuRpsUtil::setRemoveHotStrips(bool opt){ mOption_removeHotStrips = opt; };
inline void StMuRpsUtil::setStMuRpsCollectionPointer(StMuRpsCollection* const rpsColl){ mRpsColl = rpsColl; };

struct trackPointInfo {
  trackPointInfo() : mProcessed(false), mTrackPoint(nullptr){}
  bool mProcessed;
  StMuRpsTrackPoint* mTrackPoint;
};
#endif
