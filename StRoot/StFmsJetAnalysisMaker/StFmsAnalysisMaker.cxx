#include "StFmsAnalysisMaker.h"

//FMS points

#include "StFmsClusterCopy.h"
#include "StFmsPointCopy.h"
//#include "StRoot/StFmsPointMaker/StFmsPoint.h"
//#include "StRoot/StFmsPointMaker/StFmsCollection.h"

#include "StFmsDiPhoton.h"
#include "StFmsEventHeader.h"
#include "StEmcEvent.h"

#include "StJetMaker/StJetMaker2012.h"
//#include "StJetBranch.h"
//#include "StJetMaker/StJetMaker2012.h"
#include "StJetFinder/StJetFinder.h"
#include "StSpinPool/StJetEvent/StJetEventTypes.h"

//#include "StSpinPool/StFmsTriggerMaker/StFmsTriggerMaker.h"
// STAR
#include "StMessMgr.h"
#include "StEventTypes.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuEmcCollection.h"
#include "StMuDSTMaker/COMMON/StMuEmcUtil.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
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

//EEMC
#include "StEEmcUtil/database/StEEmcDb.h"
#include "StEEmcUtil/database/EEmcDbItem.h"
#include "StEEmcUtil/EEmcGeom/EEmcGeomSimple.h"


#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"

#include <assert.h>

#include <iomanip>

#include "StMcEvent/StMcEvent.hh"
#include "StMcEvent/StMcTrack.hh"
#include "StMcEvent/StMcVertex.hh"

#include "StFmsHit.h"
#include "StRoot/StFmsDbMaker/StFmsDbMaker.h"



ClassImp(StFmsAnalysisMaker)

StFmsAnalysisMaker::StFmsAnalysisMaker( const char* name, StJetMaker2012* jetmaker ):StMaker(name)
{ 

    mJetmaker = jetmaker; 
	
    // If data, StEmcADCtoEMaker will be in the chain
    if (StEmcADCtoEMaker* adc2e = (StEmcADCtoEMaker*)StMaker::GetChain()->GetMakerInheritsFrom("StEmcADCtoEMaker"))
    {
	mBemcTables = adc2e->getBemcData()->getTables();
    }
    assert(mBemcTables);
	
    EnableEdepCor = false; 
    hhitAvsid = new TH2F("hhitAvsid","FMS hits ADC vs id",2100,0,2100,1024,0,4096);	
    hhitEvsid = new TH2F("hhitEvsid","FMS hits Energy vs id",2100,0,2100,1000,0,500);	
    hVertexZ = new TH1F("hVertexZ",";z-vertex [cm]",100,-200,200);
    hVertexZyuxi = new TH1F("hVertexZyuxi",";z-vertex [cm]",100,-200,200); //run15
    hnTower = new TH2F("hnTower",";nTower",30,0,30,6,6,12); 

    //	EDepCorrection1=new TF1("EDepCorrection1","1.42349+0.827688*x-0.000409365*x*x",0,120); //FMS energy dependent correction
    //	EDepCorrection2=new TF1("EDepCorrection2","1.06967+0.979638*x-0.00154454*x*x",0,120); //FMS energy dependent correction
    emc = false; //does not include mEmcEvent in to the tree
    stphysics = false; //if stphysics = true then the maker is analyzing events with slow detector read out, vertex from TPC
    //if stphysics = false, the maker is analyzing fast triggers, vertex from BBC timebin
    for( int ew=0; ew<2; ew++ )
    {
	for( int p=0; p<16; p++ )
	{
	    for( int c=0; c<3; c++ )
	    { 
		mBbcSlew[ew][p][c] = 0.0;
	    }
	}
    }
}

StFmsAnalysisMaker::~StFmsAnalysisMaker()
{	
    LOG_INFO << " StFmsAnalysisMaker:: destructor " << endm;
    if(mFmsdiphotons)
    {
	delete mFmsdiphotons;
	mFmsdiphotons = 0;
    }
    if(mFmsphotons)
    {
	delete mFmsphotons;
	mFmsphotons = 0;
    }

}


void StFmsAnalysisMaker::Clear( const char* opt )
{	
    mNfmsphotons = -1;
    mNfmsclusters = -1;
    mFmsdiphotons->Clear();
    mFmsphotons->Clear();
    // 	if(emc&&mEmcEvent)mEmcEvent->Clear();
}

void StFmsAnalysisMaker::SetOutputFile( Char_t* filename = "default.root" )
{	
    mFilename = filename;
}

Int_t StFmsAnalysisMaker::Init()
{	
    LOG_INFO << "StFmsAnalysisMaker::Init() " << endm;
    mFile = new TFile(mFilename,"recreate");
    assert(mFile);

    mFmsDbMaker = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));

    if(!mJetmaker)
    {
	LOG_ERROR << "StFmsAnalysisMaker::Init() -- mJetMaker is null "<<endm;
	return kStErr;
    }
	
    mEeDb = (StEEmcDb*)StMaker::GetChain()->GetDataSet("StEEmcDb");
    if(!mEeDb)
    {
	LOG_ERROR << "StEEmcDb not found " <<endm;
	return kStErr;
    }

    if(mEeDb) mEeDb->setThreshold(3);

    //	mEvent = new StFmsEventHeader();
    //	if(emc)mEmcEvent = new StEmcEvent();	

    mNfmsphotons = -1;
    mNfmsclusters = -1;
    mFmsphotons   = new TObjArray(50);
    mFmsdiphotons = new TObjArray(50);


    /*
      mFmsphotons   = new TObjArray(50);
      mFmsdiphotons = new TObjArray(50);
      mSimudiphotons = new TObjArray(50);
 
      mTree = new TTree("fmsdst","fmsdst");
      mTree->Branch("fmsevent",&(mEvent),32000,99);
      if(emc)	mTree->Branch("emcevent",&(mEmcEvent),32000,99); //takes too much space
      mTree->Branch("fmsdiphotons",&(mFmsdiphotons),32000,0);
      mTree->Branch("nfmsphotons",&mNfmsphotons,"nfmsphotons/I");
      mTree->Branch("fmsphotons",&(mFmsphotons),32000,0);
      mTree->Branch("nfmsclusters",&mNfmsclusters,"nfmsclusters/I");
      mTree->Branch("fmsclusters",&(mFmsclusters),32000,0);
      mTree->Branch("simudiphotons",&(mSimudiphotons),32000,0);
    */

    
    //set branches from jet maker
    // for (size_t iBranch = 0; iBranch < mJetmaker->mJetBranches.size(); ++iBranch) {
    // 	StJetBranch* jetbranch = mJetmaker->mJetBranches[iBranch];
    // 	jetbranch->jetfinder->Init();
    // 	//		mTree->Branch(jetbranch->name,"StJetEvent",&jetbranch->event);
    // }

        
    //mTree->BranchRef();
    spDb = (StSpinDbMaker*)GetMaker("spinDb");
    assert(spDb);
	
    //Tree
    tevent = new TTree("tevent","event");
    tjetcor = new TTree("tjetcor","FMS pi0 FMS jet pair");
    tphoton = new TTree("tphoton","FMS photons");
    tFmsjet = new TTree("tFmsjet","FMS jet");


    tevent->Branch("vertexz",&mvertexz,"vertexz/F");
    tevent->Branch("TrigBits",&mTrigBits,"TrigBits/I");   
    //    tevent->Branch("fnPjet",&fmnPjet,"fnPjet/I");
    tevent->Branch("fnjet",&mnjet,"fnjet/I");
    tevent->Branch("fnph",&mnph,"fnph/I");
	
    //corelated pion
    tjetcor->Branch("Bunchid7bit",&mbunchid,"Bunchid7bit/I");
    tjetcor->Branch("bbccoincrate",&mbbccoincrate,"bbccoincrate/F");
    tjetcor->Branch("run",&mRnum,"run/I");
    tjetcor->Branch("ievt",&mievt,"ievt/I");
    tjetcor->Branch("spinbit",&mspinbit,"spinbit/I");
    tjetcor->Branch("vertexz",&mvertexz,"vertexz/F");
    tjetcor->Branch("vertexzyuxi",&mvertexzyuxi,"vertexzyuxi/F"); //for run15
    tjetcor->Branch("bbct",&mBbcTimeBin,"bbct/I");
    tjetcor->Branch("TrigBits",&mTrigBits,"TrigBits/I");  
    tjetcor->Branch("pie",&mpie,"pie/F");
    tjetcor->Branch("piz",&mpiz,"piz/F");
    tjetcor->Branch("pim",&mpim,"pim/F"); 
    tjetcor->Branch("pipt",&mpipt,"pipt/F");
    tjetcor->Branch("pieta",&mpieta,"pieta/F");
    tjetcor->Branch("piphi",&mpiphi,"piphi/F");
    tjetcor->Branch("pidet12",&mpidet12,"pidet12/I");
    tjetcor->Branch("cluid1",&mcluid1,"cluid1/I");
    tjetcor->Branch("nclph1",&mnclph1,"nclph1/I");
    tjetcor->Branch("cluid2",&mcluid2,"cluid2/I");
    tjetcor->Branch("nclph2",&mnclph2,"nclph2/I");
    tjetcor->Branch("samclu",&msamclu,"samclu/I");
    tjetcor->Branch("OpAngle",&mOpAngle,"OpAngle/F");
    tjetcor->Branch("phid1",&mphid1,"phid1/I");
    tjetcor->Branch("phid2",&mphid2,"phid2/I");

    tjetcor->Branch("mRank",&mRank,"mRank/I");
    tjetcor->Branch("Leadjet",&mLeadjet,"Leadjet/I");

    tjetcor->Branch("Corr",&mCorr,"Corr/I"); // 

    tjetcor->Branch("phiC",&mphiC,"phiC/F"); // 
    tjetcor->Branch("yphiC",&myphiC,"yphiC/F"); // 
    tjetcor->Branch("phiS",&mphiS,"phiS/F"); // 
    tjetcor->Branch("phiH",&mphiH,"phiH/F"); // 
    tjetcor->Branch("CjetEta",&mJeta,"CjetEta/F"); // 
    tjetcor->Branch("CjetPhi",&mJphi,"CjetPhi/F"); // 
    tjetcor->Branch("CjetE",&mJE,"CjetE/F"); // 
    tjetcor->Branch("CjetPt",&mJPt,"CjetPt/F"); // 
    tjetcor->Branch("Zem",&mZem,"Zem/F"); // 
    tjetcor->Branch("Jt",&mJt,"Jt/F"); // 
    tjetcor->Branch("jetdR",&mjetdR,"fjetdR/F"); //# delta R

    tjetcor->Branch("tag",&mtag,"tag/I");
    tjetcor->Branch("ph1tag",&mph1tag,"mph1tag/I");
    tjetcor->Branch("ph2tag",&mph2tag,"mph2tag/I");
    tjetcor->Branch("catagory",&mcatagory,"catagory/I");

    //fms jet
    tFmsjet->Branch("fnjet",&mnjet,"fnjet/I");
    tFmsjet->Branch("fjetntow",&mjetntow,"fjetntow/I");       
    tFmsjet->Branch("fjE",&mjetE,"fjE/F");        
    tFmsjet->Branch("fjpT",&mjetpt,"fjpT/F");        
    tFmsjet->Branch("fjeta",&mjeteta,"fjeta/F"); 
    tFmsjet->Branch("fjphi",&mjetphi,"fjphi/F");  
    tFmsjet->Branch("fjnph",&mjetnph,"fjnph/I");  
    tFmsjet->Branch("TrigBits",&mTrigBits,"TrigBits/I");  

    //photon
    tphoton->Branch("eph",&meph,"eph/F");  
    tphoton->Branch("ptph",&mptph,"ptph/F");  
    tphoton->Branch("etaph",&metaph,"etaph/F"); 
    tphoton->Branch("phiph",&mphiph,"phiph/F"); 
    tphoton->Branch("phid",&mphid,"phid/I"); 
    tphoton->Branch("photonChi2S",&mChi2S,"photonChi2S/F");
    tphoton->Branch("photonChi2L",&mChi2L,"photonChi2L/F");
    tphoton->Branch("photonRx1",&mRx1,"photonRx1/F");
    tphoton->Branch("photonRx2",&mRx2,"photonRx2/F");
    tphoton->Branch("TrigBits",&mTrigBits,"TrigBits/I");  
    return kStOk;
}
	
	
Int_t StFmsAnalysisMaker::Make()
{
    LOG_INFO << "StFmsAnalysisMaker::Make() " << endm;
    StMuDst* muDst = (StMuDst*)GetInputDS("MuDst");
    if(!muDst)
    {
	LOG_WARN << "StFmsAnalysisMaker::Make() no MuDst" << endm;
	return kStWarn;
    }


    //remove LED events;
    Bool_t checkLed = 0;
    if (muDst->event()->triggerIdCollection().nominal().isTrigger(320225))checkLed = true;
	
    Bool_t LedEvent = false;
    StL0Trigger& l0 = muDst->event()->l0Trigger();
    UInt_t mLedBit = l0.lastDsmArray(4);
    if(mLedBit & 0x0001) LedEvent = true;
	
    if(checkLed||LedEvent)
    {
	LOG_INFO << " FMS LED fired, abort " <<endm;
	return kStOk;
    }
	
    mbunchid = -1;
    mRnum = -1;
    mievt = -1;
    mBbcTimeBin = -1;
    mvertexz = -999.;
    mvertexzyuxi = -999.;
    mTrigBits = 0;
    mspinbit = -1;
    mbbccoincrate = -1; 
		
    mpie = -1;
    mpiz = -1;
    mpim = -1;
    mpipt = -1;
    mpieta = -99;
    mpiphi = -99;
    mpidet12 = -99;
    mcluid1 = -1;
    mcluid2 = -2;
    mnclph1 = -1;
    mnclph2 = -1;
    msamclu = -1;
    mOpAngle = -1.;
    mphid1 =-1;
    mphid2 =-1;
    mRank = -1;
    mLeadjet = -1;
    mCorr  =-1;

    mZem= -1;
    mJt = -1;
    mphiC = -4;
    myphiC = -4;
    mphiS = -4;
    mphiH = -4;
    mJphi = -4;
    mJeta = -1;
    mJE = -4;
    mJPt = -4;
    mjetdR = -1;
    mtag = -1;
    mph1tag = -1;
    mph2tag = -1;
    mcatagory = -1;
    //photon
    mnph = -1;      //not in photon tree    
    meph = -1.0;    
    mptph = -1.0;   
    metaph = -1.0;      
    mphiph = -99.0;
    mphid=-1;
    mChi2S = -1.0;
    mChi2L = -1.0;
    mRx1 = -1.0;
    mRx2=-1;
    //fmsjet
    mjetntow=-1;  
    mnjet=-1;		 //not in jet tree  
    mjeteta = -1;
    mjetphi = -99;
    mjetE = -1;
    mjetpt = -1;
    mjetnph = -1;

    mnTower = 0;
    int jetbuffer = 30;
    for(Int_t ijet = 0; ijet < jetbuffer; ijet++)
    {
	farrangePt[ijet] = 0;
	fsort[ijet] = -1;
    }
	
    Int_t Rnum = muDst->event()->runNumber();
    Int_t ievt = muDst->event()->eventNumber();
    StRunInfo* runInfo = &(muDst->event()->runInfo());
    Int_t fillnumber = runInfo->beamFillNumber(blue);
    Int_t bunchid = l0.bunchCrossingId7bit(Rnum);
    Int_t bbccoincrate = runInfo->bbcCoincidenceRate();
    int spinbit = -1;
    spinbit = spDb->spin4usingBX7(bunchid);
 	
    if(stphysics)
    {		//vertex from TPC, only saves the highest (positively) ranked vertex		
	Bool_t goodvertex = false;
	Int_t nVertices = muDst->numberOfPrimaryVertices();
	LOG_INFO << " nPrimaryVertices = " << nVertices << endm;
	TVector3 vtx;
	for(Int_t j = 0; j < nVertices; j++)
	{
	    StMuPrimaryVertex* vertex = muDst->primaryVertex(j);
	    assert(vertex);
	    muDst->setVertexIndex(j);
	    StThreeVectorF r = vertex->position();
	    if(vertex->ranking() <= 0)continue;
	    else
	    {
		goodvertex = true;
		vtx.SetX(r.x());
		vtx.SetY(r.y());
		vtx.SetZ(r.z());
		//	LOG_INFO <<" j = "<<j<<", ranking = "<<vertex->ranking()<<endm;
		break;	//only saves the highest (positively) ranked vertex
	    }
	}
	if(!goodvertex) { return kStOk; } //no good vertex found, abort event
	mEvent->mVertex = vtx;
	unsigned short bbcTimeBin = muDst->event()->bbcTriggerDetector().onlineTimeDifference(); //offset by 4096
	if(bbcTimeBin==0){ LOG_INFO << "  TRIGGER BIN == 0 " << endm; return kStOk; }
	mEvent->mBbcTimeBin = bbcTimeBin;
    }
    else
    {	//stfms, no tpc vertex, use fit to bbc time bin
			
	unsigned short bbcTimeBin = muDst->event()->bbcTriggerDetector().onlineTimeDifference();
	//	if(bbcTimeBin==0){ LOG_INFO << "  TRIGGER BIN == 0 " << endm; return kStOk; } //bbcTimeBin=0 means no good bbc hit --Yuxi 12/04/2013
	//	if(bbcTimeBin<3000||bbcTimeBin>5000){ LOG_INFO << "  BBC TAC out of range, abort " << endm; return kStOk; }

	float vertexZ=-999;//Oleg

	//RUn11 formula
	if(bbcTimeBin!=0)  mvertexzyuxi = 633.544 - 0.158*bbcTimeBin; //in cm
	if(bbcTimeBin==0)  mvertexzyuxi = 0; //in cm
	/*
	//for run15	
	UShort_t tdc1east, tdc1west;
	UShort_t pmt1east, pmt1west;
	UShort_t adc1east, adc1west;
	unsigned int tdcMatchEast = 0;
	unsigned int tdcMatchWest = 0;

	float   bbcTdiff = 0;
	bbcTdiff = (float)muDst->event()->triggerData()->bbcTimeDifference();
	tdc1east = muDst->event()->triggerData()->bbcEarliestTDC(east);
	tdc1west = muDst->event()->triggerData()->bbcEarliestTDC(west);

	for( int i=1; i<=16; i++ )
	{
	    if( tdc1east==muDst->event()->triggerData()->bbcTDC(east, i) )
	    {
		adc1east = muDst->event()->triggerData()->bbcADC(east, i);
		pmt1east = i-1;
		++tdcMatchEast;
	    }
	    if( tdc1west==muDst->event()->triggerData()->bbcTDC(west, i) )
	    {
		adc1west = muDst->event()->triggerData()->bbcADC(west, i);
		pmt1west = i-1;
		++tdcMatchWest;
	    }
	}
	if( tdcMatchEast==1 && tdcMatchWest==1 )
	{ // BBC slewing correction (east/west)
	    Float_t zEast = -0.3*( bbcTdiff - mBbcSlew[0][pmt1east][0] - mBbcSlew[0][pmt1east][1]/(mBbcSlew[0][pmt1east][2] + adc1east) );
	    Float_t zWest = -0.3*( bbcTdiff - mBbcSlew[1][pmt1west][0] - mBbcSlew[1][pmt1west][1]/(mBbcSlew[1][pmt1west][2] + adc1west) );
	    vertexZ = (zEast + zWest)/2.0;
	} else
	{
	    vertexZ = -30.0;
	}
	*/		
	//	mvertexz = vertexZ; //Run15
	mvertexz = mvertexzyuxi;
	hVertexZ->Fill(mvertexz);
	hVertexZyuxi->Fill(mvertexzyuxi);
	mBbcTimeBin = bbcTimeBin; 
	mspinbit = spinbit;
	mbunchid = bunchid;
	mievt = ievt;
	//	mRnum = Rnum;
        mbbccoincrate = bbccoincrate;
 
    }
    //===============deduce trigger type==================//Yuxi 02/07/2011
    // TCU channel5
    //Bit 0     Fms-HT-th0       FMS  HT threshold-0 bit
    //Bit 1     Fms-HT-th1       FMS  HT threshold-1 bit
    //Bit 2  FmsSml-Cluster-th0  FMS small-cell cluster threshold-0 bit   ~Board Sum??
    //Bit 3  FmsSml-Cluster-th1  FMS small-cell cluster threshold-1 bit
    //Bit 4  FmsSml-Cluster-th2  FMS small-cell cluster threshold-2 bit
    //Bit 5  FmsLrg-Cluster-th0  FMS large-cell cluster threshold-0 bit
    //Bit 6  FmsLrg-Cluster-th1  FMS large-cell cluster threshold-1 bit
    //Bit 7  FmsLrg-Cluster-th2  FMS large-cell cluster threshold-2 bit
    //Bit 8     Fms-JP-th0       FMS Jet Patch threshold-0 bit
    //Bit 9     Fms-JP-th1       FMS Jet Patch threshold-1 bit
    //Bit 10    Fms-JP-th2       FMS Jet Patch threshold-2 bit
    //Bit 11     Fms-dijet                 FMS dijet bit
    //Bit 12      Unused                      Unused
    //Bit 13      FMS*BEMC*JP0             FMS*BEMC*JP0
    //Bit 14       FPE                    FPE trigger bit
    //Bit 15      Unused                      Unused
    //========================================================//	

    //test trigger ids, fill in TrigIds array
    vector<int> TrigIds;

    StMuTriggerIdCollection trig = muDst->event()->triggerIdCollection();
    StTriggerId l1trig = trig.nominal();
    
    if( l1trig.isTrigger(480810) ) (TrigIds).push_back(480810);        //   FMSJP0
    if( l1trig.isTrigger(480809) ) (TrigIds).push_back(480809);        //   FMSJP1
    if( l1trig.isTrigger(480808) ) (TrigIds).push_back(480808);        //   FMSJP2
    if( l1trig.isTrigger(480824) ) (TrigIds).push_back(480824);        //   FMSLgBS1
    if( l1trig.isTrigger(480804) ) (TrigIds).push_back(480804);        //   FMSLgBS1
    if( l1trig.isTrigger(480805) ) (TrigIds).push_back(480805);        //   FMSLgBS2
    if( l1trig.isTrigger(480806) ) (TrigIds).push_back(480806);        //   FMSLgBS3
    if( l1trig.isTrigger(480821) ) (TrigIds).push_back(480821);        //   FMSSmBS1
    if( l1trig.isTrigger(480801) ) (TrigIds).push_back(480801);        //   FMSSmBS1
    if( l1trig.isTrigger(480802) ) (TrigIds).push_back(480802);        //   FMSSmBS2
    if( l1trig.isTrigger(480803) ) (TrigIds).push_back(480803);        //   FMSSmBS3
 
    if( l1trig.isTrigger(480828) ) (TrigIds).push_back(480828);        //   FMSJP2
    if( l1trig.isTrigger(480829) ) (TrigIds).push_back(480829);        //   FMSJP1
    if( l1trig.isTrigger(480830) ) (TrigIds).push_back(480830);        //   FMSJP0
    if( l1trig.isTrigger(480825) ) (TrigIds).push_back(480825);        //   FMSLgBS2
    if( l1trig.isTrigger(480826) ) (TrigIds).push_back(480826);        //   FMSLgBS3
    if( l1trig.isTrigger(480844) ) (TrigIds).push_back(480844);        //   FMSLgBS1
    if( l1trig.isTrigger(480822) ) (TrigIds).push_back(480822);        //   FMSSmBS2
    if( l1trig.isTrigger(480843) ) (TrigIds).push_back(480843);        //   FMSSmBS3
    if( l1trig.isTrigger(480841) ) (TrigIds).push_back(480841);        //   FMSSmBS1
      
    //Run11
    // if( l1trig.isTrigger(89) ) (TrigIds).push_back(89);                //   FMS*BEMC*JP0
    // if( l1trig.isTrigger(320220) ) (TrigIds).push_back(320220);        //   FMSJP1
    // if( l1trig.isTrigger(320231) ) (TrigIds).push_back(320231);        //   FMSJP2
    // if( l1trig.isTrigger(320227) ) (TrigIds).push_back(320227);        //   FMSLgBS2
    // if( l1trig.isTrigger(320226) ) (TrigIds).push_back(320226);        //   FMSLgBS1
    // if( l1trig.isTrigger(320222) ) (TrigIds).push_back(320222);        //   FMSSmBS1
    // if( l1trig.isTrigger(320223) ) (TrigIds).push_back(320223);        //   FMSSmBS2

    Int_t nIds = TrigIds.size();
    for(Int_t iid = 0; iid < nIds; iid++)
    {
	Int_t trigid = TrigIds.at(iid);
	switch(trigid){//Run11 Id
	case 89:	//FMS*BEMC*JP0
	    mTrigBits = mTrigBits | ((0x01)<<13);
	    break;
	case 320220:	//FMSJP1
	    mTrigBits = mTrigBits | ((0x01)<<9);
	    break;
	case 320231:	//FMSJP2
	    mTrigBits = mTrigBits | ((0x01)<<10);
	    break;	
	case 320227:	//FMSLgBS2
	    mTrigBits = mTrigBits | ((0x01)<<7);
	    break;
	case 320226:	//FMSLgBS1
	    mTrigBits = mTrigBits | ((0x01)<<6);
	    break;
	case 320222:	//FMSSmBS1
	    mTrigBits = mTrigBits | ((0x01)<<3);
	    break;
	case 320223:	//FMSSmBS2
	    mTrigBits = mTrigBits | ((0x01)<<4);
	    break;
	case 320224:	//FMSHT
	    mTrigBits = mTrigBits | ((0x01)<<1);
	    break;
	    //Run15
	case 480810:    //FMSJP0
	    mTrigBits = mTrigBits | ((0x01)<<8);
	    break;
	case 480809:    //FMSJP1
	    mTrigBits = mTrigBits | ((0x01)<<9);
	    break;
	case 480808:    //FMSJP2
	    mTrigBits = mTrigBits | ((0x01)<<10);
	    break;
	case 480824:    //FMSLgBS1
	    mTrigBits = mTrigBits | ((0x01)<<5);
	    break;
	case 480804:    //FMSLgBS
	    mTrigBits = mTrigBits | ((0x01)<<5);
	    break;
	case 480805:    //FMSLgBS2
	    mTrigBits = mTrigBits | ((0x01)<<6);
	    break;
	case 480806:    //FMSLgBS3
	    mTrigBits = mTrigBits | ((0x01)<<7);
	    break;
	case 480821:    //FMSSmBS1
	    mTrigBits = mTrigBits | ((0x01)<<2);
	    break;
	case 480801:    //FMSSmBS2
	    mTrigBits = mTrigBits | ((0x01)<<2);
	    break;
	case 480802:    //FMSSmBS2
	    mTrigBits = mTrigBits | ((0x01)<<3);
	    break;
	case 480803:    //FMSSmBS3
	    mTrigBits = mTrigBits | ((0x01)<<4);
	    break;
	case 480830:    //FMSJP0
	    mTrigBits = mTrigBits | ((0x01)<<8);
	    break;
	case 480829:    //FMSJP1
	    mTrigBits = mTrigBits | ((0x01)<<9);
	    break;
	case 480828:    //FMSJP2
	    mTrigBits = mTrigBits | ((0x01)<<10);
	    break;
	case 480844:    //FMSLgBS1
	    mTrigBits = mTrigBits | ((0x01)<<5);
	    break;
	case 480825:    //FMSLgBS2
	    mTrigBits = mTrigBits | ((0x01)<<6);
	    break;
	case 480826:    //FMSLgBS3
	    mTrigBits = mTrigBits | ((0x01)<<7);
	    break;
	case 480841:    //FMSSmBS1
	    mTrigBits = mTrigBits | ((0x01)<<2);
	    break;
	case 480822:    //FMSSmBS2
	    mTrigBits = mTrigBits | ((0x01)<<3);
	    break;
	case 480843:    //FMSSmBS3
	    mTrigBits = mTrigBits | ((0x01)<<4);
	    break;
	    //run15 trigger

	default:
	    break;
	}
    }
		
    if(emc)
    { //QA part has been moved to StFmsQAMaker
	//bemc hits. should add some cuts on hits (only record hits with good tower status)
	StEvent *mEvent = static_cast<StEvent*>(this->GetDataSet("StEvent"));
	if(!mEvent)
	{
	    LOG_ERROR << " no StEvent " << endm;
	    return kStErr;
	}

	StEmcGeom* geom = StEmcGeom::instance("bemc"); // for towers
	if (!geom) LOG_WARN << " No StEmcGeom!" << endm;
	assert(geom);
	StEmcCollection *emc = mEvent->emcCollection();
	if (!emc) LOG_WARN << " No StEmcCollection" << endm;
	assert(emc);
	StEmcDetector* detector=emc->detector(kBarrelEmcTowerId);
	if (!detector) LOG_WARN << " No StEmcDetector!" << endm;
	Int_t emcnhits = 0;
	if(detector)
	{
	    for(Int_t m = 1; m <= 120; m++)
	    {
		StEmcModule* module = detector->module(m);
		if(module)
		{
		    StSPtrVecEmcRawHit& rawHit=module->hits();
		    emcnhits += rawHit.size();
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
			    (mEmcEvent->mVhitAdc).push_back(adc);
			    (mEmcEvent->mVhitId).push_back(did);
			    (mEmcEvent->mVhitDetId).push_back(kBarrelEmcTowerId);
			    geom->getEtaPhi(did,tower_eta,tower_phi); // to convert software id into eta/phi
			    Float_t Et = (energy/cosh(tower_eta));

			    Float_t pedestal;
			    Float_t rms;
			    Int_t status;
			    Int_t CAP(0);
			    mBemcTables->getPedestal(BTOW,did,CAP,pedestal,rms);
			    mBemcTables->getStatus(BTOW,did,status);
			    (mEmcEvent->mVhitE).push_back(energy);
			    (mEmcEvent->mVhitEt).push_back(Et);
			    (mEmcEvent->mVhitEta).push_back(tower_eta);
			    (mEmcEvent->mVhitPhi).push_back(tower_phi);
			    (mEmcEvent->mVhitPedestal).push_back(pedestal);
			    (mEmcEvent->mVhitPedrms).push_back(rms);
			    (mEmcEvent->mVhitStatus).push_back(status);
			}
		    }
		}
	    }
	}
		
	//endcap --only include towers which passed baseline EEMC QA ( accepted by the jet maker )		
	StMuEmcCollection* muEmc = StMuDst::muEmcCollection();
	for (Int_t id = 0; id < muEmc->getNEndcapTowerADC(); ++id)
	{
	    Int_t rawadc, sec, sub, etabin;
	    muEmc->getEndcapTowerADC(id, rawadc, sec, sub, etabin);

	    // Sanity check
	    if (rawadc < 0 || rawadc >= 4095) continue;

	    assert(1 <= sec && sec <= 12);
	    assert(1 <= sub && sub <= 5);
	    assert(1 <= etabin && etabin <= 12);

	    const EEmcDbItem *dbItem = mEeDb->getT(sec,sub-1+'A',etabin);
	    if(dbItem->fail) continue; //drop broken channels
	    if(dbItem->stat) continue; // drop not working channels and jumpy pedestal channels
	    if(dbItem->gain<=0.) continue; // drop it, unless you work with ADC spectra
	    if(rawadc<dbItem->thr) continue; // drop raw ADC < ped+N*sigPed, N==3 in init

	    Double_t adc = rawadc - (dbItem->ped);
	    Double_t energy = adc/(dbItem->gain);
	    (mEmcEvent->mVhitDetId).push_back(kEndcapEmcTowerId);
	    (mEmcEvent->mVhitId).push_back((sec-1)*60+(sub-1)*12+(etabin-1));
	    (mEmcEvent->mVhitAdc).push_back(adc);
	    (mEmcEvent->mVhitE).push_back(energy);
	    const EEmcGeomSimple& geom = EEmcGeomSimple::Instance();
	    TVector3 towerLocation = geom.getTowerCenter(sec-1,sub-1,etabin-1);
	    (mEmcEvent->mVhitEta).push_back(towerLocation.Eta());
	    (mEmcEvent->mVhitPhi).push_back(towerLocation.Phi());
	    Float_t Et_eemc = (energy/cosh(towerLocation.Eta()));
	    (mEmcEvent->mVhitEt).push_back(Et_eemc);
	    (mEmcEvent->mVhitPedestal).push_back(dbItem->ped); //tower pedestal mean
	    (mEmcEvent->mVhitPedrms).push_back(dbItem->sigPed);
	    (mEmcEvent->mVhitStatus).push_back(dbItem->stat==0);

	    emcnhits++;
	}

	mEmcEvent->mNhits = emcnhits;
    } //if(emc)

    

    Float_t EcluTotalCut = 500; //GeV cut on the total cluster energies, to remove LED trails	
    Float_t ZrrCut = 0.8;

    Float_t PhECut = 1 ;// not 10
    Float_t FMSJetECut = 1 ;// not 10

 
    StJetEvent* jetFMS = 0;
    jetFMS = mJetmaker->event("AntiKtR060NHits12");

    //access fms points
    StEvent* event = static_cast<StEvent*>(GetInputDS("StEvent"));
    StFmsCollection* fms = nullptr;
    if (event)
    {
	fms = event->fmsCollection();
    }  // if
    if (!fms)
    {
	LOG_ERROR << "StFmsPointMaker did not find "
		  << "an StFmsCollection in StEvent" << endm;
    }
	  
    //---------------------part0------------------record hits    

	
    auto& hits = fms->hits();
    //  LOG_DEBUG << "Found nhits = " << hits.size() << endm;
    int n=0;
    for (auto i = hits.begin(); i != hits.end(); ++i)
    {
	int d,c;
	float hitE;
	StFmsHit* fmshit = *i; 
	//int detector = fmshit->detectorId();
	int crt   =fmshit->qtCrate();
	int slot  =fmshit->qtSlot();
	int ch    =fmshit->qtChannel();
	int adc =fmshit->adc();
	mFmsDbMaker->getReverseMap(crt,slot,ch,&d,&c);
	//if (d<8) cout<<"!!!!!!!!!!!!!!!!!!! hit id d="<<d<<" c="<<c<<endl;
	if (adc > 0)
	{  
	    hhitEvsid->Fill((d-8)*600+c,fmshit->energy());
	    hhitAvsid->Fill((d-8)*600+c,adc);
	}	
    }
    Float_t totcluE = 0.0;
		
    //---------------------part1------------------record photon > Ecut GeV    

    for(StFmsClusterIterator iclu = fms->clusters().begin(); iclu != fms->clusters().end(); iclu++)
    {		 
	mnTower = (*iclu)->nTowers();
	hnTower->Fill(mnTower,(*iclu)->detectorId());
	totcluE += (*iclu)->energy();
	if(totcluE>EcluTotalCut){ cout<<"totcluE = "<<totcluE<<" failed cut"<<endl; continue; }

	StPtrVecFmsPoint& fmspoints = (*iclu)->points();
	for(auto ipts = fmspoints.begin(); ipts != fmspoints.end(); ipts++)
	{
	    const StThreeVectorF php3 = (*ipts)->XYZ();
	    int pid=(*ipts)->detectorId()-8;
	    if(!(Fiducial(php3,pid)) )
	    {			//fiducial volume cut
		LOG_INFO << " out of fiducial volume, ph_x = "<<php3.x()<<" ph_y = "<<php3.y()<<" ph_z = "<<php3.z()<<endm;
		continue;
	    }
				
	    const StLorentzVectorF p0 = (*ipts)->fourMomentum();
	    //	cout<<"before"<<(*ipts)->energy()<<endl;
	    if (p0.e()<PhECut) continue; 
	    //	double gainCorr = EdepCor(p0.e(),phid);
	    StLorentzVectorF p0Corr= p0 ;	

	    meph = p0Corr.e();
	    mptph = p0Corr.perp();
	    mphiph = p0Corr.phi();
	    metaph = p0Corr.pseudoRapidity();
                     
                   
	    mphid =  (*ipts)->detectorId();
	    Float_t phx = (*ipts)->x();
	    Float_t phy = (*ipts)->y();
	    if(mphid <= 9)   {  mRx1=phx/5.812-floor(phx/5.812);   mRx2=phy/5.812-floor(phy/5.812);} //large cell
	    if(mphid > 9 )  {   mRx1=phx/3.822-floor(phx/3.822);   mRx2=phy/3.875-floor(phy/3.875);}

                 
	    tphoton->Fill(); 
					   
	    mFmsphotons->Add((*ipts));
 
	}
	
    }
	 
    //---------------------part2------------------record non-correlated pion
    // tag with Corr==0


    Int_t nfmsphotons = mFmsphotons->GetEntries();
    mnph = nfmsphotons;
       

    for(Int_t i = 0; i < nfmsphotons; i++)
    {
	for(Int_t j = i+1; j < nfmsphotons; j++)
	{
	    StFmsPoint* vi = (StFmsPoint*)mFmsphotons->At(i);
	    Float_t ei = vi->energy();

	    StFmsPoint* vj = (StFmsPoint*)mFmsphotons->At(j);
	    Float_t ej = vj->energy();
						
	    if (ei<PhECut || ej<PhECut) {	continue;	}
	    const StLorentzVectorF liraw = vi->fourMomentum();
	    const StLorentzVectorF ljraw = vj->fourMomentum();
           
	    Int_t ph1id = vi->detectorId();
	    Int_t ph2id = vj->detectorId();

  
	    StLorentzVectorF li = liraw*1;
	    StLorentzVectorF lj = ljraw*1;
 
	    mcluid1 = vi->parentClusterId();
	    mnclph1 = vi->nParentClusterPhotons();
	    mcluid2 = vj->parentClusterId();
	    mnclph2 = vj->nParentClusterPhotons();
	    if(mcluid1==mcluid2&&mnclph1==2&&mnclph2==2)msamclu = 1;
	    else msamclu = 0;
		
	    mCorr = 0; // non-correlated
	    mcatagory = -1;
			
	    StLorentzVectorF lpair = li + lj;
	    mpie = lpair.e();
	    mpim = lpair.m();
	    mpieta = lpair.pseudoRapidity ();
	    mpiphi = lpair.phi();
	    mpiz = TMath::Abs(li.e()-lj.e())/(li.e()+lj.e());
	    mpipt= mpie*TMath::Sin(lpair.theta());
	    mphid1=ph1id;
	    mphid2=ph2id;
	    Int_t nstbi = ph1id; 
	    Int_t nstbj = ph2id; 
	    mpidet12 = nstbj + nstbi*100; 
	    //opening angle         
	    TVector3 pj(lj.px(),lj.py(),lj.pz());
	    TVector3 pi(li.px(),li.py(),li.pz());
	    mOpAngle = pj.Angle(pi);
	    tjetcor->Fill(); 
	}//photon i	
    }//photon j
    //cout << "----> Jet address:"<< jetFMS <<"\t"<< mJetmaker <<endl;
    //cout << "----> Number of jets:"<< jetFMS->numberOfJets() <<endl;

    
    //---------------------part3------------------record jet & correlation with photon
    const int nfmsph=nfmsphotons;

    int tagPH[80];//tag for photon correlation,used in jet based combination later
    
    for (Int_t itagph = 0; itagph < 80; ++itagph)
    {
	tagPH[itagph]=0;
    }
    mnjet=jetFMS->numberOfJets();
    if(mnjet>jetbuffer)
    { //njets capped at jetbuffer(=20)
	//		cout<<"njets = "<<mnjet<<" > jetbuffer = "<<jetbuffer<<endl;
	mnjet = jetbuffer;
    }
    for (Int_t iJet = 0; iJet < mnjet; ++iJet)
    {
	StJetCandidate* jet = jetFMS->jet(iJet);
	farrangePt[iJet] = jet->pt();
    }
    TMath::Sort(jetbuffer,farrangePt,fsort);
    int jetlead[30];
    for (Int_t itagph = 0; itagph < 30; ++itagph)
    {
	jetlead[itagph]=0;
    }
    for (Int_t iJet = 0; iJet < mnjet; ++iJet)
    {
	//pT ordered
	if(fsort[iJet]==0) jetlead[iJet]=1;
	//	StJetCandidate* jet = jetFMS->jet(iJet);
	StJetCandidate* jet = jetFMS->jet(fsort[iJet]);
			
	Float_t jetE = jet->E();
	Float_t jetpt = jet->pt();
	Float_t jetEta = jet->eta();
	Float_t jetPhi = jet->phi();
	if(jetE< FMSJetECut) continue;

	mjetntow  = jet->numberOfTowers();		
	mjeteta = jetEta;
	mjetphi = jetPhi;
	mjetE = jetE;
	mjetpt = jetpt;
		 
	//jet correlate with photon
	int phcount=0;
			
	for(Int_t ipha = 0; ipha < nfmsphotons; ipha++)
	{
	    tagPH[ipha]=-1;
	    StFmsPoint* vi = (StFmsPoint*)mFmsphotons->At(ipha);
	    Int_t ph1id = vi->detectorId();// unlike fmspoint using detectorID
	    const StLorentzVectorF liraw = vi->fourMomentum();
	    //     	double nlgain1= EdepCor(ei,ph1id) ;
	    StLorentzVectorF li = liraw*1;
	    double phE1 =li.e() ;
	    if (phE1<PhECut) continue;
	    double phEta = li.pseudoRapidity ();
	    double phPhi = li.phi();
	    int phCluid  = vi->parentClusterId();
	    int phNcluph = vi->nParentClusterPhotons();
	    Float_t dphi = jetPhi - phPhi;
	    if(dphi> TMath::Pi()) dphi = -2*TMath::Pi() + dphi;
	    if(dphi<-TMath::Pi()) dphi =  2*TMath::Pi() + dphi;
	    Float_t deta = jetEta - phEta;
	    Float_t dR   = TMath::Sqrt( dphi*dphi+ deta*deta  );
	    if (dR<0.7)   {tagPH[ipha]=iJet ; ++phcount;}
 							
	}//end corelation with photon

	mjetnph=phcount;
	tFmsjet->Fill(); 
    }//jet loop

    //cout << "-------------> here <--------------------" <<endl;			
    //---------------------part4------------------// jet correlate with pion
    for (Int_t iJet = 0; iJet < mnjet; ++iJet)
    {
	for(Int_t i = 0; i < nfmsphotons; i++)
	{
	    if (tagPH[i] != iJet) continue;
	    for(Int_t j = i+1; j < nfmsphotons; j++)
	    {
		if (tagPH[j] != iJet) continue;
					   
		StFmsPoint* vi4 = (StFmsPoint*)mFmsphotons->At(i);
		//Float_t ei4 = vi4->energy();
		StFmsPoint* vj4 = (StFmsPoint*)mFmsphotons->At(j);
		//Float_t ej4 = vj4->energy();
		const StLorentzVectorF liraw = vi4->fourMomentum();
		const StLorentzVectorF ljraw = vj4->fourMomentum();
		Int_t ph1id4 = vi4->detectorId();
		Int_t ph2id4 = vj4->detectorId();

		StFmsDiPhoton* diphoton = new StFmsDiPhoton();
		//  	double gaini4 = EdepCor(ei4,ph1id4) ;
		//	double gainj4 = EdepCor(ej4,ph2id4) ;
		StLorentzVectorF li4 = liraw*1;
		StLorentzVectorF lj4 = ljraw*1;
				
		StLorentzVectorF lpair4 = li4 + lj4;
		if (lpair4.m()>0.3 || TMath::Abs(li4.e()-lj4.e())/(li4.e()+lj4.e())>ZrrCut) continue;
		if (lpair4.pseudoRapidity()>4 || lpair4.pseudoRapidity()<2.6) continue;

		diphoton->E12 = lpair4.e();
		diphoton->M12 = lpair4.m();
		diphoton->Eta = lpair4.pseudoRapidity ();
		diphoton->Phi = lpair4.phi();
		diphoton->Z12 = TMath::Abs(li4.e()-lj4.e())/(li4.e()+lj4.e());
		diphoton->E1 = li4.e() ;
		diphoton->E2 = lj4.e() ;
		diphoton->tag = iJet ;   //jettag
		diphoton->tagPH1 = i ;	//photon tag
		diphoton->tagPH2 = j ;
                	      	
		diphoton->catagory = 11 ; //correlated
		diphoton->Eta1 = li4.pseudoRapidity ();
		diphoton->Eta2 = lj4.pseudoRapidity ();
		diphoton->Phi1 = li4.phi();
		diphoton->Phi2 = lj4.phi();
		diphoton->Ph1id = ph1id4;
		diphoton->Ph2id = ph2id4;
		diphoton->Cluid1 = vi4->parentClusterId();
		diphoton->Ncluph1 = vi4->nParentClusterPhotons();
		diphoton->Cluid2 = vj4->parentClusterId();
		diphoton->Ncluph2 = vj4->nParentClusterPhotons();
		Int_t nstbi = ph1id4; //nstb starts from 1;
		Int_t nstbj = ph2id4; //nstb starts from 1;
		diphoton->Det12 = nstbj + (nstbi)*100; //same coding as PSU software    
		//opening angle         
		TVector3 pj4(lj4.px(),lj4.py(),lj4.pz());
		TVector3 pi4(li4.px(),li4.py(),li4.pz());
		diphoton->OpAngle = pj4.Angle(pi4);
		//    		diphoton->X1 = vi->x();
                //        	diphoton->X2 = vj->x();
                //        	diphoton->Y1 = vi->y();
                //        	diphoton->Y2 = vj->y();
                //      diphoton->momentum = lpair.vect(); 
		StThreeVectorD pimomentum;   pimomentum.set(lpair4.px(),lpair4.py(),lpair4.pz());
		diphoton->momentum = pimomentum;//use for collins
		mFmsdiphotons->Add(diphoton);
	    }//j
	}//i
    }//ijet

    //---------------------part5------------------//pion sorted by energy, and not using same photon 
    const  int ndiphotons = mFmsdiphotons ->GetEntries(); //for corelated diphotons
    //suppose photon only used once in every jet, so phtags would be unique
    //sorted diphoton
    Float_t farrangePHE[120];
    Int_t fsortPHE[120];
    if (ndiphotons>0)
    {
	for(Int_t idiph = 0; idiph < ndiphotons; idiph++)
	{
	    StFmsDiPhoton* fmsdiphoton = (StFmsDiPhoton*)mFmsdiphotons->At(idiph);
	    farrangePHE[idiph] = fmsdiphoton->E12;
	}
	TMath::Sort(ndiphotons,farrangePHE,fsortPHE);
    }	
    vector<int> used;//container to exclude the used photon
    for(Int_t idiph = 0; idiph < ndiphotons; idiph++)
    {
	mRank=-1;
	if (idiph==0)  mRank=1;
	StFmsDiPhoton* fmsdiphoton = (StFmsDiPhoton*)mFmsdiphotons->At(fsortPHE[idiph]);
	Float_t E12 = fmsdiphoton->E12;
	Float_t pT = 2*(fmsdiphoton->E12)*TMath::Exp(-(fmsdiphoton->Eta))/(1+TMath::Exp(-2*(fmsdiphoton->Eta)));
	Float_t piPhi = fmsdiphoton->Phi;
	Float_t piEta = fmsdiphoton->Eta;
	Float_t M12 = fmsdiphoton->M12;
	Float_t Z12 = fmsdiphoton->Z12;
	Float_t opangle = fmsdiphoton->OpAngle;
	StThreeVectorD piP= fmsdiphoton->momentum;
	int jettag = fmsdiphoton->tag  ;
	int ph1tag = fmsdiphoton->tagPH1  ;
	int ph2tag = fmsdiphoton->tagPH2  ;
	int picata = fmsdiphoton->catagory  ;//default 11
 
	int check=0;
	int sizeused = used.size();
	if (used.size()>0){
	    for (int iter =0; iter < sizeused; ++iter)
	    {
		if ( ph1tag==used[iter] || ph2tag==used[iter])  check=1; //photon used before
	    }
	}
	if (check==0 )
	{
	    used.push_back(ph1tag);
	    used.push_back(ph2tag);
	    picata=1;//accepted
	}
	if (check==1 )
	{//conflit with accepted pions
	    picata=2;//in a jet but not accepted
	}

	mLeadjet=1;
	StJetCandidate* jet = jetFMS->jet(jettag);
	if(jetlead[jettag]==1)   mLeadjet=1;      //leading jet
	Float_t jetE = jet->E();
	Float_t jetpt = jet->pt();
	Float_t jetEta = jet->eta();
	Float_t jetPhi = jet->phi();

	Float_t dphi = jetPhi - piPhi;
	if(dphi>TMath::Pi()) dphi = -2*TMath::Pi() + dphi;
	if(dphi<-TMath::Pi()) dphi = 2*TMath::Pi() + dphi;
	Float_t deta = jetEta - piEta;
	Float_t dR = TMath::Sqrt( dphi*dphi+ deta*deta  );
	Float_t Zem =  E12 /jetE;
	if (Zem > 1.001) cout<< " Zem larger than 1  "<<endl;
	//jt
	StThreeVectorF JetMomentum;
	JetMomentum.set( jet->momentum().x(),jet->momentum().y(),jet->momentum().z() );
	double jt=(JetMomentum.unit().cross(piP)).mag();
	//	cout<<"jt="<<JetMomentum.unit()<<" "<<piP<<" "<<jt<<endl;

	StThreeVectorD spinvec;
	StThreeVectorD spinvecY;
	StThreeVectorD spinvecN;  spinvecN.set(0,1,0);
	StThreeVectorD beamvec;   beamvec.set(0,0,1);
	StThreeVectorD beamvecY;  beamvecY.set(0,0,-1);//beam minus
	StThreeVectorD ZcrossJ=beamvec.cross(JetMomentum);//parton spin vector
	StThreeVectorD ZcrossJY=beamvecY.cross(JetMomentum);//parton spin vector
	StThreeVectorD Hbase=ZcrossJ.cross(JetMomentum);
	StThreeVectorD HbaseY=ZcrossJY.cross(JetMomentum);
	StThreeVectorD Sbase=ZcrossJ.cross(beamvec);
	StThreeVectorD SbaseY=ZcrossJY.cross(beamvecY);//unchange
	if (mspinbit==5) {  spinvec.set(0,1,0); spinvecY.set(0,1,0);}
	if (mspinbit==6) {  spinvec.set(0,1,0); spinvecY.set(0,-1,0);}
	if (mspinbit==9) {  spinvec.set(0,-1,0);spinvecY.set(0,1,0);}
	if (mspinbit==10) { spinvec.set(0,-1,0);spinvecY.set(0,-1,0); }
                         
	StThreeVectorD PiMomentum = fmsdiphoton->momentum;
	StThreeVectorD pivec=JetMomentum.cross(  PiMomentum.cross(JetMomentum) );
	double phiS ;
	if (  (Sbase.cross(spinvecN)).dot(beamvec)>0   ) phiS= spinvecN.angle(Sbase);//clockwise
	if (  (Sbase.cross(spinvecN)).dot(beamvec)<0   ) phiS=-spinvecN.angle(Sbase);
	double phiH ;
	if (  (Hbase.cross(pivec)).dot(JetMomentum)>0   ) phiH= pivec.angle(Hbase);
	if (  (Hbase.cross(pivec)).dot(JetMomentum)<0   ) phiH=-pivec.angle(Hbase);
	double phiSY ;
	if (  (Sbase.cross(spinvecN)).dot(beamvecY)>0   ) phiSY=spinvecN.angle(SbaseY);//clockwise
	if (  (Sbase.cross(spinvecN)).dot(beamvecY)<0   ) phiSY=-spinvecN.angle(SbaseY);
	double phiHY;
	if (  (HbaseY.cross(pivec)).dot(JetMomentum)>0   ) phiHY=pivec.angle(HbaseY);
	if (  (HbaseY.cross(pivec)).dot(JetMomentum)<0   ) phiHY=-pivec.angle(HbaseY);

	double PhiC =  phiS - phiH;
	double yPhiC = phiSY - phiHY;

	if (  PhiC< 0 - TMath::Pi())  PhiC =  2* TMath::Pi()+ PhiC;
	if (  PhiC> 0 + TMath::Pi())  PhiC = -2* TMath::Pi()+ PhiC;
	if ( yPhiC< 0 - TMath::Pi()) yPhiC =  2* TMath::Pi()+ yPhiC;
	if ( yPhiC> 0 + TMath::Pi()) yPhiC = -2* TMath::Pi()+ yPhiC;
	//		cout<<" S="<< phiS<<" SY="<< phiSY<<" H=   "<<phiH <<" C=  "<<PhiC<<" yPhiC="<<yPhiC<< endl; 	
						
	mphid1 = fmsdiphoton->Ph1id;
	mphid2 = fmsdiphoton->Ph2id;
	mpim = M12;
	mpiz = Z12;
	mpie = E12;
	mpipt = pT;
	mpieta = piEta;
	mpiphi = piPhi;
	mpidet12 =fmsdiphoton->Det12;
	mcluid1 = fmsdiphoton->Cluid1;
	mnclph1 = fmsdiphoton->Ncluph1;
	mcluid2 = fmsdiphoton->Cluid2;
	mnclph2 = fmsdiphoton->Ncluph2;
	mOpAngle = opangle;
	if(mcluid1==mcluid2&&mnclph1==2&&mnclph2==2)msamclu = 1;
	else msamclu = 0;

	mCorr=1;//correlated

	mphiC=PhiC;
	myphiC=yPhiC;
	mphiH=phiH;
	mphiS=phiS;
	mJeta=jetEta;
	mJphi=jetPhi;
	mJE=jetE;
	mJPt=jetpt;
	mZem=Zem;
	mjetdR=dR;
	mJt=jt;
	mtag=jettag;
	mph1tag=ph1tag;
	mph2tag=ph2tag;
	mcatagory=picata;

	tjetcor->Fill(); //filled per each pair
	

    }//part 5 finish 
				
    tevent->Fill();			
		    

    LOG_INFO <<"mFmsphotons->GetEntries() = " << mFmsphotons->GetEntries() << endm;
 
    return kStOk;
}

Int_t StFmsAnalysisMaker::Finish()
{
	
    mFile->cd();
	
    //	mTree->Write("",TObject::kOverwrite);
    //	mFmsdiphotons->Write();
    //	mFmsphotons->Write();
    hhitEvsid->Write();
    hhitAvsid->Write();
    hVertexZ->Write();
    hVertexZyuxi->Write();
    hnTower->Write();
	
    tjetcor->Write("",TObject::kOverwrite);
    tevent->Write("",TObject::kOverwrite);
    tphoton->Write("",TObject::kOverwrite);
    tFmsjet->Write("",TObject::kOverwrite);

    return kStOk;
}

//Fiducial volume definition from Jim Drachenberg
Bool_t StFmsAnalysisMaker::Fiducial(const StThreeVectorF photon,int mod) const
{   

    // define fiducial volume as one cell width from boundary
    float OutBoundx[4] = { 96.198-0, 96.198-0, 44.883-0,  44.883-0 };
    float InBoundx[4]  = {  3.206+0,  3.206+0,  2.841+0,   2.841+0 };
    float HiBoundy[4]  = { 95.894-0, 95.894-0, 44.5625-0, 44.5625-0};
    float LoBoundy[4]  = {-95.902+0,-95.902+0,-44.5625+0,-44.5625+0};

    if (
	((photon.x()>OutBoundx[mod])||(photon.x()<-1*OutBoundx[mod]))
	||((photon.x()<InBoundx[mod]) &&(photon.x()>-1*InBoundx[mod]))
	||((photon.y()>HiBoundy[mod]) ||(photon.y()<LoBoundy[mod]))
	)
    {
	//	LOG_INFO <<" FIDUCIAL: OUT_OF_BOUNDARy mod = "<<mod<<" coords: x = "<<photon.x()<<" y = "<<photon.y()<<endm;
	return false;
    }

    // define fiducial volume near spacers
    else  if (
	(((photon.y()>55.2100-0)||(photon.y()<-55.2180+0))&&((photon.x()>90.386-0)||(photon.x()<-90.386+0)))
	||(((photon.y()>61.0220-0)||(photon.y()<-61.0300+0))&&((photon.x()>84.574-0)||(photon.x()<-84.574+0)))
	||(((photon.y()>66.8340-0)||(photon.y()<-66.8420+0))&&((photon.x()>78.762-0)||(photon.x()<-78.762+0)))
	||(((photon.y()>72.6460-0)||(photon.y()<-72.6540+0))&&((photon.x()>7050-0)||(photon.x()<-7050+0)))
	||(((photon.y()>78.4580-0)||(photon.y()<-78.4660+0))&&((photon.x()>67.138-0)||(photon.x()<-67.138+0)))
	||(((photon.y()>84.2700-0)||(photon.y()<-84.2780+0))&&((photon.x()>61.326-0)||(photon.x()<-61.326+0)))
	||(((photon.y()>90.0820-0)||(photon.y()<-90.0900+0))&&((photon.x()>55.514-0)||(photon.x()<-55.514+0)))
	||(((photon.y()<23.73-0)&&(photon.y()>-23.73+0))&&((photon.x()<23.73-0)&&(photon.x()>-23.73+0))) //central hole, 1 cell width away
	/*		(((photon.y()>55.2100-2.9)||(photon.y()<-55.2180+2.9))&&((photon.x()>90.386-2.9)||(photon.x()<-90.386+2.9)))
			||(((photon.y()>61.0220-2.9)||(photon.y()<-61.0300+2.9))&&((photon.x()>84.574-2.9)||(photon.x()<-84.574+2.9)))
			||(((photon.y()>66.8340-2.9)||(photon.y()<-66.8420+2.9))&&((photon.x()>78.762-2.9)||(photon.x()<-78.762+2.9)))
			||(((photon.y()>72.6460-2.9)||(photon.y()<-72.6540+2.9))&&((photon.x()>72.950-2.9)||(photon.x()<-72.950+2.9)))
			||(((photon.y()>78.4580-2.9)||(photon.y()<-78.4660+2.9))&&((photon.x()>67.138-2.9)||(photon.x()<-67.138+2.9)))
			||(((photon.y()>84.2700-2.9)||(photon.y()<-84.2780+2.9))&&((photon.x()>61.326-2.9)||(photon.x()<-61.326+2.9)))
			||(((photon.y()>90.0820-2.9)||(photon.y()<-90.0900+2.9))&&((photon.x()>55.514-2.9)||(photon.x()<-55.514+2.9)))
			||(((photon.y()<23.73-1.9)&&(photon.y()>-23.73+1.9))&&((photon.x()<23.73-1.9)&&(photon.x()>-23.73+1.9))) //central hole, 1 cell width away
	*/	)
    {
	//	LOG_INFO<<"FIDUCIAL: NEAR_SPACER mod = "<<mod<<" coords: x = "<<photon.X()<<" y = "<<photon.Y()<<endm;
	return false;
    } 


    else return true;
}

 
Int_t StFmsAnalysisMaker::EdepCor( StFmsPoint* point )
{	
    if(!EnableEdepCor) return kStOk;
    if(!point){
	LOG_ERROR << "StFmsAnalysisMaker::EdepCor(),null  StFmsPoint" << endm;
	return kStErr;
    }
	
    Int_t phid = point->detectorId();
    //cout<<"cluster ID= "<<phid<<endl;
    //	Int_t nstb = Int_t((phid - 305)/20) + 1;
    Int_t nstb = phid;

    double nlgain = 1;
    double e0 = point->energy();
    if(nstb==8 || nstb==9)
    {		//large cells
			
	//		nlgain = EDepCorrection->Eval(20.)/EDepCorrection->Eval(e0); //large cells are calibrated at 25GeV (sym/asy. decayed eta?..)
	nlgain = EDepCorrection1->Eval(e0)/e0;	
    }
    else if(nstb==10 || nstb==11)
    {	//small cells
		
	//		nlgain = EDepCorrection->Eval(30.)/EDepCorrection->Eval(e0); //small cells are calibrated at 40GeV	
	nlgain = EDepCorrection2->Eval(e0)/e0;	
    }
    else
    {
	LOG_ERROR << "illegal nstb = " << nstb << endm;
    }

	
    const StLorentzVectorF ph4 = point->fourMomentum();
    StLorentzVectorF vret = ph4*nlgain;
    point->setFourMomentum(vret);
    point->setEnergy(vret.e());
    cout<<" inEdep "<<ph4.e()<<" "<<vret.e()<<" gain"<<nlgain<<endl;	
    return kStOk;
}


void StFmsAnalysisMaker::SetFmsPhotonEcut ( Float_t ecutsmall, Float_t ecutlarge )
{
	
    mFmsEcutSmall = ecutsmall;
    mFmsEcutLarge = ecutlarge;
}
Int_t StFmsAnalysisMaker::ReadBbcSlewing(const char *filename_bbc)
{
    // reading parameters for BBC slewing correction
    char s[100]; int iew, ipmt; float ca, cb, cc;
    FILE *pFile = fopen( filename_bbc, "read" );
    fgets( s, 100, pFile);

    for( int ew=0; ew<2; ew++ )
	for( int p=0; p<16; p++ )
	{
	    fscanf( pFile, " %d %d %f %f %f \n", &iew, &ipmt, &ca, &cb, &cc);
	    if( ew==iew && p+1==ipmt )
	    {
		mBbcSlew[ew][p][0] = ca;
		mBbcSlew[ew][p][1] = cb;
		mBbcSlew[ew][p][2] = cc;
	    }
	    else
	    {
		return kError;
	    }
	}
    fclose( pFile );
    //  cout << " BBC slewing: z(A+B/[C+adc])" << endl;
    for( int ew=0; ew<2; ew++ )
    {
	if( ew==0 ) cout << " East" << endl;
	if( ew==1 ) cout << " West" << endl;
	for( int p=0; p<16; p++ )
	{
	    cout << Form("PMT%2d - %7.2f %7.2f %7.2f ", p+1, mBbcSlew[ew][p][0], mBbcSlew[ew][p][1], mBbcSlew[ew][p][2]) << endl;
	}
    }
    return kStOK;
}

