/*
* @Author: Daniel
* @Date:   2017-03-29 17:53:01
* @Last Modified by:   Daniel
* @Last Modified time: 2017-04-04 10:57:08
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
#ifndef FEMTO_TCLONES_ARRAY_READER_H
#define FEMTO_TCLONES_ARRAY_READER_H

#include "TTree.h"
#include "TClonesArray.h"

template <class BranchType>
class TClonesArrayReader
{
public:
	TClonesArrayReader() {}
	~TClonesArrayReader() {}


	/* Setup the TClonesArray reader
	 *
	 * @tree         The TTree instance
	 * @branchName   The name of the branch to attach to
	 *
	 * On success the underlying _tca and _branch will be non-null
	 */
	void setup( TTree *tree, const char* branchName )
	{
		if ( nullptr == tree )
			return;
		// get the branch from the tree
		_branch = tree->GetBranch( branchName );
		if ( nullptr == _branch )
			return;
		// Create the TCA
		this->_tca = new TClonesArray( BranchType().classname() );
		// Set the address against the ttree
		tree->SetBranchAddress( branchName, &(this->_tca) );
		// turn off autodelete (should be default anyways) - reuse memory instead of new object each time
		this->_branch->SetAutoDelete( kFALSE );
	}

	/* Get an instance of the underlying type
	 * @index  the index of the object in the TClonesArray
	 *
	 * @return   BranchType * - a single object raw pointer, nullptr if error
	 */
	BranchType *get( UInt_t index )
	{
		if ( nullptr == this->_tca )
			return nullptr;
		return (BranchType*)this->_tca->At( index );
	}


	/* Get the Number of items in the TClonesArray
	 *
	 * @return   N - the number of entries in _tca
	 */
	UInt_t N()
	{
		if ( nullptr == this->_tca )
			return 0;
		return this->_tca->GetEntries();
	}

protected:
	TBranch *_branch   = nullptr;
	TClonesArray *_tca = nullptr;
};


#endif
