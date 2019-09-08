#ifndef FEMTO_TRACK_HELIX_H
#define FEMTO_TRACK_HELIX_H

#include "TObject.h"
#include "FemtoDstBranch.h"

class FemtoTrackHelix : public FemtoDstBranch
{
public:
	virtual const char* classname() const { return "FemtoTrackHelix"; }
	FemtoTrackHelix(){ init(); }
	~FemtoTrackHelix(){ }

	virtual void reset(){
		this->mPar[0] = 0;
		this->mPar[1] = 0;
		this->mPar[2] = 0;
		this->mPar[3] = 0;
		this->mPar[4] = 0;
		this->mPar[5] = 0;
		
		this->mMap0 = 0;
		this->mMap1 = 0;

		this->mDCA = 50.0;
	}
	virtual void init(){
		reset();
	}
	void copy( FemtoTrackHelix * that){
		this->mPar[0] = that->mPar[0];
		this->mPar[1] = that->mPar[1];
		this->mPar[2] = that->mPar[2];
		this->mPar[3] = that->mPar[3];
		this->mPar[4] = that->mPar[4];
		this->mPar[5] = that->mPar[5];

		this->mMap0 = that->mMap0;
		this->mMap1 = that->mMap1;

		this->mDCA = that->mDCA;
	}

	Float_t mPar[6];
	UInt_t mMap0;
	UInt_t mMap1;
	Double_t mDCA;
	ClassDef( FemtoTrackHelix, 3 )
};

#endif