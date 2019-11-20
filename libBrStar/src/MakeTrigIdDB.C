// Filename: MakeTrigIdDB.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Nov 19 21:06:45 2019 (-0500)
// URL: jlab.org/~latif

void MakeTrigIdDB()
{
    TSQLServer *db = TSQLServer::Connect("mysql://db04.star.bnl.gov:3414/Conditions_rts?timeout=60","", "");
 
    TSQLRow *row;
    TSQLResult *res;

    TStRunList *list = new TStRunList();
    TEntryList *runList = list->GetRunList();
    Int_t nEntries = runList->GetN();
    Int_t run;
    
    ofstream outFile("TriggerIdDB.json");
    if(!outFile)
    {
	cout << "Unable to create output file" <<endl;
	return;
    }
    
    outFile << "{" <<endl;
    for(Int_t index = 0; index < nEntries; ++index)
    {
	run = runList->GetEntry(index);
	TString sql = Form("SELECT idx_rn,offlineBit,name FROM triggers WHERE idx_rn=%d", run);
	res = db->Query(sql);
 
	int nrows = res->GetRowCount(); 
	int nfields = res->GetFieldCount();
       	  
	for (int i = 0; i < nrows; i++)
	{
	    row = res->Next();

	    if(i == 0)
		outFile<<"\t\""<<row->GetField(0)<<"\": {";
	    outFile<<"\""<<row->GetField(2)<<"\": "<<row->GetField(1);
	    if(i != (nrows -1))
		outFile << ", " ;
	    if(i == (nrows -1))
		outFile << "}";
	    if(i == (nrows -1) && index != (nEntries - 1))
		outFile << ", " <<endl;
	}
    }
    outFile << "\n}" <<endl;
    outFile.close();
    delete row;
    delete res; //Must delete manually
    delete db;  //Must delete manually       
}
