// Filename: rootlogon.C
// Description: Loading libBrStar automatically each time you start root.
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 15 01:22:13 2017 (-0500)
// URL: jlab.org/~latif
{
    cout << "\n\t\t=======================================================" <<endl;
    cout << "\t\t|  Welcome to Bright-STAR Analysis Framework at RHIC  |"<<endl;
    cout << "\t\t|   Type: 'help()' for a list of available options    |" <<endl;
    cout << "\t\t|   Report issues to: kabir@bnl.gov                   |" <<endl;
    cout << "\t\t=======================================================" <<endl;

    if(getenv("STARHOME") == NULL)
    {
    	cout<<"Please run \"source setup.[c]sh\" to load environments before you start ROOT"<<endl;
    	gApplication->Terminate();
    }
    else if(getenv("STARHOME") == "" || getenv("STARHOME") == "/")
	gSystem->Setenv("STARHOME", gSystem->pwd());
    
    //-------------------------------------------------------------
    // if you want to use root.exe instead of root4star, uncomment block below:
    gSystem->AddDynamicPath("/usr/lib/mysql");
    gSystem->AddDynamicPath("/usr/lib64/mysql");
    gSystem->AddDynamicPath("$OPTSTAR/lib/mysql/");
    gSystem->Load("libmysqlclient");
    //--------- Dynamically link needed shared lib----------------
    //Root
    gSystem->Load("libMinuit.so");
    
    //--------- MuDST Related --------------
    gSystem->Load("libStTableUtilities");
    gSystem->Load("libTable");
    gSystem->Load("libPhysics");
    gSystem->Load("St_base");
    gSystem->Load("StChain");
    gSystem->Load("St_Tables");
    gSystem->Load("StUtilities");        // new addition 22jul99
    gSystem->Load("StTreeMaker");
    gSystem->Load("StIOMaker");
    gSystem->Load("StarClassLibrary");
    gSystem->Load("StTriggerDataMaker"); // new starting from April 2003
    gSystem->Load("StBichsel");
    gSystem->Load("StEvent");
    gSystem->Load("StEventUtilities");
    gSystem->Load("StDbLib");
    gSystem->Load("StEmcUtil");
    gSystem->Load("StTofUtil");
    gSystem->Load("StPmdUtil");
    gSystem->Load("StPreEclMaker");
    gSystem->Load("StStrangeMuDstMaker");
    gSystem->Load("StMuDSTMaker");
    //assert(!gSystem->Load("$STARHOME/.sl73_gcc485/lib/StMuDSTMaker.so"));  // Local copy of StMuDstMaker in batch mode

    //-------------Trigger --------------
    gSystem->Load("StTriggerUtilities");
    //------------- DB -----------------
    gSystem->Load("StTpcDb");
    gSystem->Load("StSpinDbMaker");
    gSystem->Load("St_db_Maker");
    gSystem->Load("StDbBroker");
    gSystem->Load("StDbUtilities");
    gSystem->Load("StDetectorDbMaker");

    //--------- StEvent ----------------
    //gSystem->Load("StEvent");
    gSystem->Load("StEventMaker");
    gSystem->Load("StDaqLib");
    
    //-------- EEMC Related ------------
    gSystem->Load("StEEmcDbMaker");
    gSystem->Load("StEEmcUtil");
    gSystem->Load("StEmcRawMaker");
    gSystem->Load("StEmcADCtoEMaker");
    gSystem->Load("StEmcTriggerMaker");
    gSystem->Load("StDbBroker");
    gSystem->Load("StEEmcPoolmuDst");
    gSystem->Load("StEEmcA2EMaker");
    //TSIU Algorithm
    gSystem->Load("StEEmcPoolEEmcTreeContainers");  //Note: Atypical shared object name
    gSystem->Load("StEEmcTreeMaker");
    //IU Algorithm
    gSystem->Load("StEEmcHitMaker");
    gSystem->Load("StEEmcClusterMaker");    
    gSystem->Load("StEEmcPointMaker");
    gSystem->Load("StEEmcIUPi0");
    
    //----------- FMS Related -----------------
    gSystem->Load("StFmsUtil");
    gSystem->Load("${STARHOME}/.sl73_gcc485/lib/StFmsDbMaker.so"); //Local copy of StFmsDbMaker
    gSystem->Load("StFmsHitMaker");
    gSystem->Load("StFmsPointMaker");
    gSystem->Load("StFmsFpsMaker");
    
    //---------- EMC (BEMC) ------------------
    gSystem->Load("StEmcRawMaker");
    gSystem->Load("StEmcADCtoEMaker");
    gSystem->Load("StEmcTriggerMaker");
    gSystem->Load("StEpcMaker");

    //------------- BTOF ---------------
    gSystem->Load("StBTofUtil");
    
    //--------- Jet Maker -------------
    gSystem->Load("StTriggerUtilities");
    gSystem->Load("StMCAsymMaker");
    gSystem->Load("StRandomSelector");
    
    gSystem->Load("libfastjet.so");
    gSystem->Load("libsiscone.so");
    gSystem->Load("libsiscone_spherical.so");
    gSystem->Load("libfastjetplugins.so");
 
    gSystem->Load("StJetFinder");
    gSystem->Load("StJetSkimEvent");
    gSystem->Load("StJets");
    gSystem->Load("$STARHOME/.sl73_gcc485/lib/StJetEvent.so"); //Use local copy
    gSystem->Load("$STARHOME/.sl73_gcc485/lib/StUeEvent.so"); //Use local copy
    gSystem->Load("$STARHOME/.sl73_gcc485/lib/StJetMaker.so"); //Use local copy
    //gSystem->Load("StJetEvent");
    //gSystem->Load("StUeEvent");
    //gSystem->Load("StJetMaker");
    gSystem->Load("StTriggerFilterMaker");

    //------------- RP ------------------
    gSystem->Load("StMuRpsUtil.so"); //RP Afterburner

    //------------ Simulation ---------------
    gSystem->Load("StBfcTriggerFilterMaker");
    gSystem->Load("StBFChain");
    
    cout<<"\t\t done loading StRoot libraries ...."<<endl;

    cout << "\t\t  Attempting to load BrightSTAR libraries ... ";
    //------ Bright Star Maker Modules-------------
    gSystem->Load("BrTemplateMaker");
    gSystem->Load("BrEEmcMaker");
    gSystem->Load("BrRpMaker");
    gSystem->Load("BrFmsMaker");    
    gSystem->Load("BrEmcMaker");    
    gSystem->Load("BrPidMaker");    
    //gSystem->Load("BrPicoMaker");  //Need to load dependencies  
    gSystem->Load("BrFemtoMaker");    
    gSystem->Load("StFmsJetAnalysisMaker");    
    gSystem->Load("BrContainers");    
    gSystem->Load("BrNanoDstMaker");    
    gSystem->Load("BrJetMaker");    
        
    //------- Bright STAR Library --------
    //Use $STARHOME/<lib module>/lib/ prefix to load local version

    gSystem->Load("libBrStar.so");
    gSystem->Load("libStAnalysis.so");
    gSystem->Load("libRunMacros.so");
    
    gSystem->Load("libEmJetAnalysis.so");
    gSystem->Load("libDiffAnalysis.so");
    //gSystem->Load("libStarSim.so");
    
    gSystem->Load("libCepAnalysis.so");
    
    cout << "attempt completed !!\n\n";


    //gInterpreter->AddIncludePath("src/");       
    //gStyle->SetMarkerStyle(7);    // Filled square
    gStyle->SetMarkerStyle(4);      // Circle
    gStyle->SetMarkerSize(.4);      // larger than half a pixel ...
    gStyle->SetMarkerColor(kBlack);
    //gStyle->SetMarkerColor(kRed);
   
    //------------- Plotting Style -----------------------------
    TStyle* dStyle = new TStyle("Bright_Star_Style", "Bright_Star_Style");
    dStyle->Reset("Modern");

    //Size & Margins
    dStyle->SetCanvasDefH(800);
    dStyle->SetCanvasDefW(1200);

    //Basic object fill colors
    dStyle->SetCanvasBorderMode(0);
    dStyle->SetLegendFillColor(0);
    dStyle->SetCanvasColor(0);

    //Stat box
    // dStyle->SetOptFit(112);
    // dStyle->SetOptStat(10); //entries only //mean/rms should be replaced with a fit

    //Titles and Labels
    dStyle->SetLabelSize(0.05, "xyz");
    dStyle->SetTitleSize(0.055, "xyz");
    dStyle->SetTitleOffset(1.1, "x");
    dStyle->SetTitleOffset(0.8, "y");

    //Margins (correlated with title/label size)
    dStyle->SetPadBottomMargin(0.15);

    //Default Histogram Style Settings
    //dStyle->SetHistFillColor(kTeal);
    //dStyle->SetHistFillStyle(1001); //solid fill (0 for hollow) //see http://root.cern.ch/root/html/TAttFill.html
    dStyle->SetHistLineColor(kBlack);
    dStyle->SetHistMinimumZero(kTRUE);

    //Default Function Style Settings
    dStyle->SetFuncColor(kBlack);
    dStyle->SetFuncStyle(1); //see http://root.cern.ch/root/html/TAttLine.html
    dStyle->SetFuncWidth(3);

    //Default Line Style Settings
    dStyle->SetLineColor(kBlack);
    dStyle->SetLineStyle(1); //see http://root.cern.ch/root/html/TAttLine.html
    dStyle->SetLineWidth(1); //is border of TLegend

    //Default Graph Style Settings
    dStyle->SetMarkerColor(kBlack);
    dStyle->SetMarkerSize(1); //see http://root.cern.ch/root/html/TAttMarker.html
    dStyle->SetMarkerStyle(22); //see http://root.cern.ch/root/html/TAttMarker.html

    //Grid
    dStyle->SetPadGridX(kFALSE);
    dStyle->SetPadGridY(kFALSE);

    //Histogram/Graph Title Size
    dStyle->SetTitleSize(0.075, "t"); //"t": could be anything that is not "x" "y" or "z"

    dStyle->cd();

    //-----------------------------------
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

    using namespace TStar;

}

