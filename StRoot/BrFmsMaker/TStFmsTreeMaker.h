#ifndef TSTFMSTREEMAKER_H
#define TSTFMSTREEMAKER_H

#include "StMaker.h"

class StFmsCollection;
class StFmsDbMaker;
class StTriggerId;

class TFile;
class TTree;
class TH1F;
class TH2F;
class TH3F;

class TStFmsTreeMaker : public StMaker
{
public:

    TStFmsTreeMaker(const char* name = "fmsTreeMaker");
    ~TStFmsTreeMaker() {}

    virtual Int_t Init();
    virtual Int_t InitRun(int runNo);
    virtual Int_t Make();
    virtual Int_t Finish();

    virtual void GetMap(void) { mGetMap = true; }
    virtual void GetQaTree(void) { mGetQaTree = true; }
    virtual void ResetLeaves(void);
    virtual void SetOutputName(const char* name) { mOutputName = name; }
    virtual void SetReadBadChannel(const char* list);
    virtual Int_t CheckFmsTrig(const StTriggerId& trigId);

private:

    StFmsCollection* mFmsColl;
    StFmsDbMaker*    mFmsDbMk;
    TFile*           mFile;
    TTree*           mTree;

    bool mGetMap    = false; //Get FMS maps for calibFms: FmsMapBase, FmsBitShift 
    bool mGetQaHist = false; //Get QA histograms
    bool mGetQaTree = false; //Get QA tree
    bool mReadBadCh = false; //Read bad channels from external file
    std::map<int, int> mBadCh[4]; //0 for good, 1 for bad, 2 for dead, and 9 for converged

    const char* mOutputName = "fmsTree.root";
    const int mTrigIdBase = 480800; //RUN15pp200
    const int nTrigFms = 13;
    int mEvent = 0;
    int mTrig = 0;
    int mXing = 0;

    //Histograms
    TH2F* mH2_mass[4];     //Essential, default
    TH2F* mH2_massFine[4]; //Essential, more bins
    TH2F* mH2_massWide[4]; //QA, 0 < mass < 1 (GeV)

    //Tree
    static const short mHitMax = 250; //All hit, if reconstructed mass filled to any channel

    int   mTrigBit;
    float mMass;
    float mOpenA;
    float mZgg;
    
    float mPi0Px;
    float mPi0Py;
    float mPi0Pz;
    float mPi0E;
    float mPi0Pt;
    float mPi0X;
    float mPi0Y;

    float mPointPx[2];
    float mPointPy[2];
    float mPointPz[2];
    float mPointE[2];
    float mPointX[2];
    float mPointY[2];

    short mCluTowers[2];
    float mCluX[2];
    float mCluY[2];
    float mCluSigmaMax[2];
    float mCluSigmaMin[2];
    
    short mHit;
    short mCh[mHitMax];
    short mDet[mHitMax];
    short mPtb[mHitMax]; //The point this hit belong: only 0 or 1
    short mTdc[mHitMax];
    float mHitE[mHitMax];
    
    ClassDef(TStFmsTreeMaker, 1.0);
};

#endif
