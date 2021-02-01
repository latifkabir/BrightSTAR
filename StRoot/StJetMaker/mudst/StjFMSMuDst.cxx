#include "StjFMSMuDst.h"

#include "StMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEmcCollection.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuFmsCollection.h"
#include "StMuDSTMaker/COMMON/StMuFmsHit.h"

#include "StEventTypes.h"
#include "StEvent/StEvent.h"
#include "StEvent/StFmsCollection.h"
#include "StEvent/StFmsHit.h"
#include "StEvent/StFmsPoint.h"
#include "StEvent/StFmsPointPair.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StFmsDbMaker/StFmsDbMaker.h"

#include <iostream>
#include <fstream>
using namespace std;

StjFMSMuDst::StjFMSMuDst()
{
	_setVertex = false;
	_useFmsHit = false;
	_minEnergy = 0.1; //CKim, modified from 1 -> 0.1
}

void StjFMSMuDst::Init()
{
	mFmsDbMaker = (StFmsDbMaker*)StMaker::GetChain()->GetMaker("fmsDb");
	if (!mFmsDbMaker) LOG_ERROR  << "StjFMSMuDst::Init Failed to get StFmsDbMaker" << endm;
}//Init

StFmsCollection* StjFMSMuDst::findFmsCollection()
{
	StEvent* event = dynamic_cast<StEvent*>(StMaker::GetChain()->GetInputDS("StEvent"));
	return (event) ? event->fmsCollection() : StMuDst::fmsCollection();
}

StjTowerEnergyList StjFMSMuDst::getEnergyList()
{
    StjTowerEnergyList fmsEnergyList;
    mFmsColl = findFmsCollection();
    if (!mFmsColl)
    {
	LOG_ERROR  << "StjFMSMuDst::getEnergyList cannot find the StFmsCollection" << endm;
	return fmsEnergyList;
    }
    StjTowerEnergy energyDeposit;

    if (_useFmsHit == true)
    {
        const int nHits = mFmsColl->numberOfHits();
        StSPtrVecFmsHit& hits = mFmsColl->hits();
        for (int i=0; i<nHits; i++)
        {
            const int detId = hits[i]->detectorId();
	    if (detId<8 || detId>11) continue;
	    const int chId = hits[i]->channel();
            const double energy = hits[i]->energy();

            const int col = mFmsDbMaker->getColumnNumber(detId, chId);
            const int row = mFmsDbMaker->getRowNumber(detId, chId);
            StThreeVectorF v1 = mFmsDbMaker->getStarXYZ(detId, col, row);
            const double x = v1.x();
            const double y = v1.y();

            energyDeposit.adc        = mFmsDbMaker->getCorrectedAdc(detId, chId, hits[i]->adc());
            energyDeposit.detectorId = 100 + detId; //FMS detId is distributed from 8 - 11 -> 108 - 111
            energyDeposit.energy     = energy;
            energyDeposit.pedestal   = 0;
            energyDeposit.rms        = 0;
            energyDeposit.status     = 1;
            energyDeposit.towerId    = chId;
            energyDeposit.towerEta   = v1.pseudoRapidity();
            energyDeposit.towerPhi   = v1.phi();
            energyDeposit.towerR     = TMath::Sqrt(x*x + y*y);
            if (_setVertex == true)
            {
                energyDeposit.vertexX = _vx;
                energyDeposit.vertexY = _vy;
                energyDeposit.vertexZ = _vz;
            }
            else
            {
                StThreeVectorF vertex = StMuDst::event()->primaryVertexPosition();
                energyDeposit.vertexX = vertex.x();
                energyDeposit.vertexY = vertex.y();
                energyDeposit.vertexZ = vertex.z();
            }
            if (energy > _minEnergy) fmsEnergyList.push_back(energyDeposit);
	}//i, loop over hits
    }//Use FMS hit
    else //Default, use FMS points
    {
	const int nPoints = mFmsColl->numberOfPoints();
	StSPtrVecFmsPoint& points = mFmsColl->points();
	for (int i=0; i<nPoints; i++)
	{
	    const int detId = points[i]->detectorId();
	    if (detId<8 || detId>11) continue;
	    const double x = points[i]->XYZ().x();
	    const double y = points[i]->XYZ().y();
	    const double energy = points[i]->energy();
	    StLorentzVectorF v1 = points[i]->fourMomentum();

	    energyDeposit.adc        = 0;
	    energyDeposit.detectorId = 100 + detId; //FMS detId is distributed from 8 - 11 -> 108 - 111
	    energyDeposit.energy     = energy;
	    energyDeposit.pedestal   = 0;
	    energyDeposit.rms        = 0;
	    energyDeposit.status     = 1;
	    energyDeposit.towerId    = points[i]->id();
	    energyDeposit.towerEta   = v1.pseudoRapidity();
	    energyDeposit.towerPhi   = v1.phi();
	    energyDeposit.towerR     = TMath::Sqrt(x*x + y*y);
	    if (_setVertex == true)
	    {
		energyDeposit.vertexX = _vx;
		energyDeposit.vertexY = _vy;
		energyDeposit.vertexZ = _vz;
	    }
	    else 
	    {
		StThreeVectorF vertex = StMuDst::event()->primaryVertexPosition();
		energyDeposit.vertexX = vertex.x();
		energyDeposit.vertexY = vertex.y();
		energyDeposit.vertexZ = vertex.z();
	    }
	    if (energy > _minEnergy) fmsEnergyList.push_back(energyDeposit);
	}//i, loop over points
    }//Use FMS points

    return fmsEnergyList;
}//getEnergyList
