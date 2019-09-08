#ifndef STMINITREEMAKER_HH
#define STMINITREEMAKER_HH

/***************************************************************************
 *
 * $Id: StMiniTreeMaker.h 2015/04/09  Exp $ 
 * StMiniTreeMaker - class to produce miniTree for mtd related analysis
 * Author: Shuai Yang
 *--------------------------------------------------------------------------
 *
 ***************************************************************************/

#include "StMaker.h"
#include "StMiniTreeStructure.h"

#include "StThreeVectorF.hh"
#include "TLorentzVector.h"

#include <vector>
#include <map>
#ifndef ST_NO_NAMESPACES
using std::vector;
#endif

class TH1D;
class TH2D;
class TString;
class TTree;
class TFile;

class StPicoDstMaker;
class StPicoDst;
class StPicoTrack;

#if !defined(ST_NO_TEMPLATE_DEF_ARGS) || defined(__CINT__)
typedef vector<Int_t> IntVec;
typedef vector<Double_t> DoubleVec;
#else
typedef vector<Int_t, allocator<Int_t>> IntVec;
typedef vector<Double_t, allocator<Double_t>> DoubleVec;
#endif

class StMiniTreeMaker : public StMaker {
	public:
		StMiniTreeMaker(const Char_t *name = "StMiniTreeMaker");
		~StMiniTreeMaker();

		Int_t    Init();
		Int_t    InitRun(const Int_t runNumber);
		Int_t    Make();
		Int_t    Finish();

		void	 setTriggerIDs(const IntVec triggerids);
		void     setUseDefaultVtx(const Bool_t flag); 
		void     setMaxVtxR(const Double_t max);
		void     setMaxVtxZ(const Double_t max);
		void     setMaxVzDiff(const Double_t max);
		void     setMinTrackPt(const Double_t min);
		void     setMaxTrackEta(const Double_t max);
		void     setMinNHitsFit(const Int_t min);
		void     setMinNHitsFitRatio(const Double_t min);
		void     setMinNHitsDedx(const Int_t min);
		void     setMaxDca(const Double_t max);
		void     setMaxnSigmaE(const Double_t max);
		void     setMaxBeta2TOF(const Double_t max);
		void     setFillHisto(const Bool_t fill);
		void     setFillTree(const Bool_t fill);
		void     setOutFileName(const TString name);
		void     setStreamName(const TString name);
		void     setPrintMemory(const Bool_t pMem);
		void     setPrintCpu(const Bool_t pCpu);
		void     setPrintConfig(const Bool_t print);

	protected:
		void     printConfig();
		void     bookTree();
		void     bookHistos();
		Bool_t   processPicoEvent();
Bool_t isValidTrack(StPicoTrack *pTrack, TVector3 vtxPos) const;


	private:
		StPicoDstMaker *mPicoDstMaker;
		StPicoDst      *mPicoDst;

		Bool_t         mPrintMemory;         // Flag to print out memory usage
		Bool_t         mPrintCpu;            // Flag to print out CPU usage
		Bool_t         mPrintConfig;         // Flag to print out task configuration
		TString        mStreamName;          // Data stream name
		Bool_t         mDefaultVtx;          // Use Default Vertex
		Bool_t         mSelectVtxRank;       // Vertex ranking > 0
		Double_t       mMaxVtxR;             // Maximum vertex r
		Double_t       mMaxVtxZ;             // Maximum vertex z
		Double_t       mMaxVzDiff;           // Maximum VpdVz-TpcVz 
		Double_t       mMinTrkPt;            // Minimum track pt
		Double_t       mMaxTrkEta;           // Maximum track eta
		Int_t          mMinNHitsFit;         // Minimum number of hits used for track fit
		Double_t       mMinNHitsFitRatio;    // Minimum ratio of hits used for track fit
		Int_t          mMinNHitsDedx;        // Minimum number of hits used for de/dx
		Double_t       mMaxDca;              // Maximum track dca
		Double_t       mMaxnSigmaE;          // Maximum nSigmaE cut
		Double_t       mMaxBeta2TOF;         // Maximum |1-1./beta| for TpcE

		Bool_t         mFillHisto;           // Flag of fill the histogram 
		Bool_t         mFillTree;            // Flag of fill the event tree
		TFile          *fOutFile;            // Output file
		TString        mOutFileName;         // Name of the output file 
		StEvtData      event;
		TTree          *mEvent;            // Pointer to the event tree

		IntVec         mTriggerIDs;

		//define histograms ongoing...
		TH1D           *hEvent;
		TH2D           *hVtxYvsVtxX;
		TH2D           *hVPDVzvsTPCVz;
		TH1D           *hVzDiff;


		ClassDef(StMiniTreeMaker, 1)
};

inline void	StMiniTreeMaker::setTriggerIDs(const IntVec triggerids) { mTriggerIDs = triggerids; }
inline void StMiniTreeMaker::setUseDefaultVtx(const Bool_t flag) { mDefaultVtx = flag; }
inline void StMiniTreeMaker::setMaxVtxR(const Double_t max) { mMaxVtxR = max; }
inline void StMiniTreeMaker::setMaxVtxZ(const Double_t max) { mMaxVtxZ = max; }
inline void StMiniTreeMaker::setMaxVzDiff(const Double_t max) { mMaxVzDiff = max; }
inline void StMiniTreeMaker::setMinTrackPt(const Double_t min){ mMinTrkPt = min;}
inline void StMiniTreeMaker::setMaxTrackEta(const Double_t max){ mMaxTrkEta = max; }
inline void StMiniTreeMaker::setMinNHitsFit(const Int_t min) { mMinNHitsFit = min; }
inline void StMiniTreeMaker::setMinNHitsFitRatio(const Double_t min) { mMinNHitsFitRatio = min; }
inline void StMiniTreeMaker::setMinNHitsDedx(const Int_t min) { mMinNHitsDedx = min; }
inline void StMiniTreeMaker::setMaxDca(const Double_t max) { mMaxDca = max; }
inline void StMiniTreeMaker::setMaxnSigmaE(const Double_t max) { mMaxnSigmaE = max; }
inline void StMiniTreeMaker::setMaxBeta2TOF(const Double_t max) { mMaxBeta2TOF = max; }
inline void StMiniTreeMaker::setFillHisto(const Bool_t fill) { mFillHisto = fill; }
inline void StMiniTreeMaker::setFillTree(const Bool_t fill) { mFillTree = fill; }
inline void StMiniTreeMaker::setOutFileName(const TString name) { mOutFileName = name; }
inline void StMiniTreeMaker::setStreamName(const TString name) { mStreamName = name; }
inline void StMiniTreeMaker::setPrintMemory(const Bool_t pMem) { mPrintMemory = pMem; }
inline void StMiniTreeMaker::setPrintCpu(const Bool_t pCpu) { mPrintCpu = pCpu; }
inline void StMiniTreeMaker::setPrintConfig(const Bool_t print) { mPrintConfig = print; }
#endif
