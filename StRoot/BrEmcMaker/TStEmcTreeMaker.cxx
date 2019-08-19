/*
   Based on StEpcMaker class

 */

#include <Stiostream.h>
#include <TBrowser.h>
#include "TStEmcTreeMaker.h"
#include "StEpcMaker/StPointCollection.h"
#include "StEvent.h"
#include "StEventTypes.h"

ClassImp(TStEmcTreeMaker)

//_____________________________________________________________________________
TStEmcTreeMaker::TStEmcTreeMaker(const char *name):StMaker(name)
{
    mPoint = 0;
    for(int i=0;i<4;i++)
    {
        m_point_energy[i] = 0;   //! //Point Energy spectra
        m_point_eta[i] = 0;      //! //Point Eta spectra
        m_point_phi[i] = 0;      //! //Point Phi spectra
        m_point_sigeta[i] = 0;   //! //Point SigmaEta spectra
        m_point_sigphi[i] = 0;   //! //Point SigmaPhi spectra
        m_point_trmom[i] = 0;    //! //Point TrMom spectra
        m_emc_points[i] = 0;     //! //Emc Point multiplicity
    }
    m_point_flag = 0;
    mFillHisto = kTRUE;
}
//_____________________________________________________________________________
TStEmcTreeMaker::~TStEmcTreeMaker()
{}
//________________________________________________________________________
Int_t TStEmcTreeMaker::Init()
{
    cout << "Initializing TStEmcTreeMaker ...." <<endl;
    // Making QA histgrams for points
    m_point_flag= new TH1F(" Point_Flag "," Point Flag ",5,0.5,5.5);
    m_point_quality= new TH1F(" Point_Quality "," Point Quality distribution ",32,-0.5,31.5);

    const TString catname[] =
        { "Cat1", "Cat2", "Cat3", "Cat4"
        };

    for (Int_t i=0; i<4; i++)
    {
        TString name_e = catname[i] + "_Point_Energy";
        TString tit_e = catname[i] + " Point Energy";
        m_point_energy[i]= new TH1F(name_e,tit_e,150,0.,15.);

        TString name_eta = catname[i] + "_Point_Eta";
        TString tit_eta = catname[i] + " Point Eta";
        m_point_eta[i]= new TH1F(name_eta,tit_eta,100,-1.,1.);

        TString name_phi = catname[i] + "_Point_Phi";
        TString tit_phi = catname[i] + " Point Phi";
        m_point_phi[i]= new TH1F(name_phi,tit_phi,100,-3.14,3.14);
        if (mFillHisto)
        {
            TString name_sigeta = catname[i] + "_Sigma_Eta";
            TString tit_sigeta = catname[i] + " Sigma Eta";
            m_point_sigeta[i]= new TH1F(name_sigeta,tit_sigeta,100,0.,.2);

            TString name_sigphi = catname[i] + "_Sigma_Phi";
            TString tit_sigphi = catname[i] + " Sigma Phi";
            m_point_sigphi[i]= new TH1F(name_sigphi,tit_sigphi,100,0.,.2);

            TString name_points = catname[i] + "_Points_Multiplicity";
            TString tit_points = catname[i] + " Points Multiplicity";
            m_emc_points[i]= new TH1F(name_points,tit_points,200,0.,200.);

            TString name_mom = catname[i] + "_Track_Momenta";
            TString tit_mom = catname[i] + " Track Momenta ";
            m_point_trmom[i]= new TH1F(name_mom,tit_mom,150,0.,15.);
        }
    }
    return StMaker::Init();
}
//_________________________________________________________________________
Int_t TStEmcTreeMaker::Make()
{
    mEvent = (StEvent *) GetInputDS("StEvent");

    if (!mEvent)
    {
        LOG_ERROR << "No StEvent! Can not continue. " << endm;
        return kStOK; // If no event, we're done
    }
    mTheEmcCollection = mEvent->emcCollection();
    if(!mTheEmcCollection)
    {
        LOG_ERROR <<" EPC:: No EmcCollection, Cannot continue**"<<endm;
        return kStOK;
    }

    // ******Creating StPointCollection and calling findPoints
    mPoint = new StPointCollection("point");
//VP    AddData(mPoint);      // for convinience only
    AddData(mPoint);
    if(mPoint->makeEmcPoints(mEvent) != 1)
    {
        return kStOk;
    }
    else {
		LOG_DEBUG << "findEmcPoint == kStOK" << endm;
	}
   cout << "Inside Maker ...." <<endl;

    MakeHistograms(); // Fill QA histgrams
    LOG_DEBUG << "Epc: ***  Filling StEvent ***" << endm;
    if(fillStEvent() != kStOK)
    {
        LOG_WARN << "StEvent filling is not OK"<<endm;
    }
    return kStOK;
}
//_________________________________________________________________________

void TStEmcTreeMaker::MakeHistograms()
{
    Int_t mult[4]={0,0,0,0};
    if(mPoint)
    {
        Int_t nR = mPoint->NPointsReal();

        if(nR>0)
        {
            LOG_DEBUG << "Number of Emc points " << nR << endm;
            TIter next(mPoint->PointsReal());
            StEmcPoint *cl;

            for(Int_t i=0; i<nR; i++)
            {
                cl = (StEmcPoint*)next();
                if(cl)
                {
                    Int_t C = 0;
                    Int_t Q = cl->quality();
                    if( (Q&1) )
                        C = 1; // only tower
                    if( (Q&1) && (Q&4))
                        C = 2; // tower + smd eta
                    if( (Q&1) && (Q&8))
                        C = 3; // tower + smd phi
                    if( (Q&1) && (Q&4) && (Q&8))
                        C = 4; // tower + smd eta + smd phi
                    m_point_flag->Fill(C);
                    m_point_quality->Fill(Q);
                    if(C>0)
                    {
                        mult[C-1]++;
                        if(m_point_energy[C-1])
			{
                            m_point_energy[C-1]->Fill(cl->energy());
			    cout << "Energy: "<< cl->energy() <<endl;

			}
                        if(m_point_eta[C-1])
                            m_point_eta[C-1]->Fill(cl->position().pseudoRapidity());
                        if(m_point_phi[C-1])
                            m_point_phi[C-1]->Fill(cl->position().phi());
                        if(m_point_sigeta[C-1])
                            m_point_sigeta[C-1]->Fill(cl->size().x());
                        if(m_point_sigphi[C-1])
                            m_point_sigphi[C-1]->Fill(cl->size().y());
                        StPtrVecTrack& tr = cl->track();
                        for(UInt_t j = 0;j<tr.size();j++)
                        {
                            StTrack* t = tr[j];
                            if(t)
                            {
                                StTrackGeometry *g = t->geometry();
                                if(m_point_trmom[C-1])
                                    m_point_trmom[C-1]->Fill(g->momentum().mag());
                            }
                        }
                    }
                }
            }
        }
    }
    for(Int_t i = 0;i<4;i++)
        if(m_emc_points[i])
            m_emc_points[i]->Fill(mult[i]);
}

Int_t
TStEmcTreeMaker::fillStEvent()
{
    LOG_DEBUG <<"Epc::fillStEvent() ***"<<endm;

    if(mPoint)
    {
        Int_t nR = mPoint->NPointsReal();

        if(nR>0)
        {
            LOG_DEBUG << "Number of Emc points " << nR << endm;
            TIter next(mPoint->PointsReal());
            StEmcPoint *cl;

            for(Int_t i=0; i<nR; i++)
            {
                cl = (StEmcPoint*)next();
                mTheEmcCollection->addBarrelPoint(cl);
            }
        }
    }
    else {
		LOG_DEBUG << "There is no BEMC points in this event" <<endm;
	}
    return kStOK;
}
//-------------------------------------------------------
Int_t TStEmcTreeMaker::Finish()
{
    return kStOK;
}
void
TStEmcTreeMaker::Browse(TBrowser* b)
{
    // Will be see StEmcCollection in browser as separate entity (if unzero) the same as in StEvent
    if(mTheEmcCollection)
        b->Add((TObject*)mTheEmcCollection);
    TDataSet::Browse(b);
}
