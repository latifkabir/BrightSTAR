#include "StQAnalysisMaker.h"
#include "TDataSetIter.h"
#include "StDAQMaker/StDAQReader.h"
#include "StEventTypes.h"
#include "TVector3.h"
//std
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <math.h>

//root
#include "TTree.h"
#include "TFile.h"

//STAR
#include "StMessMgr.h"


//StMuDstMaker
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"
#include "StMuDSTMaker/COMMON/StMuEmcCollection.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEmcUtil.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
//Vertex & Trigger Info
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuTriggerIdCollection.h"

//StEmc
#include "StEmcClusterCollection.h"
#include "StEmcPoint.h"
#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEmcUtil/others/emcDetectorName.h"
#include "StEmcADCtoEMaker/StBemcData.h"
#include "StEmcADCtoEMaker/StEmcADCtoEMaker.h"
#include "StEmcRawMaker/defines.h"
#include "StEmcRawMaker/StBemcRaw.h"
#include "StEmcRawMaker/StBemcTables.h"
#include "StEmcRawMaker/StEmcRawMaker.h"
#include "StEmcRawMaker/defines.h"
#include "StEmcUtil/database/StBemcTables.h" 

#include "StChain.h"
#include "StMcEventMaker/StMcEventMaker.h"
#include "StMcEventTypes.hh"
#include "StMcEvent.hh"
#include "StEvent/StEvent.h"

#include "TDataSetIter.h"
#include "StEmcADCtoEMaker/StEmcADCtoEMaker.h"
//#include "StDAQMaker/StDAQReader.h"
//#include "St_db_Maker/St_db_Maker.h"

#include "StEEmcUtil/database/EEmcDbItem.h"
#include "StEEmcUtil/database/StEEmcDb.h"

#include "St_DataSet.h"
#include "St_DataSetIter.h"
#include "tables/St_g2t_event_Table.h"
#include "tables/St_particle_Table.h"
#include "tables/St_g2t_pythia_Table.h"
#include "tables/St_particle_Table.h"

//#include "StEEmcPool/StEEmcA2EMaker/StEEmcA2EMaker.h"

ClassImp(StQAnalysisMaker)


	StQAnalysisMaker::StQAnalysisMaker(const char *name, const char* outputname, StMuDstMaker* uDstMaker, const char* muDSTfilename):StMaker(name), mTree(0), mMuDstMaker(uDstMaker),mTables(new StBemcTables(1)){
		myQAfile = TString(outputname);
		myMuDstname = TString(muDSTfilename);
		mDbMaker = 0;
		mEEgeom = new EEmcGeomSimple();
		LOG_DEBUG <<"StQAnalysisMaker::StQAnalysisMaker"<<endm;
	}




StQAnalysisMaker::~StQAnalysisMaker()
{
	//
}


//_____________________________________________________________________________
/// Init - is a first method the top level StChain calls to initialize all its makers 
Int_t StQAnalysisMaker::Init()
{
	//  eemcDB= (StEEmcDbMaker*) StMaker::GetChain()->GetMaker("eemcDb");
	// assert(eemcDB);
	StEmcADCtoEMaker *adc2e = (StEmcADCtoEMaker*)StMaker::GetChain()->GetMakerInheritsFrom("StEmcADCtoEMaker");
	mTables =adc2e->getBemcData()->getTables();
	mDbMaker = (StEEmcDb*)this->GetDataSet("StEEmcDb");
	assert(mDbMaker);
	return StMaker::Init();
}
void StQAnalysisMaker::InitFile()
{

	ifstream fileFromlist(myMuDstname);
	//Make sure the file stream is good
	if(!fileFromlist)
	{
		cout << "Failed to open file " << endl;
	}

	fileFromlist >> buffername;
	//cout << buffername << endl;


	TString name(buffername);

	outfile = new TFile(myQAfile, "recreate");
	//Creating a Tree for the Quality Analysis.
	qaTree = new TTree("QAtree","QAtree");

	qaTree->Branch("runNumber",&runNumber,"runNumber/F");
	qaTree->Branch("fillnumber",&fillnumber,"fillnumber/F");
	qaTree->Branch("ZdcRate",&ZdcRate,"ZdcRate/F");
	qaTree->Branch("VPDMB_VertexX",&VPDMB_VertexX,"VPDMB_VertexX");
	qaTree->Branch("VPDMB_VertexY",&VPDMB_VertexY,"VPDMB_VertexY");
	qaTree->Branch("VPDMB_VertexZ",&VPDMB_VertexZ,"VPDMB_VertexZ");
	qaTree->Branch("VPDMB_TrackMulti",&VPDMB_TrackMulti,"VPDMB_TrackMulti/F");
	qaTree->Branch("VPDMB_TrackPt",&VPDMB_TrackPt);
	qaTree->Branch("VPDMB_TrackEta",&VPDMB_TrackEta);
	qaTree->Branch("VPDMB_TrackPhi",&VPDMB_TrackPhi);
	qaTree->Branch("VPDMB_TrackChi2",&VPDMB_TrackChi2);
	qaTree->Branch("VPDMB_TrackDcaD",&VPDMB_TrackDcaD);
	qaTree->Branch("VPDMB_TrackDcaX",&VPDMB_TrackDcaX);
	qaTree->Branch("VPDMB_TrackDcaY",&VPDMB_TrackDcaY);
	qaTree->Branch("VPDMB_TrackDcaZ",&VPDMB_TrackDcaZ);
	qaTree->Branch("VPDMB_TrackCharge",&VPDMB_TrackCharge);
	qaTree->Branch("VPDMB_TowerMulti",&VPDMB_TowerMulti,"VPDMB_TowerMulti/F");
	qaTree->Branch("VPDMB_TowerEt",&VPDMB_TowerEt);
	qaTree->Branch("VPDMB_TowerEta",&VPDMB_TowerEta);
	qaTree->Branch("VPDMB_TowerPhi",&VPDMB_TowerPhi);
	qaTree->Branch("VPDMB_NumEvents",&VPDMB_NumEvents);
	qaTree->Branch("JP0_VertexX",&JP0_VertexX,"JP0_VertexX");
	qaTree->Branch("JP0_VertexY",&JP0_VertexY,"JP0_VertexY");
	qaTree->Branch("JP0_VertexZ",&JP0_VertexZ,"JP0_VertexZ");
	qaTree->Branch("JP0_TrackMulti",&JP0_TrackMulti,"JP0_TrackMulti/F");
	qaTree->Branch("JP0_TrackPt",&JP0_TrackPt);
	qaTree->Branch("JP0_TrackEta",&JP0_TrackEta);
	qaTree->Branch("JP0_TrackPhi",&JP0_TrackPhi);
	qaTree->Branch("JP0_TrackChi2",&JP0_TrackChi2);
	qaTree->Branch("JP0_TrackDcaD",&JP0_TrackDcaD);
	qaTree->Branch("JP0_TrackDcaX",&JP0_TrackDcaX);
	qaTree->Branch("JP0_TrackDcaY",&JP0_TrackDcaY);
	qaTree->Branch("JP0_TrackDcaZ",&JP0_TrackDcaZ);
	qaTree->Branch("JP0_TrackCharge",&JP0_TrackCharge);
	qaTree->Branch("JP0_TowerMulti",&JP0_TowerMulti,"JP0_TowerMulti/F");
	qaTree->Branch("JP0_TowerEt",&JP0_TowerEt);
	qaTree->Branch("JP0_TowerEta",&JP0_TowerEta);
	qaTree->Branch("JP0_TowerPhi",&JP0_TowerPhi);
	qaTree->Branch("JP0_NumEvents",&JP0_NumEvents);
	qaTree->Branch("JP0DiJet_VertexX",&JP0DiJet_VertexX,"JP0DiJet_VertexX");
	qaTree->Branch("JP0DiJet_VertexY",&JP0DiJet_VertexY,"JP0DiJet_VertexY");
	qaTree->Branch("JP0DiJet_VertexZ",&JP0DiJet_VertexZ,"JP0DiJet_VertexZ");
	qaTree->Branch("JP0DiJet_TrackMulti",&JP0DiJet_TrackMulti,"JP0DiJet_TrackMulti/F");
	qaTree->Branch("JP0DiJet_TrackPt",&JP0DiJet_TrackPt);
	qaTree->Branch("JP0DiJet_TrackEta",&JP0DiJet_TrackEta);
	qaTree->Branch("JP0DiJet_TrackPhi",&JP0DiJet_TrackPhi);
	qaTree->Branch("JP0DiJet_TrackChi2",&JP0DiJet_TrackChi2);
	qaTree->Branch("JP0DiJet_TrackDcaD",&JP0DiJet_TrackDcaD);
	qaTree->Branch("JP0DiJet_TrackDcaX",&JP0DiJet_TrackDcaX);
	qaTree->Branch("JP0DiJet_TrackDcaY",&JP0DiJet_TrackDcaY);
	qaTree->Branch("JP0DiJet_TrackDcaZ",&JP0DiJet_TrackDcaZ);
	qaTree->Branch("JP0DiJet_TrackCharge",&JP0DiJet_TrackCharge);
	qaTree->Branch("JP0DiJet_TowerMulti",&JP0DiJet_TowerMulti,"JP0DiJet_TowerMulti/F");
	qaTree->Branch("JP0DiJet_TowerEt",&JP0DiJet_TowerEt);
	qaTree->Branch("JP0DiJet_TowerEta",&JP0DiJet_TowerEta);
	qaTree->Branch("JP0DiJet_TowerPhi",&JP0DiJet_TowerPhi);
	qaTree->Branch("JP0DiJet_NumEvents",&JP0DiJet_NumEvents);
	qaTree->Branch("JP1_VertexX",&JP1_VertexX,"JP1_VertexX");
	qaTree->Branch("JP1_VertexY",&JP1_VertexY,"JP1_VertexY");
	qaTree->Branch("JP1_VertexZ",&JP1_VertexZ,"JP1_VertexZ");
	qaTree->Branch("JP1_TrackMulti",&JP1_TrackMulti,"JP1_TrackMulti/F");
	qaTree->Branch("JP1_TrackPt",&JP1_TrackPt);
	qaTree->Branch("JP1_TrackEta",&JP1_TrackEta);
	qaTree->Branch("JP1_TrackPhi",&JP1_TrackPhi);
	qaTree->Branch("JP1_TrackChi2",&JP1_TrackChi2);
	qaTree->Branch("JP1_TrackDcaD",&JP1_TrackDcaD);
	qaTree->Branch("JP1_TrackDcaX",&JP1_TrackDcaX);
	qaTree->Branch("JP1_TrackDcaY",&JP1_TrackDcaY);
	qaTree->Branch("JP1_TrackDcaZ",&JP1_TrackDcaZ);
	qaTree->Branch("JP1_TrackCharge",&JP1_TrackCharge);
	qaTree->Branch("JP1_TowerMulti",&JP1_TowerMulti,"JP1_TowerMulti/F");
	qaTree->Branch("JP1_TowerEt",&JP1_TowerEt);
	qaTree->Branch("JP1_TowerEta",&JP1_TowerEta);
	qaTree->Branch("JP1_TowerPhi",&JP1_TowerPhi);
	qaTree->Branch("JP1_NumEvents",&JP1_NumEvents);
	qaTree->Branch("JP1DiJet_VertexX",&JP1DiJet_VertexX,"JP1DiJet_VertexX");
	qaTree->Branch("JP1DiJet_VertexY",&JP1DiJet_VertexY,"JP1DiJet_VertexY");
	qaTree->Branch("JP1DiJet_VertexZ",&JP1DiJet_VertexZ,"JP1DiJet_VertexZ");
	qaTree->Branch("JP1DiJet_TrackMulti",&JP1DiJet_TrackMulti,"JP1DiJet_TrackMulti/F");
	qaTree->Branch("JP1DiJet_TrackPt",&JP1DiJet_TrackPt);
	qaTree->Branch("JP1DiJet_TrackEta",&JP1DiJet_TrackEta);
	qaTree->Branch("JP1DiJet_TrackPhi",&JP1DiJet_TrackPhi);
	qaTree->Branch("JP1DiJet_TrackChi2",&JP1DiJet_TrackChi2);
	qaTree->Branch("JP1DiJet_TrackDcaD",&JP1DiJet_TrackDcaD);
	qaTree->Branch("JP1DiJet_TrackDcaX",&JP1DiJet_TrackDcaX);
	qaTree->Branch("JP1DiJet_TrackDcaY",&JP1DiJet_TrackDcaY);
	qaTree->Branch("JP1DiJet_TrackDcaZ",&JP1DiJet_TrackDcaZ);
	qaTree->Branch("JP1DiJet_TrackCharge",&JP1DiJet_TrackCharge);
	qaTree->Branch("JP1DiJet_TowerMulti",&JP1DiJet_TowerMulti,"JP1DiJet_TowerMulti/F");
	qaTree->Branch("JP1DiJet_TowerEt",&JP1DiJet_TowerEt);
	qaTree->Branch("JP1DiJet_TowerEta",&JP1DiJet_TowerEta);
	qaTree->Branch("JP1DiJet_TowerPhi",&JP1DiJet_TowerPhi);
	qaTree->Branch("JP1DiJet_NumEvents",&JP1DiJet_NumEvents);
	qaTree->Branch("JP2_VertexX",&JP2_VertexX,"JP2_VertexX");
	qaTree->Branch("JP2_VertexY",&JP2_VertexY,"JP2_VertexY");
	qaTree->Branch("JP2_VertexZ",&JP2_VertexZ,"JP2_VertexZ");
	qaTree->Branch("JP2_TrackMulti",&JP2_TrackMulti,"JP2_TrackMulti/F");
	qaTree->Branch("JP2_TrackPt",&JP2_TrackPt);
	qaTree->Branch("JP2_TrackEta",&JP2_TrackEta);
	qaTree->Branch("JP2_TrackPhi",&JP2_TrackPhi);
	qaTree->Branch("JP2_TrackChi2",&JP2_TrackChi2);
	qaTree->Branch("JP2_TrackDcaD",&JP2_TrackDcaD);
	qaTree->Branch("JP2_TrackDcaX",&JP2_TrackDcaX);
	qaTree->Branch("JP2_TrackDcaY",&JP2_TrackDcaY);
	qaTree->Branch("JP2_TrackDcaZ",&JP2_TrackDcaZ);
	qaTree->Branch("JP2_TrackCharge",&JP2_TrackCharge);
	qaTree->Branch("JP2_TowerMulti",&JP2_TowerMulti,"JP2_TowerMulti/F");
	qaTree->Branch("JP2_TowerEt",&JP2_TowerEt);
	qaTree->Branch("JP2_TowerEta",&JP2_TowerEta);
	qaTree->Branch("JP2_TowerPhi",&JP2_TowerPhi);
	qaTree->Branch("JP2_NumEvents",&JP2_NumEvents);
	qaTree->Branch("BAJP_VertexX",&BAJP_VertexX,"BAJP_VertexX");
	qaTree->Branch("BAJP_VertexY",&BAJP_VertexY,"BAJP_VertexY");
	qaTree->Branch("BAJP_VertexZ",&BAJP_VertexZ,"BAJP_VertexZ");
	qaTree->Branch("BAJP_TrackMulti",&BAJP_TrackMulti,"BAJP_TrackMulti/F");
	qaTree->Branch("BAJP_TrackPt",&BAJP_TrackPt);
	qaTree->Branch("BAJP_TrackEta",&BAJP_TrackEta);
	qaTree->Branch("BAJP_TrackPhi",&BAJP_TrackPhi);
	qaTree->Branch("BAJP_TrackChi2",&BAJP_TrackChi2);
	qaTree->Branch("BAJP_TrackDcaD",&BAJP_TrackDcaD);
	qaTree->Branch("BAJP_TrackDcaX",&BAJP_TrackDcaX);
	qaTree->Branch("BAJP_TrackDcaY",&BAJP_TrackDcaY);
	qaTree->Branch("BAJP_TrackDcaZ",&BAJP_TrackDcaZ);
	qaTree->Branch("BAJP_TrackCharge",&BAJP_TrackCharge);
	qaTree->Branch("BAJP_TowerMulti",&BAJP_TowerMulti,"BAJP_TowerMulti/F");
	qaTree->Branch("BAJP_TowerEt",&BAJP_TowerEt);
	qaTree->Branch("BAJP_TowerEta",&BAJP_TowerEta);
	qaTree->Branch("BAJP_TowerPhi",&BAJP_TowerPhi);
	qaTree->Branch("BAJP_NumEvents",&BAJP_NumEvents);
}


//_____________________________________________________________________________
/// Make - this method is called in loop for each event
Int_t StQAnalysisMaker::Make()
{
	vertex_z_cut  = 90;

	//2013 Triggers
	trig430001.trigger = trig430011.trigger = trig430021.trigger = trig430031.trigger = 0; // VPDMB
	trig430401.trigger = trig430411.trigger = trig430421.trigger = trig430431.trigger = 0; // JP0
	trig430407.trigger = trig430417.trigger = trig430427.trigger = trig430437.trigger = 0; // JP0*DiJet
	trig430402.trigger = trig430412.trigger = trig430422.trigger = 0; // JP1  
	trig430406.trigger = trig430416.trigger = trig430426.trigger = trig430436.trigger = 0; // JP1*DiJet
	trig430403.trigger = trig430413.trigger = 0; // JP2
	trig430424.trigger = trig430444.trigger = 0; // BAJP

	RunQA();

	return kStOK;
}

void StQAnalysisMaker::RunQA()
{
	StMuDst* muDst = 0;
	if (mMuDstMaker!=0) muDst = mMuDstMaker->muDst();
	StMuEvent* muEvent = muDst->event();
	StRunInfo* runInfo = &(muEvent->runInfo());
	fillnumber = runInfo->beamFillNumber(blue);
	runNumber = muEvent->runNumber();
	ZdcRate = muEvent->runInfo().zdcCoincidenceRate();
	//ZdcRate = muEvent->zdccoincidencerate();
	cm_energy = runInfo->centerOfMassEnergy();
	//cout << "center of mass energy: " << cm_energy << endl;
	//BBC time bin
	StBbcTriggerDetector bbc = muEvent->bbcTriggerDetector();
	bbctimebin = bbc.onlineTimeDifference(); // bbc time bin

	//Trigger information
	StMuTriggerIdCollection trig = muEvent->triggerIdCollection();
	StTriggerId l1trig = trig.nominal();
	//cout << "prescale value for VPDMB: " << l1trig.prescaleVertexsion(240025) <<  endl;

	//VPDMB
	if(l1trig.isTrigger(430001)) trig430001.trigger = 1;
	if(l1trig.isTrigger(430001)) trig430011.trigger = 1;
	if(l1trig.isTrigger(430021)) trig430021.trigger = 1;
	if(l1trig.isTrigger(430031)) trig430031.trigger = 1;
	//JP0       
	if(l1trig.isTrigger(430401)) trig430401.trigger = 1;
	if(l1trig.isTrigger(430411)) trig430411.trigger = 1;
	if(l1trig.isTrigger(430421)) trig430421.trigger = 1;
	if(l1trig.isTrigger(430431)) trig430431.trigger = 1;
	//JP0*DiJet        
	if(l1trig.isTrigger(430407)) trig430407.trigger = 1;
	if(l1trig.isTrigger(430417)) trig430417.trigger = 1;
	if(l1trig.isTrigger(430427)) trig430427.trigger = 1;
	if(l1trig.isTrigger(430437)) trig430437.trigger = 1;
	//JP1
	if(l1trig.isTrigger(430402)) trig430402.trigger = 1;
	if(l1trig.isTrigger(430412)) trig430412.trigger = 1;
	if(l1trig.isTrigger(430422)) trig430422.trigger = 1;
	//JP1*DiJet
	if(l1trig.isTrigger(430406)) trig430406.trigger = 1;
	if(l1trig.isTrigger(430416)) trig430416.trigger = 1;
	if(l1trig.isTrigger(430426)) trig430426.trigger = 1;
	if(l1trig.isTrigger(430436)) trig430436.trigger = 1;
	//JP2
	if(l1trig.isTrigger(430403)) trig430403.trigger = 1;
	if(l1trig.isTrigger(430413)) trig430413.trigger = 1;
	//BAJP
	if(l1trig.isTrigger(430424)) trig430424.trigger = 1;
	if(l1trig.isTrigger(430444)) trig430444.trigger = 1;


	//******************TPC TRACK INFORMATION BEGINS**********************************

	int nVertices = muDst->numberOfPrimaryVertices();

	Int_t nGoodTracks = 0;
	Int_t nGoodTowers=0;

	//Clear Vectors
	VPDMB_TrackPt.clear();
	VPDMB_TrackEta.clear();
	VPDMB_TrackPhi.clear();
	VPDMB_TrackChi2.clear();
	VPDMB_TrackDcaD.clear();
	VPDMB_TrackDcaX.clear();
	VPDMB_TrackDcaY.clear();
	VPDMB_TrackDcaZ.clear();
	VPDMB_TrackCharge.clear();
	VPDMB_TowerEt.clear();
	VPDMB_TowerEta.clear();
	VPDMB_TowerPhi.clear();
	VPDMB_NumEvents.clear();
	JP0_TrackPt.clear();
	JP0_TrackEta.clear();
	JP0_TrackPhi.clear();
	JP0_TrackChi2.clear();
	JP0_TrackDcaD.clear();
	JP0_TrackDcaX.clear();
	JP0_TrackDcaY.clear();
	JP0_TrackDcaZ.clear();
	JP0_TrackCharge.clear();
	JP0_TowerEt.clear();
	JP0_TowerEta.clear();
	JP0_TowerPhi.clear();
	JP0_NumEvents.clear();
	JP0DiJet_TrackPt.clear();
	JP0DiJet_TrackEta.clear();
	JP0DiJet_TrackPhi.clear();
	JP0DiJet_TrackChi2.clear();
	JP0DiJet_TrackDcaD.clear();
	JP0DiJet_TrackDcaX.clear();
	JP0DiJet_TrackDcaY.clear();
	JP0DiJet_TrackDcaZ.clear();
	JP0DiJet_TrackCharge.clear();
	JP0DiJet_TowerEt.clear();
	JP0DiJet_TowerEta.clear();
	JP0DiJet_TowerPhi.clear();
	JP0DiJet_NumEvents.clear();
	JP1_TrackPt.clear();
	JP1_TrackEta.clear();
	JP1_TrackPhi.clear();
	JP1_TrackChi2.clear();
	JP1_TrackDcaD.clear();
	JP1_TrackDcaX.clear();
	JP1_TrackDcaY.clear();
	JP1_TrackDcaZ.clear();
	JP1_TrackCharge.clear();
	JP1_TowerEt.clear();
	JP1_TowerEta.clear();
	JP1_TowerPhi.clear();
	JP1_NumEvents.clear();
	JP1DiJet_TrackPt.clear();
	JP1DiJet_TrackEta.clear();
	JP1DiJet_TrackPhi.clear();
	JP1DiJet_TrackChi2.clear();
	JP1DiJet_TrackDcaD.clear();
	JP1DiJet_TrackDcaX.clear();
	JP1DiJet_TrackDcaY.clear();
	JP1DiJet_TrackDcaZ.clear();
	JP1DiJet_TrackCharge.clear();
	JP1DiJet_TowerEt.clear();
	JP1DiJet_TowerEta.clear();
	JP1DiJet_TowerPhi.clear();
	JP1DiJet_NumEvents.clear();
	JP2_TrackPt.clear();
	JP2_TrackEta.clear();
	JP2_TrackPhi.clear();
	JP2_TrackChi2.clear();
	JP2_TrackDcaD.clear();
	JP2_TrackDcaX.clear();
	JP2_TrackDcaY.clear();
	JP2_TrackDcaZ.clear();
	JP2_TrackCharge.clear();
	JP2_TowerEt.clear();
	JP2_TowerEta.clear();
	JP2_TowerPhi.clear();
	JP2_NumEvents.clear();
	BAJP_TrackPt.clear();
	BAJP_TrackEta.clear();
	BAJP_TrackPhi.clear();
	BAJP_TrackChi2.clear();
	BAJP_TrackDcaD.clear();
	BAJP_TrackDcaX.clear();
	BAJP_TrackDcaY.clear();
	BAJP_TrackDcaZ.clear();
	BAJP_TrackCharge.clear();
	BAJP_TowerEt.clear();
	BAJP_TowerEta.clear();
	BAJP_TowerPhi.clear();
	BAJP_NumEvents.clear();

	for(int j = 0; j < nVertices; j++) //vertex loop
	{
		if(j > 0) continue;
		// Set the vertex
		StMuPrimaryVertex* vertex = muDst->primaryVertex(j);
		assert(vertex);
		muDst->setVertexIndex(j);
		StThreeVectorF r = vertex->position();
		StThreeVectorF er = vertex->posError();
		verX = r.x(); verY =r.y(); verZ = r.z();
		if(vertex->ranking() < 0) continue;
		if(fabs(r.z()) > vertex_z_cut) continue;

		//VPDMB
		if(trig430401.trigger == 1 || trig430411.trigger == 1 || trig430421.trigger == 1 || trig430431.trigger == 1)
		{
			VPDMB_VertexX = verX;
			VPDMB_VertexY = verY;
			VPDMB_VertexZ = verZ;
			VPDMB_NumEvents.push_back(1e3);
		}
		//JP0
		if(trig430401.trigger == 1 || trig430411.trigger == 1 || trig430421.trigger == 1 || trig430431.trigger == 1)
		{
			JP0_VertexX = verX;
			JP0_VertexY = verY;
			JP0_VertexZ = verZ;
			JP0_NumEvents.push_back(1e3);
		}
		//JP0DiJet
		if(trig430407.trigger == 1 || trig430417.trigger == 1 || trig430427.trigger == 1 || trig430437.trigger == 1)
		{
			JP0DiJet_VertexX = verX;
			JP0DiJet_VertexY = verY;
			JP0DiJet_VertexZ = verZ;
			JP0DiJet_NumEvents.push_back(1e3);
		}
		//JP1
		if(trig430402.trigger == 1 || trig430412.trigger == 1 || trig430422.trigger == 1)
		{
			JP1_VertexX = verX;
			JP1_VertexY = verY;
			JP1_VertexZ = verZ;
			JP1_NumEvents.push_back(1e3);
		}
		//JP1DiJet
		if(trig430406.trigger == 1 || trig430416.trigger == 1 || trig430426.trigger == 1 || trig430436.trigger == 1)
		{
			JP1DiJet_VertexX = verX;
			JP1DiJet_VertexY = verY;
			JP1DiJet_VertexZ = verZ;
			JP1DiJet_NumEvents.push_back(1e3);
		}

		//JP2
		if(trig430403.trigger == 1 || trig430413.trigger == 1)
		{
			JP2_VertexX = verX;
			JP2_VertexY = verY;
			JP2_VertexZ = verZ;
			JP2_NumEvents.push_back(1e3);
		}
		//BAJP
		if(trig430424.trigger == 1 || trig430444.trigger == 1)
		{
			BAJP_VertexX = verX;
			BAJP_VertexY = verY;
			BAJP_VertexZ = verZ;
			BAJP_NumEvents.push_back(1e3);
		}

		Int_t nTracks = muDst->GetNPrimaryTrack();

		for(int i = 0; i < nTracks; i++) // track loop
		{
			StMuTrack* track = muDst->primaryTracks(i);
			//const StMuTrack* trackGlob = track->globalTrack();
			assert(track);
			StThreeVectorF momentum = track->momentum();
			flagTrack = isUsableTrack(*track);
			if(flagTrack == false) continue;
			Float_t trackpT = track->pt();
			if(trackpT < 0.20) continue; // 200 Mev threshold

			nGoodTracks++;

			if(trig430001.trigger == 1 || trig430011.trigger == 1 || trig430021.trigger == 1 || trig430031.trigger == 1)
			{
				VPDMB_TrackPt.push_back(track->pt());
				VPDMB_TrackEta.push_back(track->eta());
				VPDMB_TrackPhi.push_back(track->phi());
				VPDMB_TrackChi2.push_back(track->chi2());

				VPDMB_TrackDcaD.push_back(track->dcaD());
				VPDMB_TrackDcaX.push_back(track->dcaGlobal(j).x());
				VPDMB_TrackDcaY.push_back(track->dcaGlobal(j).y());
				VPDMB_TrackDcaZ.push_back(track->dcaZ());
				VPDMB_TrackCharge.push_back(track->charge());
			}
			if(trig430401.trigger == 1 || trig430411.trigger == 1 || trig430421.trigger == 1 || trig430431.trigger == 1)
			{
				JP0_TrackPt.push_back(track->pt());
				JP0_TrackEta.push_back(track->eta());
				JP0_TrackPhi.push_back(track->phi());
				JP0_TrackChi2.push_back(track->chi2());

				JP0_TrackDcaD.push_back(track->dcaD());
				JP0_TrackDcaX.push_back(track->dcaGlobal(j).x());
				JP0_TrackDcaY.push_back(track->dcaGlobal(j).y());
				JP0_TrackDcaZ.push_back(track->dcaZ());
				JP0_TrackCharge.push_back(track->charge());
			}
			if(trig430407.trigger == 1 || trig430417.trigger == 1 || trig430427.trigger == 1 || trig430437.trigger == 1)
			{
				JP0DiJet_TrackPt.push_back(track->pt());
				JP0DiJet_TrackEta.push_back(track->eta());
				JP0DiJet_TrackPhi.push_back(track->phi());
				JP0DiJet_TrackChi2.push_back(track->chi2());

				JP0DiJet_TrackDcaD.push_back(track->dcaD());
				JP0DiJet_TrackDcaX.push_back(track->dcaGlobal(j).x());
				JP0DiJet_TrackDcaY.push_back(track->dcaGlobal(j).y());
				JP0DiJet_TrackDcaZ.push_back(track->dcaZ());
				JP0DiJet_TrackCharge.push_back(track->charge());
			}
			if(trig430402.trigger == 1 || trig430412.trigger == 1 || trig430422.trigger == 1)
			{
				JP1_TrackPt.push_back(track->pt());
				JP1_TrackEta.push_back(track->eta());
				JP1_TrackPhi.push_back(track->phi());
				JP1_TrackChi2.push_back(track->chi2());

				JP1_TrackDcaD.push_back(track->dcaD());
				JP1_TrackDcaX.push_back(track->dcaGlobal(j).x());
				JP1_TrackDcaY.push_back(track->dcaGlobal(j).y());
				JP1_TrackDcaZ.push_back(track->dcaZ());
				JP1_TrackCharge.push_back(track->charge());
			}
			if(trig430406.trigger == 1 || trig430416.trigger == 1 || trig430426.trigger == 1 || trig430436.trigger == 1)
			{
				JP1DiJet_TrackPt.push_back(track->pt());
				JP1DiJet_TrackEta.push_back(track->eta());
				JP1DiJet_TrackPhi.push_back(track->phi());
				JP1DiJet_TrackChi2.push_back(track->chi2());

				JP1DiJet_TrackDcaD.push_back(track->dcaD());
				JP1DiJet_TrackDcaX.push_back(track->dcaGlobal(j).x());
				JP1DiJet_TrackDcaY.push_back(track->dcaGlobal(j).y());
				JP1DiJet_TrackDcaZ.push_back(track->dcaZ());
				JP1DiJet_TrackCharge.push_back(track->charge());
			}
			if(trig430403.trigger == 1 || trig430413.trigger == 1)
			{
				JP2_TrackPt.push_back(track->pt());
				JP2_TrackEta.push_back(track->eta());
				JP2_TrackPhi.push_back(track->phi());
				JP2_TrackChi2.push_back(track->chi2());

				JP2_TrackDcaD.push_back(track->dcaD());
				JP2_TrackDcaX.push_back(track->dcaGlobal(j).x());
				JP2_TrackDcaY.push_back(track->dcaGlobal(j).y());
				JP2_TrackDcaZ.push_back(track->dcaZ());
				JP2_TrackCharge.push_back(track->charge());
			}
			if(trig430424.trigger == 1 || trig430444.trigger == 1)
			{
				BAJP_TrackPt.push_back(track->pt());
				BAJP_TrackEta.push_back(track->eta());
				BAJP_TrackPhi.push_back(track->phi());
				BAJP_TrackChi2.push_back(track->chi2());

				BAJP_TrackDcaD.push_back(track->dcaD());
				BAJP_TrackDcaX.push_back(track->dcaGlobal(j).x());
				BAJP_TrackDcaY.push_back(track->dcaGlobal(j).y());
				BAJP_TrackDcaZ.push_back(track->dcaZ());
				BAJP_TrackCharge.push_back(track->charge());
			}
		}

	}

	//**************EMC TOWER INFORMATION BEGINS***************************
	//$$$ Barrel Information 
	StEvent *mEvent = static_cast<StEvent*>(this->GetDataSet("StEvent"));

	StEmcGeom* geom = StEmcGeom::instance("bemc"); // for towers
	if (!geom) cout<<" No StEmcGeom!"<<endl;
	assert(geom); 
	StEmcCollection *emc = mEvent->emcCollection(); 
	if (!emc) cout<<" No StEmcCollection"<<endl;
	assert(emc);
	StEmcDetector* detector=emc->detector(kBarrelEmcTowerId);
	if (!detector) cout<<" No StEmcDetector!"<<endl;
	//  assert(detector);

	if(detector) 
	{
		for(Int_t m = 1; m <= 120; ++m) 
		{
			StEmcModule* module = detector->module(m);
			if(module) 
			{
				StSPtrVecEmcRawHit& rawHit=module->hits();
				for(UInt_t k = 0; k < rawHit.size(); ++k)
				{
					if(rawHit[k])
					{
						Int_t did;
						Int_t m=rawHit[k]->module();
						Int_t e=rawHit[k]->eta();
						Int_t s=abs(rawHit[k]->sub());
						Int_t adc=rawHit[k]->adc();
						Float_t energy=rawHit[k]->energy();
						Float_t tower_eta, tower_phi;
						geom->getId(m,e,s,did);
						geom->getEtaPhi(did,tower_eta,tower_phi); // to convert software id into eta/phi
						//	    flagTower = isUsableTower(did); 
						//   if(flagTower == false)
						// continue;
						//  int status;
						// mTables->getStatus(BTOW,did,status);
						// cout << "The status of the tower is: " << status << endl;

						Et = (energy/cosh(tower_eta));

						bemctowerId = did;
						if (Et >= 0.2)
						{
							nGoodTowers++;
							if(trig430001.trigger == 1 || trig430401.trigger == 1 || trig430021.trigger == 1 || trig430031.trigger == 1)
							{
								VPDMB_TowerEt.push_back(Et);
								VPDMB_TowerEta.push_back(tower_eta);
								VPDMB_TowerPhi.push_back(tower_phi);
							}
							if(trig430401.trigger == 1 || trig430411.trigger == 1 || trig430421.trigger == 1 || trig430431.trigger == 1)
							{
								JP0_TowerEt.push_back(Et);
								JP0_TowerEta.push_back(tower_eta);
								JP0_TowerPhi.push_back(tower_phi);
							}
							if(trig430407.trigger == 1 || trig430417.trigger == 1 || trig430427.trigger == 1 || trig430437.trigger == 1)
							{
								JP0DiJet_TowerEt.push_back(Et);
								JP0DiJet_TowerEta.push_back(tower_eta);
								JP0DiJet_TowerPhi.push_back(tower_phi);
							}
							if(trig430402.trigger == 1 || trig430412.trigger == 1 || trig430422.trigger == 1)
							{
								JP1_TowerEt.push_back(Et);
								JP1_TowerEta.push_back(tower_eta);
								JP1_TowerPhi.push_back(tower_phi);
							}
							if(trig430406.trigger == 1 || trig430416.trigger == 1 || trig430426.trigger == 1 || trig430436.trigger == 1)
							{
								JP1DiJet_TowerEt.push_back(Et);
								JP1DiJet_TowerEta.push_back(tower_eta);
								JP1DiJet_TowerPhi.push_back(tower_phi);
							}
							if(trig430403.trigger == 1 || trig430413.trigger == 1)
							{
								JP2_TowerEt.push_back(Et);
								JP2_TowerEta.push_back(tower_eta);
								JP2_TowerPhi.push_back(tower_phi);
							}
							if(trig430424.trigger == 1 || trig430444.trigger == 1)
							{
								BAJP_TowerEt.push_back(Et);
								BAJP_TowerEta.push_back(tower_eta);
								BAJP_TowerPhi.push_back(tower_phi);
							}
						}
					}
				}
			}
		}
	}

	if(trig430001.trigger == 1 || trig430011.trigger == 1 || trig430021.trigger == 1 || trig430031.trigger == 1)
	{
		VPDMB_TrackMulti = nGoodTracks;
		VPDMB_TowerMulti = nGoodTowers;
	}
	if(trig430401.trigger == 1 || trig430411.trigger == 1 || trig430421.trigger == 1 || trig430431.trigger == 1)
	{
		JP0_TrackMulti = nGoodTracks;
		JP0_TowerMulti = nGoodTowers;
	}
	if(trig430407.trigger == 1 || trig430417.trigger == 1 || trig430427.trigger == 1 || trig430437.trigger == 1)
	{
		JP0DiJet_TrackMulti = nGoodTracks;
		JP0DiJet_TowerMulti = nGoodTowers;
	}
	if(trig430402.trigger == 1 || trig430412.trigger == 1 || trig430422.trigger == 1)
	{
		JP1_TrackMulti = nGoodTracks;
		JP1_TowerMulti = nGoodTowers;
	}
	if(trig430406.trigger == 1 || trig430416.trigger == 1 || trig430426.trigger == 1 || trig430436.trigger == 1)
	{
		JP1DiJet_TrackMulti = nGoodTracks;
		JP1DiJet_TowerMulti = nGoodTowers;
	}
	if(trig430403.trigger == 1 || trig430413.trigger == 1)
	{
		JP2_TrackMulti = nGoodTracks;
		JP2_TowerMulti = nGoodTowers;
	}
	if(trig430424.trigger == 1 || trig430444.trigger == 1)
	{
		BAJP_TrackMulti = nGoodTracks;
		BAJP_TowerMulti = nGoodTowers;
	}

	qaTree->Fill();
}

Int_t StQAnalysisMaker::Finish()
{  
	outfile->Write();
	outfile->Close();
	delete outfile;
	return kStOk;
}

/*************Begin isUsableTrack***********
  This function will see if the tracks are uable*/

bool StQAnalysisMaker::isUsableTrack(const StMuTrack& track)
{
	if(track.flag() < 0)
		return false;

	if(track.dcaGlobal().mag()> 3.)
		return false;

	int dcaFlag = 1;
	if(true){
		Double_t limit = 3.-2.*track.pt();
		if(!((track.pt()<0.5&&track.dcaGlobal().mag()<=2.)||
					//((track.pt()>=0.5&&track.pt()<1.0)&&track.dcaGlobal().mag()<=limit)||(track.pt()>=1.0&&track.dcaGlobal().mag()<=1.0))) dcaFlag =0;
			((track.pt()>=0.5&&track.pt()<1.5)&&track.dcaGlobal().mag()<=limit)||(track.pt()>=1.0&&track.dcaGlobal().mag()<=1.0))) dcaFlag =0;
	}
	if(dcaFlag == 0)
		return false;
	if (track.topologyMap().trackFtpcEast() || track.topologyMap().trackFtpcWest()) {
		return false;
	}

	if(static_cast<double>(track.nHits())/static_cast<double>(track.nHitsPoss()) < .51)
		return false;

	if(track.nHitsFit() < 12) return false;

	return true;
}

/**************End of isUsableTrack*********/

/************* Beginning of isUsableTower ************/

bool StQAnalysisMaker::isUsableTower(int did)
{
	//now check the status:
	int status;
	mTables->getStatus(BTOW,did,status);

	//check for ADC that is 2-sigma above RMS:
	float pedestal, rms;
	int CAP(0); 
	mTables->getPedestal(BTOW,did,CAP,pedestal,rms);

	if(pedestal > 0 &&(pedestal)> 2.*rms && status==1)
		return true;
	else
		return false;
}

/************* End of isUsableTower *****************/

/***************Begin AddEEmcTowerHit*************/

void StQAnalysisMaker::AddEEmcTowerHit(Int_t sec, Int_t sub, Int_t eta, Float_t adc, Int_t layer)
{
	assert(sec >= 0 && sec <12);
	assert(sub >=0 && sub < 5);
	assert(eta >= 0 && eta <12); 
	Int_t index = 60*sec+12*sub+eta;
	assert(0<=index && index<720);
	static Char_t subsectors[] = {'A','B','C','D','E'};
	static Char_t detectors[] = {'T','P','Q','R' };
	const EEmcDbItem *dbitem = mDbMaker -> getTile(sec+1,subsectors[sub] ,eta+1,detectors[layer]); 
	assert(dbitem);
	if ( dbitem -> fail ) return;
	if ( dbitem -> stat ) return;
	etowADC =  adc;
	etowPed= dbitem->ped;
	etowGain = dbitem->gain;
	etowThres = etowPed + dbitem->sigPed * 3;
	if(etowADC < etowThres) return;
	if(etowGain < 0.0) return;
	eTowEnergy = (etowADC - etowPed +0.5) / etowGain;
	TVector3 Emomentum = mEEgeom -> getTowerCenter(sec,sub,eta);
	Emomentum = Emomentum.Unit();
	Emomentum *= eTowEnergy;
	Emc_Et = (Float_t)Emomentum.Perp();

	//BBCMB
	FindSumNumEmcTower(trig430005,Emc_Et); 
	FindSumNumEmcTower(trig430015,Emc_Et); 
	//VPDMB
	FindSumNumEmcTower(trig430001,Emc_Et); 
	FindSumNumEmcTower(trig430011,Emc_Et); 
	FindSumNumEmcTower(trig430021,Emc_Et); 
	FindSumNumEmcTower(trig430031,Emc_Et); 
	//JP0
	FindSumNumEmcTower(trig430401,Emc_Et); 
	FindSumNumEmcTower(trig430411,Emc_Et); 
	FindSumNumEmcTower(trig430421,Emc_Et); 
	FindSumNumEmcTower(trig430431,Emc_Et); 
	//JP0*DiJet
	FindSumNumEmcTower(trig430407,Emc_Et); 
	FindSumNumEmcTower(trig430417,Emc_Et); 
	FindSumNumEmcTower(trig430427,Emc_Et); 
	FindSumNumEmcTower(trig430437,Emc_Et); 
	//JP1
	FindSumNumEmcTower(trig430402,Emc_Et); 
	FindSumNumEmcTower(trig430412,Emc_Et); 
	FindSumNumEmcTower(trig430422,Emc_Et); 
	//JP1*DiJet
	FindSumNumEmcTower(trig430406,Emc_Et); 
	FindSumNumEmcTower(trig430416,Emc_Et); 
	FindSumNumEmcTower(trig430426,Emc_Et); 
	FindSumNumEmcTower(trig430436,Emc_Et); 
	//JP2
	FindSumNumEmcTower(trig430403,Emc_Et); 
	FindSumNumEmcTower(trig430413,Emc_Et); 
	//BAJP
	FindSumNumEmcTower(trig430424,Emc_Et); 
	FindSumNumEmcTower(trig430444,Emc_Et); 
}

/****************End Of AddEEmcTowerHit ************/

/***********Begin Average All ******/
void StQAnalysisMaker::AverageAll(triggerInfo &trigStruct)
{ 
	for( int nver = 0; nver < 2; nver++)
	{
		//Track Info
		if(trigStruct.numtracks[nver] != 0)
		{
			trigStruct.meanpT[nver] = trigStruct.sumpT[nver]/trigStruct.numtracks[nver];
			trigStruct.meanTrackChi2[nver] = trigStruct.sumChi2[nver]/trigStruct.numtracks[nver];
			trigStruct.meanTrackEta[nver] = trigStruct.sumEta[nver]/trigStruct.numtracks[nver];
			trigStruct.meanTrackPhi[nver] = trigStruct.sumPhi[nver]/trigStruct.numtracks[nver];
		}
		if(trigStruct.numEvents != 0)
		{
			trigStruct.meanTrackmulti[nver] = trigStruct.numtracks[nver]/trigStruct.numEvents;
			trigStruct.meanMaxpT[nver] = trigStruct.sumMaxpT[nver]/trigStruct.numEvents;
		}

		trigStruct.meanVerX[nver] = trigStruct.sumVerX[nver]/trigStruct.numVertices[nver];
		trigStruct.meanVerY[nver] = trigStruct.sumVerY[nver]/trigStruct.numVertices[nver];
		trigStruct.meanVerZ[nver] = trigStruct.sumVerZ[nver]/trigStruct.numVertices[nver]; 

	}
	//Tower Info
	if(trigStruct.numEmcTowers != 0)
	{
		trigStruct.meanEmcEt = trigStruct.sumEmcEt/trigStruct.numEmcTowers;
	}
	if(trigStruct.numEvents != 0)
	{
		trigStruct.meanEmcTowermulti = trigStruct.numEmcTowers/trigStruct.numEvents;
		trigStruct.meanEmcMaxEt = trigStruct.sumEmcMaxEt/trigStruct.numEvents;
	}
	if(trigStruct.numtowers != 0)
	{
		trigStruct.meanEt = trigStruct.sumEt/trigStruct.numtowers;
		trigStruct.meanTowerEta  = trigStruct.sumTowerEta/trigStruct.numtowers;
		trigStruct.meanTowerPhi = trigStruct.sumTowerPhi/trigStruct.numtowers;
	}
	if(trigStruct.numEvents != 0)
	{
		trigStruct.meanTowermulti = trigStruct.numtowers/trigStruct.numEvents;
		trigStruct.meanMaxEt = trigStruct.sumMaxEt/trigStruct.numEvents;
		//BBC Timebin
		trigStruct.meanbbctimebin = trigStruct.sumbbctimebin/trigStruct.numEvents;
	}
	//cout << "In AverageAll(): meanEt:meanTowerMulti:meanMaxEt:meanBBCtimebin: " <<trigStruct.meanEt << " " << trigStruct.meanTowermulti << " " << trigStruct.meanMaxEt << " "<< trigStruct.meanbbctimebin <<endl;
}


/**********************End of Average200GeV() ******************************* */

/*******************Begnning of InitStruct **************************************/

void StQAnalysisMaker::InitStruct(triggerInfo &trigStruct, int nver)
{
	for(int k = 0; k < nver; k++){
		trigStruct.numVertices[k] = trigStruct.sumVerX[k] = trigStruct.sumVerY[k] = trigStruct.sumVerZ[k] = trigStruct.meanVerX[k] = trigStruct.meanVerY[k] = trigStruct.meanVerZ[k] = 0; 
		trigStruct.sumpT[k] = trigStruct.maxpT[k] = trigStruct.meanpT[k] = trigStruct.numtracks[k] = trigStruct.sumMaxpT[k] = trigStruct.meanMaxpT[k] = trigStruct.meanTrackmulti[k] = trigStruct.meanTrackChi2[k] = trigStruct.meanTrackEta[k] = trigStruct.meanTrackPhi[k] = trigStruct.sumChi2[k] = trigStruct.sumEta[k] = trigStruct.sumPhi[k] = 0;
	}
	trigStruct.sumEt = trigStruct.maxEt = trigStruct.meanEt = trigStruct.numtowers = trigStruct.sumMaxEt = trigStruct.meanMaxEt = trigStruct.meanTowermulti = 0;
	trigStruct.sumEmcEt = trigStruct.maxEmcEt = trigStruct.meanEmcEt = trigStruct.numEmcTowers = trigStruct.sumEmcMaxEt = trigStruct.meanEmcMaxEt = trigStruct.meanEmcTowermulti = trigStruct.meanTowerEta = trigStruct.meanTowerPhi = trigStruct.sumTowerEta = trigStruct.sumTowerPhi = 0;

	trigStruct.sumbbctimebin = trigStruct.meanbbctimebin = trigStruct.numEvents = 0;
}

/*******************End of InitStruct **************************************/

/*******************Begin of FindNumSumTower **************************************/

void StQAnalysisMaker::FindSumNumTower(triggerInfo &trigStruct, Float_t Et, Float_t towerEta, Float_t towerPhi)
{
	if(trigStruct.trigger == 1)
	{
		trigStruct.numtowers++; 
		trigStruct.sumEt +=  Et;
		trigStruct.sumTowerEta +=  towerEta;
		trigStruct.sumTowerPhi +=  towerPhi;

		if(trigStruct.maxEt < Et)
		{
			trigStruct.maxEt = Et;
			//  cout << "In FindSumNumTower(): maxEt = "<< trigStruct.maxEt << endl;
		}
	}
	//  cout << "In FindSumNumTower(): NumTowers:SumEt:MaxEt: " <<  trigStruct.numtowers  << " " << trigStruct.sumEt << " " << trigStruct.maxEt << endl;
}
/*******************End of FindNumSumTower **************************************/

/*******************Begin of FindNumSumTower **************************************/

void StQAnalysisMaker::FindSumNumEmcTower(triggerInfo &trigStruct, Float_t Emc_Et)
{
	if(trigStruct.trigger == 1)
	{
		trigStruct.numEmcTowers++; 
		trigStruct.sumEmcEt +=  Emc_Et;
		if(trigStruct.maxEmcEt < Emc_Et)
		{
			trigStruct.maxEmcEt = Emc_Et;
			// cout << "In FindSumNumTower(): maxEt = "<< trigStruct.maxEt << endl;
		}
	}
	// cout << "In FindSumNumEmcTower(): NumTowers:SumEmcEt:MaxEmcEt: " <<  trigStruct.numEmcTowers  << " " << trigStruct.sumEmcEt << " " << trigStruct.maxEmcEt << endl;
}
/*******************End of FindNumSumEmcTower **************************************/

/*******************Begin of FindNumSumTrack **************************************/

void StQAnalysisMaker::FindSumNumTrack(triggerInfo &trigStruct, int nver, Float_t pT, Float_t Chi2, Float_t Eta, Float_t Phi)
{

	if(trigStruct.trigger == 1)
	{
		trigStruct.numtracks[nver]++; 
		trigStruct.sumpT[nver] += pT; 
		trigStruct.sumChi2[nver] += Chi2; 
		trigStruct.sumEta[nver] += Eta; 
		trigStruct.sumPhi[nver] += Phi; 

		if(pT > trigStruct.maxpT[nver])
		{
			trigStruct.maxpT[nver] = pT;
			//cout << "In FindSumNumTrack(): maxpT = "<< trigStruct.maxpT[nver] <<" for vertex =" << nver << endl;
		}
	}
	//cout << "In FindSumNumTrack(): NumTracks:SumpT:MaxpT: " <<  trigStruct.numtracks[nver]  << " " << trigStruct.sumpT[nver] << " " << trigStruct.maxpT[nver] << endl;
}

/*******************End of FindNumSumTrack **************************************/

/*******************Begin of FindNumSumTrack **************************************/

//void StQAnalysisMaker::FindTrackChi2(triggerInfo &trigStruct, int nver, Float_t Chi2)
//{

//if(trigStruct.trigger == 1)
//  {
//trig

/*******************End of FindNumSumTrack **************************************/



/*******************Begin of FindNumSumVertex **************************************/

void StQAnalysisMaker::FindSumNumVertex(triggerInfo &trigStruct, int nver, Float_t verX, Float_t verY, Float_t verZ)
{
	if(trigStruct.trigger == 1)
	{
		trigStruct.sumVerX[nver] += verX;
		trigStruct.sumVerY[nver] += verY;
		trigStruct.sumVerZ[nver] += verZ;
		trigStruct.numVertices[nver]++;
	}
	//cout << "In FindSumNumVertex:  sumVerX:SumVerY:SumVerZ:NumVertex:" <<  trigStruct.sumVerX[nver] << " "  <<trigStruct.sumVerY[nver] << " "<<  trigStruct.sumVerZ[nver]  << " " <<   trigStruct.numVertices[nver]  << endl;
}

/*******************Begin of FindNumSumVertex **************************************/
