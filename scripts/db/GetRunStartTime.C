// Filename: GetRunStartTime.C
// Description: 
// Author: Latif Kabir < kabir@bnl.gov >
// Created: Tue Nov 19 21:06:45 2019 (-0500)
// URL: jlab.org/~latif

void GetRunStartTime()
{
    TSQLServer *db = TSQLServer::Connect("mysql://db04.star.bnl.gov:3414/RunLog?timeout=60","", "");
 
    printf("Server info: %s\n", db->ServerInfo());
 
    TSQLRow *row;
    TSQLResult *res;
 
    // list databases available on server
    printf("\nList all databases on server %s\n", db->GetHost());
    res = db->GetDataBases();
    while ((row = res->Next()))
    {
	printf("%s\n", row->GetField(0));
	delete row;
    }
    delete res;

    Int_t runNumber = 16066033;
    //Get Torus and Solenoid setting
    TString sql = Form("select from_unixtime(startRunTime) from runDescriptor WHERE runNumber=%d", runNumber);


    res = db->Query(sql);
 
    int nrows = res->GetRowCount();
    printf("\nGot %d rows in result\n", nrows);
 
    int nfields = res->GetFieldCount();
    for (int i = 0; i < nfields; i++)
	printf("%40s", res->GetFieldName(i));
    printf("\n");
    for (int i = 0; i < nfields*40; i++)
	printf("=");
    printf("\n");
 
    for (int i = 0; i < nrows; i++)
    {
	row = res->Next();
	for (int j = 0; j < nfields; j++)
	{
	    printf("%40s", row->GetField(j));
	}
	printf("\n");
	delete row;
    }
 
    delete res; //Must delete manually
    delete db; // Must delete manually

    
    
}
