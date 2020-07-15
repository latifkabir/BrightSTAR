#ifndef STJFMSMUDST_H
#define STJFMSMUDST_H

#include "StjFMS.h"

class StFmsCollection;
class StMuDstMaker;
class StFmsDbMaker;

class StjFMSMuDst : public StjFMS
{
	public:

		StjFMSMuDst();
		virtual ~StjFMSMuDst() {}

		void Init();
		void setVertex(float vx, float vy, float vz)
		{
			_setVertex = true;
			_vx = vx;
			_vy = vy;
			_vz = vz;
		}
		void useFmsHit() { _useFmsHit = true; }
		void setMinEnergy(double minEnergy) { _minEnergy = minEnergy; }

		StFmsCollection* findFmsCollection();
		StjTowerEnergyList getEnergyList();

	private:

		StFmsDbMaker* mFmsDbMaker;
		StFmsCollection* mFmsColl;

		bool _setVertex;
		bool _useFmsHit;
		double _vx;
		double _vy;
		double _vz;
		double _minEnergy;
};

#endif // STJFMSMUDST_H
