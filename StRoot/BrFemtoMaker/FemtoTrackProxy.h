#ifndef FEMTO_TRACK_PROXY_H
#define FEMTO_TRACK_PROXY_H

#include "FemtoTrack.h"
#include "FemtoMcTrack.h"
#include "FemtoTrackHelix.h"
#include "FemtoMtdPidTraits.h"
#include "FemtoBTofPidTraits.h"
#include "TClonesArrayReader.h"



class FemtoTrackProxy {
public:
	FemtoTrack         * _track   = nullptr;
	FemtoTrackHelix    * _helix   = nullptr;
	FemtoMcTrack       * _mcTrack = nullptr;
	FemtoMtdPidTraits  * _mtdPid  = nullptr;
	FemtoBTofPidTraits * _btofPid  = nullptr;

	// extras
	bool _fMcTrack = false;
	bool _fTrack   = false;
	bool _fMtdPid  = false;
	float _mass    = 0.0;	// mass assumption

	void setMtdPidTraits( TClonesArrayReader<FemtoMtdPidTraits> &_rMtd ){
		if ( nullptr != this->_track && this->_track->mMtdPidTraitsIndex >= 0 )
			this->_mtdPid = _rMtd.get( this->_track->mMtdPidTraitsIndex );
		else 
			this->_mtdPid = nullptr;
	}
	void setBTofPidTraits( TClonesArrayReader<FemtoBTofPidTraits> &_rBTof ){
		if ( nullptr != this->_track && this->_track->mBTofPidTraitsIndex >= 0 )
			this->_btofPid = _rBTof.get( this->_track->mBTofPidTraitsIndex );
		else 
			this->_btofPid = nullptr;
	}

	void setHelix( TClonesArrayReader<FemtoTrackHelix> &_rHelices ){
		if ( nullptr != this->_track && this->_track->mHelixIndex >= 0 )
			this->_helix = _rHelices.get( this->_track->mHelixIndex );
		else 
			this->_helix = nullptr;
	}

	void assemble( uint i,
					TClonesArrayReader<FemtoTrack> &_rTracks,
					TClonesArrayReader<FemtoMtdPidTraits> &_rMtd ) {
		this->_mcTrack = nullptr;
		this->_track = _rTracks.get( i );
		setMtdPidTraits( _rMtd );

	}

	void assemble( uint i,
					TClonesArrayReader<FemtoTrack> &_rTracks,
					TClonesArrayReader<FemtoTrackHelix> &_rHelices,
					TClonesArrayReader<FemtoMtdPidTraits> &_rMtd ) {
		
		this->_mcTrack = nullptr;
		this->_track = _rTracks.get( i );
		setHelix( _rHelices );
		setMtdPidTraits( _rMtd );

	}

	void assemble( uint i,
					TClonesArrayReader<FemtoTrack> &_rTracks,
					TClonesArrayReader<FemtoTrackHelix> &_rHelices,
					TClonesArrayReader<FemtoMtdPidTraits> &_rMtd, 
					TClonesArrayReader<FemtoMcTrack> &_rMc ) {
		

		
		this->_track = _rTracks.get( i );
		this->_mcTrack   = nullptr;
		if ( this->_track->mMcIndex >= 0 )
			this->_mcTrack   = _rMc.get( this->_track->mMcIndex );
		setHelix( _rHelices );
		setMtdPidTraits( _rMtd );

	}

	void assemble( uint i,
					TClonesArrayReader<FemtoTrack> &_rTracks,
					TClonesArrayReader<FemtoTrackHelix> &_rHelices,
					TClonesArrayReader<FemtoMtdPidTraits> &_rMtd, 
					TClonesArrayReader<FemtoBTofPidTraits> &_rBTof ) {
		
		this->_mcTrack = nullptr;
		this->_track = _rTracks.get( i );
		setHelix( _rHelices );
		setMtdPidTraits( _rMtd );
		setBTofPidTraits( _rBTof );

	}

	void assemble( uint i,
					TClonesArrayReader<FemtoTrack> &_rTracks,
					TClonesArrayReader<FemtoTrackHelix> &_rHelices,
					TClonesArrayReader<FemtoBTofPidTraits> &_rBTof ) {
		
		this->_mcTrack = nullptr;
		this->_track = _rTracks.get( i );
		setHelix( _rHelices );
		setBTofPidTraits( _rBTof );

	}

	void assemble( uint i, 
					TClonesArrayReader<FemtoMcTrack> &_rMc, 
					TClonesArrayReader<FemtoTrack> &_rTracks, 
					TClonesArrayReader<FemtoMtdPidTraits> &_rMtd ){

		this->_mcTrack   = _rMc.get( i );
		if ( nullptr == this->_mcTrack ) return;
		
		if ( this->_mcTrack->mAssociatedIndex >= 0 )
			this->_track = _rTracks.get( this->_mcTrack->mAssociatedIndex );
		else 
			this->_track = nullptr;

		setMtdPidTraits( _rMtd );
	}
};

#endif
