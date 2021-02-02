#ifndef STJEEMCSMDMUDST_H
#define STJEEMCSMDMUDST_H

#include "StjEEmcSmd.h"

class StMuDstMaker;
class StEEmcTreeMaker_t;

class StjEEmcSmdMuDst : public StjEEmcSmd
{
public:

    StjEEmcSmdMuDst();
    virtual ~StjEEmcSmdMuDst() {}

    void setVertex(float vx, float vy, float vz)
    {
	_setVertex = true;
	_vx = vx;
	_vy = vy;
	_vz = vz;
    }

    void setMinEnergy(double minEnergy) { _minEnergy = minEnergy; }

    StjTowerEnergyList getEnergyList();

private:

    StEEmcTreeMaker_t* mEEmcTreeMk;
    
    bool _setVertex;
    double _vx;
    double _vy;
    double _vz;
    double _minEnergy;
};

#endif // STJEEMCSMDMUDST_H
