TCanvas *c = new TCanvas();
TGraphErrors *ptAn1g[20];
  int nph=2;

void readC(int nph1=2){
  c->Divide(3,3,0,0);
  nph = nph1;
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
  d(13);
  d(14);
  d(15);
  d(16);
  d(17);
  d(18);
}

int d(int case1  =0){


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
  counter1++;

  ifstream infile;
  int case2 = case1-1;


  if(nph==2){
    if(case2==0)    infile.open("cent/pi0/xf1/case.txt");
    if(case2==1)    infile.open("cent/pi0/nxf1/case.txt");
    if(case2==2)    infile.open("cent/pi0/xf2/case.txt");
    if(case2==3)    infile.open("cent/pi0/nxf2/case.txt");
    if(case2==4)    infile.open("cent/pi0/xf3/case.txt");
    if(case2==5)    infile.open("cent/pi0/nxf3/case.txt");


    if(case2==6)    infile.open("centN/pi0/xf1/case.txt");
    if(case2==7)    infile.open("centN/pi0/nxf1/case.txt");
    if(case2==8)    infile.open("centN/pi0/xf2/case.txt");
    if(case2==9)    infile.open("centN/pi0/nxf2/case.txt");
    if(case2==10)    infile.open("centN/pi0/xf3/case.txt");
    if(case2==11)    infile.open("centN/pi0/nxf3/case.txt");


    if(case2==12)    infile.open("centA/pi0/xf1/case.txt");
    if(case2==13)    infile.open("centA/pi0/nxf1/case.txt");
    if(case2==14)    infile.open("centA/pi0/xf2/case.txt");
    if(case2==15)    infile.open("centA/pi0/nxf2/case.txt");
    if(case2==16)    infile.open("centA/pi0/xf3/case.txt");
    if(case2==17)    infile.open("centA/pi0/nxf3/case.txt");
    }

    if(nph==3){
    if(case2==0)    infile.open("cent/3ph/xf1/case.txt");
    if(case2==1)    infile.open("cent/3ph/nxf1/case.txt");
    if(case2==2)    infile.open("cent/3ph/xf2/case.txt");
    if(case2==3)    infile.open("cent/3ph/nxf2/case.txt");
    if(case2==4)    infile.open("cent/3ph/xf3/case.txt");
    if(case2==5)    infile.open("cent/3ph/nxf3/case.txt");


    if(case2==6)    infile.open("centN/3ph/xf1/case.txt");
    if(case2==7)    infile.open("centN/3ph/nxf1/case.txt");
    if(case2==8)    infile.open("centN/3ph/xf2/case.txt");
    if(case2==9)    infile.open("centN/3ph/nxf2/case.txt");
    if(case2==10)    infile.open("centN/3ph/xf3/case.txt");
    if(case2==11)    infile.open("centN/3ph/nxf3/case.txt");


    if(case2==12)    infile.open("centA/3ph/xf1/case.txt");
    if(case2==13)    infile.open("centA/3ph/nxf1/case.txt");
    if(case2==14)    infile.open("centA/3ph/xf2/case.txt");
    if(case2==15)    infile.open("centA/3ph/nxf2/case.txt");
    if(case2==16)    infile.open("centA/3ph/xf3/case.txt");
    if(case2==17)    infile.open("centA/3ph/nxf3/case.txt");
    }

    if(nph==4){
    if(case2==0)    infile.open("cent/gt3ph/xf1/case.txt");
    if(case2==1)    infile.open("cent/gt3ph/nxf1/case.txt");
    if(case2==2)    infile.open("cent/gt3ph/xf2/case.txt");
    if(case2==3)    infile.open("cent/gt3ph/nxf2/case.txt");
    if(case2==4)    infile.open("cent/gt3ph/xf3/case.txt");
    if(case2==5)    infile.open("cent/gt3ph/nxf3/case.txt");


    if(case2==6)    infile.open("centN/gt3ph/xf1/case.txt");
    if(case2==7)    infile.open("centN/gt3ph/nxf1/case.txt");
    if(case2==8)    infile.open("centN/gt3ph/xf2/case.txt");
    if(case2==9)    infile.open("centN/gt3ph/nxf2/case.txt");
    if(case2==10)    infile.open("centN/gt3ph/xf3/case.txt");
    if(case2==11)    infile.open("centN/gt3ph/nxf3/case.txt");


    if(case2==12)    infile.open("centA/gt3ph/xf1/case.txt");
    if(case2==13)    infile.open("centA/gt3ph/nxf1/case.txt");
    if(case2==14)    infile.open("centA/gt3ph/xf2/case.txt");
    if(case2==15)    infile.open("centA/gt3ph/nxf2/case.txt");
    if(case2==16)    infile.open("centA/gt3ph/xf3/case.txt");
    if(case2==17)    infile.open("centA/gt3ph/nxf3/case.txt");
    }

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
  }

  infile.close();


 TGraphErrors *ptAn1 = new TGraphErrors(counter1-1,pt1,an1,pte1,ane1);
  TGraph *ptchi21 = new TGraph(counter1-1,pt1,chi21);
  TGraph *ptndf1 = new TGraph(counter1-1,pt1,ndf1);

  
  // TCanvas *c = new TCanvas("c","",10,10,800,600);

  TLine *t =  new TLine(1.5, 0, 7, 0);

 c->cd(int(case1+1)/2);
 if(case1%2==1){
   ptAn1->SetMarkerColor(2);
  ptAn1->SetLineColor(2);
  ptAn1->SetMarkerSize(1.2);
 ptAn1->SetMarkerStyle(20);
 ptAn1->GetYaxis()->SetRangeUser(-.03,.08);
 ptAn1->GetXaxis()->SetRangeUser(1.5,8);

 ptAn1->DrawClone("ap");
 }
 if(case1%2==0){
 ptAn1->SetMarkerSize(1.2);
 ptAn1->SetMarkerStyle(20);
 ptAn1->DrawClone("p");

 }
 t->SetLineStyle(1);
 t->SetLineWidth(2);
 t->SetLineColor(kBlue);
 t->Draw();
  
}
