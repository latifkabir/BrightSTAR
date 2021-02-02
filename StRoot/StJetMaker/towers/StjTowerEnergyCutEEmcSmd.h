//Written by C.Kim, UC Riverside (ckim@bnl.gov)
//Copied all format from StjTowerEnergyCutEt

#ifndef STJTOWERENERGYCUTEEMCSMD_H
#define STJTOWERENERGYCUTEEMCSMD_H

#include "StjTowerEnergyCut.h"

class StjTowerEnergyCutEEmcSmd : public StjTowerEnergyCut
{
	public:

		StjTowerEnergyCutEEmcSmd(double min = 0., double max = 500.0) : _min(min), _max(max) {}
		virtual ~StjTowerEnergyCutEEmcSmd() {}

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

		ClassDef(StjTowerEnergyCutEEmcSmd, 1);
};

#endif
