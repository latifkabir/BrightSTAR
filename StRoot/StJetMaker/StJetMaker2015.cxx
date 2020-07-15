/*
  2015 version of JetMaker:
  - Derivated from 2012 version by Jilong
  - Minor updates applied, mainly for FMS
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

    //Loop over jet branches
    for (size_t iBranch = 0; iBranch < mJetBranches.size(); ++iBranch)
    {
	StJetBranch* jetbranch = mJetBranches[iBranch];

	//Fill header
	jetbranch->event->mRunId   = GetRunNumber();
	jetbranch->event->mEventId = GetEventNumber();
	jetbranch->event->mDatime  = GetDateTime();

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

		//Merge BEMC and EEMC towers
		StjTowerEnergyList energyList;
		copy(bemcEnergyList.begin(),bemcEnergyList.end(),back_inserter(energyList));
		copy(eemcEnergyList.begin(),eemcEnergyList.end(),back_inserter(energyList));

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
		copy(fmsEnergyList.begin(),fmsEnergyList.end(),back_inserter(energyList));

		//Apply hadronic correction to towers
		energyList = jetbranch->anapars->correctTowerEnergyForTracks()(energyList,trackList);

		//Convert tracks and towers to Lorentz vectors
		FourList tpc4pList = StjeTrackListToStMuTrackFourVecList()(trackList);
		FourList energy4pList = StjeTowerEnergyListToStMuTrackFourVecList()(energyList);

		//Merge tracks and towers
		StProtoJet::FourVecList particles; //vector<const AbstractFourVec*>
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
		float zVtx = 0.;
		float vpdZ = -999.;
		float bbcZ = -999.;

		StMuDst* muDST = (StMuDst*)GetInputDS("MuDst");
		if (!muDST) { LOG_ERROR <<"StJetMaker2015::Make - cannot find MuDst!" <<endl; return kStErr; }

		//Check VPD zVtx
		if (muDST->btofHeader()) vpdZ = muDST->btofHeader()->vpdVz();

		//Assign zVtx
		if (vpdZ != -999.) zVtx = vpdZ; //Use vpdZ and be done with it, if it's valid
		else
		{
		    //No vpdZ or found vpdZ is NOT valid: go to BBC
		    if (mReadBbcSlewing == true) //BBCz w/ slewing
		    {
			const StTriggerData* triggerData = muDST->event()->triggerData();
			bbcZ = GetBbcZCorr(triggerData);
		    }
		    else
		    {
			const float bbcTdiff = muDST->event()->triggerData()->bbcTimeDifference();
			if (fabs(bbcTdiff) > 1.e-6) bbcZ = -0.3 * (bbcTdiff - 4096);
		    }
		    if (bbcZ != -999.) zVtx = bbcZ;
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

		//Merge BEMC and EEMC towers
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
            StjMCParticleList mcparticles = jetbranch->anapars->mcCuts()(mc.getMCParticleList());
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

//------------------------------------------------------------
Int_t StJetMaker2015::ReadBbcSlewing(const char* filename_bbc) //Written by Oleg Eyser
{
    mReadBbcSlewing = true;

    //reading parameters for BBC slewing correction
    char s[100];
    int iew, ipmt;
    float ca, cb, cc;

    FILE *pFile = fopen( filename_bbc, "read" );
    fgets( s, 100, pFile );
    for( int ew=0; ew<2; ew++ )
	for( int p=0; p<16; p++ )
	{
	    fscanf( pFile, " %d %d %f %f %f \n", &iew, &ipmt, &ca, &cb, &cc);
	    if ( ew==iew && p+1==ipmt )
	    {
		mBbcSlew[ew][p][0] = ca;
		mBbcSlew[ew][p][1] = cb;
		mBbcSlew[ew][p][2] = cc;
	    }
	    else return kError;
	}
    fclose( pFile );

    cout << "\nBBC slewing: z(A+B/[C+adc])" << endl;
    for( int ew=0; ew<2; ew++ )
    {
        if( ew==0 ) cout << " East" << endl;
        if( ew==1 ) cout << " West" << endl;
        for( int p=0; p<16; p++ )
        {
            cout << Form("PMT%2d - %7.2f %7.2f %7.2f ",
			 p+1, mBbcSlew[ew][p][0], mBbcSlew[ew][p][1], mBbcSlew[ew][p][2]) << endl;
        }
    }
    cout <<endl;

    return kStOK;
}//ReadBbcSlewing

//-------------------------------------------------------------------
Float_t StJetMaker2015::GetBbcZCorr(const StTriggerData* triggerData) //Written by Oleg Eyser, Modified by CKim
{
    //DO NOT use muDST->event()->bbcTriggerDetector() -- obsolete!!!
    Float_t bbcZ     = -999.;
    Float_t bbcTdiff = -999.;
    UShort_t tdc1east, tdc1west;
    UShort_t pmt1east, pmt1west;
    UShort_t adc1east, adc1west;
    unsigned int tdcMatchEast = 0;
    unsigned int tdcMatchWest = 0;
    bbcTdiff = (float)triggerData->bbcTimeDifference();
    tdc1east = triggerData->bbcEarliestTDC(east);
    tdc1west = triggerData->bbcEarliestTDC(west);

    //Compare TDC values to find earliest PMT (east/west)
    for ( int i=1; i<=16; i++ )
    {
        if ( tdc1east==triggerData->bbcTDC(east, i) )
        {
            adc1east = triggerData->bbcADC(east, i);
            pmt1east = i-1;
            ++tdcMatchEast;
        }
        if ( tdc1west==triggerData->bbcTDC(west, i) )
        {
            adc1west = triggerData->bbcADC(west, i);
            pmt1west = i-1;
            ++tdcMatchWest;
        }
    }

    //BBC slewing correction (east/west)
    if ( tdcMatchEast==1 && tdcMatchWest==1 )
    {
        Float_t zEast = -0.3 * (bbcTdiff
				- mBbcSlew[0][pmt1east][0] - mBbcSlew[0][pmt1east][1]/(mBbcSlew[0][pmt1east][2] + adc1east)
	    );
        Float_t zWest = -0.3 * (bbcTdiff
				- mBbcSlew[1][pmt1west][0] - mBbcSlew[1][pmt1west][1]/(mBbcSlew[1][pmt1west][2] + adc1west)
	    );
        bbcZ = (zEast + zWest)/2.0;
    }

    return bbcZ;
}//GetBbcZCorr
