//
// Sample MuDst reader
//

void RunEEmcMakerEx1(TString InputFileList="", TString outputfile="EEmcMakerEx1.root")
{
    if(InputFileList == "")
	InputFileList="root://xrdstar.rcf.bnl.gov:1095//home/starlib/home/starreco/reco/production_pp200trans_2015/ReversedFullField/P16id/2015/090/16090026/st_physics_16090026_raw_5500035.MuDst.root";	
  // Create chain

  StChain* chain = new StChain;

  // Instantiate MuDst maker

  StMuDstMaker* muDstMaker = new StMuDstMaker(0,0,"",InputFileList,".",1000,"MuDst");
  StMuDbReader* muDB = StMuDbReader::instance();

  // STAR database
  St_db_Maker* dbMaker = new St_db_Maker("StarDb", "MySQL:StarDb");

  // EEMC database
  StEEmcDbMaker* eemcDb = new StEEmcDbMaker;

  // Barrel ADC to energy maker
  StEmcADCtoEMaker* adc2e = new StEmcADCtoEMaker;
  adc2e->saveAllStEvent(true);

  TStEEmcMakerEx1* muAna = new TStEEmcMakerEx1;
  muAna->setOutFile(outputfile);

  chain->Init();
  Int_t nEntries = muDstMaker->chain()->GetEntries();
      
  // Event loop
  for (int iEvent = 1; iEvent <= nEntries; ++iEvent)
  {
    chain->Clear();
    int status = chain->Make(iEvent);
    if(iEvent%100 == 0)
	cout << "Events processed:"<<iEvent<<endl;
  }
}
