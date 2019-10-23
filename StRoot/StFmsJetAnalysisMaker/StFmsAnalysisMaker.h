#ifndef ST_FMS_ANALYSIS_MAKER_H
#define ST_FMS_ANALYSIS_MAKER_H
#include <stdio.h>
#include <stdlib.h>
#include "StSpinPool/StSpinDbMaker/StSpinDbMaker.h"
// ROOT

#include "TF1.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TObjArray.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TLorentzVector.h"

// STAR
#include "StMaker.h"
class StMuEvent;
class StMuTrack;
class StMuDstMaker;
class StBemcTables;
class StEEmcDb;


#include "StJetFinder/StProtoJet.h"
#include "StJetMaker/StJetMaker2012.h"

// just to use St3vector
#include "StJetFinder/AbstractFourVec.h"
#include "StRoot/StFmsDbMaker/StFmsDbMaker.h"

#include "StLorentzVectorF.hh"


class StFmsPoint;
class StFmsEventHeader;
class StEmcEvent;

class StFmsAnalysisMaker : public StMaker {

public:
	StFmsAnalysisMaker( const char* name = "StFmsAnalysisMaker" , StJetMaker2012* jetmaker = 0 );

	~StFmsAnalysisMaker();
	
	void Clear( const char* opt = "" );
	void SetOutputFile( Char_t* filename );
	Int_t Init();
        Int_t Make();
        Int_t Finish();
	
	void SetFmsPhotonEcut( Float_t ecutsmall = 0.0, Float_t ecutlarge = 0.0 ); //no cut for now
	Bool_t Fiducial(const StThreeVectorF photon, Int_t nstb) const; //nstb starts from 0
	Int_t EdepCor( StFmsPoint* clpoint = 0 ); //apply energy dependent corrections
	Bool_t EnableEdepCor;
	Bool_t emc;
	Bool_t stphysics;
	TH2F* hhitAvsid;
	TH2F* hhitEvsid;
	TH1F* hVertexZ  ;
	TH1F* hVertexZyuxi  ; //run15
	TH2F* hnTower ; 
//	StLorentzVectorF compute4Momentum(const StThreeVectorF& xyz, double energy);
	 Int_t ReadBbcSlewing(const char *filename_bbc);
private:
	
	TFile* mFile;
	TTree* mTree;
	TTree* tevent  ;
	TTree* tjetcor ;
	TTree* tphoton ;
	TTree* tFmsjet ;
	
	
	StFmsEventHeader* mEvent;	//! event header
	Int_t	   mNfmsphotons;
	TObjArray* mFmsdiphotons;	//-> all the FMS photon (combinatorial) pairs in the event ( in StFmsDiPhoton format )
	TObjArray* mSimudiphotons;	//-> all the FMS photon (combinatorial) pairs in the event ( in StFmsDiPhoton format )
	TObjArray* mFmsphotons;		//-> all FMS photons in the event ( in StFmsPoint format )
	Int_t 	   mNfmsclusters;
	TObjArray* mFmsclusters;	//-> all FMS clusters in the event ( in StFmsCluster format ); each cluster also contains its photon members
	TF1* EDepCorrection; 		//! energy dependent correction	
	TF1* EDepCorrection1; 		//! energy dependent correction	
	TF1* EDepCorrection2; 		//! energy dependent correction	

	StEmcEvent* mEmcEvent;		//!
	
	Char_t* mFilename;
	
	Float_t mFmsEcutSmall;
	Float_t mFmsEcutLarge;

	StBemcTables* mBemcTables;
	StEEmcDb* mEeDb;

	StJetMaker2012* mJetmaker;

	StFmsDbMaker *mFmsDbMaker;
/*	struct StJetBranch {
		StJetBranch(const char* name, StAnaPars* anapars, StJetPars* jetpars)
		: name(name)
		, anapars(anapars)
		, jetpars(jetpars)
		, jetfinder(jetpars->constructJetFinder())
		, event(new StJetEvent)
		{
		}

		TString name;
		StAnaPars* anapars;
		StJetPars* jetpars;
		StJetFinder* jetfinder;
		StJetEvent* event;
	};
*/
	StSpinDbMaker *spDb;
	 double mBbcSlew[2][16][3];
	 
	 
	 //Tree
	 //for pion
	Float_t mpipt;
	Float_t mpie ;
	Float_t mpiz ; //diphoton energy sharing
	Float_t mpim ; //diphoton mass
    Float_t mpieta ;
    Float_t mpiphi ;
    Int_t 	mpidet12 ;
	Int_t   mcluid1 ;
    Int_t   mcluid2 ;
    Int_t   mnclph1 ;
    Int_t   mnclph2 ;
    Int_t   msamclu ; //if the 2 photons come from the same cluster
	Float_t   mOpAngle;
    Int_t   mphid1 ;
    Int_t   mphid2 ;

	Int_t  mCorr;

	Float_t  mjetdR;
	Float_t  mphiC;
	Float_t  myphiC;
	Float_t  mphiS;
	Float_t  mphiH;
	Float_t  mJphi;
	Float_t  mJeta;

	Float_t  mJE;
	Float_t  mJPt;
	Float_t  mZem;
	Float_t  mJt;
 
        Int_t   mtag ;
        Int_t   mph1tag ;
        Int_t   mph2tag ;
        Int_t   mcatagory ;

	Int_t 	mbunchid ;
	Float_t mvertexz ;
	Float_t mvertexzyuxi;
    Int_t   mBbcTimeBin ;
	Int_t   mspinbit ;
	Int_t   mievt ; //event#
	Int_t   mRnum ; //run#
	UInt_t mTrigBits ; 
	float mbbccoincrate;
	Int_t mRank;
	Int_t mLeadjet;

//for fmsjet
 	Float_t farrangePt[30]; //for sorting jets
	Int_t fsort[30];	
	Int_t mjetntow;  
	Float_t mjetpt;  
	Float_t mjetE; 
	Float_t mjeteta;
	Float_t mjetphi;
    Int_t mnjet; //# of jets in the  region;
	Int_t mjetnph;
//for photon
	Int_t mnph ; 		//# of photons on the near side
	Float_t meph ;  	
	Float_t mptph ;	
	Float_t metaph;  
	Float_t mphiph;
	Int_t mphid;
    Float_t mChi2S;
	Float_t mChi2L;
    Float_t mRx1 ;
    Float_t mRx2 ;
	int mnTower;
	 
	ClassDef(StFmsAnalysisMaker,0)
};

#endif
