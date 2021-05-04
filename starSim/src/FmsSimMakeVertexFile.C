// Filename: FmsSimMakeVertexFile.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Sat May  1 02:25:52 2021 (-0400)
// URL: jlab.org/~latif

void FmsSimMakeVertexFile(TString inFile)
{
    if(gSystem->AccessPathName(inFile))
    {
	cout << "Input file NOT found" <<endl;
	return;
    }
    
    TFile *f = new TFile(inFile);
    TH1D *hist = (TH1D*)f->Get("h1vtxZ");

    TH1D *genVtxZ = new TH1D("genVtxZ", "genVtxZ", 200, -200, 200);
    ofstream outFile("vertex.txt");
    
    Double_t vz;
    for(Int_t  i = 0; i < 1000; ++i)
    {
	vz = hist->GetRandom();
	outFile << 0 << "\t" << 0  << "\t" << vz <<endl;
	genVtxZ->Fill(vz);
    }

    outFile.close();
    
    genVtxZ->Draw();
}
