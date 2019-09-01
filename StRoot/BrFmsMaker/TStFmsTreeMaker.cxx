#include "TStFmsTreeMaker.h"

#include "Stypes.h"
#include "StMessMgr.h"
#include "StLorentzVectorF.hh"

#include "StEnumerations.h"
#include "StEventTypes.h"
#include "StEvent/StEvent.h"
#include "StEvent/StFmsCollection.h"
#include "StEvent/StFmsHit.h"
#include "StEvent/StFmsPoint.h"
#include "StEvent/StFmsPointPair.h"
#include "StEvent/StTriggerData.h"
#include "StEvent/StTriggerId.h"
#include "StFmsDbMaker/StFmsDbMaker.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "Stypes.h"

#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TString.h>

#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;
ClassImp(TStFmsTreeMaker);
//-------------------------------------
void TStFmsTreeMaker::ResetLeaves(void)
{
    mHit = 0;
    for (int i=0; i<mHitMax; i++)
    {
	mCh[i]   = -999;
	mDet[i]  = -999;
	mPtb[i]  = -999;
	mTdc[i]  = -999;
	mHitE[i] = -999.;
    }

    for (int i=0; i<2; i++)
    {
	mCluTowers[i]	= -999;
	mCluX[i]		= -999.;
	mCluY[i]		= -999.;
	mCluSigmaMax[i] = -999.;
	mCluSigmaMin[i] = -999.;
	mPointE[i] = -999.;
	mPointX[i] = -999.;
	mPointY[i] = -999.;
    }

    mTrigBit = -999;
    mMass    = -999.;
    mOpenA   = -999.;
    mZgg     = -999.;

    return;
}//ResetDummyIndices

//-------------------------------------------------------
void TStFmsTreeMaker::SetReadBadChannel(const char* list)
{
    mReadBadCh = true;
    cout <<Form("Reading bad channels from %s...", list) <<endl;

    int detId, ch, bad, nBad = 0;
    ifstream in;
    in.open(list);
    if (!in.is_open())
    {
	LOG_ERROR <<"TStFmsTreeMaker::ReadBadCh - cannot open the bad channel list!\n" <<endm;
	return;
    }
    while (in.is_open())
    {
	in >> detId >> ch >> bad; //0 for good, 1 for bad, and 9 for converged
	if (!in.good()) break;

	mBadCh[detId-8].insert(std::pair<int, int>(ch, bad));
	if (bad==1) { cout <<Form("%2i %3i, %i", detId, ch, bad) <<endl; nBad++; }
    }
    in.close();
    cout <<"Total # of bad marked channels: " <<nBad <<endl <<endl;

    return;
}//SetReadBadChannel

//------------------------------------------------------------
Int_t TStFmsTreeMaker::CheckFmsTrig(const StTriggerId& trigId)
{
    int trigFired = 0;

    //Loop over RUN15 FMS triggers
    for (int a=0; a<4; a++) //4 periods
	for (int b=0; b<3; b++) //3 possible versions
	    for (int c=0; c<nTrigFms; c++) //13 triggers: SmBS1,SmBS2,SmBS3, LgBS1,LgBS2,LgBS3, DiBS, JP2,JP1,JP0,DiJP, "",LED
	    {
		int tempId = mTrigIdBase + 10000*a + 20*b + c+1;
		if (trigId.isTrigger(tempId)) trigFired |= (1 << c);
	    }

    return trigFired;
}//GetFmsTrigId

//===================================================================
TStFmsTreeMaker::TStFmsTreeMaker(const char* name) : StMaker(name) {}

//===========================
Int_t TStFmsTreeMaker::Init()
{
    mFmsDbMk = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    if (!mFmsDbMk) { LOG_ERROR <<"TStFmsTreeMaker::InitRun - !StFmsDbMaker" <<endl; return kStFatal; }

    return kStOk;
}//Init

//=======================================
Int_t TStFmsTreeMaker::InitRun(int runNo)
{
    Info("InitRun", "Start run %d...", runNo);

    mFile = new TFile(mOutputName, "RECREATE");
    mFile->SetCompressionLevel(9);
    mFile->cd();

    //Check if masked out cells still alive
    for (int a=0; a<4; a++)
    {
	const int detId = a+8;
	const int maxCh = mFmsDbMk->maxChannel(detId);

	//Printout gain correction factors currently being used
	for (int b=0; b<maxCh; b++)
	{
	    const int ch = b+1;
	    const float gainCorr = mFmsDbMk->getGainCorrection(detId, ch);

	    const char* headUp = (mBadCh[a][ch]!=0 && mBadCh[a][ch]!=9)?Form("! TYPE: %i", mBadCh[a][ch]):"";
	    cout <<Form("%2i %3i %4.3f %s", detId, ch, gainCorr, headUp) <<endl;
	    if (mReadBadCh && mBadCh[a][ch]==1 && gainCorr!=0.000)
	    {
		cout <<Form("\nWARNING!!! Bad marked channel's gainCorr is alive: d%i ch%i\n", a+8, ch) <<endl;
		return kStStop;
	    }
	}//b, ch
    }//a, detId

    if (mGetMap)
    {
	ofstream out1, out2;
	out1.open("FmsMapBase.txt");
	out2.open("FmsMapBitShift.txt");
	for (int a=0; a<4; a++)
	{
	    const int detId = a+8;
	    const int maxCh = mFmsDbMk->maxChannel(detId);
	    for (int b=0; b<maxCh; b++)
	    {
		const int ch  = b+1;
		const int bs  = mFmsDbMk->getBitShiftGain(detId, ch);
		const int col = mFmsDbMk->getColumnNumber(detId, ch);
		const int row = mFmsDbMk->getRowNumber(detId, ch);
		const float chX = mFmsDbMk->getStarXYZ(detId, ch).x();
		const float chY = mFmsDbMk->getStarXYZ(detId, ch).y();
		const float gain = mFmsDbMk->getGain(detId, ch);
		out1 <<Form("%2i %3i %4.3f %2i %2i %6.2f %6.2f", detId, ch, gain, col, row, chX, chY) <<endl;
		out2 <<Form("%2i %3i %2i", detId, ch, bs) <<endl;
	    }//b, ch
	}//a, detId
	out1.close();
	out2.close();
    }

    //Histograms
    for (int a=0; a<4; a++)
    {
	const int detId = a+8;
	const int maxCh = mFmsDbMk->maxChannel(detId);

	//Mass, essential
	mH2_mass[a] = new TH2F(Form("mass_d%i", detId), ";ch;mass", maxCh,0.5,maxCh+0.5, 50,0.,0.5);
	mH2_mass[a]->Sumw2();
	mH2_massFine[a] = new TH2F(Form("mass_d%i_fine", detId), ";ch;mass", maxCh,0.5,maxCh+0.5, 250,0.,0.5);
	mH2_massFine[a]->Sumw2();
	mH2_massWide[a] = new TH2F(Form("mass_d%i_wide", detId), ";ch;mass", maxCh,0.5,maxCh+0.5, 100,0.,1.0);
	mH2_massWide[a]->Sumw2();
    }//a, detId

    //QA tree
    if (mGetQaTree)
    {
	cout <<Form("\nCreating a tree for FMS: large disk space will be used...") <<endl;

	int iLeaf = 0;
	mTree = new TTree("T", "FMS Reconstruction Tree");
	mTree->Branch("hits", 0, "nHit/s:detId[nHit]/s:ch[nHit]/s:pointB[nHit]/s:tdc[nHit]/s:hitE[nHit]/F");
	((TLeaf*)mTree->GetBranch("hits")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mHit);  iLeaf++;
	((TLeaf*)mTree->GetBranch("hits")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mDet);  iLeaf++;
	((TLeaf*)mTree->GetBranch("hits")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mCh);   iLeaf++;
	((TLeaf*)mTree->GetBranch("hits")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mPtb);  iLeaf++;
	((TLeaf*)mTree->GetBranch("hits")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mTdc);  iLeaf++;
	((TLeaf*)mTree->GetBranch("hits")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mHitE); iLeaf=0;
	mTree->Branch("clusters", 0, "cluTowers[2]/s:cluX[2]/F:cluY[2]/F:cluMax[2]/F:cluMin[2]/F");
	((TLeaf*)mTree->GetBranch("clusters")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mCluTowers);   iLeaf++;
	((TLeaf*)mTree->GetBranch("clusters")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mCluX);        iLeaf++;
	((TLeaf*)mTree->GetBranch("clusters")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mCluY);        iLeaf++;
	((TLeaf*)mTree->GetBranch("clusters")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mCluSigmaMax); iLeaf++;
	((TLeaf*)mTree->GetBranch("clusters")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mCluSigmaMin); iLeaf=0;
	mTree->Branch("points", 0, "pointE[2]/F:pointX[2]/F:pointY[2]/F");
	((TLeaf*)mTree->GetBranch("points")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mPointE); iLeaf++;
	((TLeaf*)mTree->GetBranch("points")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mPointX); iLeaf++;
	((TLeaf*)mTree->GetBranch("points")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mPointY); iLeaf=0;
	mTree->Branch("pair", 0, "trig/I:mass/F:openA/F:zgg/F");
	((TLeaf*)mTree->GetBranch("pair")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mTrigBit); iLeaf++;
	((TLeaf*)mTree->GetBranch("pair")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mMass);    iLeaf++;
	((TLeaf*)mTree->GetBranch("pair")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mOpenA);   iLeaf++;
	((TLeaf*)mTree->GetBranch("pair")->GetListOfLeaves()->At(iLeaf))->SetAddress(&mZgg);     iLeaf=0;

	ResetLeaves();
    }

    return kStOk;
}//InitRun

//===========================
Int_t TStFmsTreeMaker::Make()
{
    mEvent++;
    if (mEvent%1000 == 0) cout <<Form("%5i processed...", mEvent) <<endl;

    StMuDst* muDST = (StMuDst*)GetInputDS("MuDst");
    if (!muDST) { LOG_ERROR <<"TStFmsTreeMaker::Make - !MuDst" <<endl; return kStErr; }
    else
    {
	//Skip events in abort gap
        mXing = muDST->event()->triggerData()->bunchId7Bit();
	if ((mXing>=30 && mXing<40) || (mXing>=110 && mXing<120)) return kStSkip;

	//Check FMS trigger, skip invalid cases
        mTrig = CheckFmsTrig(muDST->event()->triggerIdCollection().nominal());
        if (mTrig==0) { cout <<"No FMS trigger fired!" <<endl; return kStSkip; }
	for (int i=0; i<nTrigFms; i++)
	{
	    bool mTrigFire = false;
	    if (mTrig & (1<<i)) mTrigFire = true;
	    if (i==11 && mTrigFire==true) return kStSkip; //Empty trigger
	    if (i==12 && mTrigFire==true) return kStSkip; //LED
	}
    }

    StEvent* event = (StEvent*)GetInputDS("StEvent");
    if (!event) { LOG_ERROR <<"TStFmsTreeMaker::Make - !StEvent" <<endl; return kStErr; }
    else mFmsColl = (StFmsCollection*) event->fmsCollection();

    //Point pairs
    //-------------------------------------------

    vector<StFmsPointPair*>& PAIRS = mFmsColl->pointPairs();
    const int nPAIRS = mFmsColl->numberOfPointPairs();
    for (int a=0; a<nPAIRS; a++)
    {
	//Access pairs
	StFmsPointPair* pair = PAIRS[a];
	if (pair->energy() < 20.) continue;
	if (pair->mass()   > 1.0) continue;
	if (pair->zgg()    > 0.7) continue;

	//Single point clusters only
        if (pair->point(0)->nParentClusterPhotons() != 1 ||
	    pair->point(1)->nParentClusterPhotons() != 1) continue;

	const float pairE = pair->energy();
	const float pairM = pair->mass();

	//-----------------------------------

	bool fillQaTree = false;

	//Access clusters
	for (int b=0; b<2; b++)
	{
	    StFmsCluster* cluster = pair->point(b)->cluster();

            //Cluster quality cut for small cells
            bool cutClu = false;
            const int   nCells = cluster->nTowers();
            const float sigMax = cluster->sigmaMax();
            const float sigMin = cluster->sigmaMin();
            if (nCells>=4 && sigMax>0.1 && sigMin>0.1) cutClu = true;

	    StPtrVecFmsHit* hits = &(cluster->hits());
	    const int nHits = hits->size();
	    for (int c=0; c<nHits; c++)
	    {
		const int   tempDet = hits->at(c)->detectorId();
		const int   tempCh  = hits->at(c)->channel();
		const float tempE   = hits->at(c)->energy();
		if (tempDet<8 || tempDet>11) { cout <<Form("detId = %i???", tempDet) <<endl; continue; }

		//Apply cluster quality cut on small cells
		if (tempDet>=10 && cutClu==false) continue;

		//Fill histograms
		if (tempE/pairE>0.3 && pairE<40 && pairM<0.5)
		{
		    mH2_mass    [tempDet-8]->Fill(tempCh, pairM);
		    mH2_massFine[tempDet-8]->Fill(tempCh, pairM);
		}
		if (tempE/pairE>0.3 && pairE<40) mH2_massWide[tempDet-8]->Fill(tempCh, pairM);
	    }//c, loop over hits

	    if (mGetQaTree)
	    {
		//Tree, cluster and point
		mCluX[b]        = cluster->x();
		mCluY[b]        = cluster->y();
		mCluTowers[b]   = cluster->nTowers();
		mCluSigmaMax[b] = cluster->sigmaMax();
		mCluSigmaMin[b] = cluster->sigmaMin();
		mPointE[b] = pair->point(b)->energy();
		mPointX[b] = pair->point(b)->XYZ().x();
		mPointY[b] = pair->point(b)->XYZ().y();

		//Tree, hit
		for (int c=0; c<nHits; c++)
		{
		    const int   tempDet = hits->at(c)->detectorId();
		    const int   tempCh  = hits->at(c)->channel();
		    const int   tempTdc = hits->at(c)->tdc();
		    const float tempE   = hits->at(c)->energy();
		    if (tempDet<8 || tempDet>11) { cout <<Form("detId = %i???", tempDet) <<endl; continue; }
		    if (mHit > mHitMax) { LOG_ERROR <<"WARNING! nHit exceeds allowed maximum!" <<endl; continue; }

		    //Apply cluster quality cut on small cells
		    //if (tempDet>=10 && cutClu==false) continue;

		    mCh[mHit]   = tempCh;
		    mDet[mHit]  = tempDet;
		    mHitE[mHit] = tempE;
		    mPtb[mHit]  = b;
		    mTdc[mHit]  = tempTdc;
		    mHit++;

		    if (pairE<60 && pairM<0.5) fillQaTree = true;
		}//c, loop over hits
	    }//QaTree

	}//b, loop over clusters

	if (mGetQaTree && mHit!=0 && fillQaTree) //Tree, points and mass, and fill
	{
	    mTrigBit = mTrig;
	    mMass    = pairM;
	    mZgg     = pair->zgg();

	    StLorentzVectorF v0 = pair->point(0)->fourMomentum();
	    StLorentzVectorF v1 = pair->point(1)->fourMomentum();
	    mOpenA = acos((v0.px()*v1.px()+v0.py()*v1.py()+v0.pz()*v1.pz()) / (mPointE[0]*mPointE[1]));

	    mTree->Fill();
	}
	ResetLeaves();

    }//a, loop over pairs

    return kStOk;
}//Make

//=============================
Int_t TStFmsTreeMaker::Finish()
{
    mFile->Write();
    mFile->Close();
    return kStOK;
}//Finish
