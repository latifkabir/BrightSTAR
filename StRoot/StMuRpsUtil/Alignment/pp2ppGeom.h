#ifndef PP2PP_GEOM_H
#define PP2PP_GEOM_H
/*
 $Id: pp2ppGeom.h,v 1.33 2015/10/10 21:11:15 bogdan Exp $
*/
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <utility>
#include <functional>

#include "ppUtil.h"
#include "ppLVDTConst.h"

using namespace std;

const int Plane_Direction[kMAXSEQ*kMAXCHAIN] = { 1, 1,1, 1,  -1,-1,-1,-1,  1, 1,1, 1, -1,-1,-1,-1, 
                                                 1,-1,1,-1,  -1, 1,-1, 1,  1,-1,1,-1, -1, 1,-1, 1 };
// RP station Z-position [m] 
const double RP_POS_Z[ kMAXSEQ*kMAXCHAIN ] = { 
  -15.769, -15.776, -15.782, -15.789,
  -15.771, -15.777, -15.784, -15.791,
  -17.569, -17.576, -17.582, -17.589,
  -17.569, -17.576, -17.582, -17.589,
   15.769,  15.776,  15.783,  15.789,
   15.770,  15.776,  15.783,  15.790,
   17.570,  17.577,  17.584,  17.590,
   17.569,  17.576,  17.583,  17.589
};
/*
Z-positions calculation input:
Pin E1U -15786.27798 [mm]
Pin E1D -15787.98290
Pin E2U -17586.25892
Pin E2D -17586.26322

Pin W1U 15786.31681
Pin W1D 15786.96741
Pin W2U 17587.42672
Pin W2D 17586.35569

pos-A = Pin-17.2 [mm]
pos-B = Pin-10.5
pos-C = Pin- 3.8
pos-D = Pin+ 2.9
*/
// distances RP-DX [m]
const double RP_DISTZ[kMAXSEQ] = {   2.300,   2.300,   4.100,   4.100,  2.310,  2.310,  4.110,  4.110 };
// DX - {Z0}star 
const double DX_DIST = 9.800;
const double DX_LENGTH = 3.700;
// DX kick angle [rad];
const double DX_KICK = 0.01882;
// Silicon Detectors strip pitch [m]
const double STRIP_PITCH[kMAXSEQ*kMAXCHAIN] = { 
 0.0974E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03,  
 0.0974E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03, 0.0955E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03,  
 0.0974E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03,  
 0.0974E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03, 0.0974E-03, 0.1050E-03
};
// Si sensor active length [m]  
const double SI_SENS_LEN[kMAXCHAIN] = { 48.8960E-03, 80.6400E-03, 48.8960E-03, 80.6400E-03 }; 
const double SI_BASE_POS[kMAXCHAIN] = {  0.0       ,-40.3200E-03,  0.0       ,-40.3200E-03 };
// Local relative plane offsets [milimeters] 
const double SI_OFFSET[kMAXSEQ*kMAXCHAIN] = {
 0.042, -0.003, -0.042,  0.003,  0.041, -0.008, -0.041,  0.008, 
-0.022,  0.036,  0.022, -0.036, -0.111, -0.119,  0.111 , 0.119, 
-0.014, -0.175,  0.014,  0.175,  0.068,  0.133, -0.068, -0.133, 
 0.029, -0.040, -0.029,  0.040,  0.028,  0.104, -0.028, -0.104 
};
/* ( vertical (Y) positions of roman pots as from LVDT readings [mm] */                 
typedef union pp2pp_lvdt_t{
  double lvdt[11];
  struct{double y_E1U; double y_E1D; double y_E2U; double y_E2D;
         double y_W1U; double y_W1D; double y_W2U; double y_W2D;
         double datbeg; double datend; double run;};
  bool operator<(const pp2pp_lvdt_t& rhs) const
  { return run < rhs.run; }
} PP2PP_LVDT_T; 


typedef struct pp2pp_rp_positions_t{
  char revision[80];
  int status;
  int run;
  double datbeg;
  double datend;
  double position[32];
  double offset[32];
  bool operator<(const pp2pp_rp_positions_t& rhs) const
  { return run < rhs.run; }
} PP2PP_RP_POSITIONS_T;
 

class pp2ppGeom {

  int mRunNumber;
  double mDateTime;
  int mDate;
  int mTime;
  int mSearchMode;
  char mRevision[80];
  std::string mLvdtDataPath;
  std::string mOffsetDataPath;
  pp2pp_lvdt_t LVDT_DATA;
  pp2pp_rp_positions_t RP_POSITIONS;

 public:
  pp2ppGeom();
  pp2ppGeom( std::string lvdt_path, std::string offset_path  );
  ~pp2ppGeom();

  bool   GetData( int date, int time );
  bool   GetData( int run );
  double GetStrip0( int rp, int pl );
  double GetStrip0( int pl32 );
  double GetRPzpos( int pl32 );
  double GetRPzpos( int rp, int pl );
  double GetOffset( int rp, int pl );
  double GetOffset( int pl32 );
  double GetLVDT( int rp );
  pp2pp_rp_positions_t GetItAll();
  std::pair<int, int> GetDate();
  std::pair<int, int> GetTime();
  void Dump();
  //-----
 private:
  bool   ReadData();
  void   SetPositions();
  int TimeShift( int time );



};

pp2ppGeom::pp2ppGeom(){

  mLvdtDataPath = "./Calibrations/pp2pp/pp2ppRPpositions";
  mOffsetDataPath = "./Geometry/pp2pp/pp2ppOffsets";

  int k = 0;
  while( k < kMAXSEQ*kMAXCHAIN ){ RP_POSITIONS.position[k]=0.;  RP_POSITIONS.offset[k]=0.; k++;}
  printf(" pp2ppGeom default paths:       LVDT: %s \n", mLvdtDataPath.c_str());
  printf("                          RP_POSITIONS: %s \n", mOffsetDataPath.c_str());
}

pp2ppGeom::pp2ppGeom(std::string lvdt_path, std::string offset_path ){
  mLvdtDataPath = lvdt_path;
  mOffsetDataPath = offset_path;
  int k=0;
  while( k < kMAXSEQ*kMAXCHAIN ){ RP_POSITIONS.position[k]=0.;  RP_POSITIONS.offset[k]=0.; k++;}
  printf(" pp2ppGeom  paths: LVDT raw data: %s \n", mLvdtDataPath.c_str());
  printf("                   RP_POSITIONS : %s \n", mOffsetDataPath.c_str());
}
pp2ppGeom::~pp2ppGeom(){
  /* NOP */
  cout<<" pp2ppGeom: By,by! "<<endl;
}
bool pp2ppGeom::GetData( int date, int time ){
  mSearchMode = 0;
  mRunNumber = 0;
  mDate = date;
  mTime = TimeShift( time );
  mDateTime = double( date - 20000000) + double(mTime)/1000000.;
  return ReadData(); 
}
bool pp2ppGeom::GetData( int rnum ){
  mSearchMode = 1;
  mRunNumber = rnum;
  mDate = 0;
  mTime = 0;
  mDateTime = 0.0;
  return ReadData(); 
}

bool pp2ppGeom::ReadData(){

  bool CalibDataFound = false;
  bool GeomDataFound = false;

    //
    // get RP offsets
    //
    printf( " pp2ppGeom::GetData(): Initiating RP offsets using file: %s \n", mOffsetDataPath.c_str());
    std::ifstream inpGeo;
    const char *fs = mOffsetDataPath.c_str();
    inpGeo.open( fs, ios::in | ios::binary | ios::ate);
    if( inpGeo.is_open() ){
      std::streampos sizeGeo = inpGeo.tellg();
      int nGeo = sizeGeo/sizeof( pp2pp_rp_positions_t );
      std::vector< pp2pp_rp_positions_t > offset_coll( nGeo );
      inpGeo.seekg(0, ios::beg );
      inpGeo.read( (char *)(&offset_coll[0]), sizeGeo );
      int k = 0;
      while( k< nGeo ){
	bool found = ( mSearchMode ) ? (offset_coll[k].run >= mRunNumber ) : ( mDateTime >= offset_coll[k].datbeg && mDateTime <= offset_coll[k].datend );
	if ( found ){
	  RP_POSITIONS = offset_coll[k];
	  GeomDataFound = true;
	  break;
	}
	k++;
      }
      inpGeo.close();
    }else{
      printf( " pp2ppGeom::GetData(): Failed to open offsets file ! %s  \n", fs );
      printf( " pp2ppGeom::GetData(): Initiating RP offsets using LVDT data file: %s \n", mLvdtDataPath.c_str());
      if( mSearchMode == 0 ) printf( " pp2ppGeom::GetData(): Searching date  : %14.6f  \n", mDateTime);
      else printf( " pp2ppGeom::GetData(): Searching run  : %12d  \n", mRunNumber);
      std::ifstream inCalib;
      const char *fn = mLvdtDataPath.c_str();
      inCalib.open( fn, ios::in | ios::binary | ios::ate );
      if ( inCalib.is_open() ) {
	std::streampos sizeCalib = inCalib.tellg();
	int nCalib = sizeCalib/sizeof( pp2pp_lvdt_t );
	std::vector< pp2pp_lvdt_t > lvdt_coll( nCalib );
	inCalib.seekg(0, ios::beg );
	inCalib.read( (char *)(&lvdt_coll[0]), sizeCalib );
	int k = 0;
      // find requested data chunk
	while ( k< nCalib ){
	  bool found = ( mSearchMode ) ? ((int)lvdt_coll[k].run >= mRunNumber ): ( mDateTime >= lvdt_coll[k].datbeg && mDateTime <= lvdt_coll[k].datend );
	  if ( found ){
	    LVDT_DATA = lvdt_coll[k];
	    CalibDataFound = true;
	    break;
	  }
	  k++;
	}
	inCalib.close();
      }else{
	printf( " pp2ppGeom::GetData(): Failed to open file ! %s \n",fn) ;
      }    
    }
    //
    if ( GeomDataFound ) {
	printf(" pp2ppGeom::GetData(): Found RP offsets for run %10d - valid from %12.6f thru %12.6f\n", RP_POSITIONS.run, RP_POSITIONS.datbeg, RP_POSITIONS.datend);
	printf(" pp2ppGeom::GetData(): LVDT constants: %s \n", RP_POSITIONS.revision);
	sprintf( mRevision,"%s",RP_POSITIONS.revision);
#ifdef DEBUG
	for( int i=0;i<32;i++ ) {
	  if( (i+1)%8 ) printf("%7.3f, ",RP_POSITIONS.offset[i]);
	  else printf("%7.3f, \n",RP_POSITIONS.offset[i]);
	}
#endif
    }else{
      printf( " pp2ppGeom::GetData(): Restoring RP offsets from raw LVDT readings ... \n" );
      printf( " pp2ppGeom::GetData(): LVDT constants file: %s \n",LVDT_REVISION );
      if( CalibDataFound ) {
	printf(" pp2ppGeom::GetData(): found LVDT data for run %10d - valid from %12.6f thru %12.6f \n", (int)LVDT_DATA.run, LVDT_DATA.datbeg, LVDT_DATA.datend); 
	float revision;
	int ret = sscanf( LVDT_REVISION, "%*s %*s %f %*s", &revision); 
	if( ret ) sprintf(mRevision,"LVDT const Rev: %3.1f",revision);
	mRunNumber = (int)LVDT_DATA.run;
	SetPositions();
#ifdef DEBUG
	for ( int ip=0; ip<kMAXSEQ; ip++ ) printf("%7.3f, ",LVDT_DATA.lvdt[ip]); printf("\n");
#endif
      }else{
      printf ( "pp2ppGeom::GetData(): LVDT Data for date %d , time %d not found ! \n", mDate, mTime);
      printf ( "pp2ppGeom::GetData(): RP positions not available ! \n");
      exit( EXIT_FAILURE );
      }
    }
    return 1;
}

double pp2ppGeom::GetRPzpos( int pl ){ return RP_POS_Z[ pl ]; }
double pp2ppGeom::GetRPzpos( int rp, int pl ){ return RP_POS_Z[ rp*kMAXCHAIN+pl ]; }

double pp2ppGeom::GetOffset( int rp, int pl ){ return RP_POSITIONS.offset[rp*kMAXCHAIN+pl]/1000.; } 
double pp2ppGeom::GetOffset( int ip ){ return RP_POSITIONS.offset[ip]/1000.; }
 
double pp2ppGeom::GetStrip0( int rp, int chain ){  int ip = rp*kMAXCHAIN+chain;  return ( RP_POSITIONS.position[ip]+RP_POSITIONS.offset[ip])/1000.; }
double pp2ppGeom::GetStrip0( int ip ){ return ( RP_POSITIONS.position[ip]+RP_POSITIONS.offset[ip])/1000.; }

pp2pp_rp_positions_t pp2ppGeom::GetItAll() { return RP_POSITIONS; }

void pp2ppGeom::SetPositions(){

  char dum[20];
  float revision;
  int ret = sscanf( mRevision, "%s %s %f %s", dum, dum, &revision, dum); 
  if( ret ) sprintf(RP_POSITIONS.revision,"LVDT const Rev: %3.1f",revision);

  RP_POSITIONS.status = 0;
  RP_POSITIONS.run = LVDT_DATA.run;
  RP_POSITIONS.datbeg = LVDT_DATA.datbeg;
  RP_POSITIONS.datend = LVDT_DATA.datbeg;
  for( int ip=0; ip<32; ip++){
    int rp = ip/4;
    RP_POSITIONS.position[ip]= LVDT_OFFSET[ip] + LVDT_SCALE[ip]*LVDT_DATA.lvdt[rp];
    RP_POSITIONS.offset[ip] = 0.;
  }
}

double pp2ppGeom::GetLVDT( int rp ){  return LVDT_DATA.lvdt[rp]; }

std::pair<int, int> pp2ppGeom::GetDate() { std::pair< int, int>p( (int)RP_POSITIONS.datbeg, (int)RP_POSITIONS.datend); return p; }

std::pair<int, int> pp2ppGeom::GetTime() {
  int t1 = int( (RP_POSITIONS.datbeg - (int)RP_POSITIONS.datbeg)*1000000); 
  int t2 = int( (RP_POSITIONS.datend - (int)RP_POSITIONS.datend)*1000000); 
  std::pair< int, int>p( t1, t2);
  return p;
}

void pp2ppGeom::Dump(){
      printf(" pp2ppGeom::Dump: LVDT Version %s \n",mRevision);
      printf("  Status   Run        date.time       date.time  \n");
      printf("  %4d %10d %15.6f %15.6f \n", RP_POSITIONS.status, RP_POSITIONS.run, RP_POSITIONS.datbeg, RP_POSITIONS.datend);
      printf("------------------\n");
      for( int i=0;i<32;i++ ){ printf("%7.3f (%6.3f), ",RP_POSITIONS.position[i],RP_POSITIONS.offset[i]); if( (i+1)%8==0 ) printf("\n"); }
      printf("------------------\n");
}

int pp2ppGeom::TimeShift ( int time ){
  // time =: hhmmss
  int hh = time/10000;
  int mm = (time%10000)/100;
  int ss = time%100;
  ss+=1;
  if( ss>59 ){
    ss-=60;
    mm += 1;
    if( mm>59 ){
      mm -= 60;
      hh += 1;
      if( hh > 23 ){
	cout << "pp2ppGeom::TimeShift - Must change date !" << endl;
	exit(EXIT_FAILURE);
      }
    }
  }
  return ( hh*10000+ mm*100 + ss );
}
     
#endif
