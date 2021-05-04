/*
  2015 version of JetMaker:
  - Derivated from 2012 version by Jilong
  - Minor updates applied, mainly for FMS

  //Following are by Latif
  - Enable using EEMC SMD photon candidates instead of EEMC towers
  - In EM-jet mode, do not merge charged particles from TPC 
*/

#include "TFile.h"
#include "TTree.h"

#include "StAnaPars.h"
#include "StBTofHeader.h"
#include "StEvent/StTriggerData.h"
#include "StMcTrackEmu.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuTypes.hh"
#include "StMuTrackFourVec.h"
#include "StMuTrackEmu.h"
#include "StMuTowerEmu.h"
#include "StJetFinder/StJetFinder.h"
#include "StSpinPool/StJetEvent/StJetEventTypes.h"
#include "StThreeVector.hh"

#include "StjAbstractTowerEnergyCorrectionForTracks.h"
#include "StjBEMCNull.h"
#include "StjBEMCMuDst.h"
#include "StjEEMCNull.h"
#include "StjEEMCMuDst.h"
#include "StjEEmcSmdMuDst.h"
#include "StjFMSNull.h"
#include "StjFMSMuDst.h"
#include "StjMCParticleToStMuTrackFourVec.h"
#include "StjMCMuDst.h"
#include "StjTPCNull.h"
#include "StjTPCMuDst.h"
#include "StjeTowerEnergyListToStMuTrackFourVecList.h"
#include "StjeTrackListToStMuTrackFourVecList.h"

#include "StSpinPool/StUeEvent/StUeOffAxisConesEvent.h"
#include "StSpinPool/StUeEvent/StUeVertex.h"
#include "StSpinPool/StUeEvent/StUeOffAxisConesJet.h"
#include "StSpinPool/StUeEvent/StUeOffAxisCones.h"

#include "StSpinPool/StFmsTriggerMaker/StFmsTriggerMaker.h"
#include "StTriggerDataMaker/StTriggerDataMaker.h"

#include "StJetMaker2015.h"
ClassImp(StJetMaker2015);

Int_t StJetMaker2015::Init()
{
    return StJetMaker2012::Init();
}//Init

Int_t StJetMaker2015::Finish()
{
    return StJetMaker2012::Finish();
}//Finish

Int_t StJetMaker2015::Make()
{
    mEvent++;
    if (mEvent%1000 == 0) cout <<Form("%5i processed...", mEvent) <<endl;

    Int_t triggerBit = 0;
    if(mIsMCmode)
    {
	//---------------------- FMS Trigger Simulator ---------------------------------------
	StFmsTriggerMaker* fmstrig = (StFmsTriggerMaker*)GetMakerInheritsFrom("StFmsTriggerMaker");
	assert(fmstrig);

	Int_t trg1 = 0; 
	Int_t trg2 = 0 ; 
	Int_t trg3 = 0; 
	Int_t trg4 = 0; 
	Int_t trg5 = 0; 
	Int_t trg6 = 0 ;
	Int_t trg7 = 0; 
	Int_t trg8 = 0; 
	Int_t trg9 = 0 ;
  
	int FP201 = fmstrig->FP201output();
	int HT  = FP201 & 0x3;
	int SBS = (FP201>>2) & 0x7;
	int LBS = (FP201>>5) & 0x7;
	int JP  = (FP201>>8) & 0x7;
  
	if( fmstrig->FmsSmallClusterTh0())  trg1 = 1;
	if( fmstrig->FmsSmallClusterTh1())  trg2 = 1;
	if( fmstrig->FmsSmallClusterTh2())  trg3 = 1;
	if( fmstrig->FmsLargeClusterTh0())  trg4 = 1;
	if( fmstrig->FmsLargeClusterTh1())  trg5 = 1;
	if( fmstrig->FmsLargeClusterTh2())  trg6 = 1;
	if( fmstrig->FmsJetPatchTh0())  trg7 = 1;
	if( fmstrig->FmsJetPatchTh1())  trg8 = 1;
	if( fmstrig->FmsJetPatchTh2())  trg9 = 1;
	fmstrig->Clear();
   
	triggerBit = 1000000000 + trg1*100000000 + trg2*10000000 + trg3*1000000 + trg4*100000 + trg5*10000 + trg6*1000 + trg7*100 + trg8*10 + trg9;

	cout<<" TRIGGER : SmBS 1 2 LgBS 1 2 JP 1 2   : "<<trg1<<"  "<<trg2<<"  "<<trg3<<"  "<<trg4<<"  "<<trg5<<"  "<<trg6<<"  "<<trg7<<"  "<<trg8<<"  "<<trg9<<"  "<<triggerBit<<endl;
    }
    
    //Loop over jet branches
    for (size_t iBranch = 0; iBranch < mJetBranches.size(); ++iBranch)
    {
	StJetBranch* jetbranch = mJetBranches[iBranch];

	//Fill header
	jetbranch->event->mRunId   = GetRunNumber();
	jetbranch->event->mEventId = GetEventNumber();
	jetbranch->event->mDatime  = GetDateTime();
	if(mIsMCmode) jetbranch->event->mRunId = triggerBit;
	
	if (jetbranch->anapars->useTpc)
	{
	    StjTPCMuDst tpc;

	    //Save vertex index
	    int savedVertexIndex = tpc.currentVertexIndex();

	    //Keep track of number of good vertices
	    int nvertices = 0;

	    //Vertex loop
	    for (int iVertex = 0; iVertex < tpc.numberOfVertices(); ++iVertex)
	    {
		tpc.setVertexIndex(iVertex);

		//Get TPC vertex and tracks
		StjPrimaryVertex vertex = tpc.getVertex();

		//Skip pile-up vertex
		if (vertex.ranking() <= 0) continue;

		//Found good vertex
		++nvertices;
		StjTrackList trackList = tpc.getTrackList();
		if (jetbranch->anapars->changeTracks) trackList = (*jetbranch->anapars->changeTracks)(trackList);
		trackList = jetbranch->anapars->tpcCuts()(trackList);

		//Get BEMC towers
		StjTowerEnergyList bemcEnergyList;
		if (jetbranch->anapars->useBemc)
		{
		    StjBEMCMuDst bemc;
		    bemcEnergyList = bemc.getEnergyList();
		    if (jetbranch->anapars->changeTowers) (*jetbranch->anapars->changeTowers)(bemcEnergyList);
		    bemcEnergyList = jetbranch->anapars->bemcCuts()(bemcEnergyList);
		}

		//Get EEMC towers
		StjTowerEnergyList eemcEnergyList;
		if (jetbranch->anapars->useEemc)
		{
		    StjEEMCMuDst eemc;
		    eemcEnergyList = eemc.getEnergyList();
		    if (jetbranch->anapars->changeTowers) (*jetbranch->anapars->changeTowers)(eemcEnergyList);
		    eemcEnergyList = jetbranch->anapars->eemcCuts()(eemcEnergyList);
		}
		else if(!jetbranch->anapars->useEemc && jetbranch->anapars->useEemcSmd)
		{
		    StjEEmcSmdMuDst eemcSmd;
		    eemcEnergyList = eemcSmd.getEnergyList();
		    if (jetbranch->anapars->changeTowers) (*jetbranch->anapars->changeTowers)(eemcEnergyList);
		    eemcEnergyList = jetbranch->anapars->eemcSmdCuts()(eemcEnergyList);
		}

		//Get FMS towers (update at Dec. 2019, by ckim) - now it uses TPC vertex like BEMC/EEMC
		StjTowerEnergyList fmsEnergyList;
		if (jetbranch->anapars->useFms)
		{
		    StjFMSMuDst fms;
		    fms.Init();
		    if (jetbranch->anapars->useFmsHit) fms.useFmsHit();
		    fmsEnergyList = jetbranch->anapars->fmsCuts()(fms.getEnergyList());
		    //fmsEnergyList = fms.getEnergyList();
		}
		
		//Merge BEMC, EEMC and FMS towers
		StjTowerEnergyList energyList;
		copy(bemcEnergyList.begin(),bemcEnergyList.end(),back_inserter(energyList));
		copy(eemcEnergyList.begin(),eemcEnergyList.end(),back_inserter(energyList));
		copy(fmsEnergyList.begin(),fmsEnergyList.end(),back_inserter(energyList));

		//Apply hadronic correction to towers
		energyList = jetbranch->anapars->correctTowerEnergyForTracks()(energyList,trackList);

		//Convert tracks and towers to Lorentz vectors
		FourList tpc4pList = StjeTrackListToStMuTrackFourVecList()(trackList);
		FourList energy4pList = StjeTowerEnergyListToStMuTrackFourVecList()(energyList);

		//Merge tracks and towers
		StProtoJet::FourVecList particles; //vector<const AbstractFourVec*>
		if (!jetbranch->anapars->useEmJetMode)
		    copy(tpc4pList.begin(),tpc4pList.end(),back_inserter(particles));
		copy(energy4pList.begin(),energy4pList.end(),back_inserter(particles));

		//Run jet finder
		StJetFinder::JetList protojets;	//list<StProtoJet>
		jetbranch->jetfinder->findJets(protojets,particles);

		//Filter jets
		protojets = jetbranch->anapars->jetCuts()(protojets);

		//Add vertex
		StJetVertex* jetvertex = jetbranch->event->newVertex();
		copyVertex(vertex,jetvertex);

		//Add UE vertex
		for (size_t ueBranch = 0; ueBranch < mJetUeBranches.size(); ueBranch++)
		{
		    ((mJetUeBranches[ueBranch]->eventUe).at(iBranch))->setEventId(GetEventNumber());
		    StUeVertex *uevertex = (mJetUeBranches[ueBranch]->eventUe).at(iBranch)->newVertex();
		    uevertex->setPosition(vertex.position());
		    uevertex->setRanking(vertex.ranking());
		}

		//Add jets
		for (StJetFinder::JetList::const_iterator iProtoJet = protojets.begin();
		     iProtoJet != protojets.end();
		     ++iProtoJet)
		{
		    addJet(*iProtoJet,jetbranch->event,jetvertex);

		    //UE jets
		    StJetCandidate* currentjet = jetbranch->event->jet(jetbranch->event->numberOfJets()-1);
		    for (size_t ueBranch = 0; ueBranch < mJetUeBranches.size(); ueBranch++)
		    {
			StJetUeBranch *jetuebranch = mJetUeBranches.at(ueBranch);
			double radius = jetuebranch->uePars->coneRadius();
			double uedensity = addJetUe(particles,(jetuebranch->eventUe).at(iBranch),*iProtoJet,radius);
			currentjet->setUeDensity(jetuebranch->name, uedensity);
		    }
		}

		//Clean up particles
		for (StProtoJet::FourVecList::const_iterator i = particles.begin();
		     i != particles.end(); ++i) delete *i;
	    }//End vertex loop

	    //-------------------------------------------------------------------------------------
	    //No good TPC vertex was found
	    if (!nvertices)
	    {
		//Priority: VPD -> BBC w/ slewing correction -> BBC w/o slewing correction
		Double_t zVtx = 0.;
		if(!mIsMCmode) //Data
		{
		    mEmVertexMkr = (StEmVertexMaker*)GetMaker("StEmVertexMaker");
		    if(mEmVertexMkr)
			zVtx = mEmVertexMkr->GetEmVertexZ();
		    else
			LOG_ERROR << "StJetMaker2015::Make - No StEmVertexMaker found. Vertex z is set to zero !!!" <<endm; 	
		}
		else //Simulation
		{
		    StjMCMuDst mc(this);
		    StjPrimaryVertex mcvertex = mc.getMCVertex();
		    zVtx = mcvertex.position().z();
		}
		//Get BEMC towers
		StjTowerEnergyList bemcEnergyList;
		if (jetbranch->anapars->useBemc)
		{
		    StjBEMCMuDst bemc;
		    bemc.setVertex(0, 0, zVtx);
		    bemcEnergyList = bemc.getEnergyList();
		    if (jetbranch->anapars->changeTowers) (*jetbranch->anapars->changeTowers)(bemcEnergyList);
		    bemcEnergyList = jetbranch->anapars->bemcCuts()(bemcEnergyList);
		}

		//Get EEMC towers
		StjTowerEnergyList eemcEnergyList_temp, eemcEnergyList;
		if (jetbranch->anapars->useEemc)
		{
		    StjEEMCMuDst eemc;
		    eemc.setVertex(0, 0, zVtx);
		    eemcEnergyList = eemc.getEnergyList();
		    if (jetbranch->anapars->changeTowers) (*jetbranch->anapars->changeTowers)(eemcEnergyList);
		    eemcEnergyList = jetbranch->anapars->eemcCuts()(eemcEnergyList);
		}
		else if(!jetbranch->anapars->useEemc && jetbranch->anapars->useEemcSmd)
		{
		    StjEEmcSmdMuDst eemcSmd;
		    eemcSmd.setVertex(0, 0, zVtx);
		    eemcEnergyList = eemcSmd.getEnergyList();
		    if (jetbranch->anapars->changeTowers) (*jetbranch->anapars->changeTowers)(eemcEnergyList);
		    eemcEnergyList = jetbranch->anapars->eemcSmdCuts()(eemcEnergyList);
		}
		
		//Get FMS towers
		StjTowerEnergyList fmsEnergyList;
		if (jetbranch->anapars->useFms)
		{
		    StjFMSMuDst fms;
		    fms.Init();
		    if (jetbranch->anapars->useFmsHit) fms.useFmsHit();
		    fms.setVertex(0, 0, zVtx);
		    fmsEnergyList = jetbranch->anapars->fmsCuts()(fms.getEnergyList());
		}

		//Merge BEMC, EEMC and FMS towers
		StjTowerEnergyList energyList;
		copy(bemcEnergyList.begin(),bemcEnergyList.end(),back_inserter(energyList));
		copy(eemcEnergyList.begin(),eemcEnergyList.end(),back_inserter(energyList));
		copy(fmsEnergyList.begin(),fmsEnergyList.end(),back_inserter(energyList));

		//Convert towers to Lorentz vectors
		FourList energy4pList = StjeTowerEnergyListToStMuTrackFourVecList()(energyList);

		//Merge tracks and towers
		StProtoJet::FourVecList particles; // vector<const AbstractFourVec*>
		copy(energy4pList.begin(),energy4pList.end(),back_inserter(particles));

		//Run jet finder
		StJetFinder::JetList protojets;	// list<StProtoJet>
		jetbranch->jetfinder->findJets(protojets,particles);

		//Filter jets
		protojets = jetbranch->anapars->jetCuts()(protojets);

		//Add dummy vertex (0,0,0)
		StJetVertex* jetvertex = jetbranch->event->newVertex();
		jetvertex->mPosition.SetXYZ(0, 0, zVtx);

		//Add UE vertex
		for (size_t ueBranch = 0; ueBranch < mJetUeBranches.size(); ueBranch++)
		{
		    ((mJetUeBranches[ueBranch]->eventUe).at(iBranch))->setEventId(GetEventNumber());
		    StUeVertex *uevertex = (mJetUeBranches[ueBranch]->eventUe).at(iBranch)->newVertex();
		    TVector3 Pos(0, 0, zVtx);
		    uevertex->setPosition(Pos);
		}

		//Add jets
		for (StJetFinder::JetList::const_iterator iProtoJet = protojets.begin();
		     iProtoJet != protojets.end();
		     ++iProtoJet)
		{
		    addJet(*iProtoJet,jetbranch->event,jetvertex);

		    //Add UE jets
		    StJetCandidate* currentjet = jetbranch->event->jet(jetbranch->event->numberOfJets()-1);
		    for (size_t ueBranch = 0; ueBranch < mJetUeBranches.size(); ueBranch++)
		    {
			StJetUeBranch *jetuebranch = mJetUeBranches.at(ueBranch);
			double radius = jetuebranch->uePars->coneRadius();
			double uedensity = addJetUe(particles,(jetuebranch->eventUe).at(iBranch),*iProtoJet,radius);
			currentjet->setUeDensity(jetuebranch->name, uedensity);
		    }
		}

		//Clean up particles
		for (StProtoJet::FourVecList::const_iterator i = particles.begin();
		     i != particles.end();
		     ++i) delete *i;
	    }

	    //Restore vertex index
	    tpc.setVertexIndex(savedVertexIndex);
	}//End useTpc

        if (jetbranch->anapars->useMonteCarlo)
        {
            StjMCMuDst mc(this);
            StjPrimaryVertex mcvertex = mc.getMCVertex();
            StjMCParticleList mcparticles = (jetbranch->anapars->useEmJetMode) ? jetbranch->anapars->mcCuts()(mc.getMCPhotonList()) : jetbranch->anapars->mcCuts()(mc.getMCParticleList()); //For EM-jet only allow photons -- Latif
            StProtoJet::FourVecList particles; //vector<const AbstractFourVec*>
            transform(mcparticles.begin(), mcparticles.end(), back_inserter(particles),
		      StjMCParticleToStMuTrackFourVec());

            //Run jet finder
            StJetFinder::JetList protojets; // list<StProtoJet>
            jetbranch->jetfinder->findJets(protojets,particles);

            //Filter jets
            protojets = jetbranch->anapars->jetCuts()(protojets);

            //Add vertex
            StJetVertex* jetvertex = jetbranch->event->newVertex();
            copyVertex(mcvertex,jetvertex);

            //Add UE vertex
            for(size_t ueBranch = 0; ueBranch < mJetUeBranches.size(); ueBranch++)
	    {
                ((mJetUeBranches[ueBranch]->eventUe).at(iBranch))->setEventId(GetEventNumber());
                StUeVertex *uevertex = (mJetUeBranches[ueBranch]->eventUe).at(iBranch)->newVertex();
                uevertex->setPosition(mcvertex.position());
                uevertex->setRanking(mcvertex.ranking());
            }

            //Add jets
            for (StJetFinder::JetList::const_iterator iProtoJet = protojets.begin();
		 iProtoJet != protojets.end(); ++iProtoJet)
            {
                addJet(*iProtoJet,jetbranch->event,jetvertex);

                //add UE jets
                StJetCandidate* currentjet = jetbranch->event->jet(jetbranch->event->numberOfJets()-1);
                for(size_t ueBranch = 0; ueBranch < mJetUeBranches.size(); ueBranch++)
                {
                    StJetUeBranch *jetuebranch = mJetUeBranches.at(ueBranch);
                    double radius = jetuebranch->uePars->coneRadius();
                    double uedensity = addJetUe(particles, (jetuebranch->eventUe).at(iBranch), *iProtoJet, radius);
                    currentjet->setUeDensity(jetuebranch->name, uedensity);
                }
            }

            //Clean up particles
            for (StProtoJet::FourVecList::const_iterator i = particles.begin();
		 i != particles.end(); ++i) delete *i;
        }//End useMonteCarlo

    }//End loop over jet branches

    mTree  ->Fill();
    mTreeUe->Fill();

    return kStOk;
}//Make
