TCanvas *c = new TCanvas();
TGraphErrors *ptAn1g[13];
void readEta2(){
  c->Divide(4,4,0);
  gStyle->SetLabelSize(.08,"X");
  gStyle->SetLabelSize(.08,"Y");

  d(1);
  d(2);
  d(3);
  d(4);



}

int d(int case1  =0){


  float pt, pte, an, ane, chi2;
  float b1,b2,b3,b4,b5,b6,b7,b8;

  int cnt, ndf;
  int counter1 = 0;
  int counter6 = 0;

  float pt1[30], pte1[30], an1[30], ane1[30];

  float en1[30], ene1[30];
  float eta1[30], etae1[30];
  float deta1[30], detae1[30];
  float dphi1[30], dphie1[30];



  ifstream infile;

  if(case1==1)  infile.open("jEta-le2ph/xf1/case.txt");
  if(case1==2)  infile.open("jEta-ge3ph/xf1/case.txt");

  if(case1==3)  infile.open("jEta-le2ph-C/xf1/case.txt");
  if(case1==4)  infile.open("jEta-ge3ph-C/xf1/case.txt");


  while(infile){
    infile>>pt>>pte>>an>>ane>>b1>>b2>>b3>>b4>>b5>>b6>>b7>>b8>>chi2>>ndf;
    cout<<"counter1 : "<<counter1<<" "<<pt<<"  "<<pte<<"  "<<an<<"  "<<ane<<"  "<<chi2<<"  "<<ndf<<endl;

    pt1[counter1] = pt;
    pte1[counter1] = pte;
    an1[counter1] = an;
    ane1[counter1] = ane;
    
    en1[counter1] = b1;
    ene1[counter1] = b2;

    eta1[counter1] = b3;
    etae1[counter1] = b4;
   
    deta1[counter1] = b5;
    detae1[counter1] = b6;

    dphi1[counter1] = b7;
    dphie1[counter1] = b8;

    counter1++;
  }

  infile.close();


 TGraphErrors *ptAn1 = new TGraphErrors(counter1-1,eta1,an1,etae1,ane1);
 TGraphErrors *ptAn2 = new TGraphErrors(counter1-1,eta1,pt1,etae1,pte1);
 TGraphErrors *ptAn3 = new TGraphErrors(counter1-1,eta1,en1,etae1,ene1);
 TGraphErrors *ptAn4 = new TGraphErrors(counter1-1,eta1,deta1,etae1,detae1);
 // TGraphErrors *ptAn1 = new TGraphErrors(counter1-1,eta1,an1,eta1,ane1);

 c->cd(case1);
 
   ptAn1->SetMarkerColor(2);
  ptAn1->SetLineColor(2);
  ptAn1->SetMarkerSize(1.2);
 ptAn1->SetMarkerStyle(20);
 ptAn1->GetYaxis()->SetRangeUser(-.01,.07);

 ptAn1->DrawClone("ap");

 ptAn2->GetYaxis()->SetRangeUser(1,9);
 ptAn3->GetYaxis()->SetRangeUser(40,80);
 ptAn4->GetYaxis()->SetRangeUser(0,5);

 c->cd(case1+4);
 ptAn2->DrawClone("ap");
 c->cd(case1+8);
 ptAn3->DrawClone("ap");
 c->cd(case1+12);
 ptAn4->DrawClone("ap");

}
