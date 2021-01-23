int pldPhi(int cen=1){
 char  file[100];

  for(int j=0; j<4; j++){ 
 
    if(j==0)    sprintf(file,"jdPhi-le2ph/xf1/case.txt");
    if(j==2)    sprintf(file,"jdPhi-le2ph/nxf1/case.txt");
    
    if(j==1)    sprintf(file,"jdPhi-ge3ph/xf1/case.txt");
    if(j==3)    sprintf(file,"jdPhi-ge3ph/nxf1/case.txt");
    
    FILE *f = fopen(file,"w");
    
    cout<<" -------------------------------------------------  "<<endl;
    for(int i=0; i<=29; i++){ 
      int ii = j*30+i;
      
      d(f,ii,1);
      
      // d(ii,1);    
    }}
  
}

void d(FILE *f, int case1 = 0, int cen==1, int nphotons=1, float e1 = 0, float e2 = 0, float eta1=2.8, float eta2 = 4.0, float pt1=0, float pt2=30, float deta1 = -10.0,  float deta2 = 10.0,  float dphi1 = -10.0,  float dphi2 = 10.0, int xf =1){
 float phiS[40] = {
   /*  -(TMath::Pi()/2.0)*(4.0/4.0),
    -(TMath::Pi()/2.0)*(3.0/4.0),
    -(TMath::Pi()/2.0)*(2.0/4.0),
    -(TMath::Pi()/2.0)*(1.0/4.0),
    -(TMath::Pi()/2.0)*(0.0/4.0),
    (TMath::Pi()/2.0)*(1.0/4.0),
    (TMath::Pi()/2.0)*(2.0/4.0),
    (TMath::Pi()/2.0)*(3.0/4.0),
    (TMath::Pi()/2.0)*(4.0/4.0),
    
    (TMath::Pi()/2.0)*(10.0/10.0),
    (TMath::Pi()/2.0)*(11.0/10.0),
    (TMath::Pi()/2.0)*(12.0/10.0),
    (TMath::Pi()/2.0)*(13.0/10.0),
    (TMath::Pi()/2.0)*(14.0/10.0),
    (TMath::Pi()/2.0)*(15.0/10.0),
    (TMath::Pi()/2.0)*(16.0/10.0),
    (TMath::Pi()/2.0)*(17.0/10.0),
    
    (TMath::Pi()/2.0)*(18.0/10.0),
    (TMath::Pi()/2.0)*(19.0/10.0),
    (TMath::Pi()/2.0)*(20.0/10.0),
    (TMath::Pi()/2.0)*(21.0/10.0),
    (TMath::Pi()/2.0)*(22.0/10.0),
    (TMath::Pi()/2.0)*(23.0/10.0),
    (TMath::Pi()/2.0)*(24.0/10.0),
    (TMath::Pi()/2.0)*(25.0/10.0)
    
    (TMath::Pi()/2.0)*(26.0/10.0),
    (TMath::Pi()/2.0)*(27.0/10.0),
    (TMath::Pi()/2.0)*(28.0/10.0),
    (TMath::Pi()/2.0)*(29.0/10.0),
    (TMath::Pi()/2.0)*(30.0/10.0)*/
-1.57,
-1.18,
-0.79,
-0.39,
-0.00,
 0.39,
 0.79,
 1.18,
 1.57,
 1.57,
 1.73,
 1.88,
 2.04,
 2.20,
 2.36,
 2.51,
 2.67,
 2.83,
 2.98,
 3.14,
 3.30,
 3.46,
 3.61,
 3.77,
 4.08,
 4.24,
 4.40,
 4.56,
 4.71,
5.0
  };


if(case1<30){e1 = 60;  e2=120; pt1=2.0; pt2=25.5; eta1=2.8; eta2=4.0; dphi1=phiS[case1]; dphi2=phiS[case1+1]; nphotons=2; cen=1;}

  if(case1>=30&&case1<60){e1 = 60;  e2=120; pt1=2.0; pt2=25.5; eta1=2.8; eta2=4.0; dphi1=phiS[case1-30]; dphi2=phiS[case1+1-30]; nphotons=3; cen=1;}

  if(case1>=60&&case1<90){e1 = 60;  e2=120; pt1=2.0; pt2=25.5; eta1=2.8; eta2=4.0; dphi1=phiS[case1-60]; dphi2=phiS[case1+1-60]; nphotons=2; cen=1; xf=0;}

  if(case1>=90&&case1<120){e1 = 60;  e2=120; pt1=2.0; pt2=25.5; eta1=2.8; eta2=4.0; dphi1=phiS[case1-90]; dphi2=phiS[case1+1-90]; nphotons=3; cen=1; xf=0;}
 



  char file[300];
  // sprintf(file,"e_%d_%d__eta_%f_%f__pt_%f_%f__deta_%f_%f__dphi_%f_%f__nPhotons_%d__cen_%d__xf_%d.txt",e1,e2,eta1,eta2,pt1,pt2,nphotons,cen,deta1,deta2,dphi1,dphi2,xf);
  sprintf(file,"e_%d_%d__eta_%f_%f__pt_%f_%f__deta_%f_%f__dphi_%f_%f__nPhotons_%d__cen_%d__xf_%d.txt",e1,e2,eta1,eta2,pt1,pt2,deta1,deta2,dphi1,dphi2,nphotons,cen,xf);
  // sprintf(file,"e_%d_%d__eta_%f_%f__pt_%f_%f__deta_%f_%f__dphi_%f_%f__nPhotons_%d__cen_%d__xf_%d.txt",e1,e2,eta1,eta2,pt1,pt2,deta1,deta2,dphi1,dphi2,nphotons,cen,xf);

 // sprintf(file,"e_%d_%d__eta_%f_%f__pt_%f_%f__nPhotons_%d__cen_%d__xf_%d.txt",e1,e2,eta1,eta2,pt1,pt2,nphotons,cen,xf);
  cout<<file<<endl;

  float a1, a2, a3, a4, b1, b2, b3, b4, b5, b6,  b7, b8, a5;

  int a6;
  //  FILE *f = fopen(file,"r");
  ifstream infile;

  infile.open(file);
  if(infile){
    cout<<" HERE ------------------- "<<endl;
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




