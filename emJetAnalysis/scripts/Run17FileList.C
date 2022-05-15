// Filename: Run17FileList.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat May 14 14:24:37 2022 (-0400)
// URL: jlab.org/~latif

void Run17FileList()
{
    TString filePath = "Run17PhysFileList_batch3.txt";

    if(gSystem->AccessPathName(filePath))
    {
	cout << "Input file now found"<< endl;
	return;
    }
    
    ifstream inFile(filePath);
    ofstream outFile("Run17PhysRunList.json");
    
    string fileName;
    int fileCount = 0;

    const string startDelim = "st_physics_"; //Read this from configuration file
    const string stopDelim = "_raw_";
    unsigned firstDelimPos;
    unsigned endPosOfFirstDelim;
    unsigned lastDelimPos;
    string runNumberStr;
    
    outFile << "[" <<endl;
        
    while (!inFile.eof()) 
    {
	inFile >> fileName;
	// cout << fileName <<endl;
	++fileCount;

	firstDelimPos = fileName.find(startDelim);
	endPosOfFirstDelim = firstDelimPos + startDelim.length();
	lastDelimPos = fileName.find(stopDelim);
	runNumberStr = fileName.substr(endPosOfFirstDelim, lastDelimPos - endPosOfFirstDelim);
	// cout << runNumberStr <<endl;

	outFile << "{\"run\": "<< runNumberStr <<", \"data\": {\"file\": \""<< fileName <<"\", \"size\": -1, \"events\": -1}}, "<< endl;
	
    }
    outFile << "{\"run\":-1, \"data\": {\"file\": \"\", \"size\": -1, \"events\": -1}}]" <<endl;
    cout << "Total number of files: "<< fileCount <<endl;

    inFile.close();
}

