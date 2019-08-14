// $Id: St_TLA_Maker.h,v 1.15 2003/09/10 19:47:43 perev Exp $

#ifndef STAR_StQAnalysisMaker
#define STAR_StQAnalysisMaker

#ifndef StMaker_H
#include "StMaker.h"
#endif
#include "StEEmcUtil/EEmcGeom/EEmcGeomDefs.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"
#include <vector> 
class StEvent;
class TTree;
class TFile;
class StMuDstMaker;
class StMuDst;
class StMuEvent;
class StMcEvent;
class StRunInfo;
class StMuTrack;
class StEventInfo;
class StBemcTables;
class StEmcCollection;

//End-cap clases
class EEmcGeomSimple;
class StEEmcDbMaker;
class StEEmcCollection;
class StEEmcDb;
class EEmcDbItem;

// You may forward declare other classes if you have data-members
// used in pointer-only context by using declaration like
// class St_SomeExternClass;
//
// You do need in such simple case to add the include file
// (and compilation is much faster).

class StQAnalysisMaker : public StMaker {
	private:

		TTree *mTree;
		// StMEvent *muEvent;
		// StMcEvent *mcEvent;
		int evtid;                           //event number from MuDst

	protected:
		// Protected method if any
		StMuDstMaker *mMuDstMaker;


	public: 

		StQAnalysisMaker(const char *name, const char* outputname, StMuDstMaker* QAname, const char* muDSTfilename);
		virtual       ~StQAnalysisMaker();
		virtual Int_t  Init();
		virtual Int_t  Make();
		virtual Int_t  Finish();
		virtual void RunQA();
		virtual void InitFile();
		void Average500GeV();
		void Average200GeV();
		Bool_t isUsableTower(int did);
		Bool_t isUsableTrack(const StMuTrack& track);
		void AddEEmcTowerHit(Int_t sec ,Int_t sub ,Int_t eta, Float_t adc,Int_t layer);

		// virtual Int_t InitRun  (int runumber){return 0;}; // Overload empty StMaker::InitRun 
		// virtual Int_t FinishRun(int runumber){return 0;}; // Overload empty StMaker::FinishRun 
		TTree *qaTree;
		StBemcTables *mTables;
		StEEmcDb *mDbMaker;
		EEmcGeomSimple *mEEgeom;
		TFile *outfile;
		TString myQAfile, myMuDstname;
		char buffername[256];
		Float_t Et, cm_energy;
		Bool_t flagTrack, flagTower; // Checks to ensure tracks and towers are usable
		Double_t etowADC, eTowEnergy;
		Double_t etowPed, etowGain;
		Double_t etowThres;
		Float_t runNumber;
		Float_t fillnumber;
		Float_t bbctimebin;
		Float_t Emc_Et;
		//vertex information
		Float_t vRank, verX, verY, verZ;
		Int_t vertex_z_cut;
		Int_t bemctowerId; 
		Float_t ZdcRate;	

		std::vector<Float_t> VPDMB_TrackPt, VPDMB_TrackEta, VPDMB_TrackPhi, VPDMB_TrackChi2, VPDMB_TrackDcaD, VPDMB_TrackDcaX, VPDMB_TrackDcaY, VPDMB_TrackDcaZ, VPDMB_TrackCharge, VPDMB_TowerEt, VPDMB_TowerEta, VPDMB_TowerPhi, VPDMB_NumEvents;
                Float_t VPDMB_VertexX, VPDMB_VertexY, VPDMB_VertexZ, VPDMB_TrackMulti, VPDMB_TowerMulti;
		std::vector<Float_t> JP0_TrackPt, JP0_TrackEta, JP0_TrackPhi, JP0_TrackChi2, JP0_TrackDcaD, JP0_TrackDcaX, JP0_TrackDcaY, JP0_TrackDcaZ, JP0_TrackCharge, JP0_TowerEt, JP0_TowerEta, JP0_TowerPhi, JP0_NumEvents;
                Float_t JP0_VertexX, JP0_VertexY, JP0_VertexZ, JP0_TrackMulti, JP0_TowerMulti;
		std::vector<Float_t> JP0DiJet_TrackPt, JP0DiJet_TrackEta, JP0DiJet_TrackPhi, JP0DiJet_TrackChi2, JP0DiJet_TrackDcaD, JP0DiJet_TrackDcaX, JP0DiJet_TrackDcaY, JP0DiJet_TrackDcaZ, JP0DiJet_TrackCharge, JP0DiJet_TowerEt, JP0DiJet_TowerEta, JP0DiJet_TowerPhi, JP0DiJet_NumEvents;
                Float_t JP0DiJet_VertexX, JP0DiJet_VertexY, JP0DiJet_VertexZ, JP0DiJet_TrackMulti, JP0DiJet_TowerMulti;
		std::vector<Float_t> JP1_TrackPt, JP1_TrackEta, JP1_TrackPhi, JP1_TrackChi2, JP1_TrackDcaD, JP1_TrackDcaX, JP1_TrackDcaY, JP1_TrackDcaZ, JP1_TrackCharge, JP1_TowerEt, JP1_TowerEta, JP1_TowerPhi, JP1_NumEvents;
                Float_t JP1_VertexX, JP1_VertexY, JP1_VertexZ, JP1_TrackMulti, JP1_TowerMulti;
		std::vector<Float_t> JP1DiJet_TrackPt, JP1DiJet_TrackEta, JP1DiJet_TrackPhi, JP1DiJet_TrackChi2, JP1DiJet_TrackDcaD, JP1DiJet_TrackDcaX, JP1DiJet_TrackDcaY, JP1DiJet_TrackDcaZ, JP1DiJet_TrackCharge, JP1DiJet_TowerEt, JP1DiJet_TowerEta, JP1DiJet_TowerPhi, JP1DiJet_NumEvents;
                Float_t JP1DiJet_VertexX, JP1DiJet_VertexY, JP1DiJet_VertexZ, JP1DiJet_TrackMulti, JP1DiJet_TowerMulti;
		std::vector<Float_t> JP2_TrackPt, JP2_TrackEta, JP2_TrackPhi, JP2_TrackChi2, JP2_TrackDcaD, JP2_TrackDcaX, JP2_TrackDcaY, JP2_TrackDcaZ, JP2_TrackCharge, JP2_TowerEt, JP2_TowerEta, JP2_TowerPhi, JP2_NumEvents;
                Float_t JP2_VertexX, JP2_VertexY, JP2_VertexZ, JP2_TrackMulti, JP2_TowerMulti;
		std::vector<Float_t> BAJP_TrackPt, BAJP_TrackEta, BAJP_TrackPhi, BAJP_TrackChi2, BAJP_TrackDcaD, BAJP_TrackDcaX, BAJP_TrackDcaY, BAJP_TrackDcaZ, BAJP_TrackCharge, BAJP_TowerEt, BAJP_TowerEta, BAJP_TowerPhi, BAJP_NumEvents;
                Float_t BAJP_VertexX, BAJP_VertexY, BAJP_VertexZ, BAJP_TrackMulti, BAJP_TowerMulti;

		//500GeV 2009 Triggers
		struct triggerInfo
		{
			//Information Stored in qaTree
			Float_t meanpT[2], meanMaxpT[2], meanTrackmulti[2], meanTrackChi2[2], meanTrackEta[2], meanTrackPhi[2];
			Float_t meanEt, meanMaxEt, meanTowermulti, meanTowerEta, meanTowerPhi;
			Float_t meanVerX[2], meanVerY[2], meanVerZ[2], meanbbctimebin, numEvents;
			Float_t meanEmcEt, meanEmcMaxEt, meanEmcTowermulti; //emc tower information
			//Information needed but not stored in qaTree
			Int_t trigger,  numVertices[2];
			Float_t sumpT[2], numtracks[2], maxpT[2], sumMaxpT[2], sumChi2[2], sumEta[2], sumPhi[2]; //track information 
			Float_t sumVerX[2],sumVerY[2],sumVerZ[2];
			Float_t sumbbctimebin;
			Float_t sumEt, numtowers, maxEt, sumMaxEt, sumTowerEta, sumTowerPhi; //tower information
			Float_t sumEmcEt, numEmcTowers, maxEmcEt, sumEmcMaxEt; //emc tower information
		} ;


		// 2013 Triggers                                                                     
		triggerInfo trig430005, trig430015, //BBCMB                             
			    trig430001, trig430011, trig430021, trig430031, //VPDMB                               
			    trig430401, trig430411, trig430421, trig430431, //JP0                         
			    trig430407, trig430417, trig430427, trig430437, //JP0*DiJet
			    trig430402, trig430412, trig430422, //JP1                                          
			    trig430406, trig430416, trig430426, trig430436, //JP1*DiJet                                      
			    trig430403, trig430413, //JP2                                                                        
			    trig430424, trig430444; // BAJP   


		void AverageAll(triggerInfo &trigStruct);
		void InitStruct(triggerInfo &trigStruct, int nver); 
		//void FindSumNumTower(triggerInfo &trigStruct, Float_t Et);
		void FindSumNumTower(triggerInfo &trigStruct, Float_t Et, Float_t towerEta, Float_t towerPhi);
		void FindSumNumEmcTower(triggerInfo &trigStruct, Float_t Emc_Et);
		void FindSumNumTrack(triggerInfo &trigStruct, int nver, Float_t pT, Float_t Chi2, Float_t Eta, Float_t Phi);
		//void FindSumNumTrack(triggerInfo &trigStruct, int nver, Float_t Chi2);
		void FindSumNumVertex(triggerInfo &trigStruct, int nver, Float_t verX,Float_t verY, Float_t verZ);
		inline  void FindSumMaxpT(triggerInfo &trigStruct, int nver){if(trigStruct.trigger ==1){trigStruct.sumMaxpT[nver] += trigStruct.maxpT[nver];}}
		inline  void FindSumMaxEt(triggerInfo &trigStruct){if(trigStruct.trigger == 1){trigStruct.sumMaxEt += trigStruct.maxEt;}}
		inline  void FindSumEmcMaxEt(triggerInfo &trigStruct){if(trigStruct.trigger == 1){trigStruct.sumEmcMaxEt += trigStruct.maxEmcEt;}}
		inline void SetBBCTrigInfo(triggerInfo &trigStruct, Float_t bbctimebin){if(trigStruct.trigger == 1){trigStruct.sumbbctimebin += bbctimebin; }}
		inline void SetNumEvents(triggerInfo &trigStruct){if(trigStruct.trigger == 1){trigStruct.numEvents++;}} //inline function
		/// Displayed on session exit, leave it as-is please ...
		//virtual const char *GetCVS() const {
			//static const char cvs[]="Tag $Name:  $ $Id: St_TLA_Maker.h,v 1.15 2003/09/10 19:47:43 perev Exp $ built "__DATE__" "__TIME__ ; 
			//return cvs;
		//}

		ClassDef(StQAnalysisMaker,0)   //StAF chain virtual base class for Makers
};

#endif


