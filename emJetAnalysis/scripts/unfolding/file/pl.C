int pl(int cen=0){
 char  file[100];

  for(int j=0; j<6; j++){ 
 
    if(j==0&&cen==0)    sprintf(file,"jxF-1ph/xf1/case.txt");
    if(j==1&&cen==0)    sprintf(file,"jxF-1ph/nxf1/case.txt");
    if(j==2&&cen==0)    sprintf(file,"jxF-pi0/xf1/case.txt");
    if(j==3&&cen==0)    sprintf(file,"jxF-pi0/nxf1/case.txt");
    if(j==4&&cen==0)    sprintf(file,"jxF-3ph/xf1/case.txt");
    if(j==5&&cen==0)    sprintf(file,"jxF-3ph/nxf1/case.txt");

    if(j==0&&cen==1)    sprintf(file,"jxF-1ph-C/xf1/case.txt");
    if(j==1&&cen==1)    sprintf(file,"jxF-1ph-C/nxf1/case.txt");
    if(j==2&&cen==1)    sprintf(file,"jxF-pi0-C/xf1/case.txt");
    if(j==3&&cen==1)    sprintf(file,"jxF-pi0-C/nxf1/case.txt");
    if(j==4&&cen==1)    sprintf(file,"jxF-3ph-C/xf1/case.txt");
    if(j==5&&cen==1)    sprintf(file,"jxF-3ph-C/nxf1/case.txt");
   
    FILE *f = fopen(file,"w");
    
    cout<<" -------------------------------------------------  "<<endl;
    for(int i=0; i<7; i++){ 
      int ii = j*10+i;
      
      d(f,ii,cen);
      
      // d(ii,1);    
    }}
  
}

void  d(FILE *f, int case = 0,  int cen=0, float e1 = 100, float e2 = 150, float eta1=2.8, float eta2 = 4.0, float pt1=0, float pt2=30, int nphotons=1, int xf =1){

  if(case==0){e1 = 20;  e2=40;}
  if(case==1){e1 = 40;  e2=50;}
  if(case==2){e1 = 50;  e2=60;}
  if(case==3){e1 = 60;  e2=70;}
  if(case==4){e1 = 70;  e2=85;}
  if(case==5){e1 = 85;  e2=100;}
  if(case==6){e1 = 100; e2=150;}
  
  if(case==10){e1 = 20;  e2=40;  xf=0;}
  if(case==11){e1 = 40;  e2=50;  xf=0;}
  if(case==12){e1 = 50;  e2=60;  xf=0;}
  if(case==13){e1 = 60;  e2=70;  xf=0;}
  if(case==14){e1 = 70;  e2=85;  xf=0;}
  if(case==15){e1 = 85;  e2=100; xf=0;}
  if(case==16){e1 = 100; e2=150; xf=0;}
  
  if(case==20){e1 = 20;  e2=40;  nphotons=2;}
  if(case==21){e1 = 40;  e2=50;  nphotons=2;}
  if(case==22){e1 = 50;  e2=60;  nphotons=2;}
  if(case==23){e1 = 60;  e2=70;  nphotons=2;}
  if(case==24){e1 = 70;  e2=85;  nphotons=2;}
  if(case==25){e1 = 85;  e2=100; nphotons=2;}
  if(case==26){e1 = 100; e2=150; nphotons=2;}
  
  if(case==30){e1 = 20;  e2=40;  xf=0; nphotons=2;}
  if(case==31){e1 = 40;  e2=50;  xf=0; nphotons=2;}
  if(case==32){e1 = 50;  e2=60;  xf=0; nphotons=2;}
  if(case==33){e1 = 60;  e2=70;  xf=0; nphotons=2;}
  if(case==34){e1 = 70;  e2=85;  xf=0; nphotons=2;}
  if(case==35){e1 = 85;  e2=100; xf=0; nphotons=2;}
  if(case==36){e1 = 100; e2=150; xf=0; nphotons=2;} 
  
  if(case==40){e1 = 20;  e2=40;  nphotons=3;}
  if(case==41){e1 = 40;  e2=50;  nphotons=3;}
  if(case==42){e1 = 50;  e2=60;  nphotons=3;}
  if(case==43){e1 = 60;  e2=70;  nphotons=3;}
  if(case==44){e1 = 70;  e2=85;  nphotons=3;}
  if(case==45){e1 = 85;  e2=100; nphotons=3;}
  if(case==46){e1 = 100; e2=150; nphotons=3;}
  
  if(case==50){e1 = 20;  e2=40;  xf=0;   nphotons=3;}
  if(case==51){e1 = 40;  e2=50;  xf=0;   nphotons=3;}
  if(case==52){e1 = 50;  e2=60;  xf=0;   nphotons=3;}
  if(case==52){e1 = 60;  e2=70;  xf=0;   nphotons=3;}
  if(case==54){e1 = 70;  e2=85;  xf=0;   nphotons=3;}
  if(case==55){e1 = 85;  e2=100; xf=0;   nphotons=3;}
  if(case==56){e1 = 100; e2=150; xf=0;   nphotons=3;}
  


  char file[200];
  sprintf(file,"e_%d_%d__eta_%f_%f__pt_%f_%f__nPhotons_%d__cen_%d__xf_%d.txt",e1,e2,eta1,eta2,pt1,pt2,nphotons,cen,xf);
  float a1, a2, a3, a4, a5;
  int a6;
  //  FILE *f = fopen(file,"r");
  ifstream infile;
  infile.open(file);
  if(infile){
  //while(infile){
    infile>>a1>>a2>>a3>>a4>>a5>>a6;
    fprintf(f,"%f %f %f %f %f %d \n ",a3,a4,-a1,a2,a5,a6);
    //scanf(f,"%f %f %f %f %f %d "&a1,&a2,&a3,&a4,&a5,&a6);
  cout<<a1<<"  "<<a2<<"  "<<a3<<" "<<a4<<"  "<<a5<<"  "<<a6<<endl;
  }

  //  infile.close(file);
  //fclose(f);



}




