// Filename: rootlogon.C
// Description: Loading libBrStar automatically eact time you start root.
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Wed Nov 15 01:22:13 2017 (-0500)
// URL: latifkabir.github.io
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

    // Dynamically link needed shared lib
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
    
    cout << "\t\t  Attempting to load libBrStar.so ... ";
    int status1 = gSystem->Load("libBrStar.so");
    if(status1>-1)
	cout << "attempt completed !!\n";
    else
	cout << "attempt failed.\n";

    // cout << "\t\t  Attempting to load libBrStarAnalysis.so ...";
    // int status2=gSystem->Load("libBrStarAnalysis.so");
    // if(status2>-1)
    // 	cout << "attempt completed !!\n\n";
    // else
    // 	cout << "attempt failed.\n\n";
    //gInterpreter->AddIncludePath("src/");       
    //gStyle->SetMarkerStyle(7);    // Filled square
    gStyle->SetMarkerStyle(4);    // Circle
    gStyle->SetMarkerSize(.4);     // larger than half a pixel ...
    gStyle->SetMarkerColor(kBlack);
    //gStyle->SetMarkerColor(kRed);

    //------------ Preety Color platter -----------------------
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}

