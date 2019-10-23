/*****************************************************************************
 *
 * $Id: StFmsCluster.cxx,v 2.4 2015/10/21 14:52:09 ullrich Exp $
 *
 * Author: Thomas Burton, Yuxi Pan, 2014
 * ***************************************************************************
 *
 * Description: Implementation of StFmsCluster, a group of adjacent FMS towers
 *
 * ***************************************************************************
 *
 * $Log: StFmsCluster.cxx,v $
 * Revision 2.4  2015/10/21 14:52:09  ullrich
 * Modified print out format and added info.
 *
 * Revision 2.3  2015/09/01 18:29:01  ullrich
 * Changes due to adding StFpsSlat and interconnection between slats and points.
 *
 * Revision 2.2  2015/08/26 16:51:59  ullrich
 * Added print out fct and operator.
 *
 * Revision 2.1  2015/02/14 18:56:00  ullrich
 * Initial Revision.
 *
 *
 *****************************************************************************/
#include "StFmsClusterCopy.h"

#include "StMessMgr.h"


StFmsClusterCopy::StFmsClusterCopy()
: StObject(), mEnergy(0.) { /* no op */ }

StFmsClusterCopy::~StFmsClusterCopy() { /* no op */ }


