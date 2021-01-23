int plEta(int cen=0){
 char  file[100];

  for(int j=0; j<8; j++){ 
 
    if(j==0)    sprintf(file,"jEta-le2ph/xf1/case.txt");
    if(j==1)    sprintf(file,"jEta-ge3ph/xf1/case.txt");
    
    if(j==2)    sprintf(file,"jEta-le2ph-C/xf1/case.txt");
    if(j==3)    sprintf(file,"jEta-ge3ph-C/xf1/case.txt");

    if(j==4)    sprintf(file,"jEta-le2ph/xf2/case.txt");
    if(j==5)    sprintf(file,"jEta-ge3ph/xf2/case.txt");
    
    if(j==6)    sprintf(file,"jEta-le2ph-C/xf2/case.txt");
    if(j==7)    sprintf(file,"jEta-ge3ph-C/xf2/case.txt");
   
    FILE *f = fopen(file,"w");
    
    cout<<" -------------------------------------------------  "<<endl;
    for(int i=0; i<=9; i++){ 
      int ii = j*10+i;
      
      d(f,ii,cen);
      
      // d(ii,1);    
    }}
  
}

void  d(FILE *f, int case = 0,  int cen=0, float e1 = 100, float e2 = 150, float eta1=2.8, float eta2 = 4.0, float pt1=0, float pt2=30, int nphotons=1, int xf =1){

if(case==0){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.6; eta2=2.8; nphotons=2;}
  if(case==1){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.8; eta2=2.95; nphotons=2;}
  if(case==2){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.95; eta2=3.1; nphotons=2;}
  if(case==3){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.1; eta2=3.25; nphotons=2;}
  if(case==4){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.25; eta2=3.4; nphotons=2;}
  if(case==5){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.4; eta2=3.55; nphotons=2;}
  if(case==6){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.55; eta2=3.7; nphotons=2;}
  if(case==7){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.7; eta2=3.85; nphotons=2;}
  if(case==8){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.85; eta2=4.0; nphotons=2;}
  if(case==9){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=4.0; eta2=4.2; nphotons=2;}


  if(case==10){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.6; eta2=2.8; nphotons=3;}
  if(case==11){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.8; eta2=2.95; nphotons=3;}
  if(case==12){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.95; eta2=3.1; nphotons=3;}
  if(case==13){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.1; eta2=3.25; nphotons=3;}
  if(case==14){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.25; eta2=3.4; nphotons=3;}
  if(case==15){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.4; eta2=3.55; nphotons=3;}
  if(case==16){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.55; eta2=3.7; nphotons=3;}
  if(case==17){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.7; eta2=3.85; nphotons=3;}
  if(case==18){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.85; eta2=4.0; nphotons=3;}
  if(case==19){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=4.0; eta2=4.2; nphotons=3;}
  

  if(case==20){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.6; eta2=2.8; nphotons=2; cen=1;}
  if(case==21){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.8; eta2=2.95; nphotons=2; cen=1;}
  if(case==22){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.95; eta2=3.1; nphotons=2; cen=1;}
  if(case==23){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.1; eta2=3.25; nphotons=2; cen=1;}
  if(case==24){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.25; eta2=3.4; nphotons=2; cen=1;}
  if(case==25){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.4; eta2=3.55; nphotons=2; cen=1;}
  if(case==26){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.55; eta2=3.7; nphotons=2; cen=1;}
  if(case==27){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.7; eta2=3.85; nphotons=2; cen=1;}
  if(case==28){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.85; eta2=4.0; nphotons=2; cen=1;}
  if(case==29){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=4.0; eta2=4.2; nphotons=2; cen=1;}


  if(case==30){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.6; eta2=2.8; nphotons=3; cen=1;}
  if(case==31){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.8; eta2=2.95; nphotons=3; cen=1;}
  if(case==32){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=2.95; eta2=3.1; nphotons=3; cen=1;}
  if(case==33){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.1; eta2=3.25; nphotons=3; cen=1;}
  if(case==34){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.25; eta2=3.4; nphotons=3; cen=1;}
  if(case==35){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.4; eta2=3.55; nphotons=3; cen=1;}
  if(case==36){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.55; eta2=3.7; nphotons=3; cen=1;}
  if(case==37){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.7; eta2=3.85; nphotons=3; cen=1;}
  if(case==38){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=3.85; eta2=4.0; nphotons=3; cen=1;}
  if(case==39){e1 = 40;  e2=100; pt1=2.0; pt2=25.5; eta1=4.0; eta2=4.2; nphotons=3; cen=1;}

  /////////////

  if(case==40){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.6; eta2=2.8; nphotons=2;}
  if(case==41){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.8; eta2=2.95; nphotons=2;}
  if(case==42){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.95; eta2=3.1; nphotons=2;}
  if(case==43){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.1; eta2=3.25; nphotons=2;}
  if(case==44){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.25; eta2=3.4; nphotons=2;}
  if(case==45){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.4; eta2=3.55; nphotons=2;}
  if(case==46){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.55; eta2=3.7; nphotons=2;}
  if(case==47){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.7; eta2=3.85; nphotons=2;}
  if(case==48){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.85; eta2=4.0; nphotons=2;}
  if(case==49){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=4.0; eta2=4.2; nphotons=2;}


  if(case==50){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.6; eta2=2.8; nphotons=3;}
  if(case==51){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.8; eta2=2.95; nphotons=3;}
  if(case==52){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.95; eta2=3.1; nphotons=3;}
  if(case==53){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.1; eta2=3.25; nphotons=3;}
  if(case==54){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.25; eta2=3.4; nphotons=3;}
  if(case==55){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.4; eta2=3.55; nphotons=3;}
  if(case==56){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.55; eta2=3.7; nphotons=3;}
  if(case==57){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.7; eta2=3.85; nphotons=3;}
  if(case==58){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.85; eta2=4.0; nphotons=3;}
  if(case==59){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=4.0; eta2=4.2; nphotons=3;}
  

  if(case==60){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.6; eta2=2.8; nphotons=2; cen=1;}
  if(case==61){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.8; eta2=2.95; nphotons=2; cen=1;}
  if(case==62){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.95; eta2=3.1; nphotons=2; cen=1;}
  if(case==63){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.1; eta2=3.25; nphotons=2; cen=1;}
  if(case==64){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.25; eta2=3.4; nphotons=2; cen=1;}
  if(case==65){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.4; eta2=3.55; nphotons=2; cen=1;}
  if(case==66){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.55; eta2=3.7; nphotons=2; cen=1;}
  if(case==67){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.7; eta2=3.85; nphotons=2; cen=1;}
  if(case==68){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.85; eta2=4.0; nphotons=2; cen=1;}
  if(case==69){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=4.0; eta2=4.2; nphotons=2; cen=1;}


  if(case==70){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.6; eta2=2.8; nphotons=3; cen=1;}
  if(case==71){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.8; eta2=2.95; nphotons=3; cen=1;}
  if(case==72){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=2.95; eta2=3.1; nphotons=3; cen=1;}
  if(case==73){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.1; eta2=3.25; nphotons=3; cen=1;}
  if(case==74){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.25; eta2=3.4; nphotons=3; cen=1;}
  if(case==75){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.4; eta2=3.55; nphotons=3; cen=1;}
  if(case==76){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.55; eta2=3.7; nphotons=3; cen=1;}
  if(case==77){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.7; eta2=3.85; nphotons=3; cen=1;}
  if(case==78){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=3.85; eta2=4.0; nphotons=3; cen=1;}
  if(case==79){e1 = 60;  e2=100; pt1=2.0; pt2=25.5; eta1=4.0; eta2=4.2; nphotons=3; cen=1;}


  char file[300];
  sprintf(file,"e_%d_%d__eta_%f_%f__pt_%f_%f__nPhotons_%d__cen_%d__xf_%d.txt",e1,e2,eta1,eta2,pt1,pt2,nphotons,cen,xf);
  cout<<file<<endl;

  float a1, a2, a3, a4, b1, b2, b3, b4, b5, b6,  b7, b8, a5;

  int a6;
  //  FILE *f = fopen(file,"r");
  ifstream infile;

  infile.open(file);
  if(infile){
    //while(infile){
    //fprintf(fS,"%5.3f  %5.4f   %5.2f %5.2f  %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %4d",AN, ANe, PT, PTe,  EN, ENe,  ETA, ETAe,  DPHI, DPHIe,  DETA, DETAe, chiSquare,ndf-2);

    infile>>a1>>a2>>a3>>a4>>b1>>b2>>b3>>b4>>b5>>b6>>b7>>b8>>a5>>a6;
    fprintf(f,"%f %f %f %f %f  %f %f  %f %f  %f %f  %f %f  %d \n ",a3,a4,-a1,a2,b1,b2,b3,b4,b5,b6,b7,b8,a5,a6);
    //scanf(f,"%f %f %f %f %f %d "&a1,&a2,&a3,&a4,&a5,&a6);
    cout<<a1<<"  "<<a2<<"  "<<a3<<" "<<a4<<"  "<<b1<<" "<<b2<<"  "<<b3<<" "<<b4<<"  "<<b5<<" "<<b6<<"  "<<b7<<" "<<b8<<"  "<<a5<<"  "<<a6<<endl;
  }

  //  infile.close(file);
  //fclose(f);



}




