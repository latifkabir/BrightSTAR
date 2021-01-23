TCanvas *c = new TCanvas();
TGraphErrors *ptAn1g[13];
void readxF(){
  c->Divide(3,2);

  d(1);
  d(2);
  d(3);
  d(4);
  d(5);
  d(6);
  d(7);
  d(8);
  d(9);
  d(10);
  d(11);
  d(12);


}

int d(int case1  =0){

  //int  counter11==0;

  float pt, pte, an, ane, chi2;
  int ndf;
  int cnt;
  int counter1 = 0;
  int counter2 = 0;
  int counter3 = 0;

  int counter4 = 0;
  int counter5 = 0;
  int counter6 = 0;

  float pt1[30], pte1[30], an1[30], ane1[30], chi21[30];
  float ndf1[30];
  float pt2[30], pte2[30], an2[30], ane2[30], chi22[30];
  float ndf2[30];
  float pt3[30], pte3[30], an3[30], ane3[30], chi23[30];
  float ndf3[30];

 float pt4[30], pte4[30], an4[30], ane4[30], chi24[30];
  float ndf4[30];
  float pt5[30], pte5[30], an5[30], ane5[30], chi25[30];
  float ndf5[30];
  float pt6[30], pte6[30], an6[30], ane6[30], chi26[30];
  float ndf6[30];

  float anP, ptP;
  //  counter1++;

  ifstream infile;
  if(case1%2==0)continue;
  if(case1==1)  infile.open("jxF-1ph/xf1/case.txt");
  if(case1==2)  infile.open("jxF-1ph/nxf1/case.txt");

  if(case1==3)  infile.open("jxF-pi0/xf1/case.txt");
  if(case1==4)  infile.open("jxF-pi0/nxf1/case.txt");

  if(case1==5)  infile.open("jxF-3ph/xf1/case.txt");
  if(case1==6)  infile.open("jxF-3ph/nxf1/case.txt");

  if(case1==7)  infile.open("jxF-1ph-C/xf1/case.txt");
  if(case1==8)  infile.open("jxF-1ph-C/nxf1/case.txt");

  if(case1==9)  infile.open("jxF-pi0-C/xf1/case.txt");
  if(case1==10)  infile.open("jxF-pi0-C/nxf1/case.txt");

  if(case1==11)  infile.open("jxF-3ph-C/xf1/case.txt");
  if(case1==12)  infile.open("jxF-3ph-C/nxf1/case.txt");


  while(infile){
    infile>>pt>>pte>>an>>ane>>chi2>>ndf;
    cout<<"counter1 : "<<counter1<<" "<<pt<<"  "<<pte<<"  "<<an<<"  "<<ane<<"  "<<chi2<<"  "<<ndf<<endl;

   
    pt1[counter1] = pt;
    pte1[counter1] = pte;
    an1[counter1] = an;
    ane1[counter1] = ane;
    chi21[counter1] = chi2;
    ndf1[counter1] = ndf;
   
    counter1++;
    //    if(counter1==1 && counter11==0) {counter1=0; counter11=10;}
  }

  infile.close();

  for(int iv=0; iv<counter1; iv++){
    cout<<"case pt --- >  " <<case1<<"  "<<pt1[iv] << endl;
  }

 TGraphErrors *ptAn1 = new TGraphErrors(counter1-1,pt1,an1,pte1,ane1);
  TGraph *ptchi21 = new TGraph(counter1-1,pt1,chi21);
  TGraph *ptndf1 = new TGraph(counter1-1,pt1,ndf1);

  
  // TCanvas *c = new TCanvas("c","",10,10,800,600);


 c->cd(int(case1+1)/2);
 if(case1%2==1){
   ptAn1->SetMarkerColor(2);
  ptAn1->SetLineColor(2);
  ptAn1->SetMarkerSize(1.2);
 ptAn1->SetMarkerStyle(20);
 ptAn1->GetYaxis()->SetRangeUser(-.03,.07);
 ptAn1->GetXaxis()->SetRangeUser(20,130);

 ptAn1->DrawClone("ap");
 }
 if(case1%2==0){
 ptAn1->SetMarkerSize(1.2);
 ptAn1->SetMarkerStyle(20);
 ptAn1->DrawClone("p");

 }
    TLine *t =  new TLine(30, 0, 120, 0);
 t->SetLineStyle(1);
 t->SetLineWidth(2);
 t->SetLineColor(kBlue);
 t->Draw();

}
