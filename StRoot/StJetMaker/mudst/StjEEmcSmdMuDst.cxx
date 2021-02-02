#include "StjEEmcSmdMuDst.h"

#include "StMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEmcCollection.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"

#include "StEventTypes.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"

#include "StEEmcPool/StEEmcTreeMaker/StEEmcTreeMaker.h"
#include "StEEmcPool/EEmcTreeContainers/EEmcParticleCandidate.h"

#include <iostream>
#include <fstream>
using namespace std;

StjEEmcSmdMuDst::StjEEmcSmdMuDst()
{
    _setVertex = false;
    _minEnergy = 0.01; //To be confirmed -- Latif
}


StjTowerEnergyList StjEEmcSmdMuDst::getEnergyList()
{
    StjTowerEnergyList smdEnergyList;
    
    mEEmcTreeMk = static_cast<StEEmcTreeMaker_t*>(StMaker::GetChain()->GetMaker("EEmcTreeWriter"));
    if (!mEEmcTreeMk)
    {
	cout <<"StjEEmcSmdSmd::getEnergyList - !StEEmcTreeMaker. Returning empty energy list" <<endl;
	return smdEnergyList;
    }

    TIter arrItr = mEEmcTreeMk->getEEmcParticleCandidateIter1(); // Iterator for TClonesArray of EEmcParticleCandidate_t    
    StjTowerEnergy energyDeposit;
    Int_t nPoints = 0;
    EEmcParticleCandidate_t *point;
    while ((point = (EEmcParticleCandidate_t*)arrItr.Next()))
    {
	++nPoints;

	energyDeposit.adc        = 0;
	energyDeposit.detectorId = 0;
	energyDeposit.energy     = point->E;
	energyDeposit.pedestal   = 0;
	energyDeposit.rms        = 0;
	energyDeposit.status     = 1;
	energyDeposit.towerId    = 0;
	energyDeposit.towerEta   = point->position.PseudoRapidity();
	energyDeposit.towerPhi   = point->position.Phi();
	energyDeposit.towerR     = point->position.Perp();
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
	if (point->E > _minEnergy) smdEnergyList.push_back(energyDeposit);
    }//loop over points

    return smdEnergyList;
}//getEnergyList
