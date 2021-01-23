TCanvas *c = new TCanvas();
TGraphErrors *ptAn1g[13];
void readdPhi(){
  c->Divide(4,5,0);

  d(1);
  d(2);
  d(3);
  d(4);

}

int d(int case1  =0){
 gStyle->SetLabelSize(.08,"X");
 gStyle->SetLabelSize(.08,"Y");
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

  if(case1==1)    infile.open("jdPhi-le2ph/xf1/case.txt");
  if(case1==3)    infile.open("jdPhi-le2ph/nxf1/case.txt");
  
  if(case1==2)    infile.open("jdPhi-ge3ph/xf1/case.txt");
  if(case1==4)    infile.open("jdPhi-ge3ph/nxf1/case.txt");
  
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
   
    deta1[counter1] = b7;
    detae1[counter1] = b8;

    dphi1[counter1] = b5;
    dphie1[counter1] = b6;

    counter1++;
  }

  infile.close();


 TGraphErrors *ptAn1 = new TGraphErrors(counter1-1,dphi1,an1,dphie1,ane1);
 TGraphErrors *ptAn2 = new TGraphErrors(counter1-1,dphi1,pt1,dphie1,pte1);
 TGraphErrors *ptAn3 = new TGraphErrors(counter1-1,dphi1,en1,dphie1,ene1);
 TGraphErrors *ptAn4 = new TGraphErrors(counter1-1,dphi1,eta1,dphie1,etae1);
 TGraphErrors *ptAn5 = new TGraphErrors(counter1-1,dphi1,deta1,dphie1,detae1);

 // TGraphErrors *ptAn1 = new TGraphErrors(counter1-1,eta1,an1,eta1,ane1);

 c->cd(case1);
 
   ptAn1->SetMarkerColor(2);
  ptAn1->SetLineColor(2);
  ptAn1->SetMarkerSize(.3);
 ptAn1->SetMarkerStyle(20);
 ptAn1->GetYaxis()->SetRangeUser(-.02,.05);
 ptAn1->DrawClone("ap");

 if(case1<5){
  TLine *t =  new TLine(-1.5, 0, 5.2, 0);
 t->SetLineStyle(1);
 t->SetLineWidth(2);
 t->SetLineColor(kBlue);
 t->Draw();
 }

 ptAn2->GetYaxis()->SetRangeUser(4,4);
 ptAn3->GetYaxis()->SetRangeUser(60,80);
 ptAn4->GetYaxis()->SetRangeUser(3,4);
 ptAn5->GetYaxis()->SetRangeUser(2,4);

 c->cd(case1+4);
 ptAn2->DrawClone("ap");
 c->cd(case1+8);
 ptAn3->DrawClone("ap");
 c->cd(case1+12);
 ptAn4->DrawClone("ap");
 c->cd(case1+16);
 ptAn5->DrawClone("ap");

}
