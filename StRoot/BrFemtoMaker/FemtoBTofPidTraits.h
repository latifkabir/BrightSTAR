#ifndef FEMTO_BTOF_PID_TRAITS_H
#define FEMTO_BTOF_PID_TRAITS_H

#include "TObject.h"
#include "FemtoDstBranch.h"

class FemtoBTofPidTraits : public FemtoDstBranch
{
public:
	virtual const char* classname() const { return "FemtoBTofPidTraits"; }
	FemtoBTofPidTraits(){ reset(); }
	~FemtoBTofPidTraits(){ }

	void reset(){
		this->mBTofBeta      = 0.0;
		this->mBTof          = 0.0;
		this->mLength        = 0.0;
		this->mBTofYLocal    = 0.0;
		this->mBTofZLocal    = 0.0;
		this->mBTofMatchFlag = 0.0;
		this->mIdTruth       = -1;
	}
	void copy( FemtoBTofPidTraits * that){
		this->mBTofBeta      = that->mBTofBeta;
		this->mBTof          = that->mBTof;
		this->mLength        = that->mLength;
		this->mBTofYLocal    = that->mBTofYLocal;
		this->mBTofZLocal    = that->mBTofZLocal;
		this->mBTofMatchFlag = that->mBTofMatchFlag;
		this->mIdTruth       = that->mIdTruth;
	}

	Float_t mBTofBeta;			// beta
	Float_t mBTof;				// tof
	Float_t mLength;
	Float_t  mBTofYLocal;		// ylocal 
	Float_t  mBTofZLocal;		// zlocal
	UChar_t  mBTofMatchFlag;	// 0 - no match, 1 - one-to-one, 2 - one-to-multiple
	Short_t  mIdTruth;			// the idTruth used to id which MCTrack caused this info, only used in MC
	
	ClassDef( FemtoBTofPidTraits, 2 )
};

#endif