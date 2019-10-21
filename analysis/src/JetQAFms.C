#include "StRootInclude.h"
#include "RootInclude.h"
#include "cppInclude.h"
#include "TStTrigDef.h"

void JetQA1(
            Int_t nentries,
	    TString jetfile,
	    TString skimfile,
	    TString outfile
    )
{
    cout << "nentries = " << nentries << endl;
    cout << "jetfile  = " << jetfile  << endl;
    cout << "skimfile = " << skimfile << endl;
    cout << "outfile = " << outfile << endl;

    // Load libraries
    // gSystem->Load("StJetEvent");
    // gSystem->Load("StJetSkimEvent");

    // Open jet & skim files
    TChain* jetChain = new TChain("jet");
    TChain* skimChain = new TChain("jetSkimTree");

    Int_t nFiles_jet, nFiles_skim;
    nFiles_jet = jetChain->Add(jetfile);
    nFiles_skim = skimChain->Add(skimfile);

    Int_t nEvents = jetChain->GetEntries();
    if(nentries == -1 || nentries > nEvents)
	nentries = nEvents;

    cout << "Number of files added: "<<nFiles_jet <<endl;
    cout << "Total Entries to be processed: "<< nentries <<endl;

    // Set jet buffer
    StJetEvent* jetEvent = 0;
    jetChain->SetBranchAddress("AntiKtR060NHits12",&jetEvent);

    // Set skim buffer
    StJetSkimEvent* skimEvent = 0;
    skimChain->SetBranchAddress("skimEventBranch",&skimEvent);

    // Open output file for writing
    TFile* ofile = TFile::Open(outfile,"recreate");
    assert(ofile);

    // Book histograms
    // Jet Information
    TH1F* hjetpTJP1 = new TH1F("hjetpTJP1",";jet p_{T} [GeV]",95,5,100);
    TH1F* hjetpTJP2 = new TH1F("hjetpTJP2",";jet p_{T} [GeV]",95,5,100);
    TH1F* hjetpTBHT3 = new TH1F("hjetpTBHT3",";jet p_{T} [GeV]",95,5,100);
    TH1F* hjetpTAJP = new TH1F("hjetpTAJP",";jet p_{T} [GeV]",95,5,100);
    TH1F* hjetRtJP1 = new TH1F("hjetRtJP1",";jet R_{T} [GeV]",24,0,1.2);
    TH1F* hjetRtJP2 = new TH1F("hjetRtJP2",";jet R_{T} [GeV]",24,0,1.2);
    TH1F* hjetRtBHT3 = new TH1F("hjetRtBHT3",";jet R_{T} [GeV]",24,0,1.2);
    TH1F* hjetRtAJP = new TH1F("hjetRtAJP",";jet R_{T} [GeV]",24,0,1.2);
    TH1F* hjetetaJP1 = new TH1F("hjetetaJP1",";jet #eta",66,-1.5,2.5);
    TH1F* hjetetaJP2 = new TH1F("hjetetaJP2",";jet #eta",66,-1.5,2.5);
    TH1F* hjetetaBHT3 = new TH1F("hjetetaBHT3",";jet #eta",66,-1.5,2.5);
    TH1F* hjetetaAJP = new TH1F("hjetetaAJP",";jet #eta",66,-1.5,2.5);
    TH1F* hjetphiJP1 = new TH1F("hjetphiJP1",";jet #phi",120,-3.14159,3.14159);
    TH1F* hjetphiJP2 = new TH1F("hjetphiJP2",";jet #phi",120,-3.14159,3.14159);
    TH1F* hjetphiBHT3 = new TH1F("hjetphiBHT3",";jet #phi",120,-3.14159,3.14159);
    TH1F* hjetphiAJP = new TH1F("hjetphiAJP",";jet #phi",120,-3.14159,3.14159);
    TH1F* hjetnumJP1 = new TH1F("hjetnumJP1",";number of jets",20,0,20);
    TH1F* hjetnumJP2 = new TH1F("hjetnumJP2",";number of jets",20,0,20);
    TH1F* hjetnumBHT3 = new TH1F("hjetnumBHT3",";number of jets ",20,0,20);
    TH1F* hjetnumAJP = new TH1F("hjetnumAJP",";number of jets",20,0,20); 
    TH1F* hjetnumTotal = new TH1F("hjetnumTotal",";total number of jets",20,0,20); 
    TH1F* hrunnumber = new TH1F("hrunnumber","Run Number",347,10085115,10103041);
    TH2F* hardvssoftTrigJP1 = new TH2F("hardvssoftTrigJP1","hardware vs software Triggers: JP1",2,0,2,2,0,2);
    TH2F* hardvssoftTrigJP2 = new TH2F("hardvssoftTrigJP2","hardware vs software Triggers: JP2",2,0,2,2,0,2);
    TH2F* hardvssoftTrigBHT3 = new TH2F("hardvssoftTrigBHT3","hardware vs software Triggers: BHT3",2,0,2,2,0,2);
    TH2F* hardvssoftTrigAJP = new TH2F("hardvssoftTrigAJP","hardware vs software Triggers: AJP",2,0,2,2,0,2);
    // Track Information
    TH1F* hpTJP1 = new TH1F("hpTJP1",";track p_{T} [GeV]",100,0,10);
    TH1F* hpTJP2 = new TH1F("hpTJP2",";track p_{T} [GeV]",100,0,10);
    TH1F* hpTBHT3 = new TH1F("hpTBHT3",";track p_{T} [GeV]",100,0,10);
    TH1F* hpTAJP = new TH1F("hpTAJP",";track p_{T} [GeV]",100,0,10);
    TH1F* hetaJP1 = new TH1F("hetaJP1",";track #eta",66,-1.5,1.5);
    TH1F* hetaJP2 = new TH1F("hetaJP2",";track #eta",66,-1.5,1.5);
    TH1F* hetaBHT3 = new TH1F("hetaBHT3",";track #eta ",66,-1.5,1.5);
    TH1F* hetaAJP = new TH1F("hetaAJP",";track #eta",66,-1.5,1.5);
    TH1F* hphiJP1 = new TH1F("hphiJP1",";track #phi",120,-3.14159,3.14159);
    TH1F* hphiJP2 = new TH1F("hphiJP2",";track #phi",120,-3.14159,3.14159);
    TH1F* hphiBHT3 = new TH1F("hphiBHT3",";track #phi ",120,-3.14159,3.14159);
    TH1F* hphiAJP = new TH1F("hphiAJP",";track #phi ",120,-3.14159,3.14159);
    TH1F* hnumJP1 = new TH1F("hnumJP1",";track #",100,0,100);
    TH1F* hnumJP2 = new TH1F("hnumJP2",";track #",100,0,100);
    TH1F* hnumBHT3 = new TH1F("hnumBHT3",";track # ",100,0,100);
    TH1F* hnumAJP = new TH1F("hnumAJP",";track # ",100,0,100);
    TH1F* hchiSqJP1 = new TH1F("hchiSqJP1",";track #chi^{2}",100,0,7);
    TH1F* hchiSqJP2 = new TH1F("hchiSqJP2",";track #chi^{2}",100,0,7);
    TH1F* hchiSqBHT3 = new TH1F("hchiSqBHT3",";track  #chi^{2} ",100,0,7);
    TH1F* hchiSqAJP = new TH1F("hchiSqAJP",";track #chi^{2} ",100,0,7);
    TH1F* hnFitJP1 = new TH1F("hnFitJP1","hnFitJP1",50,0,50);
    TH1F* hnFitJP2 = new TH1F("hnFitJP2","hnFitJP2",50,0,50);
    TH1F* hnFitBHT3 = new TH1F("hnFitBHT3","hnFitBHT3",50,0,50);
    TH1F* hnFitAJP = new TH1F("hnFitAJP","hnFitAJP",50,0,50);
    TH1F* hnFitPossJP1 = new TH1F("hnFitPossJP1","hnFitPossJP1",50,0,50);
    TH1F* hnFitPossJP2 = new TH1F("hnFitPossJP2","hnFitPossJP2",50,0,50);
    TH1F* hnFitPossBHT3 = new TH1F("hnFitPossBHT3","hnFitPossBHT3",50,0,50);
    TH1F* hnFitPossAJP = new TH1F("hnFitPossAJP","hnFitPossAJP",50,0,50);
    TH1F* hdEdxJP1 = new TH1F("hdEdxJP1","hdEdxJP1",100,0,5);
    TH1F* hdEdxJP2 = new TH1F("hdEdxJP2","hdEdxJP2",100,0,5);
    TH1F* hdEdxBHT3 = new TH1F("hdEdxBHT3","hdEdxBHT3",100,0,5);
    TH1F* hdEdxAJP = new TH1F("hdEdxAJP","hdEdxAJP",100,0,5);
    TH1F* hdcaMagJP1 = new TH1F("hdcaMagJP1","hdcaMagJP1",30,0,3);
    TH1F* hdcaMagJP2 = new TH1F("hdcaMagJP2","hdcaMagJP2",30,0,3);
    TH1F* hdcaMagBHT3 = new TH1F("hdcaMagBHT3","hdcaMagBHT3",30,0,3);
    TH1F* hdcaMagAJP = new TH1F("hdcaMagAJP","hdcaMagAJP",30,0,3);
    TH1F* hdcaDJP1 = new TH1F("hdcaDJP1","hdcaDJP1",30,-3,3);
    TH1F* hdcaDJP2 = new TH1F("hdcaDJP2","hdcaDJP2",30,-3,3);
    TH1F* hdcaDBHT3 = new TH1F("hdcaDBHT3","hdcaDBHT3",30,-3,3);
    TH1F* hdcaDAJP = new TH1F("hdcaDAJP","hdcaDAJP",30,-3,3);
    TH1F* hdcaZJP1 = new TH1F("hdcaZJP1","hdcaZJP1",30,-3,3);
    TH1F* hdcaZJP2 = new TH1F("hdcaZJP2","hdcaZJP2",30,-3,3);
    TH1F* hdcaZBHT3 = new TH1F("hdcaZBHT3","hdcaZBHT3",30,-3,3);
    TH1F* hdcaZAJP = new TH1F("hdcaZAJP","hdcaZAJP",30,-3,3);
    TH1F* hfragJP1 = new TH1F("hfragJP1","hfragJP1",100,0,1);
    TH1F* hfragJP2 = new TH1F("hfragJP2","hfragJP2",100,0,1);
    TH1F* hfragBHT3 = new TH1F("hfragBHT3","hfragBHT3",100,0,1);
    TH1F* hfragAJP = new TH1F("hfragAJP","hfragAJP",100,0,1);
    TH1F* hjTJP1 = new TH1F("hjTJP1","hjTJP1",100,0,5);
    TH1F* hjTJP2 = new TH1F("hjTJP2","hjTJP2",100,0,5);
    TH1F* hjTBHT3 = new TH1F("hjTBHT3","hjTBHT3",100,0,5);
    TH1F* hjTAJP = new TH1F("hjTAJP","hjTAJP",100,0,5);
    //Histograms to test the chi2 --------------------------------
    TH1F* hChiSq1JP1 = new TH1F("hChiSq1JP1","hChiSq1JP1",100,0,5);
    TH1F* hChiSq1JP2 = new TH1F("hChiSq1JP2","hChiSq1JP2",100,0,5);
    TH1F* hChiSq1BHT3 = new TH1F("hChiSq1BHT3","hChiSq1BHT3",100,0,5);
    TH1F* hChiSq1AJP = new TH1F("hChiSq1AJP","hChiSq1AJP",100,0,5);

    TH1F* hChiSq2JP1 = new TH1F("hChiSq2JP1","hChiSq2JP1",100,0,5);
    TH1F* hChiSq2JP2 = new TH1F("hChiSq2JP2","hChiSq2JP2",100,0,5);
    TH1F* hChiSq2BHT3 = new TH1F("hChiSq2BHT3","hChiSq2BHT3",100,0,5);
    TH1F* hChiSq2AJP = new TH1F("hChiSq2AJP","hChiSq2AJP",100,0,5);

    TH1F* hdcaMag1JP1 = new TH1F("hdcaMag1JP1","hdcaMag1JP1",30,0,3);
    TH1F* hdcaMag1JP2 = new TH1F("hdcaMag1JP2","hdcaMag1JP2",30,0,3);
    TH1F* hdcaMag1BHT3 = new TH1F("hdcaMag1BHT3","hdcaMag1BHT3",30,0,3);
    TH1F* hdcaMag1AJP = new TH1F("hdcaMag1AJP","hdcaMag1AJP",30,0,3);
    TH1F* hdcaD1JP1 = new TH1F("hdcaD1JP1","hdcaD1JP1",30,-3,3);
    TH1F* hdcaD1JP2 = new TH1F("hdcaD1JP2","hdcaD1JP2",30,-3,3);
    TH1F* hdcaD1BHT3 = new TH1F("hdcaD1BHT3","hdcaD1BHT3",30,-3,3);
    TH1F* hdcaD1AJP = new TH1F("hdcaD1AJP","hdcaD1AJP",30,-3,3);
    TH1F* hdcaZ1JP1 = new TH1F("hdcaZ1JP1","hdcaZ1JP1",30,-3,3);
    TH1F* hdcaZ1JP2 = new TH1F("hdcaZ1JP2","hdcaZ1JP2",30,-3,3);
    TH1F* hdcaZ1BHT3 = new TH1F("hdcaZ1BHT3","hdcaZ1BHT3",30,-3,3);
    TH1F* hdcaZ1AJP = new TH1F("hdcaZ1AJP","hdcaZ1AJP",30,-3,3);

    TH1F* hdcaMag2JP1 = new TH1F("hdcaMag2JP1","hdcaMag2JP1",30,0,3);
    TH1F* hdcaMag2JP2 = new TH1F("hdcaMag2JP2","hdcaMag2JP2",30,0,3);
    TH1F* hdcaMag2BHT3 = new TH1F("hdcaMag2BHT3","hdcaMag2BHT3",30,0,3);
    TH1F* hdcaMag2AJP = new TH1F("hdcaMag2AJP","hdcaMag2AJP",30,0,3);
    TH1F* hdcaD2JP1 = new TH1F("hdcaD2JP1","hdcaD2JP1",30,-3,3);
    TH1F* hdcaD2JP2 = new TH1F("hdcaD2JP2","hdcaD2JP2",30,-3,3);
    TH1F* hdcaD2BHT3 = new TH1F("hdcaD2BHT3","hdcaD2BHT3",30,-3,3);
    TH1F* hdcaD2AJP = new TH1F("hdcaD2AJP","hdcaD2AJP",30,-3,3);
    TH1F* hdcaZ2JP1 = new TH1F("hdcaZ2JP1","hdcaZ2JP1",30,-3,3);
    TH1F* hdcaZ2JP2 = new TH1F("hdcaZ2JP2","hdcaZ2JP2",30,-3,3);
    TH1F* hdcaZ2BHT3 = new TH1F("hdcaZ2BHT3","hdcaZ2BHT3",30,-3,3);
    TH1F* hdcaZ2AJP = new TH1F("hdcaZ2AJP","hdcaZ2AJP",30,-3,3);

    //----------------------------------------------------------
    TH1F* hposTrackpTJP1 = new TH1F("hposTrackpTJP1",";postive track p_{T} [GeV]",100,0,10);
    TH1F* hnegTrackpTJP1 = new TH1F("hnegTrackpTJP1",";negative track p_{T} [GeV]",100,0,10);
    TH1F* hposTrackpTJP2 = new TH1F("hposTrackpTJP2",";postive track p_{T} [GeV]",100,0,10);
    TH1F* hnegTrackpTJP2 = new TH1F("hnegTrackpTJP2",";negative track p_{T} [GeV]",100,0,10);
    TH1F* hposTrackpTBHT3 = new TH1F("hposTrackpTBHT3",";postive track p_{T} [GeV]",100,0,10);
    TH1F* hnegTrackpTBHT3 = new TH1F("hnegTrackpTBHT3",";negative track p_{T} [GeV]",100,0,10);
    TH1F* hposTrackpTAJP = new TH1F("hposTrackpTAJP",";postive track p_{T} [GeV]",100,0,10);
    TH1F* hnegTrackpTAJP = new TH1F("hnegTrackpTAJP",";negative track p_{T} [GeV]",100,0,10);

    TH3F* hprTrackpTetaphiJP2 = new TH3F("prtrackpTetaphiJP2","Primary Track p_{T} vs #eta vs #phi",120,-3.141592,3.141592,200,-1.5,1.5,100,0.0,10.0);
    TH3F* hprTrackpTetaphiPosJP2 = new TH3F("prtrackpTetaphiPosJP2","Postive Primary Track p_{T} vs #eta vs #phi",120,-3.141592,3.141592,200,-1.5,1.5,100,0.0,10.0);
    TH3F* hprTrackpTetaphiNegJP2 = new TH3F("prtrackpTetaphiNegJP2","Negative Primary Track p_{T} vs #eta vs #phi",120,-3.141592,3.141592,200,-1.5,1.5,100,0.0,10.0);
  
    TH2F* htracketaVsphiJP1 = new TH2F("htracketaphiJP1","track #eta vs #phi",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htracketaVsphiJP2 = new TH2F("htracketaphiJP2","track #eta vs #phi",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htracketaVsphiBHT3 = new TH2F("htracketaphiBHT3","track #eta vs #phi",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htracketaVsphiAJP = new TH2F("htracketaphiAJP","track #eta vs #phi",66,-1.5,1.5,120,-3.14159,3.14159);

    TH2F* htracklowpT_etaVsphiJP1 = new TH2F("htracklowpT_etaphiJP1","track #eta vs #phi: low pT",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htracklowpT_etaVsphiJP2 = new TH2F("htracklowpT_etaphiJP2","track #eta vs #phi: low pT",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htracklowpT_etaVsphiBHT3 = new TH2F("htracklowpT_etaphiBHT3","track #eta vs #phi: low pT",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htracklowpT_etaVsphiAJP = new TH2F("htracklowpT_etaphiAJP","track #eta vs #phi: low pT",66,-1.5,1.5,120,-3.14159,3.14159);

    TH2F* htracklowpT_pTVsphiJP1 = new TH2F("htracklowpT_pTphiJP1","track p_{T} vs #phi: low pT",300,-150,150,120,-3.14159,3.14159);
    TH2F* htracklowpT_pTVsphiJP2 = new TH2F("htracklowpT_pTphiJP2","track p_{T} vs #phi: low pT",300,-150,150,120,-3.14159,3.14159);
    TH2F* htracklowpT_pTVsphiBHT3 = new TH2F("htracklowpT_pTphiBHT3","track p_{T} vs #phi: low pT",300,-150,150,120,-3.14159,3.14159);
    TH2F* htracklowpT_pTVsphiAJP = new TH2F("htracklowpT_pTphiAJP","track p_{T} vs #phi: low pT",300,-150,150,120,-3.14159,3.14159);

    TH2F* htrack30pT_etaVsphiJP1 = new TH2F("htrack30pT_etaphiJP1","track #eta vs #phi: track pT > 30GeV",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htrack30pT_etaVsphiJP2 = new TH2F("htrack30pT_etaphiJP2","track #eta vs #phi: track pT > 30GeV",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htrack30pT_etaVsphiBHT3 = new TH2F("htrack30pT_etaphiBHT3","track #eta vs #phi: track pT > 30GeV",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htrack30pT_etaVsphiAJP = new TH2F("htrack30pT_etaphiAJP","track #eta vs #phi: track pT > 30GeV",66,-1.5,1.5,120,-3.14159,3.14159);

    TH2F* htrack30pT_pTVsphiJP1 = new TH2F("htrack30pT_pTphiJP1","track p_{T} vs #phi: track pT > 30GeV",300,-150,150,120,-3.14159,3.14159);
    TH2F* htrack30pT_pTVsphiJP2 = new TH2F("htrack30pT_pTphiJP2","track p_{T} vs #phi: track pT > 30GeV",300,-150,150,120,-3.14159,3.14159);
    TH2F* htrack30pT_pTVsphiBHT3 = new TH2F("htrack30pT_pTphiBHT3","track p_{T} vs #phi: track pT > 30GeV",300,-150,150,120,-3.14159,3.14159);
    TH2F* htrack30pT_pTVsphiAJP = new TH2F("htrack30pT_pTphiAJP","track p_{T} vs #phi: track pT > 30GeV",300,-150,150,120,-3.14159,3.14159);

    TH2F* htrack40pT_etaVsphiJP1 = new TH2F("htrack40pT_etaphiJP1","track #eta vs #phi: track pT > 40GeV",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htrack40pT_etaVsphiJP2 = new TH2F("htrack40pT_etaphiJP2","track #eta vs #phi: track pT > 40GeV",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htrack40pT_etaVsphiBHT3 = new TH2F("htrack40pT_etaphiBHT3","track #eta vs #phi: track pT > 40GeV",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htrack40pT_etaVsphiAJP = new TH2F("htrack40pT_etaphiAJP","track #eta vs #phi: track pT > 40GeV",66,-1.5,1.5,120,-3.14159,3.14159);

    TH2F* htrack40pT_pTVsphiJP1 = new TH2F("htrack40pT_pTphiJP1","track p_{T} vs #phi: track pT > 40GeV",300,-150,150,120,-3.14159,3.14159);
    TH2F* htrack40pT_pTVsphiJP2 = new TH2F("htrack40pT_pTphiJP2","track p_{T} vs #phi: track pT > 40GeV",300,-150,150,120,-3.14159,3.14159);
    TH2F* htrack40pT_pTVsphiBHT3 = new TH2F("htrack40pT_pTphiBHT3","track p_{T} vs #phi: track pT > 40GeV",300,-150,150,120,-3.14159,3.14159);
    TH2F* htrack40pT_pTVsphiAJP = new TH2F("htrack40pT_pTphiAJP","track p_{T} vs #phi: track pT > 40GeV",300,-150,150,120,-3.14159,3.14159);

    TH2F* htrack50pT_etaVsphiJP1 = new TH2F("htrack50pT_etaphiJP1","track #eta vs #phi: track pT > 50GeV",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htrack50pT_etaVsphiJP2 = new TH2F("htrack50pT_etaphiJP2","track #eta vs #phi: track pT > 50GeV",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htrack50pT_etaVsphiBHT3 = new TH2F("htrack50pT_etaphiBHT3","track #eta vs #phi: track pT > 50GeV",66,-1.5,1.5,120,-3.14159,3.14159);
    TH2F* htrack50pT_etaVsphiAJP = new TH2F("htrack50pT_etaphiAJP","track #eta vs #phi: track pT > 50GeV",66,-1.5,1.5,120,-3.14159,3.14159);

    TH2F* htrack50pT_pTVsphiJP1 = new TH2F("htrack50pT_pTphiJP1","track p_{T} vs #phi: track pT > 50GeV",300,-150,150,120,-3.14159,3.14159);
    TH2F* htrack50pT_pTVsphiJP2 = new TH2F("htrack50pT_pTphiJP2","track p_{T} vs #phi: track pT > 50GeV",300,-150,150,120,-3.14159,3.14159);
    TH2F* htrack50pT_pTVsphiBHT3 = new TH2F("htrack50pT_pTphiBHT3","track p_{T} vs #phi: track pT > 50GeV",300,-150,150,120,-3.14159,3.14159);
    TH2F* htrack50pT_pTVsphiAJP = new TH2F("htrack50pT_pTphiAJP","track p_{T} vs #phi: track pT > 50GeV",300,-150,150,120,-3.14159,3.14159);

    TH2F* hradiusJP1 = new TH2F("hjet_trackR_JP1","hjet_trackR_JP1",11,0,1.1,95,5,100);
    TH2F* hradiusJP2 = new TH2F("hjet_trackR_JP2","hjet_trackR_JP2",11,0,1.1,95,5,100);
    TH2F* hradiusBHT3 = new TH2F("hjet_trackR_BHT3","hjet_trackR_BHT3",11,0,1.1,95,5,100);

    //Tower Information
    TH1F* htowerE_JP1 = new TH1F("htowerE_JP1",";tower E [GeV]",100,0,10);
    TH1F* htowerE_JP2 = new TH1F("htowerE_JP2",";tower E [GeV]",100,0,10);
    TH1F* htowerE_BHT3 = new TH1F("htowerE_BHT3",";tower E [GeV]",100,0,10);
    TH1F* htowerE_AJP = new TH1F("htowerE_AJP",";tower E [GeV]",100,0,10);
    TH1F* htowereta_JP1 = new TH1F("htowereta_JP1",";tower #eta",66,-1.5,1.5);
    TH1F* htowereta_JP2 = new TH1F("htowereta_JP2",";tower #eta",66,-1.5,1.5);
    TH1F* htowereta_BHT3 = new TH1F("htowereta_BHT3",";tower #eta",66,-1.5,1.5);
    TH1F* htowereta_AJP = new TH1F("htowereta_AJP",";tower #eta",66,-1.5,1.5);
    TH1F* htowerphi_JP1 = new TH1F("htowerphi_JP1",";tower #phi",80,-4,4);
    TH1F* htowerphi_JP2 = new TH1F("htowerphi_JP2",";tower #phi",80,-4,4);
    TH1F* htowerphi_BHT3 = new TH1F("htowerphi_BHT3",";tower #phi",80,-4,4);
    TH1F* htowerphi_AJP = new TH1F("htowerphi_AJP",";tower #phi",80,-4,4);
    TH1F* htowernum_JP1 = new TH1F("htowernum_JP1",";tower #",100,0,100);
    TH1F* htowernum_JP2 = new TH1F("htowernum_JP2",";tower #",100,0,100);
    TH1F* htowernum_BHT3 = new TH1F("htowernum_BHT3",";tower # ",100,0,100);
    TH1F* htowernum_AJP = new TH1F("htowernum_AJP",";tower # ",100,0,100);
    TH1F* htowerfragJP1 = new TH1F("htowerfragJP1","htowerfragJP1",100,0,1);
    TH1F* htowerfragJP2 = new TH1F("htowerfragJP2","htowerfragJP2",100,0,1);
    TH1F* htowerfragBHT3 = new TH1F("htowerfragBHT3","htowerfragBHT3",100,0,1);
    TH1F* htowerfragAJP = new TH1F("htowerfragAJP","htowerfragAJP",100,0,1);
    TH1F* htowerjTJP1 = new TH1F("htowerjTJP1","htowerjTJP1",100,0,5);
    TH1F* htowerjTJP2 = new TH1F("htowerjTJP2","htowerjTJP2",100,0,5);
    TH1F* htowerjTBHT3 = new TH1F("htowerjTBHT3","htowerjTBHT3",100,0,5);
    TH1F* htowerjTAJP = new TH1F("htowerjTAJP","htowerjTAJP",100,0,5);
    TH2F* htowerphiVeta_JP1 = new TH2F("htowerphiVeta_JP1",";tower #eta V #phi",120,-3.14159,3.14159,76,-1.5,2.3);
    //  TH2F* htowerphiVetaInRange_JP1 = new TH2F("htowerphiVetaInRange_JP1",";tower #eta V #phi",120,-3.14159,3.14159,76,-1.5,2.3);
    TH2F* htowerphiVeta_JP2 = new TH2F("htowerphiVeta_JP2",";tower #eta V #phi",120,-3.14159,3.14159,76,-1.5,2.3);
    TH2F* htowerphiVeta_BHT3 = new TH2F("htowerphiVeta_BHT3",";tower #eta V #phi",120,-3.14159,3.14159,76,-1.5,2.3);
    TH1F* hbemcsoftIdJP1 = new TH1F("hbemcsoftIdJP1","hbemcsoftIdJP1",4800,0.5,4800.5);
    TH1F* hbemcsoftIdJP2 = new TH1F("hbemcsoftIdJP2","hbemcsoftIdJP2",4800,0.5,4800.5);
    TH1F* hbemcsoftIdBHT3 = new TH1F("hbemcsoftIdBHT3","hbemcsoftIdBHT3",4800,0.5,4800.5);
    TH1F* heemcsoftIdJP1 = new TH1F("heemcsoftIdJP1","heemcsoftIdJP1",720,0.5,720.5);
    TH1F* heemcsoftIdJP2 = new TH1F("heemcsoftIdJP2","heemcsoftIdJP2",720,0.5,720.5);
    TH1F* heemcsoftIdBHT3 = new TH1F("heemcsoftIdBHT3","heemcsoftIdBHT3",720,0.5,720.5);

    TH2F* htower_radiusJP1 = new TH2F("hjet_towerR_JP1","hjet_towerR_JP1",11,0,1.1,95,5,100);
    TH2F* htower_radiusJP2 = new TH2F("hjet_towerR_JP2","hjet_towerR_JP2",11,0,1.1,95,5,100);
    TH2F* htower_radiusBHT3 = new TH2F("hjet_towerR_BHT3","hjet_towerR_BHT3",11,0,1.1,95,5,100);


    //Vertex Information
    TH1F* hjetZverJP1 = new TH1F("hjetZverJP1","hjetZverJP1",1000,-100,100);
    TH1F* hjetZverJP2 = new TH1F("hjetZverJP2","hjetZverJP2",1000,-100,100);
    TH1F* hjetZverBHT3 = new TH1F("hjetZverBHT3","hjetZverBHT3",1000,-100,100);
    TH1F* hjetZverAJP = new TH1F("hjetZverAJP","hjetZverAJP",1000,-100,100);
    TH1F* hjetXverJP1 = new TH1F("hjetXverJP1","hjetXverJP1",1000,-2,2);
    TH1F* hjetXverJP2 = new TH1F("hjetXverJP2","hjetXverJP2",1000,-2,2);
    TH1F* hjetXverBHT3 = new TH1F("hjetXverBHT3","hjetXverBHT3",1000,-2,2);
    TH1F* hjetXverAJP = new TH1F("hjetXverAJP","hjetXverAJP",1000,-2,2);
  
    TH1F* hjetYverJP1 = new TH1F("hjetYverJP1","hjetYverJP1",1000,-2,2);
    TH1F* hjetYverJP2 = new TH1F("hjetYverJP2","hjetYverJP2",1000,-2,2);
    TH1F* hjetYverBHT3 = new TH1F("hjetYverBHT3","hjetYverBHT3",1000,-2,2);
    TH1F* hjetYverAJP = new TH1F("hjetYverAJP","hjetYverAJP",1000,-2,2);

    Int_t numtrackJP1, numtrackJP2, numtrackBHT3, numtrackAJP;
    Int_t numtowerJP1, numtowerJP2, numtowerBHT3, numtowerAJP ;
    Int_t numJetJP1, numJetJP2, numJetBHT3,numJetAJP, numJetTotal;
    Int_t numSoftTrigJP1, numSoftTrigJP2, numSoftTrigBHT3, numSoftTrigAJP;
    Int_t numHardTrigJP1, numHardTrigJP2, numHardTrigBHT3, numHardTrigAJP;
    Int_t numTrigJP1, numTrigJP2, numTrigBHT3, numTrigAJP;
    Double_t chargeTrackJP1,chargeTrackJP2,chargeTrackBHT3,chargeTrackAJP;
    Double_t radiusJP1,detaJP1,dphiJP1,radiusJP2,detaJP2,dphiJP2,radiusBHT3,detaBHT3,dphiBHT3;
    Double_t towerEtJP1, towerEtJP2, towerEtBHT3;
    chargeTrackJP1 = chargeTrackJP2 = chargeTrackBHT3 = chargeTrackAJP = 0;
    numSoftTrigJP1 = numSoftTrigJP2 =  numSoftTrigBHT3 = numSoftTrigAJP = 0;
    numHardTrigJP1 = numHardTrigJP2 =  numHardTrigBHT3 = numHardTrigAJP = 0;
    numTrigJP1 = numTrigJP2 = numTrigBHT3 = numTrigAJP = 0;
    radiusJP1 = detaJP1 = dphiJP1 =  radiusJP2 = detaJP2 = dphiJP2 = radiusBHT3 = detaBHT3 = dphiBHT3 =100;

    Int_t trig1 = TStTrigDef::GetTrigId("FMS-JP0");
    Int_t trig2 = TStTrigDef::GetTrigId("FMS-JP1");
    Int_t trig3 = TStTrigDef::GetTrigId("FMS-JP2");
    Int_t trig4 = TStTrigDef::GetTrigId("FMS-DiJP");
    
    //----- Event loop ----------
    //Loads entries from Jet Tree and Skim Tree simultaneously
    for (int iEntry = 0; iEntry < nentries; ++iEntry)
    {
	if (jetChain->GetEvent(iEntry) <= 0 || skimChain->GetEvent(iEntry) <= 0) break;

	// Should not be null
	assert(jetEvent && skimEvent);

	// Enforce event synchronization
	assert(jetEvent->runId() == skimEvent->runId() && jetEvent->eventId() == skimEvent->eventId());

	if (iEntry % 1000 == 0) cout << iEntry << endl;
	hrunnumber->Fill(jetEvent->runId());

	//FMS-JP1 trigger
	StJetSkimTrig* trigJP1 = skimEvent->trigger(trig2);
	if (!trigJP1) trigJP1 = skimEvent->trigger(trig2);
	//FMS-JP2 trigger
	StJetSkimTrig* trigJP2 = skimEvent->trigger(trig3);
	if (!trigJP2) trigJP2 = skimEvent->trigger(trig3);
	//FMS-JP0 trigger 
	StJetSkimTrig* trigBHT3 = skimEvent->trigger(trig1); //trigBHT3 to be interpreted as trigJP0
	if (!trigBHT3) trigBHT3 = skimEvent->trigger(trig1);
	//FMS-DiJP
	StJetSkimTrig* trigAJP = skimEvent->trigger(trig4);   //trigAJP to be intepreted as trigDiJP
	if (!trigAJP) trigAJP = skimEvent->trigger(trig4);

	map<int,int> barrelJetPatches = skimEvent->barrelJetPatchesAboveTh(1);
	map<int,int> endcapJetPatches = skimEvent->endcapJetPatchesAboveTh(1);
	map<int,int> overlapJetPatches = skimEvent->overlapJetPatchesAboveTh(1);

	StJetVertex* vertex = jetEvent->vertex();
	if (!vertex) continue;
	if(vertex->ranking() < 0) continue;
	if(fabs(vertex->position().z()) > 80.0) continue;

	Int_t htrigJP1, htrigJP2,htrigBHT3,htrigAJP = 0;
	Int_t strigJP1, strigJP2,strigBHT3,strigAJP = 0;
	if(trigJP1)
	{ 
	    if (trigJP1->didFire() == 1){ htrigJP1 = 1; numHardTrigJP1++;}
	    if(trigJP1->shouldFire() == 1){strigJP1 = 1; numSoftTrigJP1++;}
	    if(trigJP1->didFire() == 1 && trigJP1->shouldFire() == 1){ numTrigJP1++;}
	}
	if(trigJP2)
	{ 
	    if(trigJP2->didFire() == 1){htrigJP2 = 1; numHardTrigJP2++;}
	    if(trigJP2->shouldFire() == 1){strigJP2 = 1; numSoftTrigJP2++;}
	    if(trigJP2->didFire() == 1 && trigJP2->shouldFire() == 1){ numTrigJP2++;}
	}
	if(trigBHT3)
	{    
	    if(trigBHT3->didFire() == 1){htrigBHT3 = 1; numHardTrigBHT3++;}
	    if(trigBHT3->shouldFire() == 1){strigBHT3 = 1; numSoftTrigBHT3++;}
	    if(trigBHT3->didFire() == 1 && trigBHT3->shouldFire() == 1){ numTrigBHT3++;}
	}
	if(trigAJP)
	{    
	    if(trigAJP->didFire() == 1){htrigAJP = 1; numHardTrigAJP++;}
	    if(trigAJP->shouldFire() == 1){strigAJP = 1; numSoftTrigAJP++;}
	    if(trigAJP->didFire() == 1 && trigAJP->shouldFire() == 1){ numTrigAJP++;}
	}
	hardvssoftTrigJP1->Fill(strigJP1,htrigJP1);
	hardvssoftTrigJP2->Fill(strigJP2,htrigJP2);
	hardvssoftTrigBHT3->Fill(strigBHT3,htrigBHT3);
	hardvssoftTrigAJP->Fill(strigAJP,htrigAJP);
    
	numJetJP1 = numJetJP2 = numJetBHT3 = numJetAJP = numJetTotal =0;
	// Loop over jets
	for (int iJet = 0; iJet < vertex->numberOfJets(); ++iJet)
	{
	    StJetCandidate* jet = vertex->jet(iJet);
	    numtrackJP1 = numtrackJP2 = numtrackBHT3 = numtrackAJP = 0;
	    numtowerJP1 = numtowerJP2 = numtowerBHT3 = numtowerAJP = 0;
	    bool found = false;
	    if(trigJP1)
	    {
		if (trigJP1->didFire())
		{
		    if(trigJP1->shouldFire() ==1 )
		    {
			hjetpTJP1->Fill(jet->pt());
			hjetetaJP1->Fill(jet->eta());
			hjetphiJP1->Fill(jet->phi());
			hjetRtJP1->Fill(jet->neutralFraction());
			hjetXverJP1->Fill(vertex->position().x());
			hjetYverJP1->Fill(vertex->position().y());
			hjetZverJP1->Fill(vertex->position().z());
			numJetJP1++;
			found = true;
		    }
		}
	    }
	    if(trigJP2)
	    {
		if (trigJP2->didFire())
		{
		    if(trigJP2->shouldFire() ==1 )
		    {
			hjetpTJP2->Fill(jet->pt());
			hjetetaJP2->Fill(jet->eta());
			hjetphiJP2->Fill(jet->phi());
			hjetRtJP2->Fill(jet->neutralFraction());
			hjetXverJP2->Fill(vertex->position().x());
			hjetYverJP2->Fill(vertex->position().y());
			hjetZverJP2->Fill(vertex->position().z());
			numJetJP2++;
			found = true;
		    }
		}
	    }
	    if(trigBHT3)
	    {
		if (trigBHT3->didFire())
		{
		    if(trigBHT3->shouldFire() ==1 )
		    {
			hjetpTBHT3->Fill(jet->pt());
			hjetetaBHT3->Fill(jet->eta());
			hjetphiBHT3->Fill(jet->phi());
			hjetRtBHT3->Fill(jet->neutralFraction());
			hjetXverBHT3->Fill(vertex->position().x());
			hjetYverBHT3->Fill(vertex->position().y());
			hjetZverBHT3->Fill(vertex->position().z());
			numJetBHT3++;
			found = true;
		    }
		}
	    }
	    if(trigAJP)
	    {
		if (trigAJP->didFire())
		{
		    if(trigAJP->shouldFire() ==1 )
		    {
			hjetpTAJP->Fill(jet->pt());
			hjetetaAJP->Fill(jet->eta());
			hjetphiAJP->Fill(jet->phi());
			hjetRtAJP->Fill(jet->neutralFraction());
			hjetXverAJP->Fill(vertex->position().x());
			hjetYverAJP->Fill(vertex->position().y());
			hjetZverAJP->Fill(vertex->position().z());
			numJetAJP++;
			found = true;
		    }
		}
	    }
	    if(found == true) numJetTotal++;
	    //      if (jet->rt() > 0.98) continue;
	
	    //      if (!matchedToJetPatch(jet,barrelJetPatches,endcapJetPatches,overlapJetPatches)) continue; //geometric trigger. Is this jet responsible for the trigger?
	
	    // Loop over tracks
	    for (int iTrack = 0; iTrack < jet->numberOfTracks(); ++iTrack)
	    {
		StJetTrack* track = jet->track(iTrack);

		//Fill JP1 Track Track Information
		if (trigJP1)
		{
		    if (trigJP1->didFire())
		    {
			if(trigJP1->shouldFire() ==1 )
			{
			    hpTJP1->Fill(track->pt());
			    hetaJP1->Fill(track->eta());
			    hphiJP1->Fill(track->phi());
			    htracketaVsphiJP1->Fill(track->eta(),track->phi());
			    hnFitJP1->Fill(track->nHitsFit());
			    hnFitPossJP1->Fill(track->nHitsPoss());
			    Double_t dE_dxJP1 = track->dEdx()*1000000;
			    hdEdxJP1->Fill(dE_dxJP1);
			    hdcaMagJP1->Fill(track->dca().Mag());
			    hdcaDJP1->Fill(track->dcaD());
			    hdcaZJP1->Fill(track->dcaZ());
			    hfragJP1->Fill(track->frag());
			    hjTJP1->Fill(track->jt());
			    if(track->pt() >=  5.0)
			    {
				if(fabs(track->eta()) < 1.0)
				{
				    hChiSq1JP1->Fill(track->chi2());
				    hdcaMag1JP1->Fill(track->dca().Mag());
				    hdcaD1JP1->Fill(track->dcaD());
				    hdcaZ1JP1->Fill(track->dcaZ());
				}
				if(fabs(track->eta()) > 1.0)
				{
				    hChiSq2JP1->Fill(track->chi2());
				    hdcaMag2JP1->Fill(track->dca().Mag());
				    hdcaD2JP1->Fill(track->dcaD());
				    hdcaZ2JP1->Fill(track->dcaZ());
				}
			    }
			    dphiJP1 = track->phi() - jet->phi();
			    detaJP1 = track->eta() - jet->eta();
			    radiusJP1 = TMath::Sqrt(detaJP1*detaJP1 + dphiJP1*dphiJP1);
			    hradiusJP1->Fill(radiusJP1,jet->pt());

			    if(track->charge() > 0.0) hposTrackpTJP1->Fill(track->pt());
			    if(track->charge() < 0.0) hnegTrackpTJP1->Fill(track->pt());
			    chargeTrackJP1 = track->charge() * track->pt();
			    if(track->pt() < 30.0)
			    {
				htracklowpT_etaVsphiJP1->Fill(track->eta(),track->phi());
				htracklowpT_pTVsphiJP1->Fill(chargeTrackJP1,track->phi());
			    }
			    if(track->pt() > 30.0)
			    {
				htrack30pT_etaVsphiJP1->Fill(track->eta(),track->phi());
				htrack30pT_pTVsphiJP1->Fill(chargeTrackJP1,track->phi());
			    }
			    if(track->pt() > 40.0)
			    {
				htrack40pT_etaVsphiJP1->Fill(track->eta(),track->phi());
				htrack40pT_pTVsphiJP1->Fill(chargeTrackJP1,track->phi());
			    }
			    if(track->pt() > 50.0)
			    {
				htrack50pT_etaVsphiJP1->Fill(track->eta(),track->phi());
				htrack50pT_pTVsphiJP1->Fill(chargeTrackJP1,track->phi());
			    }
			    hchiSqJP1->Fill(track->chi2());
			    numtrackJP1++;
			}
		    }
		}
		//Fill JP2 Track Information
		if (trigJP2)
		{
		    if (trigJP2->didFire())
		    {
			if(trigJP2->shouldFire() ==1 )
			{
			    hpTJP2->Fill(track->pt());
			    hetaJP2->Fill(track->eta());
			    hphiJP2->Fill(track->phi());
			    htracketaVsphiJP2->Fill(track->eta(),track->phi());
			    hnFitJP2->Fill(track->nHitsFit());
			    hnFitPossJP2->Fill(track->nHitsPoss());
			    Double_t dE_dxJP2 = track->dEdx()*1000000;
			    hdEdxJP2->Fill(dE_dxJP2);
			    hdcaMagJP2->Fill(track->dca().Mag());
			    hdcaDJP2->Fill(track->dcaD());
			    hdcaZJP2->Fill(track->dcaZ());
			    hfragJP2->Fill(track->frag());
			    hjTJP2->Fill(track->jt());
			    if(track->pt() >=  5.0)
			    {
				if(fabs(track->eta()) < 1.0)
				{
				    hChiSq1JP2->Fill(track->chi2());
				    hdcaMag1JP2->Fill(track->dca().Mag());
				    hdcaD1JP2->Fill(track->dcaD());
				    hdcaZ1JP2->Fill(track->dcaZ());
				}
				if(fabs(track->eta()) > 1.0)
				{
				    hChiSq2JP2->Fill(track->chi2());
				    hdcaMag2JP2->Fill(track->dca().Mag());
				    hdcaD2JP2->Fill(track->dcaD());
				    hdcaZ2JP2->Fill(track->dcaZ());
				}
			    }
			    dphiJP2 = track->phi() - jet->phi();
			    detaJP2 = track->eta() - jet->eta();
			    radiusJP2 = TMath::Sqrt(detaJP2*detaJP2 + dphiJP2*dphiJP2);
			    hradiusJP2->Fill(radiusJP2,jet->pt());
			    hprTrackpTetaphiJP2->Fill(track->phi(),track->eta(),track->pt());
			    if(track->charge() > 0.0)
			    {
				hprTrackpTetaphiPosJP2->Fill(track->phi(),track->eta(),track->pt());
				hposTrackpTJP2->Fill(track->pt());
			    }
			    if(track->charge() < 0.0)
			    {
				hprTrackpTetaphiNegJP2->Fill(track->phi(),track->eta(),track->pt());
				hnegTrackpTJP2->Fill(track->pt());
			    }
			    chargeTrackJP2 = track->charge() * track->pt();
			    if(track->pt() < 30.0)
			    {
				htracklowpT_etaVsphiJP2->Fill(track->eta(),track->phi());
				htracklowpT_pTVsphiJP2->Fill(chargeTrackJP2,track->phi());
			    }
			    if(track->pt() > 30.0)
			    {
				htrack30pT_etaVsphiJP2->Fill(track->eta(),track->phi());
				htrack30pT_pTVsphiJP2->Fill(chargeTrackJP2,track->phi());
			    }
			    if(track->pt() > 40.0)
			    {
				htrack40pT_etaVsphiJP2->Fill(track->eta(),track->phi());
				htrack40pT_pTVsphiJP2->Fill(chargeTrackJP2,track->phi());
			    }
			    if(track->pt() > 50.0)
			    {
				htrack50pT_etaVsphiJP2->Fill(track->eta(),track->phi());
				htrack50pT_pTVsphiJP2->Fill(chargeTrackJP2,track->phi());
			    }
			    hchiSqJP2->Fill(track->chi2());
			    numtrackJP2++;
			}
		    }
		}
		//Fill BHT3 Track Information
		if (trigBHT3)
		{
		    if (trigBHT3->didFire())
		    {
			if(trigBHT3->shouldFire() ==1 )
			{
			    hpTBHT3->Fill(track->pt());
			    hetaBHT3->Fill(track->eta());
			    hphiBHT3->Fill(track->phi());
			    htracketaVsphiBHT3->Fill(track->eta(),track->phi());
			    hnFitBHT3->Fill(track->nHitsFit());
			    hnFitPossBHT3->Fill(track->nHitsPoss());
			    Double_t dE_dxBHT3 = track->dEdx()*1000000;
			    hdEdxBHT3->Fill(dE_dxBHT3);
			    hdcaMagBHT3->Fill(track->dca().Mag());
			    hdcaDBHT3->Fill(track->dcaD());
			    hdcaZBHT3->Fill(track->dcaZ());
			    hfragBHT3->Fill(track->frag());
			    hjTBHT3->Fill(track->jt());
			    if(track->pt() >=  5.0)
			    {
				if(fabs(track->eta()) < 1.0)
				{
				    hChiSq1BHT3->Fill(track->chi2());
				    hdcaMag1BHT3->Fill(track->dca().Mag());
				    hdcaD1BHT3->Fill(track->dcaD());
				    hdcaZ1BHT3->Fill(track->dcaZ());
				}
				if(fabs(track->eta()) > 1.0)
				{
				    hChiSq2BHT3->Fill(track->chi2());
				    hdcaMag2BHT3->Fill(track->dca().Mag());
				    hdcaD2BHT3->Fill(track->dcaD());
				    hdcaZ2BHT3->Fill(track->dcaZ());
				}
			    }
			    dphiBHT3 = track->phi() - jet->phi();
			    detaBHT3 = track->eta() - jet->eta();
			    radiusBHT3 = TMath::Sqrt(detaBHT3*detaBHT3 + dphiBHT3*dphiBHT3);
			    hradiusBHT3->Fill(radiusBHT3,jet->pt());
			    if(track->charge() > 0.0) hposTrackpTBHT3->Fill(track->pt());
			    if(track->charge() < 0.0) hnegTrackpTBHT3->Fill(track->pt());
			    chargeTrackBHT3 = track->charge() * track->pt();
			    if(track->pt() < 30.0)
			    {
				htracklowpT_etaVsphiBHT3->Fill(track->eta(),track->phi());
				htracklowpT_pTVsphiBHT3->Fill(chargeTrackBHT3,track->phi());
			    }
			    if(track->pt() > 30.0)
			    {
				htrack30pT_etaVsphiBHT3->Fill(track->eta(),track->phi());
				htrack30pT_pTVsphiBHT3->Fill(chargeTrackBHT3,track->phi());
			    }
			    if(track->pt() > 40.0)
			    {
				htrack40pT_etaVsphiBHT3->Fill(track->eta(),track->phi());
				htrack40pT_pTVsphiBHT3->Fill(chargeTrackBHT3,track->phi());
			    }
			    if(track->pt() > 50.0)
			    {
				htrack50pT_etaVsphiBHT3->Fill(track->eta(),track->phi());
				htrack50pT_pTVsphiBHT3->Fill(chargeTrackBHT3,track->phi());
			    }
			    hchiSqBHT3->Fill(track->chi2());
			    numtrackBHT3++;
			}
		    }
		}
		//Fill AJP Track Information
		if (trigAJP)
		{
		    if (trigAJP->didFire())
		    {
			if(trigAJP->shouldFire() ==1 )
			{
			    hpTAJP->Fill(track->pt());
			    hetaAJP->Fill(track->eta());
			    hphiAJP->Fill(track->phi());
			    htracketaVsphiAJP->Fill(track->eta(),track->phi());
			    hnFitAJP->Fill(track->nHitsFit());
			    hnFitPossAJP->Fill(track->nHitsPoss());
			    Double_t dE_dxAJP = track->dEdx()*1000000;
			    hdEdxAJP->Fill(dE_dxAJP);
			    hdcaMagAJP->Fill(track->dca().Mag());
			    hdcaDAJP->Fill(track->dcaD());
			    hdcaZAJP->Fill(track->dcaZ());
			    hfragAJP->Fill(track->frag());
			    hjTAJP->Fill(track->jt());
			    if(track->pt() >=  5.0)
			    {
				if(fabs(track->eta()) < 1.0)
				{
				    hChiSq1AJP->Fill(track->chi2());
				    hdcaMag1AJP->Fill(track->dca().Mag());
				    hdcaD1AJP->Fill(track->dcaD());
				    hdcaZ1AJP->Fill(track->dcaZ());
				}
				if(fabs(track->eta()) > 1.0)
				{
				    hChiSq2AJP->Fill(track->chi2());
				    hdcaMag2AJP->Fill(track->dca().Mag());
				    hdcaD2AJP->Fill(track->dcaD());
				    hdcaZ2AJP->Fill(track->dcaZ());
				}
			    }
			    if(track->charge() > 0.0) hposTrackpTAJP->Fill(track->pt());
			    if(track->charge() < 0.0) hnegTrackpTAJP->Fill(track->pt());
			    chargeTrackAJP = track->charge() * track->pt();
			    if(track->pt() < 30.0)
			    {
				htracklowpT_etaVsphiAJP->Fill(track->eta(),track->phi());
				htracklowpT_pTVsphiAJP->Fill(chargeTrackAJP,track->phi());
			    }
			    if(track->pt() > 30.0)
			    {
				htrack30pT_etaVsphiAJP->Fill(track->eta(),track->phi());
				htrack30pT_pTVsphiAJP->Fill(chargeTrackAJP,track->phi());
			    }
			    if(track->pt() > 40.0)
			    {
				htrack40pT_etaVsphiAJP->Fill(track->eta(),track->phi());
				htrack40pT_pTVsphiAJP->Fill(chargeTrackAJP,track->phi());
			    }
			    if(track->pt() > 50.0)
			    {
				htrack50pT_etaVsphiAJP->Fill(track->eta(),track->phi());
				htrack50pT_pTVsphiAJP->Fill(chargeTrackAJP,track->phi());
			    }
			    hchiSqAJP->Fill(track->chi2(),jet->pt());
			    numtrackAJP++;
			}
		    }
		}	
	    }	// End loop over tracks
	    //loop over Towers
	    for(int iTower = 0; iTower < jet->numberOfTowers(); iTower++)
	    {
		StJetTower* tower  = jet->tower(iTower);
		//Fill JP1 Tower Information
		if (trigJP1)
		{
		    if (trigJP1->didFire())
		    {
			if(trigJP1->shouldFire() ==1 )
			{
			    towerEtJP1 = tower->energy()/cosh(tower->eta());
			    htowerE_JP1->Fill(towerEtJP1);
			    htowereta_JP1->Fill(tower->eta());
			    htowerphi_JP1->Fill(tower->phi());
			    htowerphiVeta_JP1->Fill(tower->phi(),tower->eta());
			    htowerfragJP1->Fill(tower->frag());
			    htowerjTJP1->Fill(tower->jt());
			    if(tower->detectorId() == 9){hbemcsoftIdJP1->Fill(tower->id());}
			    if(tower->detectorId() == 13){heemcsoftIdJP1->Fill(tower->id());}
			    dphiJP1 = tower->phi() - jet->phi();
			    detaJP1 = tower->eta() - jet->eta();
			    radiusJP1 = TMath::Sqrt(detaJP1*detaJP1 + dphiJP1*dphiJP1);
			    htower_radiusJP1->Fill(radiusJP1,jet->pt());		
			    numtowerJP1++; 
			}
		    }
		}
		if (trigJP2)
		{
		    if (trigJP2->didFire())
		    {
			if(trigJP2->shouldFire() ==1 )
			{
			    towerEtJP2 = tower->energy()/cosh(tower->eta());
			    htowerE_JP2->Fill(towerEtJP2);
			    htowereta_JP2->Fill(tower->eta());
			    htowerphi_JP2->Fill(tower->phi());
			    htowerfragJP2->Fill(tower->frag());
			    htowerjTJP2->Fill(tower->jt());
			    htowerphiVeta_JP2->Fill(tower->phi(),tower->eta());
			    if(tower->detectorId() == 9){hbemcsoftIdJP2->Fill(tower->id());}
			    if(tower->detectorId() == 13){heemcsoftIdJP2->Fill(tower->id());}
			    dphiJP2 = tower->phi() - jet->phi();
			    detaJP2 = tower->eta() - jet->eta();
			    radiusJP2 = TMath::Sqrt(detaJP2*detaJP2 + dphiJP2*dphiJP2);
			    htower_radiusJP2->Fill(radiusJP2,jet->pt());
			    numtowerJP2++; 
			}
		    }
		}
		if (trigBHT3)
		{
		    if (trigBHT3->didFire())
		    {
			if(trigBHT3->shouldFire() ==1 )
			{
			    towerEtBHT3 = tower->energy()/cosh(tower->eta());
			    htowerE_BHT3->Fill(towerEtBHT3);
			    htowereta_BHT3->Fill(tower->eta());
			    htowerphi_BHT3->Fill(tower->phi());
			    htowerfragBHT3->Fill(tower->frag());
			    htowerjTBHT3->Fill(tower->jt());
			    htowerphiVeta_BHT3->Fill(tower->phi(),tower->eta());
			    if(tower->detectorId() == 9){hbemcsoftIdBHT3->Fill(tower->id());}
			    if(tower->detectorId() == 13){heemcsoftIdBHT3->Fill(tower->id());}
			    dphiBHT3 = tower->phi() - jet->phi();
			    detaBHT3 = tower->eta() - jet->eta();
			    radiusBHT3 = TMath::Sqrt(detaBHT3*detaBHT3 + dphiBHT3*dphiBHT3);
			    htower_radiusBHT3->Fill(radiusBHT3,jet->pt());
			    numtowerBHT3++; 
			}
		    }
		}

		if (trigAJP)
		{
		    if (trigAJP->didFire())
		    {
			if(trigAJP->shouldFire() ==1 )
			{
			    htowerE_AJP->Fill(tower->energy());
			    htowereta_AJP->Fill(tower->eta());
			    htowerphi_AJP->Fill(tower->phi());
			    htowerfragAJP->Fill(tower->frag());
			    htowerjTAJP->Fill(tower->jt());
			    numtowerAJP++; 
			}
		    }
		}	  
	    }// End loop over towers
	    //Filling the track and tower multiplicities
	    if (numtrackJP1 != 0)
	    {
		hnumJP1->Fill(numtrackJP1);
	    }
	    if (numtrackJP2 != 0)
	    {
		hnumJP2->Fill(numtrackJP2);
	    }
	    if (numtrackBHT3 != 0)
	    {
		hnumBHT3->Fill(numtrackBHT3);
	    }
	    if (numtrackAJP != 0)
	    {
		hnumAJP->Fill(numtrackAJP);
	    }
	    if (numtowerJP1 != 0)
	    {
		htowernum_JP1->Fill(numtowerJP1);
	    }
	    if (numtowerJP2 != 0)
	    {
		htowernum_JP2->Fill(numtowerJP2);
	    }
	    if (numtowerBHT3 != 0)
	    {
		htowernum_BHT3->Fill(numtowerBHT3);
	    }
	    if (numtowerAJP != 0)
	    {
		htowernum_AJP->Fill(numtowerAJP);
	    }      
	} // End loop over jets

	if (numJetJP1 != 0){hjetnumJP1->Fill(numJetJP1);}
	if (numJetJP2 != 0){hjetnumJP2->Fill(numJetJP2);}
	if (numJetBHT3 != 0){hjetnumBHT3->Fill(numJetBHT3);}
	if (numJetAJP != 0){hjetnumAJP->Fill(numJetAJP);}
	hjetnumTotal->Fill(numJetTotal);
    } // End event loop
    cout << "aahardtrigJP1: " << numHardTrigJP1 << endl;
    cout << "aahardtrigJP2: " << numHardTrigJP2 << endl;
    cout << "aahardtrigBHT3: " << numHardTrigBHT3 << endl;
    cout << "aahardtrigAJP: " << numHardTrigAJP << endl;

    cout << "bbsofttrigJP1: " << numSoftTrigJP1 << endl;
    cout << "bbsofttrigJP2: " << numSoftTrigJP2 << endl;
    cout << "bbsofttrigBHT3: " << numSoftTrigBHT3 << endl;
    cout << "bbsofttrigAJP: " << numSoftTrigAJP << endl;

    cout << "ccbothtrigJP1: " << numTrigJP1 << endl;
    cout << "ccbothtrigJP2: " << numTrigJP2 << endl;
    cout << "ccbothtrigBHT3: " << numTrigBHT3 << endl;
    cout << "ccbothtrigAJP: " << numTrigAJP << endl;
    // Write histograms and close output file
    ofile->Write();
    ofile->Close();
}

bool matchedToJetPatch(const StJetCandidate* jet,
		       const map<int,int>& barrelJetPatches,
		       const map<int,int>& endcapJetPatches,
		       const map<int,int>& overlapJetPatches)
{
    for (map<int,int>::const_iterator it = barrelJetPatches.begin(); it != barrelJetPatches.end(); ++it)
    {
	int id = it->first;
	int adc = it->second;
	float eta, phi;
	assert(StJetCandidate::getBarrelJetPatchEtaPhi(id,eta,phi));
	float deta = jet->detEta() - eta;
	float dphi = TVector2::Phi_mpi_pi(jet->phi() - phi);
	if (fabs(deta) < 0.6 && fabs(dphi) < 0.6) return true;
    }

    for (map<int,int>::const_iterator it = endcapJetPatches.begin(); it != endcapJetPatches.end(); ++it)
    {
	int id = it->first;
	int adc = it->second;
	float eta, phi;
	assert(StJetCandidate::getEndcapJetPatchEtaPhi(id,eta,phi));
	float deta = jet->detEta() - eta;
	float dphi = TVector2::Phi_mpi_pi(jet->phi() - phi);
	if (fabs(deta) < 0.6 && fabs(dphi) < 0.6) return true;
    }

    for (map<int,int>::const_iterator it = overlapJetPatches.begin(); it != overlapJetPatches.end(); ++it)
    {
	int id = it->first;
	int adc = it->second;
	float eta, phi;
	assert(StJetCandidate::getOverlapJetPatchEtaPhi(id,eta,phi));
	float deta = jet->detEta() - eta;
	float dphi = TVector2::Phi_mpi_pi(jet->phi() - phi);
	if (fabs(deta) < 0.6 && fabs(dphi) < 0.6) return true;
    }

    return false;
}
