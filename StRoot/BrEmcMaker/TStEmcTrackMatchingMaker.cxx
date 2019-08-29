// Filename: TStEmcTrackMatchingMaker.cxx
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Mon Aug 19 17:37:54 2019 (-0400)
// URL: jlab.org/~latif


#include "TStEmcTrackMatchingMaker.h"
#include "StEvent/StEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StEvent/StEmcCollection.h"

ClassImp(TStEmcTrackMatchingMaker)

//_____________________________________________________________________________ 
TStEmcTrackMatchingMaker::TStEmcTrackMatchingMaker(const char *name):StMaker(name)
{
    //
}

//_____________________________________________________________________________ 
TStEmcTrackMatchingMaker::~TStEmcTrackMatchingMaker()
{
    //
}


//_____________________________________________________________________________ 
Int_t TStEmcTrackMatchingMaker::Init()
{
    // Create tables
    
    // Create Histograms
    
    // Get pointer to database
    // mDbMaker = static_cast<StFmsDbMaker*>(GetMaker("fmsDb"));
    // if (!mDbMaker)
    // {
    // 	LOG_ERROR <<"TStFmsTreeMaker::InitRun - !StFmsDbMaker" <<endl;
    // 	return kStFatal;
    // }
    return kStOK;
}


//_____________________________________________________________________________
Int_t TStEmcTrackMatchingMaker::Make()
{
    mMuDst = (StMuDst*)GetInputDS("MuDst");
    mEvent = (StEvent*)GetInputDS("StEvent");
    if(!mMuDst)
    {
	cout << "TStEmcTrackMatchingMaker::Make - No StMuDst event!" <<endl;
	return kStFatal;
    }
    
    StEvent *stEvent = mMuDst->createStEvent();
    stEvent->statistics();
    //mEvent->statistics();

    MatchToTracks(stEvent);
    
    return kStOK;
}

Int_t TStEmcTrackMatchingMaker::MatchToTracks(StEvent *event)
{
    if(!event)
        return 0;
    mEmcCollection = mMuDst->emcCollection();
    if(!mEmcCollection)
    {
	cout<<"No EMC data for this event"<<endl;
	return kStSkip;
    }
    StSPtrVecEmcPoint& mEmcPoints = mEmcCollection->barrelPoints();
    
    cout << "------> No of points for matching: "<< mEmcPoints.size() <<endl;

    Float_t field = 0.5;
    StEventSummary *evtSummary = event->summary();
    if (evtSummary)
        field = evtSummary->magneticField()/10;

    Int_t nR = mEmcPoints.size();
    StEmcGeom* geom = StEmcGeom::instance("bemc");
    if(nR>0)
    {
        LOG_DEBUG << "Matching to tracks... NP = " << nR << endm;
        StSPtrVecTrackNode& tracks=event->trackNodes();
        Int_t nTracks =  tracks.size();
        StThreeVectorD momentum,position;
        for(Int_t t=0;t<nTracks;t++)
        {
            StTrack *track = tracks[t]->track(0);
            if(track)
            {
                if(track->geometry())
                {
                    Bool_t tok = mPosition->trackOnEmc(&position,&momentum,
                                                       track,(Double_t)field,
                                                       (Double_t)geom->Radius());
                    if(tok)
                    {
                        Float_t eta = position.pseudoRapidity();
                        Float_t phi = position.phi();
                        if(fabs(eta)<1)
                        {
                            TIter next(PointsReal());
                            StEmcPoint *cl;

                            for(Int_t i=0; i<nR; i++)
                            {
                                cl = (StEmcPoint*)next();
                                if(cl)
                                {
                                    StThreeVectorF pos = cl->position();
                                    Float_t etaP = pos.pseudoRapidity();
                                    Float_t phiP = pos.phi();
                                    Float_t D = sqrt(cl->deltaEta()*cl->deltaEta()+cl->deltaPhi()*cl->deltaPhi());
                                    Float_t d = sqrt((eta-etaP)*(eta-etaP)+(phi-phiP)*(phi-phiP));
                                    if(d<D)
                                    {
                                        cl->setDeltaEta(eta-etaP);
                                        cl->setDeltaPhi(phi-phiP);
                                    }

                                    StThreeVectorF err = cl->positionError();
                                    Float_t etaE = err.pseudoRapidity();
                                    Float_t phiE = err.phi();

                                    Float_t dPhi = fabs(phi-phiP);
                                    if (dPhi>TMath::Pi())
                                        dPhi=2*TMath::Pi()-dPhi;

                                    if(fabs(eta-etaP)<fabs(etaE) && dPhi<fabs(phiE))
                                    {
                                        Int_t Category = cl->quality();
                                        Category = Category | 16;
                                        cl->setQuality(Category);
                                        cl->addTrack(track);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return 0;
}    

//_____________________________________________________________________________
Int_t TStEmcTrackMatchingMaker::Finish()
{
    //Write histograms to root file etc.

    return kStOK;
}
