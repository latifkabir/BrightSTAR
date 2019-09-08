#ifndef FEMTO_DST_BRANCH_H
#define FEMTO_DST_BRANCH_H

#include "TObject.h"

class FemtoDstBranch : public TObject
{
public:
	// virtual const char* branchName() const = 0;
	virtual const char* classname() const = 0;

	virtual ~FemtoDstBranch() {};

	ClassDef( FemtoDstBranch, 1 )
};

#endif