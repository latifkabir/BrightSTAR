/****************************************************************************
 *
 * $Id: StFmsCluster.h,v 2.2 2015/08/26 16:51:59 ullrich Exp $
 *
 * Author: Thomas Burton, Yuxi Pan, 2014
 ****************************************************************************
 *
 * Description: Implementation of StFmsCluster, a group of
 *              adjacent FMS towers.
 * A cluster is formed by the energy depositions (hits) of one or more
 * particles over a group of FMS towers.
 ****************************************************************************
 *
 * $Log: StFmsCluster.h,v $
 * Revision 2.2  2015/08/26 16:51:59  ullrich
 * Added print out fct and operator.
 *
 * Revision 2.1  2015/02/14 18:56:00  ullrich
 * Initial Revision.
 *
 *
 ****************************************************************************/
#ifndef STFMSCLUSTERCOPY_H
#define STFMSCLUSTERCOPY_H

#include "StLorentzVectorF.hh"

#include "StObject.h"

//#include "StContainers.h"  // For StPtrVecFmsHit, StPtrVecFmsPoint
#include "StEnumerations.h"


class StFmsClusterCopy : public StObject {
public:
    StFmsClusterCopy();
    ~StFmsClusterCopy();
    
    float energy() const;
    int nTowers() const;
    int nPhotons() const;
    int detectorId() const;
    Float_t mEnergy;  // Total energy contained in this cluster (0th moment)
	int mDetectorId;
	int mNTowers;
	int mnPhotons;

private:

    ClassDef(StFmsClusterCopy, 1)
};


//
//   Inline functions
//
inline float StFmsClusterCopy::energy() const { return mEnergy; }
inline int StFmsClusterCopy::detectorId() const { return mDetectorId; }
inline int StFmsClusterCopy::nTowers() const { return mNTowers; }
inline int StFmsClusterCopy::nPhotons() const { return mnPhotons; }



#endif  // STFMSCLUSTER_H
