/*
* @Author: Daniel
* @Date:   2017-04-04 10:23:43
* @Last Modified by:   Daniel
* @Last Modified time: 2017-06-01 11:32:33
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
#ifndef FEMTO_TCLONES_ARRAY_WRITER_H
#define FEMTO_TCLONES_ARRAY_WRITER_H

#include "TTree.h"
#include "TClonesArray.h"

/* NOTE
 * Works with any class branchType, as long as it implements:
 * const char* classname() const { return "classname"; }
 */

template<class BranchType>
class TClonesArrayWriter
{
public:
	TClonesArrayWriter() {}
	~TClonesArrayWriter() {}

	void createBranch( TTree *tree, const char* name, int buffSize = 256000, int splitLevel = 99){
		_tca = new TClonesArray( BranchType().classname() );
		tree->Branch( name, &this->_tca, buffSize, splitLevel );
	}

	void add( BranchType &track ){
		if ( nullptr == this->_tca ) return;
		BranchType *new_track = new ((*this->_tca)[this->_n]) BranchType( );
		new_track->copy( &track );
		this->_n++;
	}

	void add( BranchType *track ){
		if ( nullptr == this->_tca || nullptr == track) return;
		BranchType *new_track = new ((*this->_tca)[this->_n]) BranchType( );
		new_track->copy( track );
		this->_n++;
	}

	void reset(){
		this->_n = 0;
		if( nullptr != this->_tca ) 
			this->_tca->Clear();
	}

	UInt_t N() const { return _n; }
	BranchType *at( UInt_t i ){
		if ( nullptr == _tca )
			return nullptr;
		return (BranchType*)_tca->At( i );
	}

protected:
	TClonesArray        * _tca = nullptr;
	UInt_t                _n   = 0;
};

#endif