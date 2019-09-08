#include "headers.h"
#include "StMiniTreeMaker.h"


ClassImp(StMiniTreeMaker)

	//_____________________________________________________________________________
	StMiniTreeMaker::StMiniTreeMaker(const Char_t *name) : StMaker(name), mFillTree(1), mPrintConfig(1), mPrintMemory(0), mPrintCpu(0), mStreamName(""), fOutFile(0), mOutFileName(""), mEvent(0), mDefaultVtx(1), mSelectVtxRank(1), mMaxVtxR(1.e4), mMaxVtxZ(1.e4), mMaxVzDiff(1.e4), mMinTrkPt(0.2), mMaxTrkEta(2.5), mMinNHitsFit(12), mMinNHitsFitRatio(0.4), mMinNHitsDedx(10), mMaxDca(10.), mMaxnSigmaE(2.5), mMaxBeta2TOF(0.03)
{
	// default constructor

	// run17 st_physics 
	mTriggerIDs.clear();
	mTriggerIDs.push_back(570204);     // pp@510 BHT1*VPD30
	mTriggerIDs.push_back(570215);     // pp@510 BHT2*BBCMB 
	mTriggerIDs.push_back(570201);     // pp@510 BHT3 
}
//_____________________________________________________________________________
StMiniTreeMaker::~StMiniTreeMaker()
{
	// default destructor
}
//_____________________________________________________________________________
Int_t StMiniTreeMaker::Init()
{
	//	refMultCorr = CentralityMaker::instance()->getgRefMultCorr_VpdMBnoVtx()
	//	refMultCorr = new StRefMultCorr("grefmult_VpdMBnoVtx");

	cout<<"StMiniTreeMaker::Init()"<<endl;
	if(!mOutFileName.Length()){
		LOG_ERROR << "StMiniTreeMaker:: no output file specified for tree and histograms." << endm;
		return kStERR;
	}
	fOutFile = new TFile(mOutFileName.Data(),"recreate");
	LOG_INFO << "StMiniTreeMaker:: create the output file to store the tree and histograms: " << mOutFileName.Data() << endm;

	if(mFillTree)    bookTree();
	if(mFillHisto)    bookHistos();
	return kStOK;
}
//_____________________________________________________________________________
Int_t StMiniTreeMaker::InitRun(const Int_t runnumber)
{
	LOG_INFO<<"Grab runnumber: "<<runnumber<<endm;
	return kStOK;
}
//_____________________________________________________________________________
Int_t StMiniTreeMaker::Finish()
{
	if(fOutFile){
		fOutFile->cd();
		fOutFile->Write();
		fOutFile->Close();
		LOG_INFO << "StMiniTreeMaker::Finish() -> write out tree in " << mOutFileName.Data() << endm;
	}
	if(mPrintConfig) printConfig();
	return kStOK;
}
//_____________________________________________________________________________
Int_t StMiniTreeMaker::Make()
{
	memset(&event, 0, sizeof(event)); //initial the mEvtData structure

	StTimer timer;
	if(mPrintMemory) StMemoryInfo::instance()->snapshot();
	if(mPrintCpu)    timer.start();

	mPicoDstMaker = (StPicoDstMaker *)GetMaker("picoDst");
	if(Debug()){
		LOG_INFO<<"PicoDstMaker pointer: "<<mPicoDstMaker<<endm;
	}

	if(mPicoDstMaker){
		if(Debug()) LOG_INFO<<"Use Pico file as input"<<endm;
		mPicoDst = mPicoDstMaker->picoDst();
		if(!mPicoDst){
			LOG_WARN<<"No PicoDst !"<<endm;
			return kStOK;
		}
	}
	else{
		LOG_WARN<<"No StPicoDstMaker !"<<endm;
		return kStOK;
	}


	if(!processPicoEvent())  return kStOK;

	if(mFillTree) mEvent->Fill();

	if(mPrintMemory){
		StMemoryInfo::instance()->snapshot();
		StMemoryInfo::instance()->print();
	}

	if(mPrintCpu){
		timer.stop();
		LOG_INFO << "CPU time for StMiniTreeMaker::Make(): " 
			<< timer.elapsedTime() << "sec " << endm;
	}

	return kStOK;
}
//_____________________________________________________________________________
Bool_t StMiniTreeMaker::processPicoEvent()
{
	if(Debug()) LOG_INFO<<"processPicoEvent"<<endm;
	if(Debug()) LOG_INFO<<"trigger size="<<mTriggerIDs.size()<<endm;
	if(mFillHisto) hEvent->Fill(0.5);

	StPicoEvent *picoEvent = mPicoDst->event();
	if(!picoEvent){
		LOG_WARN<<"No event level information !"<<endm;
		return kFALSE;
	}

	Bool_t validTrigger = kFALSE;

	Int_t nTrigs = 0;
	Int_t htid =0;
	for(unsigned int i=0;i<mTriggerIDs.size();i++){
		if(picoEvent->isTrigger(mTriggerIDs[i])){
			validTrigger = kTRUE;
			htid += (1<i) ;
			nTrigs++;
		}
	}

	if(!validTrigger){
	//	return kFALSE;
	}

	event.runnumber          = picoEvent->runId();
	event.eventnumber        = picoEvent->eventId();
	event.nrefmult        = picoEvent->refMult();
	event.bbcRate        = picoEvent->BBCx();
	event.zdcRate        = picoEvent->ZDCx();
	event.bfield         = picoEvent->bField();
	event.vpdVz          = picoEvent->vzVpd();
	event.ranking        = picoEvent->ranking();
	event.HTID = htid;

	TVector3 vtxPos    = picoEvent->primaryVertex();
	event.vx        = vtxPos.x();
	event.vy        = vtxPos.y();
	event.vz        = vtxPos.z();
	if(Debug()){
		LOG_INFO<<"RunId: "<<event.runnumber<<endm;
		LOG_INFO<<"EventId: "<<event.eventnumber<<endm;
		LOG_INFO<<"mZDCX: "<<event.zdcRate<<endm;
		LOG_INFO<<"VPD Vz: "<<event.vpdVz<<" \tTPC Vz: "<<event.vz<<endm;
	}

	if(mFillHisto){
		hVtxYvsVtxX->Fill(event.vx, event.vy);
		hVPDVzvsTPCVz->Fill(event.vz, event.vpdVz);
		hVzDiff->Fill(event.vz - event.vpdVz);
	}

	if(TMath::Abs(vtxPos.x())<1.e-5 && TMath::Abs(vtxPos.y())<1.e-5 && TMath::Abs(vtxPos.z())<1.e-5) return kFALSE;
	if(mFillHisto) hEvent->Fill(7.5);
	if(sqrt(vtxPos.x()*vtxPos.x()+vtxPos.y()*vtxPos.y())>=mMaxVtxR) return kFALSE;
	if(mFillHisto) hEvent->Fill(8.5);
	if(TMath::Abs(vtxPos.z())>=mMaxVtxZ) return kFALSE;
	if(mFillHisto) hEvent->Fill(9.5);
	if(TMath::Abs(event.vz - event.vpdVz)>=mMaxVzDiff) return kFALSE;
	if(mFillHisto) hEvent->Fill(10.5);



	Int_t nNodes = mPicoDst->numberOfTracks();
	if(Debug()){
		LOG_INFO<<"# of global tracks in picoDst: "<<nNodes<<endm;
	}

	Short_t nTrks    = 0;
	event.npTracks = nNodes; 
	int ie =0;
	//fill electron candidate
	for(Int_t i=0;i<nNodes;i++){
		StPicoTrack *pTrack = mPicoDst->track(i);
		if(!pTrack) continue;
		if(!pTrack->isPrimary())continue;
		if(! isValidTrack(pTrack, vtxPos)) continue;

		Int_t bemcPidTraitsIndex = pTrack->bemcPidTraitsIndex();
		if(bemcPidTraitsIndex<0)continue;

		TVector3 pMom               = pTrack->pMom();
		event.ep[ie]=pMom.Mag(); 
		event.ept[ie]=pMom.Perp();
		event.ephi[ie]=pMom.Phi();
		event.eeta[ie]=pMom.PseudoRapidity();

		event.edca[ie]= (pTrack->origin()-vtxPos).Mag();
		event.emSigmaPion[ie]=pTrack->nSigmaPion();
		event.emSigmaKaon[ie]=pTrack->nSigmaKaon();
		event.emSigmaProton[ie]=pTrack->nSigmaProton();
		event.emSigmaElectron[ie]=pTrack->nSigmaElectron();

		StPicoBEmcPidTraits *bemcpidtraits = mPicoDst->bemcPidTraits(bemcPidTraitsIndex);
		if(!bemcpidtraits)continue;
		event.eenergy[ie]=bemcpidtraits->bemcE();
		event.eenergy0[ie]=bemcpidtraits->bemcE0();
		event.ephidist[ie]=bemcpidtraits->bemcPhiDist();
		event.ezdist[ie]=bemcpidtraits->bemcZDist();
		event.eneta[ie]=bemcpidtraits->bemcSmdNEta();
		event.enphi[ie]=bemcpidtraits->bemcSmdNPhi();
		event.eadc[ie] = bemcpidtraits->bemcAdc0();

		Int_t bTofPidTraitsIndex          = pTrack->bTofPidTraitsIndex();
		event.etofmatchflag[ie]     = -1; 
		event.eylocal[ie]        = -999;
		event.ezlocal[ie]        = -999;
		event.ebeta[ie]         = -999;

		if(bTofPidTraitsIndex>=0){
			StPicoBTofPidTraits *btofPidTraits = mPicoDst->btofPidTraits(bTofPidTraitsIndex);
			event.etofmatchflag[ie] = btofPidTraits->btofMatchFlag(); 
			event.eylocal[ie]    = btofPidTraits->btofYLocal();
			event.ezlocal[ie]    = btofPidTraits->btofZLocal();
			event.ebeta[ie]     = btofPidTraits->btofBeta();
		}

		event.echarge[ie]=pTrack->charge();
		event.etrackID[ie]=i;
		event.enftpts[ie]=pTrack->nHitsFit();
		event.endedxpts[ie]=pTrack->nHitsDedx();
		event.enhitsposs[ie]=pTrack->nHitsMax();
		ie++;
	}
	//if(ie<1)return kFALSE;
	event.neTracks = ie;

	// fill tpc track
	for(Int_t i=0;i<nNodes;i++){
		StPicoTrack *pTrack = mPicoDst->track(i);
		if(!pTrack) continue;
		if(!pTrack->isPrimary())continue;

		if(! isValidTrack(pTrack, vtxPos)) continue;

		event.trackID[nTrks]            = i;  
		event.charge[nTrks]           = pTrack->charge();
		TVector3 pMom               = pTrack->pMom();
		event.pt[nTrks]               = pMom.Perp();
		event.eta[nTrks]              = pMom.PseudoRapidity();
		event.phi[nTrks]              = pMom.Phi();

		event.nftpts[nTrks]         = pTrack->nHitsFit();
		event.nhitsposs[nTrks]        = pTrack->nHitsMax();
		event.ndedxpts[nTrks]        = pTrack->nHitsDedx();
		event.mSigmaElectron[nTrks]          = pTrack->nSigmaElectron();
		event.mSigmaPion[nTrks]          = pTrack->nSigmaPion();
		event.mSigmaKaon[nTrks]          = pTrack->nSigmaKaon();
		event.mSigmaProton[nTrks]          = pTrack->nSigmaProton();
		event.dca[nTrks]              = (pTrack->origin()-vtxPos).Mag();

		Int_t bTofPidTraitsIndex          = pTrack->bTofPidTraitsIndex();
		event.tofmatchflag[nTrks]     = -1; 
		event.ylocal[nTrks]        = -999;
		event.zlocal[nTrks]        = -999;
		event.beta[nTrks]         = -999;

		if(bTofPidTraitsIndex>=0){
			StPicoBTofPidTraits *btofPidTraits = mPicoDst->btofPidTraits(bTofPidTraitsIndex);
			event.tofmatchflag[nTrks] = btofPidTraits->btofMatchFlag(); 
			event.ylocal[nTrks]    = btofPidTraits->btofYLocal();
			event.zlocal[nTrks]    = btofPidTraits->btofZLocal();
			event.beta[nTrks]     = btofPidTraits->btofBeta();
		}
		nTrks++;

	}
	event.npTracks = nTrks; 

	//if(nTrks==0 ) return kFALSE;

	if(Debug()){
		LOG_INFO<<"# of primary tracks stored: "<<event.nprimary<<endm;
	}


	return kTRUE;
}
//_____________________________________________________________________________
Bool_t StMiniTreeMaker::isValidTrack(StPicoTrack *pTrack, TVector3 vtxPos) const
{
	Float_t pt  = pTrack->pMom().Perp();
	Float_t eta = pTrack->pMom().PseudoRapidity();
	//Float_t dca = pTrack->dca();
	//Float_t dca = (pTrack->dca()-vtxPos).mag();
	Float_t dca = (pTrack->origin()-vtxPos).Mag();

	if(pt<mMinTrkPt)                            return kFALSE;
	if(TMath::Abs(eta)>mMaxTrkEta)              return kFALSE;
	if(pTrack->nHitsFit()<mMinNHitsFit)         return kFALSE;
	if(pTrack->nHitsFit()*1./pTrack->nHitsMax()<mMinNHitsFitRatio) return kFALSE;
	//if(pTrack->nHitsDedx()<mMinNHitsDedx)       return kFALSE;
	if(dca>mMaxDca)                             return kFALSE;

	return kTRUE;
}
//_____________________________________________________________________________
void StMiniTreeMaker::bookTree()
{
	LOG_INFO << "StMiniTreeMaker:: book the event tree to be filled." << endm;

	mEvent = new TTree("EVENT","EVENT");
	mEvent->SetAutoSave(100000000); // 100 MB

	// event information
	mEvent->Branch("runnumber",&event.runnumber,"runnumber/I");
	mEvent->Branch("eventnumber",&event.eventnumber,"eventnumber/I");
	mEvent->Branch("nprimary",&event.nprimary,"nprimary/I");
	mEvent->Branch("nglobal",&event.nglobal,"nglobal/I");
	mEvent->Branch("nrefmult",&event.nrefmult,"nrefmult/I");
	mEvent->Branch("npTracks",&event.npTracks,"npTracks/I");
	mEvent->Branch("ngTracks",&event.ngTracks,"ngTracks/I");
	mEvent->Branch("neTracks",&event.neTracks,"neTracks/I");
	mEvent->Branch("HTID",&event.HTID,"HTID/I");
	mEvent->Branch("zdcRate",&event.zdcRate,"zdcRate/D");
	mEvent->Branch("bbcRate",&event.bbcRate,"bbcRate/D");
	mEvent->Branch("vx",&event.vx,"vx/F");
	mEvent->Branch("vy",&event.vy,"vy/F");
	mEvent->Branch("vz",&event.vz,"vz/F");
	mEvent->Branch("ranking",&event.ranking,"ranking/F");
	mEvent->Branch("bfield",&event.bfield,"bfield/F");
	mEvent->Branch("vpdVz",&event.vpdVz,"vpdVz/F");

	mEvent->Branch("ep",&event.ep,"ep[neTracks]/F");
	mEvent->Branch("ept",&event.ept,"ept[neTracks]/F");
	mEvent->Branch("ephi",&event.ephi,"ephi[neTracks]/F");
	mEvent->Branch("epathlength",&event.epathlength,"epathlength[neTracks]/F");
	mEvent->Branch("edca",&event.edca,"edca[neTracks]/F");
	mEvent->Branch("eeta",&event.eeta,"eeta[neTracks]/F");
	mEvent->Branch("ededx",&event.ededx,"ededx[neTracks]/F");
	mEvent->Branch("emSigmaPion",&event.emSigmaPion,"emSigmaPion[neTracks]/F");
	mEvent->Branch("emSigmaKaon",&event.emSigmaKaon,"emSigmaKaon[neTracks]/F");
	mEvent->Branch("emSigmaProton",&event.emSigmaProton,"emSigmaProton[neTracks]/F");
	mEvent->Branch("emSigmaElectron",&event.emSigmaElectron,"emSigmaElectron[neTracks]/F");
	mEvent->Branch("echarge",&event.echarge,"echarge[neTracks]/I");
	mEvent->Branch("etrackID",&event.etrackID,"etrackID[neTracks]/I");
	mEvent->Branch("enftpts",&event.enftpts,"enftpts[neTracks]/I");
	mEvent->Branch("endedxpts",&event.endedxpts,"endedxpts[neTracks]/I");
	mEvent->Branch("enhitsposs",&event.enhitsposs,"enhitsposs[neTracks]/I");

	mEvent->Branch("eenergy",&event.eenergy,"eenergy[neTracks]/F");
	mEvent->Branch("eenergy0",&event.eenergy0,"eenergy0[neTracks]/F");
	mEvent->Branch("ephidist",&event.ephidist,"ephidist[neTracks]/F");
	mEvent->Branch("ezdist",&event.ezdist,"ezdist[neTracks]/F");
	mEvent->Branch("eneta",&event.eneta,"eneta[neTracks]/I");
	mEvent->Branch("enphi",&event.enphi,"enphi[neTracks]/I");
	mEvent->Branch("edist",&event.edist,"edist[neTracks]/F");
	mEvent->Branch("eadc",&event.eadc,"eadc[neTracks]/I");
	mEvent->Branch("eemcmod",&event.eemcmod,"eemcmod[neTracks]/I");
	mEvent->Branch("eemceta",&event.eemceta,"eemceta[neTracks]/I");
	mEvent->Branch("eemcsub",&event.eemcsub,"eemcsub[neTracks]/I");
	mEvent->Branch("eemcx",&event.eemcx,"eemcx[neTracks]/D");
	mEvent->Branch("eemcy",&event.eemcy,"eemcy[neTracks]/D");
	mEvent->Branch("eemcz",&event.eemcz,"eemcz[neTracks]/D");
	mEvent->Branch("etofmatchflag",&event.etofmatchflag,"etofmatchflag[neTracks]/I");
	mEvent->Branch("etof",&event.etof,"etof[neTracks]/F");
	mEvent->Branch("etofpathlength",&event.etofpathlength,"etofpathlength[neTracks]/F");
	mEvent->Branch("ebeta",&event.ebeta,"ebeta[neTracks]/F");

	mEvent->Branch("p",&event.p,"p[npTracks]/F");
	mEvent->Branch("pt",&event.pt,"pt[npTracks]/F");
	mEvent->Branch("phi",&event.phi,"phi[npTracks]/F");
	mEvent->Branch("pathlength",&event.pathlength,"pathlength[npTracks]/F");
	mEvent->Branch("dca",&event.dca,"dca[npTracks]/F");
	mEvent->Branch("eta",&event.eta,"eta[npTracks]/F");
	mEvent->Branch("dedx",&event.dedx,"dedx[npTracks]/F");
	mEvent->Branch("mSigmaPion",&event.mSigmaPion,"mSigmaPion[npTracks]/F");
	mEvent->Branch("mSigmaKaon",&event.mSigmaKaon,"mSigmaKaon[npTracks]/F");
	mEvent->Branch("mSigmaProton",&event.mSigmaProton,"mSigmaProton[npTracks]/F");
	mEvent->Branch("mSigmaElectron",&event.mSigmaElectron,"mSigmaElectron[npTracks]/F");
	mEvent->Branch("charge",&event.charge,"charge[npTracks]/I");
	mEvent->Branch("trackID",&event.trackID,"trackID[npTracks]/I");
	mEvent->Branch("nftpts",&event.nftpts,"nftpts[npTracks]/I");
	mEvent->Branch("ndedxpts",&event.ndedxpts,"ndedxpts[npTracks]/I");
	mEvent->Branch("nhitsposs",&event.nhitsposs,"nhitsposs[npTracks]/I");

	mEvent->Branch("tofmatchflag",&event.tofmatchflag,"tofmatchflag[npTracks]/I");
	mEvent->Branch("tof",&event.tof,"tof[npTracks]/F");
	mEvent->Branch("tofpathlength",&event.tofpathlength,"tofpathlength[npTracks]/F");
	mEvent->Branch("beta",&event.beta,"beta[npTracks]/F");

	//AddObj(mEvent,".hist");
}
void StMiniTreeMaker::bookHistos()
{
	hEvent = new TH1D("hEvent","Event statistics",25,0,25);
	hVtxYvsVtxX = new TH2D("hVtxYvsVtxX","hVtxYvsVtxX; V_{x} (cm); V_{y} (cm)",120,-3,3,120,-3,3);
	hVPDVzvsTPCVz = new TH2D("hVPDVzvsTPCVz","hVPDVzvsTPCVz; TPC V_{z} (cm); VPD V_{z} (cm)",200,-50,50,200,-50,50);
	hVzDiff = new TH1D("hVzDiff","hVzDiff; TPC V_{z} - VPD V_{z} (cm)",80,-20,20);
}
//_____________________________________________________________________________
void StMiniTreeMaker::printConfig()
{
	const char *decision[2] = {"no","yes"};
	printf("=== Configuration for StMiniTreeMaker ===\n");
	printf("Fill the miniDst tree: %s\n",decision[mFillTree]);
	printf("Fill the QA histo: %s\n",decision[mFillHisto]);
	printf("Use default vertex: %s\n",decision[mDefaultVtx]);
	printf("Select positive vertex ranking: %s\n",decision[mSelectVtxRank]);
	printf("Maximum |Vr|: %1.2f\n",mMaxVtxR);
	printf("Maximum |Vz|: %1.2f\n",mMaxVtxZ);
	printf("Maximum |VzDiff|: %1.2f\n",mMaxVzDiff);
	printf("Minimum track pt: %1.2f\n",mMinTrkPt);
	printf("Maximum track |eta| : %1.2f\n",mMaxTrkEta);
	printf("Minimum number of fit hits: %d\n",mMinNHitsFit);
	printf("Minimum ratio of fit hits: %1.2f\n",mMinNHitsFitRatio);
	printf("Minimum number of dedx hits: %d\n",mMinNHitsDedx);
	printf("Maximum dca: %1.2f\n",mMaxDca);
	printf("Maximum |nSigmaE| for TPCe: %1.2f\n",mMaxnSigmaE);
	printf("Maximum |1-1/beta| for TPCe: %1.2f\n",mMaxBeta2TOF);
	printf("=======================================\n");
}
