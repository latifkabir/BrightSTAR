#ifndef StFmsDiPhoton_HH
#define StFmsDiPhoton_HH

#include "StObject.h"

#include <vector>
#include "StarClassLibrary/StThreeVectorF.hh"
#include "TVector.h"
#ifndef ST_NO_NAMESPACES
using std::vector;
#endif


//__________________________________________
//class that stores info. related to photon pairs on FMS
//copied from DiPhoton class--Yuxi Pan

class StFmsDiPhoton : public StObject {

public:
	StFmsDiPhoton(){};
	~StFmsDiPhoton(){};
	
	Float_t E1; //energy of the 1st daughter photon (with higher energy)
	Float_t E2;
	Float_t X1; //photon position in cell coordinate
	Float_t X2;
	Float_t Y1; 
	Float_t Y2;
	Float_t E12;
	Float_t Eta1;
	Float_t Eta2;
	Float_t Eta;
	Float_t Phi1;
	Float_t Phi2;
	Float_t Cluid1; //index of the cluster that produced the 1st photon
	Float_t Cluid2;
	Float_t Ncluph1; //# of photons produced by the parent cluster of the 1st photon
	Float_t Ncluph2;
	Float_t Phi;
	Float_t M12;
	Float_t Z12;
	Float_t OpAngle; //opening angle;
	Float_t Det12;
	Float_t Ph1id;	//photon id
	Float_t Ph2id;
	Float_t Chi2; // fitting chi2
	Float_t RX1;
	Float_t RX2;//relatie position
	Float_t Re1;
	Float_t Re2;//reserve
	StThreeVectorF  momentum;
	//for collins
	Int_t tag;
	Int_t tagPH1;
	Int_t tagPH2;
	Int_t catagory;


private:

ClassDef(StFmsDiPhoton,1)

};

typedef vector<StFmsDiPhoton*> StPtrVecFmsDiPhoton; 
typedef StPtrVecFmsDiPhoton::iterator StFmsDiPhotonIterator;
typedef StPtrVecFmsDiPhoton::const_iterator StFmsDiPhotonConstIterator;

#endif

