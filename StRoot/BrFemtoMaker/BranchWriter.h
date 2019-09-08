/*
* @Author: Daniel Brandenburg
* @Date:   2017-04-04 10:26:37
* @Last Modified by:   Daniel
* @Last Modified time: 2017-04-04 10:53:45
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
#ifndef BRANCH_WRITER_H
#define BRANCH_WRITER_H

#include <memory>
// #include "FemtoDstFormat/FemtoEvent.h"
#include "TTree.h"

template <class BranchType>
class BranchWriter
{
public:
	BranchWriter() {}
	~BranchWriter() {}

	void createBranch( TTree * tree, const char* name, int buffSize = 256000, int splitLevel = 99 ){
		this->_event = std::shared_ptr<BranchType>(new BranchType());
		tree->Branch( name, this->_event.get(), buffSize, splitLevel );
	}

	void set( BranchType &event ){
		this->_event->copy( &event );
	}
	void set( BranchType *event ){
		this->_event->copy( event );
	}
	

protected:
	std::shared_ptr<BranchType> _event = nullptr;
};
#endif