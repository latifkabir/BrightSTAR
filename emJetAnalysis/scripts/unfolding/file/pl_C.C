int pl_C(int nph=2){

 char  file[100];

  for(int j=0; j<18; j++){ 
    if(nph==2){
    if(j==0)    sprintf(file,"cent/pi0/xf1/case.txt");
    if(j==1)    sprintf(file,"cent/pi0/nxf1/case.txt");
    if(j==2)    sprintf(file,"cent/pi0/xf2/case.txt");
    if(j==3)    sprintf(file,"cent/pi0/nxf2/case.txt");
    if(j==4)    sprintf(file,"cent/pi0/xf3/case.txt");
    if(j==5)    sprintf(file,"cent/pi0/nxf3/case.txt");


    if(j==6)    sprintf(file,"centN/pi0/xf1/case.txt");
    if(j==7)    sprintf(file,"centN/pi0/nxf1/case.txt");
    if(j==8)    sprintf(file,"centN/pi0/xf2/case.txt");
    if(j==9)    sprintf(file,"centN/pi0/nxf2/case.txt");
    if(j==10)    sprintf(file,"centN/pi0/xf3/case.txt");
    if(j==11)    sprintf(file,"centN/pi0/nxf3/case.txt");


    if(j==12)    sprintf(file,"centA/pi0/xf1/case.txt");
    if(j==13)    sprintf(file,"centA/pi0/nxf1/case.txt");
    if(j==14)    sprintf(file,"centA/pi0/xf2/case.txt");
    if(j==15)    sprintf(file,"centA/pi0/nxf2/case.txt");
    if(j==16)    sprintf(file,"centA/pi0/xf3/case.txt");
    if(j==17)    sprintf(file,"centA/pi0/nxf3/case.txt");
    }

    if(nph==3){
    if(j==0)    sprintf(file,"cent/3ph/xf1/case.txt");
    if(j==1)    sprintf(file,"cent/3ph/nxf1/case.txt");
    if(j==2)    sprintf(file,"cent/3ph/xf2/case.txt");
    if(j==3)    sprintf(file,"cent/3ph/nxf2/case.txt");
    if(j==4)    sprintf(file,"cent/3ph/xf3/case.txt");
    if(j==5)    sprintf(file,"cent/3ph/nxf3/case.txt");


    if(j==6)    sprintf(file,"centN/3ph/xf1/case.txt");
    if(j==7)    sprintf(file,"centN/3ph/nxf1/case.txt");
    if(j==8)    sprintf(file,"centN/3ph/xf2/case.txt");
    if(j==9)    sprintf(file,"centN/3ph/nxf2/case.txt");
    if(j==10)    sprintf(file,"centN/3ph/xf3/case.txt");
    if(j==11)    sprintf(file,"centN/3ph/nxf3/case.txt");


    if(j==12)    sprintf(file,"centA/3ph/xf1/case.txt");
    if(j==13)    sprintf(file,"centA/3ph/nxf1/case.txt");
    if(j==14)    sprintf(file,"centA/3ph/xf2/case.txt");
    if(j==15)    sprintf(file,"centA/3ph/nxf2/case.txt");
    if(j==16)    sprintf(file,"centA/3ph/xf3/case.txt");
    if(j==17)    sprintf(file,"centA/3ph/nxf3/case.txt");
    }

    if(nph==4){
    if(j==0)    sprintf(file,"cent/gt3ph/xf1/case.txt");
    if(j==1)    sprintf(file,"cent/gt3ph/nxf1/case.txt");
    if(j==2)    sprintf(file,"cent/gt3ph/xf2/case.txt");
    if(j==3)    sprintf(file,"cent/gt3ph/nxf2/case.txt");
    if(j==4)    sprintf(file,"cent/gt3ph/xf3/case.txt");
    if(j==5)    sprintf(file,"cent/gt3ph/nxf3/case.txt");


    if(j==6)    sprintf(file,"centN/gt3ph/xf1/case.txt");
    if(j==7)    sprintf(file,"centN/gt3ph/nxf1/case.txt");
    if(j==8)    sprintf(file,"centN/gt3ph/xf2/case.txt");
    if(j==9)    sprintf(file,"centN/gt3ph/nxf2/case.txt");
    if(j==10)    sprintf(file,"centN/gt3ph/xf3/case.txt");
    if(j==11)    sprintf(file,"centN/gt3ph/nxf3/case.txt");


    if(j==12)    sprintf(file,"centA/gt3ph/xf1/case.txt");
    if(j==13)    sprintf(file,"centA/gt3ph/nxf1/case.txt");
    if(j==14)    sprintf(file,"centA/gt3ph/xf2/case.txt");
    if(j==15)    sprintf(file,"centA/gt3ph/nxf2/case.txt");
    if(j==16)    sprintf(file,"centA/gt3ph/xf3/case.txt");
    if(j==17)    sprintf(file,"centA/gt3ph/nxf3/case.txt");
    }

    
    FILE *f = fopen(file,"w");
    
    cout<<" -------------------------------------------------  "<<endl;
    for(int i=0; i<5; i++){ 
      int ii = j*10+i;
      
      d(f,ii,nph);
      
      // d(ii,1);    
    }}
  
}

void d(FILE*f, int case1 = 0, int nphotons=1, int cen==0, float e1 = 0, float e2 = 0, float eta1=2.8, float eta2 = 4.0, float pt1=0, float pt2=30, float deta1 = -10.0,  float deta2 = 10.0,  float dphi1 = -10.0,  float dphi2 = 10.0, int xf =1){

  e1 = 0; e2=0;  
  if(case1==0){e1 = 40;  e2=60; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;}
  if(case1==1){e1 = 40;  e2=60; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;}
  if(case1==2){e1 = 40;  e2=60; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;}
  if(case1==3){e1 = 40;  e2=60; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;}
  if(case1==4){e1 = 40;  e2=60; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;}

  if(case1==10){e1 = 40;  e2=60; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0; xf=0;}
  if(case1==11){e1 = 40;  e2=60; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;xf=0;}
  if(case1==12){e1 = 40;  e2=60; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;xf=0;}
  if(case1==13){e1 = 40;  e2=60; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;xf=0;}
  if(case1==14){e1 = 40;  e2=60; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;xf=0;}

  if(case1==20){e1 = 60;  e2=80; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;    cen=0;}
  if(case1==21){e1 = 60;  e2=80; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;    cen=0;}
  if(case1==22){e1 = 60;  e2=80; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;    cen=0;}
  if(case1==23){e1 = 60;  e2=80; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;    cen=0;}
  if(case1==24){e1 = 60;  e2=80; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;    cen=0;}

  if(case1==30){e1 = 60;  e2=80; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;    cen=0; xf=0;}
  if(case1==31){e1 = 60;  e2=80; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;    cen=0;xf=0;}
  if(case1==32){e1 = 60;  e2=80; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;    cen=0;xf=0;}
  if(case1==33){e1 = 60;  e2=80; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;    cen=0;xf=0;}
  if(case1==34){e1 = 60;  e2=80; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;    cen=0;xf=0;}


  if(case1==40){e1 = 80; e2=100; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;}
  if(case1==41){e1 = 80; e2=100; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;}
  if(case1==42){e1 = 80; e2=100; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;}
  if(case1==43){e1 = 80; e2=100; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;}
  if(case1==44){e1 = 80; e2=100; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;}

  if(case1==50){e1 = 80; e2=100; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0; xf=0;}
  if(case1==51){e1 = 80; e2=100; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;xf=0;}
  if(case1==52){e1 = 80; e2=100; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;xf=0;}
  if(case1==53){e1 = 80; e2=100; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;xf=0;}
  if(case1==54){e1 = 80; e2=100; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-10; dphi2=10;   cen=0;xf=0;}


  //////////////

if(case1==60){e1 = 40;  e2=60; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;}
  if(case1==61){e1 = 40;  e2=60; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;}
  if(case1==62){e1 = 40;  e2=60; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;}
  if(case1==63){e1 = 40;  e2=60; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;}
  if(case1==64){e1 = 40;  e2=60; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;}

  if(case1==70){e1 = 40;  e2=60; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1; xf=0;}
  if(case1==71){e1 = 40;  e2=60; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;xf=0;}
  if(case1==72){e1 = 40;  e2=60; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;xf=0;}
  if(case1==73){e1 = 40;  e2=60; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;xf=0;}
  if(case1==74){e1 = 40;  e2=60; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;xf=0;}


  if(case1==80){e1 = 60;  e2=80; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;    cen=1;}
  if(case1==81){e1 = 60;  e2=80; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;    cen=1;}
  if(case1==82){e1 = 60;  e2=80; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;    cen=1;}
  if(case1==83){e1 = 60;  e2=80; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;    cen=1;}
  if(case1==84){e1 = 60;  e2=80; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;    cen=1;}

  if(case1==90){e1 = 60;  e2=80; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;    cen=1; xf=0;}
  if(case1==91){e1 = 60;  e2=80; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;    cen=1;xf=0;}
  if(case1==92){e1 = 60;  e2=80; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;    cen=1;xf=0;}
  if(case1==93){e1 = 60;  e2=80; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;    cen=1;xf=0;}
  if(case1==94){e1 = 60;  e2=80; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;    cen=1;xf=0;}



  if(case1==100){e1 = 80; e2=100; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;}
  if(case1==101){e1 = 80; e2=100; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;}
  if(case1==102){e1 = 80; e2=100; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;}
  if(case1==103){e1 = 80; e2=100; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;}
  if(case1==104){e1 = 80; e2=100; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;}

  if(case1==110){e1 = 80; e2=100; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1; xf=0;}
  if(case1==111){e1 = 80; e2=100; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;xf=0;}
  if(case1==112){e1 = 80; e2=100; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;xf=0;}
  if(case1==113){e1 = 80; e2=100; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;xf=0;}
  if(case1==114){e1 = 80; e2=100; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=-1.57; dphi2=1.57;   cen=1;xf=0;}


  //////////////



if(case1==120){e1 = 40;  e2=60; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;}
  if(case1==121){e1 = 40;  e2=60; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;}
  if(case1==122){e1 = 40;  e2=60; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;}
  if(case1==123){e1 = 40;  e2=60; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;}
  if(case1==124){e1 = 40;  e2=60; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;}

  if(case1==130){e1 = 40;  e2=60; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1; xf=0;}
  if(case1==131){e1 = 40;  e2=60; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;xf=0;}
  if(case1==132){e1 = 40;  e2=60; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;xf=0;}
  if(case1==133){e1 = 40;  e2=60; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;xf=0;}
  if(case1==134){e1 = 40;  e2=60; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;xf=0;}



  if(case1==140){e1 = 60;  e2=80; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;    cen=1;}
  if(case1==141){e1 = 60;  e2=80; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;    cen=1;}
  if(case1==142){e1 = 60;  e2=80; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;    cen=1;}
  if(case1==143){e1 = 60;  e2=80; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;    cen=1;}
  if(case1==144){e1 = 60;  e2=80; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;    cen=1;}

  if(case1==150){e1 = 60;  e2=80; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;    cen=1; xf=0;}
  if(case1==151){e1 = 60;  e2=80; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;    cen=1;xf=0;}
  if(case1==152){e1 = 60;  e2=80; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;    cen=1;xf=0;}
  if(case1==153){e1 = 60;  e2=80; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;    cen=1;xf=0;}
  if(case1==154){e1 = 60;  e2=80; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;    cen=1;xf=0;}



  if(case1==160){e1 = 80; e2=100; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;}
  if(case1==161){e1 = 80; e2=100; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;}
  if(case1==162){e1 = 80; e2=100; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;}
  if(case1==163){e1 = 80; e2=100; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;}
  if(case1==164){e1 = 80; e2=100; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;}

  if(case1==170){e1 = 80; e2=100; pt1=2.0; pt2=3.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1; xf=0;}
  if(case1==171){e1 = 80; e2=100; pt1=3.0; pt2=3.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;xf=0;}
  if(case1==172){e1 = 80; e2=100; pt1=3.5; pt2=4.0; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;xf=0;}
  if(case1==173){e1 = 80; e2=100; pt1=4.0; pt2=5.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;xf=0;}
  if(case1==174){e1 = 80; e2=100; pt1=5.5; pt2=12.5; eta1=2.8; eta2=4.0; dphi1=3.14-1.57; dphi2=3.14+1.57;   cen=1;xf=0;}
 
  
  if(e1==0&&e2==0)break;
  
  //cout<<e1<<"  "<<e2<<endl;

  char file[300];
   
  printf(": e_%d_%d__eta_%f_%f__pt_%f_%f__deta_%f_%f__dphi_%f_%f__nPhotons_%d__cen_%d__xf_%d.txt \n",e1,e2,eta1,eta2,pt1,pt2,deta1,deta2,dphi1,dphi2,nphotons,cen,xf);

  sprintf(file,"e_%d_%d__eta_%f_%f__pt_%f_%f__deta_%f_%f__dphi_%f_%f__nPhotons_%d__cen_%d__xf_%d.txt",e1,e2,eta1,eta2,pt1,pt2,deta1,deta2,dphi1,dphi2,nphotons,cen,xf);
  cout<<file<<endl;

  float a1, a2, a3, a4, b1, b2, b3, b4, b5, b6,  b7, b8, a5;

  int a6;
  //  FILE *f = fopen(file,"r");
  ifstream infile;

  infile.open(file);
  if(infile){
    cout<<" HERE ------------------- "<<endl;
    //while(infile){
    //printf(fS,"%5.3f  %5.4f   %5.2f %5.2f  %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %4d",AN, ANe, PT, PTe,  EN, ENe,  ETA, ETAe,  DPHI, DPHIe,  DETA, DETAe, chiSquare,ndf-2);

    infile>>a1>>a2>>a3>>a4>>b1>>b2>>b3>>b4>>b5>>b6>>b7>>b8>>a5>>a6;
    //    fprintf(f,"%f %f %f %f %f  %f %f  %f %f  %f %f  %f %f  %d \n ",a3,a4,-a1,a2,b1,b2,b3,b4,b5,b6,b7,b8,a5,a6);
    //    fprintf(f,"%f %f %f %f %f  %f %f  %f %f  %f %f  %f %f  %d \n ",a3,a4,-a1,a2,b1,b2,b3,b4,b5,b6,b7,b8,a5,a6);
    fprintf(f,"%f %f %f %f %f %d \n ",a3,a4,-a1,a2,a5,a6);

    //scanf(f,"%f %f %f %f %f %d "&a1,&a2,&a3,&a4,&a5,&a6);
    cout<<a1<<"  "<<a2<<"  "<<a3<<" "<<a4<<"  "<<b1<<" "<<b2<<"  "<<b3<<" "<<b4<<"  "<<b5<<" "<<b6<<"  "<<b7<<" "<<b8<<"  "<<a5<<"  "<<a6<<endl;
  }

  //  infile.close(file);
  //fclose(f);



}




