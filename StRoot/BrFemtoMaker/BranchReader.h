/*
* @Author: Daniel Brandenburg
* @Date:   2017-03-29 17:53:01
* @Last Modified by:   Daniel
* @Last Modified time: 2017-04-04 10:48:51
*
* MIT License
* 
* Copyright (c) 2017 Daniel Brandenburg
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* 
*/
#ifndef BRANCH_READER_H
#define BRANCH_READER_H

#include <memory>
#include "TTree.h"
#include "TBranch.h"

template<class BranchType>
class BranchReader
{
public:
	BranchReader() {}
	~BranchReader() {}

	void setup( TTree* tree, const char* branchName )
	{
		if ( nullptr == tree ) return;
		_branch = tree->GetBranch( branchName );
		if ( nullptr == _branch ) return;
		tree->SetBranchAddress( branchName, &(this->_object) );
	}

	BranchType* get() { return _object; }

protected:
	TBranch *_branch    = nullptr;
	BranchType* _object = nullptr;
};

#endif