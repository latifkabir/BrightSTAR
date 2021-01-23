void ana(){
  

TCanvas *c = new TCanvas();
   c->SetLeftMargin(.3);   
  c->SetRightMargin(.3);
  c->SetTopMargin(.3);
  c->SetBottomMargin(.3);
 c->Divide(3,5,0,0);


c->SetTickx(1);
   c->SetTicky(1);
   c->SetFrameBorderMode(0);
   c->SetFrameBorderMode(0);   

 gPad->SetTickx(1);
 gPad->SetTicky(1);

tex = new TLatex(0.1084813,0.9640479,"EM-Jet Energy = 40-60 GeV                              60-80 GeV                                       80-100 GeV");
   tex->SetTextFont(42);
   tex->SetTextSize(0.02506266);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(0.9230769,0.9560586,"no. photons =  1                  2                       3                     4                   5");
   tex->SetTextFont(42);
   tex->SetTextSize(0.02663116);
   tex->SetTextAngle(270);
   tex->SetLineWidth(2);
   tex->Draw();

 TH1F *h = new TH1F("h","",100000,-3,15);
 h->SetLineColor(kBlack);
 h->SetLineStyle(2);
 h->SetLineWidth(1);

TFile *file1 = TFile::Open("xfLT0.root");
 ph1(c,h,ge1,ge2,ge3,1,2,3,1,1);
 ph1(c,h,ge4,ge5,ge6,4,5,6,1,2);
 ph1(c,h,ge7,ge8,ge9,7,8,9,1,3);
 ph1(c,h,ge10,ge11,ge12,10,11,12,1,4);
 ph1(c,h,ge13,ge14,ge15,13,14,15,1,5);


TFile *file0 = TFile::Open("xfGT0.root");
 ph1(c,h,gge1,gge2,gge3,1,2,3,2,1);
 ph1(c,h,gge4,gge5,gge6,4,5,6,2,2);
 ph1(c,h,gge7,gge8,gge9,7,8,9,2,3);
 ph1(c,h,gge10,gge11,gge12,10,11,12,2,4);
 ph1(c,h,gge13,gge14,gge15,13,14,15,2,5);


 //canvas, zero-line, sys-error1,2,3, pad1,2,3, whetherxF>0,<0, nphotonFile
}



void ph1(TCanvas *c, TH1F *h , TGraphErrors *g1,TGraphErrors *g2,TGraphErrors *g3, int c1, int c2, int c3, int nxfg, int file){

 gPad->SetTickx(1);
 gPad->SetTicky(1);
  float pt, pte, an, ane, chi2;
  int ndf;

  int counter1 = 0;
  int counter2 = 0;
  int counter3 = 0;

  int counter4 = 0;
  int counter5 = 0;
  int counter6 = 0;

  float pt1[20], pte1[20], an1[20], ane1[20], chi21[20];
  float ndf1[20];
  float pt2[20], pte2[20], an2[20], ane2[20], chi22[20];
  float ndf2[20];
  float pt3[20], pte3[20], an3[20], ane3[20], chi23[20];
  float ndf3[20];

 float pt4[20], pte4[20], an4[20], ane4[20], chi24[20];
  float ndf4[20];
  float pt5[20], pte5[20], an5[20], ane5[20], chi25[20];
  float ndf5[20];
  float pt6[20], pte6[20], an6[20], ane6[20], chi26[20];
  float ndf6[20];

  char nfile[100];

ifstream infile;  
  sprintf(nfile,"j%dPh/xf1/case.txt",file);
  infile.open(nfile);
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
  cout<<" -------- "<<endl;
  ifstream infile;
  sprintf(nfile,"j%dPh/xf2/case.txt",file);
  infile.open(nfile);
  // infile.open("j1Ph/xf2/case.txt");
  while(infile){
    infile>>pt>>pte>>an>>ane>>chi2>>ndf;
    cout<<"counter2 :  "<<counter2<<" "<<pt<<"  "<<pte<<"  "<<an<<"  "<<ane<<"  "<<chi2<<"  "<<ndf<<endl;
    pt2[counter2] = pt;
    pte2[counter2] = pte;
    an2[counter2] = an;
    ane2[counter2] = ane;
    chi22[counter2] = chi2;
    ndf2[counter2] = ndf;
    counter2++;
  }
  infile.close();
    cout<<" -------- "<<endl;
  ifstream infile;
  sprintf(nfile,"j%dPh/xf3/case.txt",file);
  infile.open(nfile);
  // infile.open("j1Ph/xf3/case.txt");
  while(infile){
    infile>>pt>>pte>>an>>ane>>chi2>>ndf;
    cout<<"counter3 : "<<counter3<<" "<<pt<<"  "<<pte<<"  "<<an<<"  "<<ane<<"  "<<chi2<<"  "<<ndf<<endl;
    pt3[counter3] = pt;
    pte3[counter3] = pte;
    an3[counter3] = an;
    ane3[counter3] = ane;
    chi23[counter3] = chi2;
    ndf3[counter3] = ndf;
    counter3++;
  }
  infile.close();
  cout<<" -------- "<<endl;
 ifstream infile;
  sprintf(nfile,"j%dPh/nxf1/case.txt",file);
  infile.open(nfile);
  // infile.open("j1Ph/nxf1/case.txt");
  while(infile){
    infile>>pt>>pte>>an>>ane>>chi2>>ndf;
  cout<<"counter4 : "<<counter4<<" "<<pt<<"  "<<pte<<"  "<<an<<"  "<<ane<<"  "<<chi2<<"  "<<ndf<<endl;
    
    pt4[counter4] = pt;
    pte4[counter4] = pte;
    an4[counter4] = an;
    ane4[counter4] = ane;
    chi24[counter4] = chi2;
    ndf4[counter4] = ndf;
    counter4++;
  }
  infile.close();
  cout<<" -------- "<<endl;

 ifstream infile;
 //infile.open("j1Ph/nxf2/case.txt");
  sprintf(nfile,"j%dPh/nxf2/case.txt",file);
  infile.open(nfile);
  while(infile){
    infile>>pt>>pte>>an>>ane>>chi2>>ndf;
  
  cout<<"counter5 : "<<counter5<<" "<<pt<<"  "<<pte<<"  "<<an<<"  "<<ane<<"  "<<chi2<<"  "<<ndf<<endl;
    pt5[counter5] = pt;
    pte5[counter5] = pte;
    an5[counter5] = an;
    ane5[counter5] = ane;
    chi25[counter5] = chi2;
    ndf5[counter5] = ndf;
    counter5++;
  }
  infile.close();
  cout<<" -------- "<<endl;

 ifstream infile;
 // infile.open("j1Ph/nxf3/case.txt");
  sprintf(nfile,"j%dPh/nxf3/case.txt",file);
  infile.open(nfile);
  while(infile){
    infile>>pt>>pte>>an>>ane>>chi2>>ndf;
  cout<<"counter6 : "<<counter6<<" "<<pt<<"  "<<pte<<"  "<<an<<"  "<<ane<<"  "<<chi2<<"  "<<ndf<<endl;
    pt6[counter6] = pt;
    pte6[counter6] = pte;
    an6[counter6] = an;
    ane6[counter6] = ane;
    chi26[counter6] = chi2;
    ndf6[counter6] = ndf;
    counter6++;
  }
  infile.close();
  cout<<" -------- "<<endl;


  TGraphErrors *ptAn1 = new TGraphErrors(counter1-1,pt1,an1,pte1,ane1);
  TGraph *ptchi21 = new TGraph(counter1-1,pt1,chi21);
  TGraph *ptndf1 = new TGraph(counter1-1,pt1,ndf1);

  TGraphErrors *ptAn2 = new TGraphErrors(counter2-1,pt2,an2,pte2,ane2);
  TGraph *ptchi22 = new TGraph(counter2-1,pt2,chi22);
  TGraph *ptndf2 = new TGraph(counter2-1,pt2,ndf2);

  TGraphErrors *ptAn3 = new TGraphErrors(counter3-1,pt3,an3,pte3,ane3);
  TGraph *ptchi23 = new TGraph(counter3-1,pt3,chi23);
  TGraph *ptndf3 = new TGraph(counter3-1,pt3,ndf3);


  TGraphErrors *ptAn4  = new TGraphErrors(counter4-1,pt4 ,an4,pte4 ,ane4 );
  TGraph *ptchi24  = new TGraph(counter4 -1,pt4 ,chi24 );
  TGraph *ptndf4  = new TGraph(counter4 -1,pt4 ,ndf4 );

  TGraphErrors *ptAn5 = new TGraphErrors(counter5-1,pt5,an5,pte5,ane5);
  TGraph *ptchi25 = new TGraph(counter5-1,pt5,chi25);
  TGraph *ptndf5 = new TGraph(counter5-1,pt5,ndf5);

  TGraphErrors *ptAn6 = new TGraphErrors(counter6-1,pt6,an6,pte6,ane6);
  TGraph *ptchi26 = new TGraph(counter6-1,pt6,chi26);
  TGraph *ptndf6 = new TGraph(counter6-1,pt6,ndf6);



   ptAn1->SetTitle(" ");
  ptAn2->SetTitle("");
  ptAn3->SetTitle(" ");



 leg = new TLegend(0.4,0.6,0.89,0.89);
    leg->AddEntry(ptAn1,"x_{F} > 0","p");
    leg->AddEntry(ptAn4,"x_{F} < 0","p");


ptchi21->SetTitle("blue - NDF, black - #chi^{2}");
  ptchi22->SetTitle("");
  ptchi23->SetTitle("");


  if(c1==1)  ptAn1->GetYaxis()->SetTitle("A_{N}");
  //  ptAn4->GetYaxis()->SetTitle("A_{N}");
  if(c3==15)ptAn3->GetXaxis()->SetTitle("p^{EMJet}_{T} GeV/c");

  ptchi21->GetYaxis()->SetTitle("NDF, #chi^{2}");
ptchi21->GetXaxis()->SetTitle("p_{T}");
ptchi22->GetXaxis()->SetTitle("p_{T}");
ptchi23->GetXaxis()->SetTitle("p_{T}");

 ptchi21->GetYaxis()->SetRangeUser(0,38);
  ptAn1->GetYaxis()->SetRangeUser(-.028,.069);
  ptAn1->GetXaxis()->SetRangeUser(1.5,12.5);

 ptchi21->SetMarkerStyle(5);
 ptchi21->SetMarkerSize(.2);
 ptndf1->SetMarkerStyle(5);
 ptndf1->SetMarkerSize(.2);
 ptndf1->SetMarkerColor(kBlue);
 ptAn1->SetMarkerStyle(20);
 ptAn1->SetMarkerSize(.2);

 ptchi22->GetYaxis()->SetRangeUser(0,38);
 ptAn2->GetYaxis()->SetRangeUser(-.028,.069);
  ptAn2->GetXaxis()->SetRangeUser(1.5,12.5);
 ptchi22->SetMarkerStyle(5);
 ptchi22->SetMarkerSize(.2);
 ptndf2->SetMarkerStyle(5);
 ptndf2->SetMarkerSize(.2);
 ptndf2->SetMarkerColor(kBlue);
 ptAn2->SetMarkerStyle(20);
 ptAn2->SetMarkerSize(.2);

 ptchi23->GetYaxis()->SetRangeUser(0,38);
 ptAn3->GetYaxis()->SetRangeUser(-.028,.069);
  ptAn3->GetXaxis()->SetRangeUser(1.5,12.5);
 ptchi23->SetMarkerStyle(5);
 ptchi23->SetMarkerSize(.2);
 ptndf3->SetMarkerStyle(5);
 ptndf3->SetMarkerSize(.2);
 ptndf3->SetMarkerColor(kBlue);
 ptAn3->SetMarkerStyle(20);
 ptAn3->SetMarkerSize(.2);


 ptchi24->GetYaxis()->SetRangeUser(0,38);
 ptAn4->GetYaxis()->SetRangeUser(-.028,.069);
  ptAn4->GetXaxis()->SetRangeUser(1.5,12.5);
 ptchi24->SetMarkerStyle(5);
 ptchi24->SetMarkerSize(.2);
 ptndf4->SetMarkerStyle(5);
 ptndf4->SetMarkerSize(.2);
 ptndf4->SetMarkerColor(kBlue);
 ptAn4->SetMarkerStyle(24);
 ptAn4->SetMarkerSize(.2);


 ptchi25->GetYaxis()->SetRangeUser(0,38);
 ptAn5->GetYaxis()->SetRangeUser(-.028,.069);
  ptAn5->GetXaxis()->SetRangeUser(1.5,12.5);
 ptchi25->SetMarkerStyle(5);
 ptchi25->SetMarkerSize(.2);
 ptndf5->SetMarkerStyle(5);
 ptndf5->SetMarkerSize(.2);
 ptndf5->SetMarkerColor(kBlue);
 ptAn5->SetMarkerStyle(24);
 ptAn5->SetMarkerSize(.2);


 ptchi26->GetYaxis()->SetRangeUser(0,38);
 ptAn6->GetYaxis()->SetRangeUser(-.028,.069);
ptAn6->GetXaxis()->SetRangeUser(-.028,.069);
  ptAn6->GetXaxis()->SetRangeUser(1.5,12.5);
 ptchi26->SetMarkerStyle(5);
 ptchi26->SetMarkerSize(.2);
 ptndf6->SetMarkerStyle(5);
 ptndf6->SetMarkerSize(.2);
 ptndf6->SetMarkerColor(kBlue);
 ptAn6->SetMarkerStyle(24);
 ptAn6->SetMarkerSize(.2);

 ptAn1->SetMarkerColor(kRed+2);
 ptAn2->SetMarkerColor(kRed+2);
 ptAn3->SetMarkerColor(kRed+2);

 ptAn1->SetLineColor(kRed+2);
 ptAn2->SetLineColor(kRed+2);
 ptAn3->SetLineColor(kRed+2);


 /*
 ptAn1->GetXaxis()->SetRangeUser(1.,17.);
 ptAn2->GetXaxis()->SetRangeUser(1.,17.);
 ptAn3->GetXaxis()->SetRangeUser(1,17);
 ptAn4->GetXaxis()->SetRangeUser(1,17);
 ptAn5->GetXaxis()->SetRangeUser(1,17);
 ptAn6->GetXaxis()->SetRangeUser(1,17);

 */


 // TCanvas *c = new TCanvas("c","",10,10,800,600);
 // c->SetLeftMargin(0.2);
 //c->SetBottomMargin(0.2);
 //c->SetRightMargin(0.2);

 // c->Divide(3,2,0,0);
 if(nxfg==2){
 g1->SetFillColor(kRed-9);
 g2->SetFillColor(kRed-9);
 g3->SetFillColor(kRed-9);

 c->cd(c1);
 c->cd(c1)->SetTickx();
 c->cd(c1)->SetTicky();
 ptAn1->GetYaxis()->SetNdivisions(503 );
 ptAn1->GetYaxis()->SetLabelSize(0.18);
 ptAn1->GetYaxis()->SetTitleSize(0.14);
 ptAn1->GetXaxis()->SetNdivisions(405);
 ptAn1->GetXaxis()->SetLabelSize(0.18);
 ptAn1->GetXaxis()->SetTitleSize(0.14);

 ptAn1->Draw("ap");
 g1->Draw("2");
 ptAn1->Draw("p");
 ptAn4->Draw("same,p");
 h->Draw("same");
 
 if(c3==15)leg->Draw();

 c->cd(c2);
 c->cd(c2)->SetTickx();
 c->cd(c2)->SetTicky();
 ptAn2->GetYaxis()->SetNdivisions(503 );
 ptAn2->GetYaxis()->SetLabelSize(0.18);
 ptAn2->GetYaxis()->SetTitleSize(0.14);
 ptAn2->GetXaxis()->SetNdivisions(405);
 ptAn2->GetXaxis()->SetLabelSize(0.18);
 ptAn2->GetXaxis()->SetTitleSize(0.14);

 ptAn2->Draw("ap");
 g2->Draw("2");
  ptAn2->Draw("p");
 ptAn5->Draw("same,p");
 h->Draw("same");

 c->cd(c3);
 c->cd(c3)->SetTickx();
 c->cd(c3)->SetTicky();
 ptAn3->GetYaxis()->SetNdivisions(503 );
 ptAn3->GetYaxis()->SetLabelSize(0.18);
 ptAn3->GetYaxis()->SetTitleSize(0.14);
 ptAn3->GetXaxis()->SetNdivisions(405);
 ptAn3->GetXaxis()->SetLabelSize(0.18);
 ptAn3->GetXaxis()->SetTitleSize(0.14);

 ptAn3->Draw("ap");
 g3->Draw("2");
  ptAn3->Draw("p");
 ptAn6->Draw("same,p");
 h->Draw("same");

 }

 if(nxfg==1) {
 g1->SetFillColor(kGray);
 g2->SetFillColor(kGray);
 g3->SetFillColor(kGray);

 c->cd(c1);
  g1->Draw("2");
 ptAn4->Draw("same,p");
 h->Draw("same");

 c->cd(c2);
  g2->Draw("2");
 ptAn5->Draw("same,p");
 h->Draw("same");

 c->cd(c3);
  g3->Draw("2");
  // ptAn3->Draw("p");
 ptAn6->Draw("same,p");
 h->Draw("same");

}
 c->SaveAs("AN_pFinal.pdf");
 /*
 float x[2] = {1.5, 12.5};
 float y[2] = {-1, 1};
  TGraph *ref1= new TGraph(2,x,y);
  ref1->GetYaxis()->SetRangeUser(-.028,.069);
  TGraph *ref2= new TGraph(2,x,y);
  ref2->GetYaxis()->SetRangeUser(-.028,.069);
  TGraph *ref3= new TGraph(2,x,y);
  ref3->GetYaxis()->SetRangeUser(-.028,.069);
  TGraph *ref4= new TGraph(2,x,y);
  ref4->GetYaxis()->SetRangeUser(-.028,.069);
  TGraph *ref5= new TGraph(2,x,y);
  ref5->GetYaxis()->SetRangeUser(-.028,.069);
  TGraph *ref6= new TGraph(2,x,y);
  ref6->GetYaxis()->SetRangeUser(-.028,.069);

  ref1->GetYaxis()->SetTitle("A_{N}");
  ref4->GetYaxis()->SetTitle("A_{N}");
  ref6->GetXaxis()->SetTitle("p_{T} GeV/c");

  */




}
