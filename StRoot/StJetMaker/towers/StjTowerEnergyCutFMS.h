//Written by C.Kim, UC Riverside (ckim@bnl.gov)
//Copied all format from StjTowerEnergyCutEt

#ifndef STJTOWERENERGYCUTFMS_H
#define STJTOWERENERGYCUTFMS_H

#include "StjTowerEnergyCut.h"

class StjTowerEnergyCutFMS : public StjTowerEnergyCut
{
	public:

		StjTowerEnergyCutFMS(double min = 0., double max = 500.0) : _min(min), _max(max) {}
		virtual ~StjTowerEnergyCutFMS() {}

		bool operator()(const StjTowerEnergy& deposit)
		{
			double energy = deposit.energy;
			if (energy <= _min) return true;
			if (energy >  _max) return true;

			return false;
		}

	private:

		double _min;
		double _max;

		ClassDef(StjTowerEnergyCutFMS, 1);
};

#endif
