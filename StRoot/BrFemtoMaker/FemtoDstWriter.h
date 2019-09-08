#ifndef FEMTO_DST_WRITER_H
#define FEMTO_DST_WRITER_H

#include <string>

// StRoot
#include "StMaker.h"
#include "StThreeVectorF.hh"
#include "StThreeVectorD.hh"


// ROOT
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
// Project
#include "FemtoEvent.h"
#include "FemtoTrack.h"
#include "FemtoTrackHelix.h"
#include "FemtoMtdPidTraits.h"
#include "FemtoBTofPidTraits.h"
#include "BranchWriter.h"
#include "TClonesArrayWriter.h"


class StEvent;
class StMuEvent;
class StTrack;
class StTrackNode;
class StGlobalTrack;
class StMuDst;
class StMuTrack;
class StMuPrimaryVertex;

class FemtoDstWriter : public StMaker 
{
public:
	FemtoDstWriter( const Char_t *name = "FemtoDstWriter" );
	~FemtoDstWriter();
	
	Int_t Init();
	Int_t Make();
	Int_t Finish();

	void setOutputFileName( std::string fn){
		this->_outputFilename = fn;
	}

protected:

	std::string _outputFilename;

	void addTrack( StMuTrack * track );
	// void addTrackHelix( StMuTrack * track );
	// void addMcTrack( StMcTrack * mcTrack, StTrack *rcTrack );
	// void addMcVertex( StMcVertex * mcVertex );

	void addMtdPidTraits( StMuTrack * track );
	void addBTofPidTraits( StMuTrack * track );

	StMuPrimaryVertex *chooseVertex();

	// JSON Export
	void writeJSONRecord();
	std::string jsonEvent( StMuEvent * event );
	std::string jsonTrack( StMuTrack * track, bool prependComma = true );
	std::string jsonTrackHeader( );
	std::string jsonTrackFooter( );

	TTree *_tree;
	TFile *_rootFile;

	FemtoEvent         _fmtEvent;
	FemtoTrack         _fmtTrack;
	FemtoTrackHelix    _fmtHelix;
	FemtoMtdPidTraits  _fmtMtdPid;
	FemtoBTofPidTraits _fmtBTofPid;

// The Branch Writers
#ifndef __CINT__	// gets confused by std::shared_ptr<>
	BranchWriter<FemtoEvent> _few;
	TClonesArrayWriter<FemtoTrack> _ftw;
	TClonesArrayWriter<FemtoMtdPidTraits> _fmtdw;
	TClonesArrayWriter<FemtoBTofPidTraits> _fbtofw;
	TClonesArrayWriter<FemtoTrackHelix> _fhw;

	StThreeVectorD     _pvPosition;
#endif


	StEvent            *_StEvent;
	StMuEvent          *_StMuEvent;
	StMuDst            *_StMuDst;

	double calculateDCA(StGlobalTrack *globalTrack, StThreeVectorF vtxPos) const;

	/***************************************************/
	// SHIM, in std starting with c++17
	template <typename T>
	T clamp(const T& n, const T& lower, const T& upper) {
		return std::max(lower, std::min(n, upper));
	}
	/***************************************************/

	TH1F *h_event_stats, *h_delta_vz, *h_delta_vz_wide,*h_delta_vz_w_pos_ranking, *h_ranking, *h_vz, *h_vr;
	
	TH1F *h_GRefMult, *h_RefMult, *h_nBTofMatched, *h_nBTofHits, *h_nVpdHits;
	TH2F *h_GRefMult_RefMult, *h_vz_RefMult, *h_vz_GRefMult, *h_delta_vz_RefMult, *h_delta_vz_GRefMult, *h_vpd_vz_RefMult, *h_vpd_vz_GRefMult, *h_n_verts_GRefMult, *h_nBTofHits_RefMult, *h_nBTofMatched_RefMult;

	bool MonteCarlo = false;

	ClassDef(FemtoDstWriter, 1)
};


#endif
