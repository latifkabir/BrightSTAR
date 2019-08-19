/*
  Based on StEpcMaker class.
*/

#ifndef TSTEMCTREEMAKER_H
#define TSTEMCTREEMAKER_H
#include "StMaker.h"
#include "StMessMgr.h"
#include <TH2.h>

class StEvent;
class StEmcCollection;
class StPointCollection;

class TStEmcTreeMaker : public StMaker
{
private:
    StEvent*             mEvent;
    StEmcCollection*     mTheEmcCollection;
    StPointCollection*   mPoint;
    void                 MakeHistograms();    // Filling QA Histograms

protected:
    TH1F *m_point_flag;        //! //Point Flag spectra
    TH1F *m_point_quality;        //! //Point quality spectra
    TH1F *m_point_energy[4];   //! //Point Energy spectra
    TH1F *m_point_eta[4];      //! //Point Eta spectra
    TH1F *m_point_phi[4];      //! //Point Phi spectra
    // the following histograms will be created and filled only if the
    // mFilHisto is set to kTRUE
    TH1F *m_point_sigeta[4];   //! //Point SigmaEta spectra
    TH1F *m_point_sigphi[4];   //! //Point SigmaPhi spectra
    TH1F *m_point_trmom[4];    //! //Point TrMom spectra
    TH1F *m_emc_points[4];     //! //Emc Point multiplicity

    Bool_t mFillHisto;

public:
    TStEmcTreeMaker(const char *name="epc");
    virtual ~TStEmcTreeMaker();
    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
    virtual Int_t fillStEvent();
    void    setPrint(Bool_t a)
    {
		LOG_INFO << "::setPrint() is obsolete.  Use logger config file to set verbosity instead." << endm;
    }///< Obsolete function; users can control messages with logger config file.
    void  setFillHisto(Bool_t a)
    {
        mFillHisto = a;
    } ///< Turns on/off histogram filling
    virtual void  Browse(TBrowser* b); // StEvent staf will be visible in browser

    virtual const char *GetCVS() const
    {
        static const char cvs[]=
            "Tag $Name:  $ $Id: TStEmcTreeMaker.h,v 1.13 2014/08/06 11:43:08 jeromel Exp $ built " __DATE__ " " __TIME__ ;
        return cvs;
    }

    ClassDef(TStEmcTreeMaker,0)// EMC-Track match maker
};

#endif







