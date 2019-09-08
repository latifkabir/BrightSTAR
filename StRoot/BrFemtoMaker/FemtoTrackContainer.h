#ifndef FEMTO_TRACK_CONTAINER_H
#define FEMTO_TRACK_CONTAINER_H

#include "FemtoTrack.h"
#include "FemtoMcTrack.h"
#include "FemtoMtdPidTraits.h"
#include "FemtoBTofPidTraits.h"
#include "FemtoTrackHelix.h"
#include "TClonesArrayReader.h"
#include "FemtoTrackProxy.h"

#ifndef __CINT__
#include <memory>


class FemtoTrackContainer {
public:
	shared_ptr<FemtoTrack>         _track=nullptr;
	shared_ptr<FemtoTrackHelix>    _helix=nullptr;
	shared_ptr<FemtoMcTrack>       _mcTrack=nullptr;
	shared_ptr<FemtoMtdPidTraits>  _mtdPid=nullptr;
	shared_ptr<FemtoBTofPidTraits> _btofPid=nullptr;
	FemtoTrackProxy _proxy;

	size_t _runId = 0;
	size_t _eventId = 0;

	void setMtdPidTraits( TClonesArrayReader<FemtoMtdPidTraits> &_rMtd ){
		if ( nullptr != this->_track && this->_track->mMtdPidTraitsIndex >= 0 && _rMtd.get( this->_track->mMtdPidTraitsIndex ) ){
			this->_mtdPid = shared_ptr<FemtoMtdPidTraits>( new FemtoMtdPidTraits() );
			this->_mtdPid->copy( _rMtd.get( this->_track->mMtdPidTraitsIndex ) );
		}
		else 
			this->_mtdPid = nullptr;
	}

	void setBTofPidTraits( TClonesArrayReader<FemtoBTofPidTraits> &_rBTof ){
		if ( nullptr != this->_track && this->_track->mBTofPidTraitsIndex >= 0 && _rBTof.get( this->_track->mBTofPidTraitsIndex ) ){
			this->_btofPid = shared_ptr<FemtoBTofPidTraits>( new FemtoBTofPidTraits() );
			this->_btofPid->copy( _rBTof.get( this->_track->mBTofPidTraitsIndex ) );
		}
		else 
			this->_btofPid = nullptr;
	}

	void setHelix( TClonesArrayReader<FemtoTrackHelix> &_rHelices ){
		if ( nullptr != this->_track && this->_track->mHelixIndex >= 0 && _rHelices.get( this->_track->mHelixIndex ) ){
			this->_helix = shared_ptr<FemtoTrackHelix>( new FemtoTrackHelix() );
			this->_helix->copy( _rHelices.get( this->_track->mHelixIndex ) );
		}
		else 
			this->_helix = nullptr;
	}



	void assemble( uint i,
					TClonesArrayReader<FemtoTrack> &_rTracks,
					TClonesArrayReader<FemtoMtdPidTraits> &_rMtd ){

		this->_mcTrack = nullptr;

		if ( _rTracks.get( i ) ){
			this->_track = shared_ptr<FemtoTrack>( new FemtoTrack() );
			this->_track->copy( _rTracks.get( i ) );
			setMtdPidTraits( _rMtd );
		}
		else 
			this->_track = nullptr;

		_proxy._mcTrack = nullptr;
		_proxy._helix = nullptr;
		_proxy._track = this->_track.get();
		_proxy._mtdPid = this->_mtdPid.get();
	}

	void assemble( uint i,
					TClonesArrayReader<FemtoTrack> &_rTracks,
					TClonesArrayReader<FemtoTrackHelix> &_rHelices,
					TClonesArrayReader<FemtoMtdPidTraits> &_rMtd ){

		this->_mcTrack = nullptr;

		if ( _rTracks.get( i ) ){
			this->_track = shared_ptr<FemtoTrack>( new FemtoTrack() );
			this->_track->copy( _rTracks.get( i ) );
			setMtdPidTraits( _rMtd );
			setHelix( _rHelices );
		}
		else 
			this->_track = nullptr;

		_proxy._mcTrack = nullptr;
		_proxy._track = this->_track.get();
		_proxy._helix = this->_helix.get();
		_proxy._mtdPid = this->_mtdPid.get();
	}

	void assemble( uint i,
					TClonesArrayReader<FemtoTrack> &_rTracks,
					TClonesArrayReader<FemtoTrackHelix> &_rHelices,
					TClonesArrayReader<FemtoMtdPidTraits> &_rMtd,
					TClonesArrayReader<FemtoBTofPidTraits> &_rBTof ){

		this->_mcTrack = nullptr;

		if ( _rTracks.get( i ) ){
			this->_track = shared_ptr<FemtoTrack>( new FemtoTrack() );
			this->_track->copy( _rTracks.get( i ) );
			setMtdPidTraits( _rMtd );
			setBTofPidTraits( _rBTof );
			setHelix( _rHelices );
		}
		else 
			this->_track = nullptr;

		_proxy._mcTrack = nullptr;
		_proxy._track = this->_track.get();
		_proxy._helix = this->_helix.get();
		_proxy._mtdPid = this->_mtdPid.get();
		_proxy._btofPid = this->_btofPid.get();
	}

	void assemble( uint i, 
					TClonesArrayReader<FemtoMcTrack> &_rMc, 
					TClonesArrayReader<FemtoTrack> &_rTracks, 
					TClonesArrayReader<FemtoMtdPidTraits> &_rMtd ){

		if ( nullptr == _rMc.get( i ) ) return;
		this->_mcTrack = shared_ptr<FemtoMcTrack>( new FemtoMcTrack(  ) );
		this->_mcTrack->copy( _rMc.get( i ) );
		
		if ( this->_mcTrack->mAssociatedIndex >= 0 )
			assemble( this->_mcTrack->mAssociatedIndex, _rTracks, _rMtd );
		else { 
			this->_track = nullptr;
			this->_mtdPid = nullptr;
		}

		// dangerous, thanks obama
		_proxy._mcTrack = this->_mcTrack.get();
		_proxy._track = this->_track.get();
		_proxy._mtdPid = this->_mtdPid.get();
	}

};

#endif

#endif
