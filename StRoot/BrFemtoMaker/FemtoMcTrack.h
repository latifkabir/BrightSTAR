#ifndef FEMTO_MC_TRACK_H 
#define FEMTO_MC_TRACK_H 
 
#include "TObject.h" 
#include "TVector3.h" 
 
#include "FemtoDstBranch.h" 
 
class FemtoMcTrack : public FemtoDstBranch 
{ 
public: 
	virtual const char* classname() const { return "FemtoMcTrack"; } 
	FemtoMcTrack(){ 
		reset(); 
	} 
	~FemtoMcTrack(){ 
	} 

	void reset(){ 
		mPt              = 0;
		mEta             = 0;
		mPhi             = 0;
		mId              = 0;
		mGeantPID        = 0;
		mCharge          = 0;
		mEnergy          = 0;
		mNHits           = 0;

		mParentIndex     = -1;
		mAssociatedIndex = -1;
		mStartVertexIndex = -1;
		mStopVertexIndex = -1;
	} 

	void copy( FemtoMcTrack * that ) { 
		this->mPt        = that->mPt;
		this->mEta       = that->mEta;
		this->mPhi       = that->mPhi;
		this->mId        = that->mId;
		this->mGeantPID  = that->mGeantPID;
		this->mCharge    = that->mCharge;
		this->mEnergy    = that->mEnergy;
		this->mNHits     = that->mNHits;

		mParentIndex     = that->mParentIndex;
		mAssociatedIndex = that->mAssociatedIndex;
		mStartVertexIndex = that->mStartVertexIndex;
		mStopVertexIndex = that->mStopVertexIndex;
	} 

	TVector3 momentum() { TVector3 v; v.SetPtEtaPhi( mPt, mEta, mPhi ); return v; } 
	void momentum( float pt, float eta, float phi ) { 
		this->mPt = pt; 
		this->mEta = eta; 
		this->mPhi = phi; 
	} 



	Float_t   mPt;        // primary track px
	Float_t   mEta;       // primary track py
	Float_t   mPhi;       // primary track pz
	UShort_t  mId;        // track Id 
	UShort_t  mGeantPID; 
	Char_t    mCharge; 
	Float_t   mEnergy; 
	Char_t    mNHits; 

	Short_t   mParentIndex; 
	Short_t   mAssociatedIndex;
	Short_t   mStartVertexIndex; 
	Short_t   mStopVertexIndex; 

	ClassDef( FemtoMcTrack, 2 ) 
}; 

#endif