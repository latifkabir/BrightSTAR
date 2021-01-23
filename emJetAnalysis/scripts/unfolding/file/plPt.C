int plPt(){
  plPt1(1);
  plPt1(2);
  plPt1(3);
  plPt1(4);
  plPt1(5);
  plPt1(6);

}

int plPt1(int nph=1){

  char  file[100];
  int cen =0 ;  
  for(int j=0; j<6; j++){ 
    cout<<" j -------------------------------------------------  "<<j<<endl;
    if(j==0)    sprintf(file,"j%dPh/xf1/case.txt",nph);
    if(j==1)    sprintf(file,"j%dPh/nxf1/case.txt",nph);
    if(j==2)    sprintf(file,"j%dPh/xf2/case.txt",nph);
    if(j==3)    sprintf(file,"j%dPh/nxf2/case.txt",nph);
    if(j==4)    sprintf(file,"j%dPh/xf3/case.txt",nph);
    if(j==5)    sprintf(file,"j%dPh/nxf3/case.txt",nph);

    FILE *f = fopen(file,"w");
  for(int i=0; i<9; i++){ 
    int ii = j*10+i;
    // d(ii);    
    d(f,ii,cen,nph);     
 }}

  cen=1;

  for(int j=0; j<6; j++){ 
    cout<<" j -------------------------------------------------  "<<j<<endl;
    if(j==0)    sprintf(file,"j%dPhC/xf1/case.txt",nph);
    if(j==1)    sprintf(file,"j%dPhC/nxf1/case.txt",nph);
    if(j==2)    sprintf(file,"j%dPhC/xf2/case.txt",nph);
    if(j==3)    sprintf(file,"j%dPhC/nxf2/case.txt",nph);
    if(j==4)    sprintf(file,"j%dPhC/xf3/case.txt",nph);
    if(j==5)    sprintf(file,"j%dPhC/nxf3/case.txt",nph);

    FILE *f = fopen(file,"w");
  for(int i=0; i<9; i++){ 
    int ii = j*10+i;
    // d(ii);    
    d(f,ii,cen,nph);     
 }}

}

void  d(FILE *f, int case = 0,  int cen=0,  int nphotons=1, float e1 = 100, float e2 = 150, float eta1=2.8, float eta2 = 4.0, float pt1=0, float pt2=30,int xf =1){


  if(case==0){e1 = 40;  e2=60; pt1=2.0; pt2=2.5; }
  if(case==1){e1 = 40;  e2=60; pt1=2.5; pt2=3.0; }
  if(case==2){e1 = 40;  e2=60; pt1=3.0; pt2=3.2; }
  if(case==3){e1 = 40;  e2=60; pt1=3.2; pt2=3.5; }
  if(case==4){e1 = 40;  e2=60; pt1=3.5; pt2=4.0; }
  if(case==5){e1 = 40;  e2=60; pt1=4.0; pt2=4.5; }
  if(case==6){e1 = 40;  e2=60; pt1=4.5; pt2=5.5; }
  if(case==7){e1 = 40;  e2=60; pt1=5.5; pt2=7.5; }
  if(case==8){e1 = 40;  e2=60; pt1=7.5; pt2=25.5; }

  if(case==10){e1 = 40;  e2=60; pt1=2.0; pt2=2.5; xf=0; }
  if(case==11){e1 = 40;  e2=60; pt1=2.5; pt2=3.0; xf=0; }
  if(case==12){e1 = 40;  e2=60; pt1=3.0; pt2=3.2; xf=0; }
  if(case==13){e1 = 40;  e2=60; pt1=3.2; pt2=3.5; xf=0; }
  if(case==14){e1 = 40;  e2=60; pt1=3.5; pt2=4.0; xf=0; }
  if(case==15){e1 = 40;  e2=60; pt1=4.0; pt2=4.5; xf=0; }
  if(case==16){e1 = 40;  e2=60; pt1=4.5; pt2=5.5; xf=0; }
  if(case==17){e1 = 40;  e2=60; pt1=5.5; pt2=7.5; xf=0; }
  if(case==18){e1 = 40;  e2=60; pt1=7.5; pt2=25.5; xf=0;}

  if(case==20){e1 = 60;  e2=80; pt1=2.0; pt2=2.5; }
  if(case==21){e1 = 60;  e2=80; pt1=2.5; pt2=3.0; }
  if(case==22){e1 = 60;  e2=80; pt1=3.0; pt2=3.2; }
  if(case==23){e1 = 60;  e2=80; pt1=3.2; pt2=3.5; }
  if(case==24){e1 = 60;  e2=80; pt1=3.5; pt2=4.0; }
  if(case==25){e1 = 60;  e2=80; pt1=4.0; pt2=4.5; }
  if(case==26){e1 = 60;  e2=80; pt1=4.5; pt2=5.5; }
  if(case==27){e1 = 60;  e2=80; pt1=5.5; pt2=7.5; }
  if(case==28){e1 = 60;  e2=80; pt1=7.5; pt2=25.5; }


  if(case==30){e1 = 60;  e2=80; pt1=2.0; pt2=2.5; xf=0; }
  if(case==31){e1 = 60;  e2=80; pt1=2.5; pt2=3.0; xf=0; }
  if(case==32){e1 = 60;  e2=80; pt1=3.0; pt2=3.2; xf=0; }
  if(case==33){e1 = 60;  e2=80; pt1=3.2; pt2=3.5; xf=0; }
  if(case==34){e1 = 60;  e2=80; pt1=3.5; pt2=4.0; xf=0; }
  if(case==35){e1 = 60;  e2=80; pt1=4.0; pt2=4.5; xf=0; }
  if(case==36){e1 = 60;  e2=80; pt1=4.5; pt2=5.5; xf=0; }
  if(case==37){e1 = 60;  e2=80; pt1=5.5; pt2=7.5; xf=0; }
  if(case==38){e1 = 60;  e2=80; pt1=7.5; pt2=25.5; xf=0;}

  if(case==40){e1 = 80;  e2=100; pt1=2.0; pt2=2.5; }
  if(case==41){e1 = 80;  e2=100; pt1=2.5; pt2=3.0; }
  if(case==42){e1 = 80;  e2=100; pt1=3.0; pt2=3.2; }
  if(case==43){e1 = 80;  e2=100; pt1=3.2; pt2=3.5; }
  if(case==44){e1 = 80;  e2=100; pt1=3.5; pt2=4.0; }
  if(case==45){e1 = 80;  e2=100; pt1=4.0; pt2=4.5; }
  if(case==46){e1 = 80;  e2=100; pt1=4.5; pt2=5.5; }
  if(case==47){e1 = 80;  e2=100; pt1=5.5; pt2=7.5; }
  if(case==48){e1 = 80;  e2=100; pt1=7.5; pt2=25.5; }

  if(case==50){e1 = 80;  e2=100; pt1=2.0; pt2=2.5; xf=0; }
  if(case==51){e1 = 80;  e2=100; pt1=2.5; pt2=3.0; xf=0; }
  if(case==52){e1 = 80;  e2=100; pt1=3.0; pt2=3.2; xf=0; }
  if(case==53){e1 = 80;  e2=100; pt1=3.2; pt2=3.5; xf=0; }
  if(case==54){e1 = 80;  e2=100; pt1=3.5; pt2=4.0; xf=0; }
  if(case==55){e1 = 80;  e2=100; pt1=4.0; pt2=4.5; xf=0; }
  if(case==56){e1 = 80;  e2=100; pt1=4.5; pt2=5.5; xf=0; }
  if(case==57){e1 = 80;  e2=100; pt1=5.5; pt2=7.5; xf=0; }
  if(case==58){e1 = 80;  e2=100; pt1=7.5; pt2=25.5; xf=0;}


  char file[200];
  sprintf(file,"e_%d_%d__eta_%f_%f__pt_%f_%f__nPhotons_%d__cen_%d__xf_%d.txt",e1,e2,eta1,eta2,pt1,pt2,nphotons,cen,xf);
  cout<<file<<endl;
  float a1, a2, a3, a4, a5;
  int a6;
  //  FILE *f = fopen(file,"r");
  ifstream infile;
  infile.open(file);
  if(infile){
  //while(infile){
    infile>>a1>>a2>>a3>>a4>>a5>>a6;
   
    fprintf(f,"%f %f %f %f %f %d \n ",a3,a4,-a1,a2,a5,a6);

  cout<<a1<<"  "<<a2<<"  "<<a3<<" "<<a4<<"  "<<a5<<"  "<<a6<<endl;
  }

  //  infile.close(file);
  //  fclose(f);



}




