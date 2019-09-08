/*
* @Author: Daniel
* @Date:   2017-06-22 16:03:12
* @Last Modified by:   Daniel
* @Last Modified time: 2017-06-23 11:48:56
*/

#ifndef FEMTO_MC_VERTEX_H 
#define FEMTO_MC_VERTEX_H 
 
#include "TObject.h" 
#include "TVector3.h" 
 
#include "FemtoDstBranch.h" 
 
class FemtoMcVertex : public FemtoDstBranch 
{ 
public: 
	virtual const char* classname() const { return "FemtoMcVertex"; } 
	FemtoMcVertex(){ 
		reset(); 
	} 
	~FemtoMcVertex(){ 
	} 

	void reset(){ 
		mId           = 0;
		mX            = -999;
		mY            = -999;
		mZ            = -999;
		mGeantProcess = -999;
		mParentIndex  = -1;
	} 

	void copy( FemtoMcVertex * that ) { 
		this->mId           = that->mId;
		this->mX            = that->mX;
		this->mY            = that->mY;
		this->mZ            = that->mZ;
		this->mGeantProcess = that->mGeantProcess;
		this->mParentIndex  = that->mParentIndex;
	} 

	UShort_t  mId;
	Float_t   mX;       // position x
	Float_t   mY;       // position y
	Float_t   mZ;       // position z

	Short_t   mParentIndex; // 0 indexed parent ID
	Long_t    mGeantProcess;
	

	ClassDef( FemtoMcVertex, 2 ) 
}; 

#endif