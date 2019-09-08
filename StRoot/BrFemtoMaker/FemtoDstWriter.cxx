
#include "BranchWriter.h"
#include "FemtoDstWriter.h"

#include "StThreeVectorF.hh"

#include "StEvent.h"
#include "StTrack.h"
#include "StGlobalTrack.h"
#include "StTrackGeometry.h"
#include "StDcaGeometry.h"
#include "StMtdPidTraits.h"
#include "StBTofPidTraits.h"
#include "StPhysicalHelixD.hh"
#include "StThreeVectorD.hh"
#include "StEvent/StTrackNode.h"
#include "StEvent/StGlobalTrack.h"
#include "StEvent/StRunInfo.h"
#include "StEvent/StEventInfo.h"
#include "StEvent/StPrimaryVertex.h"
#include "StEvent/StBTofHeader.h"
#include "StEvent/StTriggerData.h"


#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMtdHit.h"
#include "StMuDSTMaker/COMMON/StMuMtdPidTraits.h"
#include "StMuDSTMaker/COMMON/StMuBTofHit.h"


#include "StarClassLibrary/StParticleDefinition.hh"

// STL
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

ClassImp(FemtoDstWriter)


FemtoDstWriter::FemtoDstWriter( const Char_t *name ) : StMaker( name )
{
	this->_outputFilename = "FemtoDst.root";

	SetDebug(1);
}

FemtoDstWriter::~FemtoDstWriter()
{

}

Int_t FemtoDstWriter::Init()
{

	LOG_INFO << "INITIALIZE" <<endm;
	this->_rootFile = new TFile( this->_outputFilename.c_str(), "RECREATE" );
	this->_rootFile->cd();
	this->_tree = new TTree( "FemtoDst", "FemtoDst with MC info" );

	// Create Event Branch
	_few.createBranch( this->_tree, "Event" );
	_ftw.createBranch( this->_tree, "Tracks" );
	// _fmtdw.createBranch( this->_tree, "MtdPidTraits" );
	_fbtofw.createBranch( this->_tree, "BTofPidTraits" );
	// this->_fhw.createBranch( this->_tree, "Helices" );


	h_event_stats = new TH1F( "event_stats", "", 10, 0, 10 );
	h_vz = new TH1F( "vz", ";TPC_vz (cm)", 500, -250, 250 );
	h_vr = new TH1F( "vr", ";TPC_vr (cm)", 500, 0, 5 );
	h_delta_vz = new TH1F( "delta_vz", ";VPD_vz - TPC_vz (cm)", 400, -50, 50 );
	h_delta_vz_wide = new TH1F( "delta_vz_wide", ";VPD_vz - TPC_vz (cm)", 500, -500, 500 );
	h_delta_vz_w_pos_ranking = new TH1F( "delta_vz_w_pos_ranking", ";VPD_vz - TPC_vz (cm)", 200, -50, 50 );
	h_ranking = new TH1F( "ranking", ";ranking", 20, -5, 5 );

	h_GRefMult = new TH1F( "gRefMult", ";gRefMult", 1000, 0, 1000 );
	h_RefMult = new TH1F( "RefMult", ";RefMult", 1000, 0, 1000 );
	h_GRefMult_RefMult = new TH2F( "gRefMult_vs_RefMult", ";RefMult;gRefMult", 500, 0, 1000, 500, 0, 1000 );
	h_vz_RefMult = new TH2F( "vz_vs_RefMult", ";RefMult; vZ", 500, 0, 1000, 400, -200, 200 );
	h_vz_GRefMult = new TH2F( "vz_vs_GRefMult", ";GRefMult; vZ", 500, 0, 1000, 400, -200, 200 );
	h_vpd_vz_RefMult = new TH2F( "vpd_vz_vs_RefMult", ";RefMult;TPC_vz - VPD_vz", 500, 0, 1000, 400, -50, 50 );
	h_vpd_vz_GRefMult = new TH2F( "vpd_vz_vs_GRefMult", ";GRefMult;TPC_vz - VPD_vz", 500, 0, 1000, 400, -50, 50 );

	h_delta_vz_RefMult = new TH2F( "delta_vz_vs_RefMult", ";RefMult;TPC_vz - VPD_vz", 500, 0, 1000, 400, -50, 50 );
	h_delta_vz_GRefMult = new TH2F( "delta_vz_vs_GRefMult", ";GRefMult;TPC_vz - VPD_vz", 500, 0, 1000, 400, -50, 50 );

	h_n_verts_GRefMult = new TH2F( "n_verts_GRefMult", ";GRefMult;nVertices", 500, 0, 1000, 20, 0, 20 );

	h_nBTofMatched = new TH1F( "n_btof_matched", ";nBTofMatched", 50, 0, 50 );
	h_nBTofHits = new TH1F( "n_btof_hits", ";nBTofHits", 100, 0, 100 );
	h_nVpdHits = new TH1F( "n_vpd_hits", "nVpdHits", 50, 0, 50 );

	h_nBTofHits_RefMult = new TH2F( "nBTofHits_vs_RefMult", ";RefMult;nBTofHits", 500, 0, 1000, 100, 0, 100 );
	h_nBTofMatched_RefMult = new TH2F( "nBTofMatched_vs_RefMult", ";RefMult;nBTofMatched", 500, 0, 1000, 50, 0, 50 );

	return kStOK;
}

Int_t FemtoDstWriter::Finish()
{
	LOG_INFO << "FINISH" <<endm;
	this->_rootFile->Write();
	this->_rootFile->Close();
	return kStOK;
}

Int_t FemtoDstWriter::Make()
{
	LOG_DEBUG << "FemtoDstWriter::Make()" << endm;
	
	// Check that the DS are present
	StMuDstMaker *muDstMaker = (StMuDstMaker*) GetMaker( "MuDst" );
	if (nullptr == this->_StMuEvent) {
		LOG_INFO << "No StMuDstMaker Found, cannot proceed" << endm;
		return kStWarn;
	}
	this->_StMuDst = muDstMaker->muDst();
	if (nullptr == this->_StMuDst) {
		LOG_INFO << "No StMuDst Found, cannot proceed" << endm;
		return kStWarn;
	}

	this->_StMuEvent = this->_StMuDst->event();
	if (nullptr == this->_StMuEvent) {
		LOG_INFO << "No StMuEvent Found, cannot proceed" << endm;
		return kStWarn;
	}


	h_event_stats->Fill(0);
	

	// Run 11 Au+Au 200 GeV UPC_Main Triggers
	bool passTrigger = this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 350007 ); 
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 350017 );

	// Run 10 Au+Au 200 GeV UPC_Main Trigger
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260750 );
	
	// Run12 U+U 193 GeV Upc_Main_Protected triggers
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 400601 );
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 400611 );
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 400621 );
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 400631 );
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 400641 );

	// Run 14 Au+Au 200 GeV UPC_Main Trigger (450701, 450711)
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 450701 );
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 450711 );


	// Run 10 Au+Au 200 GeV Minimum Bias
	bool passMinBiasTrigger = false;
	passMinBiasTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260001 );
	passMinBiasTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260011 );
	passMinBiasTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260021 );
	passMinBiasTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260031 );

	bool passZDCMonTrigger = false;
	passZDCMonTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260002 );
	// if ( passZDCMonTrigger ) { cout << "ZDCMon: " << 260002 << endl; }
	passZDCMonTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260012 );
	// if ( passZDCMonTrigger ) { cout << "ZDCMon: " << 260012 << endl; }
	passZDCMonTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260022 );
	// if ( passZDCMonTrigger ) { cout << "ZDCMon: " << 260022 << endl; }



	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260001 );
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260011 );
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260021 );
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260031 );

	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260002 );
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260012 );
	passTrigger |= this->_StMuEvent->triggerIdCollection().nominal().isTrigger( 260022 );

	


	if ( false == passTrigger ){
		// LOG_INFO << "REJECT TRIGGER" << endm;
		// LOG_INFO << "=============" << endm;
		// for ( size_t i = 0; i < this->_StMuEvent->triggerIdCollection().nominal().triggerIds().size(); i++ ){
		// 	LOG_INFO  << "trigger : " << this->_StMuEvent->triggerIdCollection().nominal().triggerIds()[i] << endm;
		// }
		return kStOK; //comment out for MC 
	}

	// if (false == passZDCMonTrigger){
	// 	return kStOK;
	// }
	

	LOG_INFO << "PASS TRIGGER" << endm;

	

	h_event_stats->Fill(1);


	// Fill Event Info
	this->_fmtEvent.mRunId    = this->_StMuEvent->runId();
	this->_fmtEvent.mEventId  = this->_StMuEvent->eventId();
	this->_fmtEvent.mGRefMult = this->_StMuEvent->grefmult();
	this->_fmtEvent.mRefMult  = this->_StMuEvent->refMult();
	this->_fmtEvent.mNVerts   = this->_StMuDst->primaryVertices()->GetEntries();

	this->_fmtEvent.mZDCEast    = this->_StMuEvent->triggerData()->zdcUnAttenuated(east);
	this->_fmtEvent.mZDCWest    = this->_StMuEvent->triggerData()->zdcUnAttenuated(west);
	this->_fmtEvent.mBBCEast    = this->_StMuEvent->triggerData()->bbcADCSum(east);
	this->_fmtEvent.mBBCWest    = this->_StMuEvent->triggerData()->bbcADCSum(west);
	this->_fmtEvent.mBBCLEast   = this->_StMuEvent->triggerData()->bbcADCSumLargeTile(east);
	this->_fmtEvent.mBBCLWest   = this->_StMuEvent->triggerData()->bbcADCSumLargeTile(west);

	this->_fmtEvent.mTofMultTrg = this->_StMuEvent->triggerData()->tofMultiplicity();

	for(Int_t j=0; j<8; j++){
		   // = td->dsmTF201Ch(j);
		this->_fmtEvent.mLastDsmBit[j] = this->_StMuEvent->triggerData()->lastDSM(j);
	}


	/******************************************/
	/* VERTEX CHOICE */
	/******************************************/
	StMuPrimaryVertex *pVertex = chooseVertex(); // choose best vertex based on tof matches
	
	//StMuPrimaryVertex *pVertex = this->_StMuDst->primaryVertex(); // choose default
	if ( nullptr == pVertex ){
		LOG_INFO << "No Primary Vertex Found, skipping event" << endm;
		return kStWarn;
	}

	// if ( passZDCMonTrigger) {
	// 	// require low ref mult to save space and time
	// 	if ( this->_StMuEvent->refMult() > 20 )
	// 		return kStOK;
	// }

	h_event_stats->Fill(2);

	this->_pvPosition = pVertex->position();
	this->_fmtEvent.vertex( this->_pvPosition.x(), this->_pvPosition.y(), this->_pvPosition.z() );
	this->_fmtEvent.mRanking = pVertex->ranking();

	float vr = sqrt( pow(this->_pvPosition.x(), 2) + pow(this->_pvPosition.y(), 2) );
	h_vr->Fill( vr );
	h_vz->Fill( this->_pvPosition.z()  );

	
	// LOG_INFO << "vpdVz = " << this->_StMuDst->btofHeader() << endm;
	// LOG_INFO << "vpdVz = " << (nullptr == this->_StMuDst->btofHeader() ) << endm;

	if ( nullptr != this->_StMuDst->btofHeader ){
		this->_fmtEvent.mVpdVz = this->_StMuDst->btofHeader()->vpdVz();
		LOG_DEBUG << "vpdVz = " << this->_StMuDst->btofHeader()->vpdVz() << endm;
	} else {
		this->_fmtEvent.mVpdVz = -399;
	}
	


	// event cuts
	if ( fabs( this->_fmtEvent.mPrimaryVertex_mX3 ) > 100 ){
		return kStOK;
	}

	h_event_stats->Fill(3);


	float deltaVz = this->_fmtEvent.mVpdVz - this->_pvPosition.z();
	if ( this->_fmtEvent.mVpdVz < -398 )
		deltaVz = -399;
	
	h_delta_vz->Fill( deltaVz );
	h_delta_vz_wide->Fill( deltaVz );
	if (this->_StMuDst->primaryVertex()->ranking() >= 0)
		h_delta_vz_w_pos_ranking->Fill( deltaVz );


	// if ( fabs( this->_fmtEvent.mPrimaryVertex_mX3 - this->_fmtEvent.mWeight ) >= 6 ){
	// 	return kStOK;
	// }

	


	h_ranking->Fill( this->_StMuDst->primaryVertex()->ranking() / 1e6 );
	// if ( this->_StMuDst->primaryVertex()->ranking() < 0 ){
	// 	return kStOK;
	// }
	

	// count the number of VPD hits:
	this->_fmtEvent.mNBTofHits = 0;
	 int nMax = this->_StMuDst->numberOfBTofHit();
			
	this->_fmtEvent.mNVpdEast = 0;
	this->_fmtEvent.mNVpdWest = 0;
	for( int i=0; i < nMax; i++ ) {
		StMuBTofHit *aHit = (StMuBTofHit *)this->_StMuDst->btofHit(i);
		int trayId = aHit->tray();
		int index = (trayId - 121)* 19 + aHit->cell();
		if( 122 == trayId && aHit->tot() >= 10 && aHit->tot() < 40 )//East
	      this->_fmtEvent.mNVpdEast++;
	    else if( 121 == trayId && aHit->tot() >= 10 && aHit->tot() < 40 ) // West
	      this->_fmtEvent.mNVpdWest++;
	  	else if ( trayId < 121 ){
	  		this->_fmtEvent.mNBTofHits++;
	  	}
	}


	h_GRefMult->Fill( this->_fmtEvent.mGRefMult );
	h_RefMult->Fill( this->_fmtEvent.mRefMult );
	h_vz_RefMult->Fill( this->_fmtEvent.mRefMult, this->_fmtEvent.mPrimaryVertex_mX3 );
	h_vz_GRefMult->Fill( this->_fmtEvent.mGRefMult, this->_fmtEvent.mPrimaryVertex_mX3 );

	h_delta_vz_RefMult->Fill( this->_fmtEvent.mRefMult, deltaVz );
	h_delta_vz_GRefMult->Fill( this->_fmtEvent.mGRefMult, deltaVz ); 

	h_vpd_vz_RefMult->Fill( this->_fmtEvent.mRefMult, this->_fmtEvent.mVpdVz );
	h_vpd_vz_GRefMult->Fill( this->_fmtEvent.mGRefMult, this->_fmtEvent.mVpdVz );

	h_n_verts_GRefMult->Fill( this->_fmtEvent.mGRefMult, this->_fmtEvent.mNVerts );

	// RESET containers
	this->_ftw.reset();
	this->_fhw.reset();
	this->_fmtdw.reset();
	this->_fbtofw.reset();

	size_t nPrimary = this->_StMuDst->numberOfPrimaryTracks();
	this->_fmtEvent.mNTracks = nPrimary;
	this->_fmtEvent.mNBTofMatched = 0;

	// Histograms
	h_GRefMult_RefMult->Fill( this->_fmtEvent.mGRefMult, this->_fmtEvent.mRefMult );
	h_nBTofHits->Fill( this->_fmtEvent.mNBTofHits );
	h_nVpdHits->Fill( this->_fmtEvent.mNVpdEast + this->_fmtEvent.mNVpdWest );
	h_nBTofHits_RefMult->Fill( this->_fmtEvent.mRefMult, this->_fmtEvent.mNBTofHits );

	// if ( this->_fmtEvent.mNTracks != 2 )
	// 	return kStOK;

	for (int iNode = 0; iNode < nPrimary; iNode++ ){
		
		StMuTrack*	tPrimary 	= (StMuTrack*)this->_StMuDst->primaryTracks(iNode);
		if ( !tPrimary ) continue;

		StMuTrack*	tGlobal 	= (StMuTrack*)tPrimary->globalTrack();

		// analyzeTrack( iNode, nPrimaryGood );
		
		this->_fmtTrack.reset();
		// addMtdPidTraits( tPrimary );
		addBTofPidTraits( tPrimary );
		// addTrackHelix( track );
		addTrack( tPrimary );
	}

	_few.set( this->_fmtEvent );


	h_nBTofMatched->Fill( this->_fmtEvent.mNBTofMatched );
	
	h_nBTofMatched_RefMult->Fill( this->_fmtEvent.mRefMult, this->_fmtEvent.mNBTofMatched );

	
	this->_tree->Fill();
	h_event_stats->Fill(4);

	// if ( this->_fmtEvent.mGRefMult <= 4 )
	// 	writeJSONRecord();
	
	

	return kStOK;

}


void FemtoDstWriter::writeJSONRecord(){

	std::string record;
	record = jsonEvent( this->_StMuEvent );
	record += jsonTrackHeader();

	size_t nPrimary = this->_StMuDst->numberOfPrimaryTracks();
	Int_t n_glob = this->_StMuDst->GetNGlobalTrack();
	for ( int i = 0; i < nPrimary; i++ ) {
		// StMuTrack *track = this->_StMuDst->globalTracks(i);
		StMuTrack *track = (StMuTrack*)this->_StMuDst->primaryTracks(i)->globalTrack();
		if ( i == 0 )
			record += jsonTrack( track, false );
		else 
			record += jsonTrack( track );

	}

	record += jsonTrackFooter();

	std::stringstream sstr;
	sstr << "EVENT_" << this->_StMuEvent->runId() << "_" << this->_StMuEvent->eventId() << ".json";
	ofstream outf( sstr.str().c_str() );
	outf << record;
	outf.close();
}

std::string FemtoDstWriter::jsonEvent( StMuEvent * event ){
	std::stringstream sstr;

	sstr << "{\n"
	<< "\t\"EVENT\": { \n"

	<< "\t\"runid\": " << event->runNumber() << ",\n"
	<< "\t\"evtid\": " << event->eventId() << ",\n"
	<< "\t\"time\": " << event->eventInfo().time() << ",\n"
	<< "\t\"type\": \"" << event->eventInfo().type() << "\",\n"
	<< "\t\"s_nn\": " << event->runInfo().centerOfMassEnergy() << ",\n"
	<< "\t\"part_yellow\": \"" << "au" << "\",\n"
	<< "\t\"part_blue\": \"" << "au" << "\",\n"
	<< "\t\"e_yellow\": " << "200" << ",\n"
	<< "\t\"e_blue\": " << "200" << ",\n"
	<< "\t\"pv\": [" 	<< event->primaryVertexPosition().x() << ","
	<< event->primaryVertexPosition().y() << ","
	<< event->primaryVertexPosition().z() << "]\n"
	<< "},\n\n";
	return sstr.str();
}

std::string FemtoDstWriter::jsonTrackHeader( ){
	return  "\"TRACKS\": { \n"
			"\t\"tracks\": [\n";
}

std::string FemtoDstWriter::jsonTrackFooter( ){
	return  "] }\n}\n";
}


std::string FemtoDstWriter::jsonTrack( StMuTrack * track, bool prependComma ){

	StPhysicalHelixD helix = track->helix();
	float length = track->lengthMeasured();

	int MAX_POINTS = 0;	
    StThreeVector<double> xyz;
	double nsig = 0;
    int hypothesis = 0;

	MAX_POINTS = 10*length*helix.curvature() + 1;		
	nsig = 1000;
	hypothesis = 0;
	if (fabs(track->nSigmaElectron()) < nsig) {
		nsig = fabs(track->nSigmaElectron());
		hypothesis = 1;
	}
	if (fabs(track->nSigmaPion()) < nsig) {
		nsig = track->nSigmaPion();
		hypothesis = 2;
	}
	if (fabs(track->nSigmaProton()) < nsig) {
		nsig = track->nSigmaProton();
		hypothesis = 3;
	}
	if (fabs(track->nSigmaKaon()) < nsig) {
		nsig = track->nSigmaKaon();
		hypothesis = 4;
	}
	
	float pt = length / float(MAX_POINTS);

	std::stringstream sstr;
	if ( prependComma )
		sstr << " ,";
	sstr << "{ "; 
		sstr << "\t\"pt\": " << TMath::Floor(track->pt() * 1000 +  0.5) / 1000 << ",";
		sstr << "\t\"t\": " << hypothesis << ", ";
		sstr << "\t\"e\": " << TMath::Floor(track->eta() * 1000 +  0.5) / 1000 << ",";
		sstr << "\t\"p\": " << TMath::Floor(track->phi() * 1000 +  0.5) / 1000 << ",";
		
		if (track->primaryTrack()) {
			sstr << "\t\"pr\": " << 1 << ",";
		} else {
			sstr << "\t\"pr\": " << 0 << ",";
		}


		sstr << "\t\"c\": " << track->charge() << ",";
		
		sstr << "\t\"pts\": [";
			for (int j = -10; j <= MAX_POINTS * 2; j++) {
				xyz = helix.at(j*pt);
				if (xyz.perp() < 5.0 || xyz.perp() > 205.0) break; // point is outside of TPC range
				if ( j != -10 ) {
					sstr << ", [" << int(xyz.x()) << "," << int(xyz.y()) << "," << int(xyz.z()) << "]";
				} else {
					sstr << "[" << int(xyz.x()) << "," << int(xyz.y()) << "," << int(xyz.z()) << "]";
				}
			}
		sstr << "\t]\n";
		sstr << "}\n";

	return sstr.str();

}

void FemtoDstWriter::addTrack( StMuTrack *muTrack )
{

	if ( nullptr == muTrack ){
		LOG_INFO << "WARN, null StMuTrack" << endm;
		return;
	}
	// LOG_INFO << "nHitsPoss = " << globalTrack->numberOfPossiblePoints() << " == " << muTrack->nHitsPoss() << endm << endl;
	// LOG_INFO << "nHitsFit = " << globalTrack->fitTraits().numberOfFitPoints() << " == " << muTrack->nHitsFit() << endm << endl;
	// LOG_INFO << "dEdx = " << muTrack->dEdx() << endm;

	this->_fmtTrack.mQ         = muTrack->charge();
	this->_fmtTrack.mNHitsMax  = muTrack->nHitsPoss();
	this->_fmtTrack.mNHitsFit  = muTrack->nHitsFit();
	this->_fmtTrack.mNHitsDedx = muTrack->nHitsDedx();

	this->_fmtTrack.mDedx           = ( muTrack->dEdx() * 1.e6 );
	this->_fmtTrack.mNSigmaElectron = muTrack->nSigmaElectron();
	this->_fmtTrack.mNSigmaPion     = muTrack->nSigmaPion();
	this->_fmtTrack.mNSigmaKaon     = muTrack->nSigmaKaon();
	this->_fmtTrack.mNSigmaProton   = muTrack->nSigmaProton();

	StThreeVectorF p = muTrack->p();
	this->_fmtTrack.mPt  = p.perp();
	this->_fmtTrack.mEta = p.pseudoRapidity();
	this->_fmtTrack.mPhi = p.phi();

	this->_fmtTrack.mId = muTrack->id() - 1; // id starts at 1
	
	this->_fmtTrack.mDCA = ( muTrack->dcaGlobal().mag() );

	this->_ftw.add( this->_fmtTrack );
}


double FemtoDstWriter::calculateDCA(StGlobalTrack *globalTrack, StThreeVectorF vtxPos) const
{
	if( nullptr == globalTrack ) 
		return 999;
	
	StDcaGeometry	*dcaGeom = globalTrack->dcaGeometry();
	
	if( nullptr == dcaGeom ) 
		return 999;
	
	StPhysicalHelixD dcaHelix = dcaGeom->helix();
	return dcaHelix.distance(vtxPos,kFALSE);
}

void FemtoDstWriter::addMtdPidTraits( StMuTrack *muTrack )
{
	const StMuTrack *global = muTrack->globalTrack();
	StMuMtdPidTraits mtdPid = global->mtdPidTraits();
	const StMuMtdHit *hit = global->mtdHit();

	if ( nullptr == hit ){
		return;
	}

	this->_fmtMtdPid.reset();
	this->_fmtMtdPid.mDeltaY            = mtdPid.deltaY();
	this->_fmtMtdPid.mDeltaZ            = mtdPid.deltaZ();
	this->_fmtMtdPid.mMatchFlag         = mtdPid.matchFlag();
	this->_fmtMtdPid.mDeltaTimeOfFlight = mtdPid.timeOfFlight() - mtdPid.expTimeOfFlight();
	this->_fmtMtdPid.mMtdHitChan        = (hit->backleg() - 1) * 60 + (hit->module() - 1) * 12 + hit->cell();
	this->_fmtMtdPid.mIdTruth           = hit->idTruth() - 1;	// minus one to index at 0

	this->_fmtTrack.mMtdPidTraitsIndex = this->_fmtdw.N();

	this->_fmtdw.add( this->_fmtMtdPid );
}


void FemtoDstWriter::addBTofPidTraits( StMuTrack *muTrack )
{

	const StMuTrack *track = muTrack->primaryTrack( );
	if ( nullptr == track ) 
		return;
	StMuBTofPidTraits btofPid = track->btofPidTraits();

	this->_fmtBTofPid.reset();
	this->_fmtBTofPid.mBTofYLocal    = btofPid.yLocal();
	this->_fmtBTofPid.mBTofZLocal    = btofPid.zLocal();
	this->_fmtBTofPid.mBTofMatchFlag = btofPid.matchFlag();
	this->_fmtBTofPid.mBTof          = btofPid.timeOfFlight();
	this->_fmtBTofPid.mLength        = btofPid.pathLength();
	if ( this->_fmtBTofPid.mBTofMatchFlag > 0 )
		this->_fmtEvent.mNBTofMatched++;
	// this->_fmtBTofPid.mIdTruth       =  btofPid.tofHit()->idTruth() - 1; // minus 1 to index at 0
	
	double b = btofPid.beta();
	if ( b < 0 )
		b = 0;
	this->_fmtBTofPid.mBTofBeta = b;


	this->_fmtTrack.mBTofPidTraitsIndex = this->_fbtofw.N();

	this->_fbtofw.add( this->_fmtBTofPid );

}


StMuPrimaryVertex* FemtoDstWriter::chooseVertex() {


	// Loop over all the vertices
	// count the number of BTof Matched tracks
	// top choice is first vertex with 2 tracks both matched to BTOF
	// then look for first vertex with 2 track and 1 BTof Match
	// then settle for first vertex with 2 tracks
	// if not found return NULL

	int iCurrentVertex = this->_StMuDst->currentVertexIndex();
	int nVertices = this->_StMuDst->primaryVertices()->GetEntriesFast();
	vector<int> vnPrimary;
	vector<int> vnMatched;

	int iDoubleMatch = -1;
	int iSingleMatch = -1;
	int iNoMatch = -1;

	for ( Int_t iVert = 0; iVert < nVertices; iVert++ ){
		StMuPrimaryVertex *pVert = this->_StMuDst->primaryVertex( iVert );
		if ( nullptr == pVert ) continue;
		StThreeVectorD pVPos = pVert->position();
		this->_StMuDst->setVertexIndex( iVert );
		size_t nPrimary = this->_StMuDst->numberOfPrimaryTracks();

		// LOG_INFO << " Vertex[ " << iVert << " ].z = " << pVPos.z() << " has " << nPrimary << " primary tracks" << endm;


		if ( fabs( pVPos.z() ) > 100 ) {
			vnPrimary.push_back(0);
			vnMatched.push_back(0);
			continue;
		}

		
		
		this->_fmtEvent.mNTracks = nPrimary;
		this->_fmtEvent.mNBTofMatched = 0;

		int nBTofMatch = 0;
		for (int iNode = 0; iNode < nPrimary; iNode++ ){
			
			StMuTrack*	tPrimary 	= (StMuTrack*)this->_StMuDst->primaryTracks(iNode);
			if ( !tPrimary ) continue;

			StMuBTofPidTraits btofPid = tPrimary->btofPidTraits();
			if ( btofPid.matchFlag() > 0 )
				nBTofMatch ++;
		}

		vnPrimary.push_back(nPrimary);
		vnMatched.push_back(nBTofMatch);

		if ( iDoubleMatch < 0 && nPrimary >= 2 && nBTofMatch >= 2 )
			iDoubleMatch = iVert;
		if ( iSingleMatch < 0 && nPrimary >= 2 && nBTofMatch == 1 )
			iSingleMatch = iVert;
		if ( iNoMatch < 0 && nPrimary == 2 && nBTofMatch == 0 )
			iNoMatch = iVert;

	}

	LOG_INFO << "iDoubleMatch=" << iDoubleMatch << ", iSingleMatch=" << iSingleMatch << ", iNoMatch=" << iNoMatch << endm;
	int nPossibleVerts = 0;
	if ( iDoubleMatch >= 0) nPossibleVerts++;
	if ( iSingleMatch >= 0) nPossibleVerts++;
	if ( iNoMatch >= 0) nPossibleVerts++;
	LOG_INFO << "nPossibleVerts=" << nPossibleVerts << endm;


	if ( iDoubleMatch >= 0 ){
		this->_StMuDst->setVertexIndex( iDoubleMatch );
		return this->_StMuDst->primaryVertex( iDoubleMatch );
	}
	if ( iSingleMatch >= 0 ){
		this->_StMuDst->setVertexIndex( iSingleMatch );
		return this->_StMuDst->primaryVertex( iSingleMatch );
	}
	if ( iNoMatch >= 0 ){
		this->_StMuDst->setVertexIndex( iNoMatch );
		return this->_StMuDst->primaryVertex( iNoMatch );
	}

	this->_StMuDst->setVertexIndex( iCurrentVertex );
	return this->_StMuDst->primaryVertex( iCurrentVertex );
}
