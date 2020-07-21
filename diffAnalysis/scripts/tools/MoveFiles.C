// Filename: GenerateFillDB.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Fri Apr 10 12:52:01 2020 (-0400)
// URL: jlab.org/~latif


void MoveFiles()
{
    // TStFillNoDB db;
    // vector <int> v = db.GetAllFillNos();

    // for(int i = 0; i < v.size(); ++i)
    // {
    // 	TString path = Form("/star/u/kabir/GIT/BrightSTAR/scratch/FmsPhotonAngDist_Fill_%i/\*", v[i]);
    // 	TString command = ".! mv " + path + " /star/u/kabir/GIT/BrightSTAR/scratch/FmsPhotonAngDist/.";
    // 	cout << command <<endl;

    // 	gROOT->ProcessLine(command);
    // }


    ifstream inFile("/star/u/kabir/GIT/BrightSTAR/scratch/FmsPhotonAngDist/runList.txt");
    int run;
    while(!inFile.eof())
    {
	inFile >> run;
	TString pth = "/star/u/kabir/GIT/BrightSTAR/scratch/FmsPhotonAngDist/";
	pth += run;
	pth += "/*";
	TString command = ".! mv " + pth + " /star/u/kabir/GIT/BrightSTAR/scratch/FmsPhotonAngDist/.";
	cout << command <<endl;
	gROOT->ProcessLine(command);
    }
    
}
