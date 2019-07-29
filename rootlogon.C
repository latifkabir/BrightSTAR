// Filename: rootlogon.C
// Description: Loading libBrStar automatically each time you start root.
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 15 01:22:13 2017 (-0500)
// URL: jlab.org/~latif
{
    cout << "\n\t\t=======================================================" <<endl;
    cout << "\t\t|  Welcome to Bright STAR Analysis Framework at RHIC  |"<<endl;
    cout << "\t\t|   Type: 'help()' for a list of available options    |" <<endl;
    cout << "\t\t|   Report issues to: kabir@bnl.gov                   |" <<endl;
    cout << "\t\t=======================================================" <<endl;

    if(getenv("STARHOME") == NULL)
    {
    	cout<<"Please run \"source setup.[c]sh\" to load environments before you start ROOT"<<endl;
	gApplication->Terminate();
    }

    //--------- Dynamically link needed shared lib----------------
    //MuDST Related
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
    cout<<"\t\t done loading libStMuDSTMaker.so ...."<<endl;

    //-------- EEMC Related ------------
    gSystem->Load("St_db_Maker");
    gSystem->Load("StEEmcDbMaker");
    gSystem->Load("StEEmcUtil");
    gSystem->Load("StEmcRawMaker");
    gSystem->Load("StEmcADCtoEMaker");
    gSystem->Load("StEmcTriggerMaker");
    gSystem->Load("StDbBroker");
    gSystem->Load("StEEmcPoolmuDst");
    gSystem->Load("StEEmcA2EMaker");
    gSystem->Load("StEEmcPoolEEmcTreeContainers");
    
    //----------- FMS Related -----------------
    gSystem->Load("StFmsUtil");
    gSystem->Load("StFmsDbMaker");

    //-------------Trigger --------------
    gSystem->Load("StTriggerUtilities");

    //------------- Spin DB -----------------
    gSystem->Load("StSpinDbMaker");
    
    //------ Bright Maker -------------
    gSystem->Load("libBrEEmcMaker");
    gSystem->Load("libBrRpMaker");
    gSystem->Load("libBrExampleMaker");
    gSystem->Load("libRunMacros");
    //gSystem->Load("libPi0MakerIU");
    
    //------- Bright STAR Library --------
    cout << "\t\t  Attempting to load libBrStar.so ... ";
    int status1 = gSystem->Load("libBrStar.so");
    if(status1>-1)
	cout << "attempt completed !!\n";
    else
	cout << "attempt failed.\n";

    cout << "\t\t  Attempting to load libStAnalysis.so ...";
    int status2=gSystem->Load("libStAnalysis.so");
    if(status2>-1)
    	cout << "attempt completed !!\n\n";
    else
    	cout << "attempt failed.\n\n";
    //gInterpreter->AddIncludePath("src/");       
    //gStyle->SetMarkerStyle(7);    // Filled square
    gStyle->SetMarkerStyle(4);    // Circle
    gStyle->SetMarkerSize(.4);     // larger than half a pixel ...
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
   
}

