// Filename: SeparateRunsByTime.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Jul 16 23:54:44 2024 (-0400)
// URL: jlab.org/~latif

void SeparateRunsByTime()
{
    ifstream inFile("/star/u/kabir/GIT/BrightSTAR/database/Run15FillStartTime.txt");

    if(!inFile)
    {
	cout << "Unable to open input file" << endl;
	return;
    }
    
    int fillNo;
    int energy;
    int startTime;
    int endTime;

    while(!inFile.eof())
    {
	inFile >> fillNo >> energy >> startTime >> endTime;
	cout << fillNo << "\t" << energy << "\t" << startTime << "\t" << endTime <<endl;
    }

    inFile.close();
    
    // TFile *f = new TFile("/star/u/kabir/GIT/BrightSTAR/dst/emJet/run15/pass6_1GeVPtCut/fms/NanoJetTree_EjRunEmJetTreeMaker_16066033.root");
    // TTree *T = (TTree*)f->Get("T");
    // T->Scan("jetEvents.mSkimEvent.mUnixTime");

    TStFillNoDB fillDb;
    TStRunList runListDb;
    TEntryList *runList = runListDb.GetRunList(-1, -1);
    runList->Print("all");

    // for(Int_t r = 0; r < runList->GetN(); ++r)
    // {
    // 	//runNumber = runList[r];
    // 	runNumber = runList->GetEntry(r);
    // 	fileName = fileNamePrefix + to_string(runNumber) + ".root";
    // 	if(gSystem->AccessPathName(fileName))
    // 	{
    // 	    cout << "File Not found:" << fileName <<endl;
    // 	    cout << "SKIPPED" <<endl;
    // 	    continue;
    // 	}

    // 	//Skip wrongly filled spin pattern runs: See Run 15 Polarization file. Fill: 18764
    // 	fillNoFmData = fillDb.GetFillNo(runNumber); //Get it from the DST in the future	
    // }
    
    
}
